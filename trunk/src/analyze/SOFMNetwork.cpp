//! \verbatim
//! ###########################################################################
//! # SDSS sorting prototype - Stage I
//! #
//! # Copyright (c) 2009 Aick in der Au
//! # All rights reserved.
//! ###########################################################################
//!
//!      created by : Aick in der Au <aick.inderau@gmail.com>
//!      created on : 1/19/2009
//! additional docs : none
//!  responsibility : 1. Aick in der Au
//!                   2. 
//! \endverbatim
//!
//! \file  SOFMNetwork.cpp

#include "analyze/SOFMNetwork.h"

#include <conio.h>
#include <math.h>
#include <float.h>
#include <assert.h>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <map>
#include <omp.h>

#include "sdsslib/random.h"
#include "sdsslib/defines.h"
#include "sdsslib/helpers.h"
#include "sdsslib/filehelpers.h"
#include "sdsslib/Timer.h"
#include "sdsslib/spectraVFS.h"
#include "sdsslib/spectraHelpers.h"
#include "sdsslib/mathhelpers.h"
#include "sdsslib/XMLExport.h"
#include "sdsslib/XMLParser.h"
#include "sdsslib/HTMLExport.h"




static
void intensityToRGB(float _intensity, float *_pRGB )
{
	_intensity *= 3.f;
	_pRGB[2] = MIN(_intensity,1.f);
	_pRGB[1] = CLAMP(_intensity-1.f,0.f,1.f);
	_pRGB[0] = CLAMP(_intensity-2.f,0.f,1.f);
}


//Parameters(100,1,0.5,0.1,0.5,1)
//Parameters(100,26,1.0,0.001,0.5,1)
//Parameters(100,26,0.15,0.05,10,1)
//Parameters(100,26,1.0,0.001,0.5,2)
//Parameters(100,1,0.15,0.05,10,1)
//Parameters(100,26,0.25,0.01,1,0.5) << best!
SOFMNetwork::Parameters SOFMNetwork::Parameters::defaultParameters(100,26,0.25f,0.01f,1.f,0.5f); // default parameters


SOFMNetwork::Parameters::Parameters( size_t _numSteps, size_t _randomSeed, float _lRateBegin, float _lRateEnd, float _radiusBegin, float _radiusEnd )
:numSteps(_numSteps)
,randomSeed(_randomSeed)
,lRateBegin(_lRateBegin)
,lRateEnd(_lRateEnd)
,radiusBegin(_radiusBegin)
,radiusEnd(_radiusEnd)
,exportSubPage(false)
,waitForUser(false)
{
}



SOFMNetwork::SOFMNetwork( SpectraVFS *_pSourceVFS, bool bContinueComputation )
:m_pSourceVFS(_pSourceVFS)
,m_numSpectra(_pSourceVFS->getNumSpectra())
,m_gridSize(0)
,m_gridSizeSqr(0)
,m_currentStep(0)
,m_params( Parameters::defaultParameters )
,m_Min(FLT_MAX)
,m_Max(-FLT_MAX)
,m_logFile("sofm_log.txt")
{
	std::string sstrSOFMFileName("");
	if ( !readSettings("settings.xml", sstrSOFMFileName) )
	{
		//error
		Helpers::print( std::string("Error reading settings from settings.xml. Abortion.\n"), &m_logFile );
		exit(0);
	}

/*	// test begin
	float f = 0.001f;
	for ( size_t i=0;i<m_numSpectra;i++ )
	{
		Spectra *a = m_pSourceVFS->beginWrite( i );
		a->setSine( f );
		f += 0.0000125f;
		m_pSourceVFS->endWrite( i );
	}
	// test end
*/

	Helpers::print( std::string("Spectra VFS cache line size ") + Helpers::numberToString( SpectraVFS::CACHELINESIZE ) + " spectra.\n", &m_logFile );
	Helpers::print( std::string("Spectra VFS number of cache lines ") + Helpers::numberToString( SpectraVFS::CACHELINES ) + ".\n", &m_logFile );
	Helpers::print( std::string("That allows us to pack ") + Helpers::numberToString( SpectraVFS::CACHELINES*SpectraVFS::CACHELINESIZE ) + " spectra into main memory.\n", &m_logFile );
	Helpers::print( std::string("We can eat up ") + Helpers::numberToString( static_cast<float>(2*SpectraVFS::CACHELINES*SpectraVFS::CACHELINESIZE*sizeof(Spectra))/(1024.f*1024.f*1024.f) ) + " GB of main memory for clustering .\n", &m_logFile );


	calcMinMaxInputDS();


	if ( !bContinueComputation )
	{
		size_t gridSizeMin = static_cast<size_t>(ceilf(sqrtf((float)m_numSpectra+1)));//*1.2
		if ( m_gridSize < gridSizeMin )
		{
			std::string sstrString( std::string("Grid size ") );
			sstrString += Helpers::numberToString(m_gridSize);
			sstrString += std::string("  too small. Clamping to ");
			sstrString += Helpers::numberToString(gridSizeMin);
			sstrString += std::string(".\n\n");
			Helpers::print( sstrString, &m_logFile );
			m_gridSize = gridSizeMin;
		}
		m_gridSizeSqr = m_gridSize*m_gridSize;

		Helpers::print( std::string("Start clustering using ")+Helpers::numberToString(m_numSpectra)+
			            std::string(" spectra. Grid size is ")+Helpers::numberToString(m_gridSize)+std::string(".\n"), &m_logFile );

		// generate random filled cluster and load it.
		std::string sstrNet("sofmnet.bin");
		SpectraVFS::write( m_gridSize, m_Min, m_Max*0.01f, sstrNet );
		m_pNet = new SpectraVFS( sstrNet, false );
		reset(m_params);

		renderIcons();
		exportHistograms("export");

		// initialize with input data
		Helpers::print( std::string("Initializing network with input data.\n"), &m_logFile );

		Rnd r(m_params.randomSeed);
		for ( size_t i=0;i<m_gridSizeSqr;i++ )
		{
			size_t xp = i % m_gridSize;
			size_t yp = i / m_gridSize;
			const size_t inset = 0;
			if ( xp >= inset && xp <= (m_gridSize-inset) && yp >= inset && yp <= (m_gridSize-inset) )
			{
				Spectra *a = m_pNet->beginWrite( i );
				size_t spectraIndex = r.randomInt(m_numSpectra-1);
				Spectra *b = m_pSourceVFS->beginRead( spectraIndex );
				a->set( *b );
				m_pSourceVFS->endRead( spectraIndex );
				m_pNet->endWrite( i );
			}
		}

	}
	else
	{

		Helpers::print( std::string("Continue clustering at step ")+Helpers::numberToString(m_currentStep)+
			            std::string(" using ")+Helpers::numberToString(m_numSpectra)+
						std::string(" spectra. Grid size is ")+Helpers::numberToString(m_gridSize)+std::string(".\n"), &m_logFile );

		m_pNet = new SpectraVFS( sstrSOFMFileName, false );

		m_gridSizeSqr = m_pNet->getNumSpectra();

		if ( m_gridSizeSqr == 0 || 
			m_gridSize*m_gridSize != m_gridSizeSqr )
		{
			// error
			Helpers::print( std::string("Error reading ") + sstrSOFMFileName + std::string(". Abortion.\n"), &m_logFile );
			exit(0);
		}
	}

	Helpers::print( std::string("Initialization finished.\n"), &m_logFile );
}


SOFMNetwork::~SOFMNetwork()
{
	delete m_pNet;
}


void SOFMNetwork::writeSettings( const std::string &_sstrFileName )
{
	std::string sstrXML("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");

	XMLExport::xmlElementBegin( "SETTINGS", 0, sstrXML );
	XMLExport::xmlElementEndBegin( sstrXML );

	XMLExport::xmlSingleElementBegin( "STEP", 1, sstrXML );
	XMLExport::xmlAddAttribute( "current", static_cast<unsigned int>(m_currentStep), sstrXML );
	XMLExport::xmlAddAttribute( "total", static_cast<unsigned int>(m_params.numSteps), sstrXML );
	XMLExport::xmlSingleElementEnd( sstrXML );

	XMLExport::xmlSingleElementBegin( "GRIDSIZE", 1, sstrXML );
	XMLExport::xmlAddAttribute( "value", static_cast<unsigned int>(m_gridSize), sstrXML );
	XMLExport::xmlSingleElementEnd( sstrXML );

	XMLExport::xmlSingleElementBegin( "RANDOMSEED", 1, sstrXML );
	XMLExport::xmlAddAttribute( "value", static_cast<unsigned int>(m_params.randomSeed), sstrXML );
	XMLExport::xmlSingleElementEnd( sstrXML );

	XMLExport::xmlSingleElementBegin( "LEARNRATE", 1, sstrXML );
	XMLExport::xmlAddAttribute( "begin", m_params.lRateBegin, sstrXML );
	XMLExport::xmlAddAttribute( "end", m_params.lRateEnd, sstrXML );
	XMLExport::xmlSingleElementEnd( sstrXML );

	XMLExport::xmlSingleElementBegin( "RADIUS", 1, sstrXML );
	XMLExport::xmlAddAttribute( "begin", m_params.radiusBegin, sstrXML );
	XMLExport::xmlAddAttribute( "end", m_params.radiusEnd, sstrXML );
	XMLExport::xmlSingleElementEnd( sstrXML );

	XMLExport::xmlSingleElementBegin( "SPECTRUM", 1, sstrXML );
	XMLExport::xmlAddAttribute( "file", m_pNet->getFileName(), sstrXML );
	XMLExport::xmlSingleElementEnd( sstrXML );

	XMLExport::xmlElementBegin( "EXPORT", 1, sstrXML );
	XMLExport::xmlElementEndBegin( sstrXML );

	XMLExport::xmlSingleElementBegin( "SUBPAGES", 2, sstrXML );
	XMLExport::xmlAddAttribute( "value", (int)m_params.exportSubPage, sstrXML );
	XMLExport::xmlSingleElementEnd( sstrXML );

	XMLExport::xmlSingleElementBegin( "WAITFORUSER", 2, sstrXML );
	XMLExport::xmlAddAttribute( "value", (int)m_params.waitForUser, sstrXML );
	XMLExport::xmlSingleElementEnd( sstrXML );


	XMLExport::xmlElementEnd( "EXPORT", 1, sstrXML );
	XMLExport::xmlElementEnd( "SETTINGS", 0, sstrXML );

	std::ofstream fon(_sstrFileName.c_str());
	fon<<sstrXML;
}

bool SOFMNetwork::readSettings( const std::string &_sstrFileName, std::string &_sstrSOFMFileName )
{
	XMLParser p;

	//what we read:
	//		<STEP current="3" total="100"/>
	//		<GRIDSIZE x="29" y="29"/>
	//		<RANDOMSEED value="26"/>
	//		<LEARNRATE begin="0.25" end="0.01"/>
	//		<RADIUS begin="1" end="0.5"/>
	//		<SPECTRUM size="3952" file="sofmnet.bin"/>
	if (!p.loadXMLFromFile( _sstrFileName ))
	{
		return false;
	}

	bool bSuccess = true;
	size_t spectraSize = 0;

	bSuccess &= p.getChildValue("STEP", "current", m_currentStep );
	bSuccess &= p.getChildValue("STEP", "total", m_params.numSteps );
	bSuccess &= p.getChildValue("GRIDSIZE", "value", m_gridSize );
	bSuccess &= p.getChildValue("RANDOMSEED", "value", m_params.randomSeed );
	bSuccess &= p.getChildValue("LEARNRATE", "begin", m_params.lRateBegin );
	bSuccess &= p.getChildValue("LEARNRATE", "end", m_params.lRateEnd );
	bSuccess &= p.getChildValue("RADIUS", "begin", m_params.radiusBegin );
	bSuccess &= p.getChildValue("RADIUS", "end", m_params.radiusEnd );
//	bSuccess &= p.getChildValue("SPECTRUM", "size", spectraSize );
	bSuccess &= p.getChildValue("SPECTRUM", "file", _sstrSOFMFileName );

	if ( !bSuccess)
		Helpers::print( std::string("Error: some setting value could not loaded.\n"), &m_logFile );


	//bSuccess &= ( spectraSize == sizeof(Spectra) );
	//if ( !bSuccess)
	//	Helpers::Print( std::string("Error: Given spectrum size matches not with spectrum size from dump file.\n"), &m_logFile );

	p.gotoChild();
	
	do
	{
		if (p.getCurrentTag()=="EXPORT")
		{
			break;
		}
	} while (p.gotoSibling());

	size_t sp=0;
	p.getChildValue("SUBPAGES", "value", sp );
	m_params.exportSubPage = (sp>0);
	p.getChildValue("WAITFORUSER", "value", sp );
	m_params.waitForUser = (sp>0);

	return bSuccess;
}


void SOFMNetwork::calcMinMax( SpectraVFS &_vfs, float &_outMin, float &_outMax )
{
	_outMin = FLT_MAX;
	_outMax = -FLT_MAX;

	const size_t numSpectra = _vfs.getNumSpectra();

	// calc min/max
	for ( size_t i=0;i<numSpectra;i++ )
	{
		Spectra *a = _vfs.beginRead( i );

		if ( _outMin > a->m_Min ) 
		{
			_outMin = a->m_Min;
		}
		if ( _outMax < a->m_Max) 
		{
			_outMax = a->m_Max;
		}
		_vfs.endRead( i );
	}
	// for sdds: -560 .. ~20.000 Angström
}


void SOFMNetwork::calcMinMaxInputDS()
{
	Helpers::print( std::string("Calculating min/max of input.\n"), &m_logFile );
	calcMinMax( *m_pSourceVFS, m_Min, m_Max );
	Helpers::print( std::string("global min / max: ") + Helpers::numberToString(m_Min) + std::string(" / " ) + Helpers::numberToString(m_Max) + std::string("\n" ), &m_logFile );
}



void SOFMNetwork::exportHistograms( const std::string &_sstrExportDirectory )
{
	std::string sstrDir( FileHelpers::getFilePath(_sstrExportDirectory) );
	if ( !sstrDir.empty() ) {
		sstrDir += "/";
	}

	Helpers::print( std::string("Exporting energy maps.\n"), &m_logFile );
	std::vector<float> energymap;
	std::vector<float> toatalenergymap;
	std::vector<float> zmap;

	for ( size_t i=0;i<m_numSpectra;i++ )
	{
		Spectra *a = m_pSourceVFS->beginRead( i );
		energymap.push_back(a->m_Max);
		toatalenergymap.push_back(a->getTotalEnergy());
		zmap.push_back(a->m_Z);
		m_pSourceVFS->endRead( i );
	}

	std::sort( energymap.begin(), energymap.end() );
	std::sort( toatalenergymap.begin(), toatalenergymap.end() );
	std::sort( zmap.begin(), zmap.end() );

	const size_t width = 800;
	const size_t height = 533;

	SpectraHelpers::renderDiagramToDisk(&energymap[0], energymap.size(), 4, 0, width, height, sstrDir+std::string("energymap.png") );
	SpectraHelpers::renderDiagramToDisk(&toatalenergymap[0], toatalenergymap.size(), 4, 0, width, height, sstrDir+std::string("toatalenergymap.png") );
	SpectraHelpers::renderDiagramToDisk(&zmap[0], zmap.size(), 4, 0, width, height, sstrDir+std::string("zmap.png") );
}


void SOFMNetwork::renderIcons()
{
	Helpers::print( std::string("Rendering Icons.\n"), &m_logFile );

	float globalmax = fabs(m_Max);
	if (globalmax==0.0f) {
		globalmax = 0.0001f;
	}

	// render icons
	int plateOld = -1;	
	for ( size_t i=0;i<m_numSpectra;i++ )
	{
		Spectra *a = m_pSourceVFS->beginRead( i );
		int plate = a->getPlate();
		std::string sstrDir("export/");
		sstrDir += Spectra::plateToString(plate) + std::string("/");
		if ( plate != plateOld )
		{
			plateOld = plate;
			CreateDirectory( sstrDir.c_str(), NULL );
		}
		std::string sstrFilename(sstrDir);
		sstrFilename += a->getFileName();
		sstrFilename += ".png";
		float localmax = fabs(a->m_Max);
		float redness = localmax;
		if (redness != 0.f)
		{
			redness = MathHelpers::logf(redness,globalmax);
			redness *= redness*2.f;
		}

//		redness = (float)i*2.f/(float)m_numSpectra;

		SpectraHelpers::renderSpectraIconToDisk(*a, sstrFilename, 100, 100, localmax, redness );

		m_pSourceVFS->endRead( i );
	}
}

Spectra SOFMNetwork::getSOFMSpectra( size_t _cellX, size_t _cellY )
{
	assert( m_pNet != NULL );
	assert( _cellX < m_gridSize );
	assert( _cellY < m_gridSize );
	size_t nIndex( _cellY*m_gridSize+_cellX );
	Spectra resultSpectra =	*m_pNet->beginRead( nIndex );
	m_pNet->endRead( nIndex );
	return resultSpectra;
}



void SOFMNetwork::reset( const Parameters &_params )
{
	m_params = _params;
	m_currentStep = 0;
	m_Random.initRandom( m_params.randomSeed );
}



void SOFMNetwork::adaptNetwork( const Spectra &_spectrum, size_t _bestMatchIndex, float _adaptionThreshold, float _sigmaSqr, float _lRate )
{
	const size_t xpBestMatch = _bestMatchIndex % m_gridSize;
	const size_t ypBestMatch = _bestMatchIndex / m_gridSize;
	const float sigmaSqr2 = _sigmaSqr*2.f;

	// adjust weights of the whole network
	size_t c=0;
	for ( size_t y=0;y<m_gridSize;y++)
	{
		const float tdisty = static_cast<float>(y)-static_cast<float>(ypBestMatch);
		const float tdisty2=tdisty*tdisty;

		for ( size_t x=0;x<m_gridSize;x++)
		{
			const float tdistx = static_cast<float>(x)-static_cast<float>(xpBestMatch);
			const float tdistx2 = tdistx*tdistx;
			const float tdistall = tdistx2+tdisty2;
			//const float mexican_hat_term = 1.f-tdistall/sigmaSqr;
			//const float hxy = exp(-(tdistall)/sigmaSqr2);							// original
			//const float hxy = exp(-(tdistall)/sigmaSqr2)*mexican_hat_term;		// Mexican hat
			const float hxy = exp(-sqrtf(tdistall)/sigmaSqr2);						// spike

			const float lratehsx = _lRate*hxy;

			if ( lratehsx > _adaptionThreshold )
			{
				Spectra *a = m_pNet->beginWrite( c );
				for ( size_t i=0;i<Spectra::numSamples;i++ )
				{
					a->m_Amplitude[i] += lratehsx*(_spectrum.m_Amplitude[i]-a->m_Amplitude[i]);
				}
				m_pNet->endWrite( c );
			}
			c++;
		}
	}
}

void SOFMNetwork::searchBestMatchComplete( const std::vector<size_t> &_spectraIndexList, 
										  size_t _spectraIndexListOffset, 
										  BestMatch *_pBestMatchBatch, 
										  size_t _numBestMatchElements, 
										  bool _bOnFrameOnly )
{
	assert( _pBestMatchBatch != NULL );
	assert( _numBestMatchElements <= SpectraVFS::CACHELINESIZE );

	const int numElements = static_cast<int>(_numBestMatchElements);
	
	for ( size_t i = 0;i < m_gridSizeSqr;i++)
	{
		Spectra *a = m_pNet->beginRead( i );

		Spectra *src[SpectraVFS::CACHELINESIZE];

		// begin read for all src spectra
		for ( int k=0;k<_numBestMatchElements;k++)
		{
			// skip spectra that are not on-frame if we are in on-frame search mode
			if ( _bOnFrameOnly && !_pBestMatchBatch[k].bOnFrame ) {
				continue;
			}
			const size_t spectraIndex = _spectraIndexList[_spectraIndexListOffset+k];
			src[k] = m_pSourceVFS->beginRead(spectraIndex);
		}


#pragma omp parallel for
		for ( int k=0;k<numElements;k++)
		{
			BestMatch &currentBestMatch = _pBestMatchBatch[k];
			// perform comparision only if we are:
			// a) on-frame 
			// b) or for all spectra if on-onframe search mode is not activated.
			if ( !_bOnFrameOnly || currentBestMatch.bOnFrame ) 
			{
				Spectra &currentSpectra = *src[k];

				const float errMin = a->compare( currentSpectra );
				
				if (errMin < currentBestMatch.error && a->isEmpty() )
				{
					currentBestMatch.error = errMin;
					currentBestMatch.index = i;
				}
			}
		}

		// end read for all src spectra
		for ( int k=0;k<numElements;k++)
		{
			if ( _bOnFrameOnly && !_pBestMatchBatch[k].bOnFrame ) {
				continue;
			}
			const size_t spectraIndex = _spectraIndexList[_spectraIndexListOffset+k];
			m_pSourceVFS->endRead(spectraIndex);
		}

		m_pNet->endRead( i );
	}
}
 
void SOFMNetwork::searchBestMatchLocal( const std::vector<size_t> &_spectraIndexList, 
									   size_t _spectraIndexListOffset, 
									   BestMatch *_pBestMatchBatch, 
									   size_t _numBestMatchElements )
{
	assert( _pBestMatchBatch != NULL );
	assert( _numBestMatchElements <= SpectraVFS::CACHELINESIZE );

	const int numElements = static_cast<int>(_numBestMatchElements);

	Spectra *src[SpectraVFS::CACHELINESIZE];
	for ( int k=0;k<_numBestMatchElements;k++)
	{
		const size_t spectraIndex = _spectraIndexList[_spectraIndexListOffset+k];
		src[k] = m_pSourceVFS->beginRead(spectraIndex);
	}

	for ( int k=0;k<_numBestMatchElements;k++)
	{
		BestMatch &currentBestMatch = _pBestMatchBatch[k];
		Spectra &currentSpectra = *src[k];

		const int xpBestMatchOld = currentSpectra.m_Index % m_gridSize;
		const int ypBestMatchOld = currentSpectra.m_Index / m_gridSize;

		const int xMin = MAX( xpBestMatchOld-s_searchRadius, 0 );
		const int yMin = MAX( ypBestMatchOld-s_searchRadius, 0 );
		const int xMax = MIN( xpBestMatchOld+s_searchRadius, m_gridSize );
		const int yMax = MIN( ypBestMatchOld+s_searchRadius, m_gridSize );

		for ( int y=xMin;y<yMax;y++ )
		{
			for ( int x=xMin;x<xMax;x++ )
			{
				const size_t spectraIndex = getIndex( x, y );
				Spectra *a = m_pNet->beginRead( spectraIndex );

				const float errMin = a->compare( currentSpectra );

				if (errMin < currentBestMatch.error && a->isEmpty() )
				{
					currentBestMatch.error = errMin;
					currentBestMatch.index = spectraIndex;
					currentBestMatch.bOnFrame = (x==xMin) || (x==(xMax-1)) || (y==yMin) || (y==yMax-1); // are we sitting on the border of the search frame ?
				}

				m_pNet->endRead( spectraIndex );
			}
		}
	}

	for ( int k=0;k<_numBestMatchElements;k++)
	{
		const size_t spectraIndex = _spectraIndexList[_spectraIndexListOffset+k];
		m_pSourceVFS->endRead(spectraIndex);
	}

	Helpers::print( std::string("complete search for all on framers.\n"), &m_logFile );

	// do global search for all on-framers
	searchBestMatchComplete( _spectraIndexList, _spectraIndexListOffset, _pBestMatchBatch, _numBestMatchElements, true );
}

void SOFMNetwork::process()
{
	if ( m_currentStep > m_params.numSteps )
	{
		Helpers::print( std::string("Clustering finished (success).\n"), &m_logFile );
		exit(1);
	}

	if ( m_currentStep == m_params.numSteps )
	{
		exportToHTML("export/full", true);
	} else
	if ( m_currentStep == m_params.numSteps/2 )
	{
		exportToHTML("export/half", false);
	} else
	if ( m_currentStep == m_params.numSteps/4 )
	{
		exportToHTML("export/quarter", false);
	} else
	if ( m_currentStep == 1 )
	{
		exportToHTML("export/first", true);
	} else
	if ( m_currentStep > 1 ) 
	{
		exportToHTML("export/current", false);
	}

	std::string sstrLog("Calculating step ");
	sstrLog += Helpers::numberToString( m_currentStep ) + " / " + Helpers::numberToString( m_params.numSteps ) + "\n";
	Helpers::print( sstrLog, &m_logFile );

	const float lPercent = static_cast<float>(m_currentStep)/static_cast<float>(m_params.numSteps);
	const float lRate = m_params.lRateBegin*pow(m_params.lRateEnd/m_params.lRateBegin,lPercent);
	const float adaptionThreshold = m_params.lRateEnd*0.01f;
	const float sigma = m_params.radiusBegin*pow(m_params.radiusEnd/m_params.radiusBegin,lPercent);
	const float sigmaSqr = sigma*sigma;


	// select random spectra from spectra dataset
	std::vector<size_t> spectraIndexList;

	// store all indicies in a list
	for ( size_t i=0;i<m_numSpectra;i++) 
	{
		spectraIndexList.push_back(i);
	}

	// shake well
	for ( size_t i=0;i<m_numSpectra*2;i++)
	{
		size_t ind0 = m_Random.randomInt(m_numSpectra-1);
		size_t ind1 = m_Random.randomInt(m_numSpectra-1);

		size_t hui = spectraIndexList[ind0];
		spectraIndexList[ind0] = spectraIndexList[ind1];
		spectraIndexList[ind1] = hui;
	}

	// clear names
	for ( size_t i = 0;i < m_gridSizeSqr;i++)
	{
		Spectra *a = m_pNet->beginWrite( i );
		a->m_SpecObjID = 0;
		m_pNet->endWrite( i );
	}

	std::vector<size_t> spectraCollisionList;

	// for each training spectra..
	size_t j=0;
	while (j<m_numSpectra)
	{
		const int jInc = MIN( SpectraVFS::CACHELINESIZE, (MIN(m_numSpectra, j+SpectraVFS::CACHELINESIZE)-j));
		//_cprintf( "." );


		// initialize best match batch
		BestMatch bestMatch[SpectraVFS::CACHELINESIZE];
		for ( int k=0;k<jInc;k++)
		{
			bestMatch[k].error = FLT_MAX;
			bestMatch[k].index = 0;
		}

		// retrieve best match neuron for a cache line batch of source spectra
		Timer t;

		const bool bFullSearch = ((m_currentStep % MAX((m_params.numSteps/s_globalSearchFraction),1)) == 0) || (m_currentStep<5);

		if (bFullSearch) 
		{
			Helpers::print( "using complete search.\n" );
			searchBestMatchComplete( spectraIndexList, j, bestMatch, jInc );
		}
		else
		{
			Helpers::print( "using local search.\n" );
			searchBestMatchLocal( spectraIndexList, j, bestMatch, jInc );
		}


		double searchTime = t.getElapsedSecs();
		Helpers::print( std::string("Best match search time: ")+Helpers::numberToString<float>(searchTime)+std::string("\n"), &m_logFile );

		t.start();

		for ( int k=0;k<jInc;k++)
		{
			BestMatch &currentBestMatch = bestMatch[k];
			const size_t spectraIndex = spectraIndexList.at(j+k);
			Spectra &currentSpectra = *m_pSourceVFS->beginWrite(spectraIndex);

			Spectra *a = m_pNet->beginWrite( currentBestMatch.index );

			// remember best match position to NW for faster search
			currentSpectra.m_Index = currentBestMatch.index;


			// set name of best match neuron
			if ( a->isEmpty() )
			{
				a->m_SpecObjID = currentSpectra.m_SpecObjID;
				a->m_Index = spectraIndex;

			}
			else
			{
				// collision handling
				// this cell in our cluster is already occupied by another neuron/spectra match
				// check errors  
				Spectra *b = m_pSourceVFS->beginRead(a->m_Index);
				const float errorOld = a->compare( *b );
				m_pSourceVFS->endRead(a->m_Index);

				if ( errorOld < currentBestMatch.error )
				{
					// old best match wins and holds its cell-
					// put current best match in collision list for further processing
					spectraCollisionList.push_back( spectraIndex );
				}
				else
				{
					// new best match wins, put old match into collision list
					spectraCollisionList.push_back( a->m_Index );
					a->m_SpecObjID = currentSpectra.m_SpecObjID;
					a->m_Index = spectraIndex;
				}
			}
			m_pNet->endWrite( currentBestMatch.index );


			//_cprintf(":");

			adaptNetwork( currentSpectra, currentBestMatch.index, adaptionThreshold, sigmaSqr, lRate );

			m_pSourceVFS->endWrite(spectraIndex);
		}
		double adaptionTime = t.getElapsedSecs();
		Helpers::print( std::string("NW adaption time: ")+Helpers::numberToString<float>(adaptionTime)+std::string("\n"), &m_logFile );


		j += jInc;
	}

	//_cprintf( "=" );

	Timer t;
	// collision handling
	// for each collision spectra..
	for ( size_t j=0;j<spectraCollisionList.size();j++)
	{
		const size_t spectraIndex = spectraCollisionList.at(j);
		Spectra &currentSpectra = *m_pSourceVFS->beginWrite(spectraIndex);

		// retrieve first best match neuron
		float min = FLT_MAX;
		size_t bestMatch = 0;
		
		for ( size_t i = 0;i < m_gridSizeSqr;i++)
		{
			Spectra *a = m_pNet->beginRead( i );
			float minErr =a->compare( currentSpectra );

			if ( minErr < min && a->isEmpty() )
			{
				min = minErr;
				bestMatch = i;
			}
			m_pNet->endRead( i );
		}

		//_cprintf(":");

		Spectra *a = m_pNet->beginWrite( bestMatch );
		a->m_SpecObjID = currentSpectra.m_SpecObjID;
		a->m_Index = spectraIndex;
		m_pNet->endWrite( bestMatch );

		// remember best match position to NW for faster search
		currentSpectra.m_Index = bestMatch;

		adaptNetwork( currentSpectra, bestMatch, adaptionThreshold, sigmaSqr, lRate );

		m_pSourceVFS->endWrite(spectraIndex);
		//_cprintf(".");
	}


	double collisionTime = t.getElapsedSecs();
	Helpers::print( std::string("Collision NW adaption time: ")+Helpers::numberToString<float>(collisionTime)+std::string("\n"), &m_logFile );
	Helpers::print( std::string("Flushing cluster table to disk.\n"), &m_logFile );

	m_pNet->flush();

	m_currentStep++;
}


void SOFMNetwork::calcUMatrix( const std::string &_sstrFilenName, bool _bUseLogScale, bool _bShowEmpty, bool _bNormalize )
{
	if ( m_pNet == NULL || m_pNet->getNumSpectra() == 0 )
	{
		return;
	}
	assert( !_sstrFilenName.empty() );

	float *pUMatrix = new float[m_gridSizeSqr];
	float *pRGBMap = new float[m_gridSizeSqr*3];

	float maxErr = 0.0f;

	// flash
	for (size_t i=0;i<m_gridSizeSqr;i++) 
	{
		pUMatrix[i] = 0.0f;
	}

	// calc errors
	for ( size_t y=1;y<m_gridSize-1;y++ )
	{
		for ( size_t x=1;x<m_gridSize-1;x++ )
		{
			const size_t i = CALC_ADRESS(x,y,m_gridSize);
			const size_t iLeft = CALC_ADRESS(x-1,y,m_gridSize);
			const size_t iRight = CALC_ADRESS(x+1,y,m_gridSize);
			const size_t iUp = CALC_ADRESS(x,y-1,m_gridSize);
			const size_t iBottom = CALC_ADRESS(x,y+1,m_gridSize);
			Spectra *spCenter = m_pNet->beginRead( i );
			Spectra *spLeft = m_pNet->beginRead( iLeft );
			Spectra *spRight = m_pNet->beginRead( iRight );
			Spectra *spTop = m_pNet->beginRead( iUp );
			Spectra *spBottom = m_pNet->beginRead( iBottom );

			if ( _bShowEmpty || !spCenter->isEmpty() )
			{
				ISSE_ALIGN Spectra backupCenter (*spCenter);
				ISSE_ALIGN Spectra backupLeft(*spLeft);
				ISSE_ALIGN Spectra backupRight(*spRight);
				ISSE_ALIGN Spectra backupTop(*spTop);
				ISSE_ALIGN Spectra backupBottom(*spBottom);

				if ( _bNormalize )
				{
					backupCenter.normalize();
					backupLeft.normalize();
					backupRight.normalize();
					backupTop.normalize();
					backupBottom.normalize();
				}

				pUMatrix[i] += backupCenter.compare( backupLeft );
				pUMatrix[i] += backupCenter.compare( backupRight );
				pUMatrix[i] += backupCenter.compare( backupTop );
				pUMatrix[i] += backupCenter.compare( backupBottom );

				maxErr = MAX( maxErr, pUMatrix[i] );
			}


			m_pNet->endRead( i );
			m_pNet->endRead( iLeft );
			m_pNet->endRead( iRight );
			m_pNet->endRead( iUp );
			m_pNet->endRead( iBottom );
		}
	}
	
	// normalize
	if ( maxErr > 0.0f )
	{
		for (size_t i=0;i<m_gridSizeSqr;i++) 
		{
			float scale;
			if ( _bUseLogScale )
			{
				// logarithmic scale
				scale  = log10f(pUMatrix[i]+1.f)/log10f(maxErr);
			}
			else
			{
				// linear scale
				scale  = pUMatrix[i] /= maxErr;;
			}

			intensityToRGB( scale,  &pRGBMap[i*3] );
		}
	}


	SpectraHelpers::saveIntensityMap( pRGBMap, m_gridSize, m_gridSize, _sstrFilenName );

	delete[] pUMatrix;
	delete[] pRGBMap;
}



void SOFMNetwork::calcDifferenceMap( const std::string &_sstrFilenName, bool _bUseLogScale, bool _bNormalize )
{
	if ( m_pNet == NULL || m_pNet->getNumSpectra() == 0 )
	{
		return;
	}
	if ( m_pSourceVFS == NULL || m_pSourceVFS->getNumSpectra() == 0 )
	{
	}

	assert( !_sstrFilenName.empty() );

	float *pUMatrix = new float[m_gridSizeSqr];
	float *pRGBMap = new float[m_gridSizeSqr*3];

	float maxErr = 0.0f;

	// calc errors
	for ( size_t i=0;i<m_gridSizeSqr;i++ )
	{
		Spectra *spNet = m_pNet->beginRead( i );
		ISSE_ALIGN Spectra backupNet(*spNet);

		if ( spNet->isEmpty() )
		{
			// mark empty cells
			pUMatrix[i] = -1.f;
		}
		else
		{
			Spectra *spSource = m_pSourceVFS->beginRead( spNet->m_Index );
			ISSE_ALIGN Spectra backupSource(*spSource);

			if ( _bNormalize )
			{
				backupSource.normalize();
				backupNet.normalize();
			}
			pUMatrix[i] = backupNet.compare( backupSource );
			maxErr = MAX( maxErr, pUMatrix[i] );
			m_pSourceVFS->endRead( spNet->m_Index );
		}
		m_pNet->endRead( i );
	}

	// normalize
	if ( maxErr > 0.0f )
	{
		for (size_t i=0;i<m_gridSizeSqr;i++) 
		{
			if ( pUMatrix[i] >= 0.0f )
			{
				float scale;
				if ( _bUseLogScale )
				{
					// logarithmic scale
					scale  = log10f(pUMatrix[i]+1.f)/log10f(maxErr);
				}
				else
				{
					// linear scale
					scale  = pUMatrix[i] /= maxErr;
				}

				intensityToRGB( scale, &pRGBMap[i*3] );
			}
			else
			{
				// mark empty cells
				pRGBMap[i*3]   = 0.5f;
				pRGBMap[i*3+1] = 0.5f;
				pRGBMap[i*3+2] = 0.5f;
			}
		}
	}


	SpectraHelpers::saveIntensityMap( pRGBMap, m_gridSize, m_gridSize, _sstrFilenName );

	delete[] pUMatrix;
	delete[] pRGBMap;
}


void SOFMNetwork::generateHTMLInfoPages( const std::string &_sstrMapBaseName )
{
	Helpers::print( "Generating info pages for spectra\n", &m_logFile );

	std::string sstrMainHTMLDoc;
	std::string sstrHTMLDocTemplate = HTMLExport::loadHTMLTemplate();

	const float compareInvariance = 0.1f;
	const size_t numHiScoresEntries = 20;


	float *pErrMap = new float[m_gridSizeSqr];
	float *pRGBMap = new float[m_gridSizeSqr*3];

	std::map<float,size_t> comparisonMap;
	int plateOld = -1;	
	for ( size_t i=0;i<m_numSpectra;i++ )
	{
		Spectra *a = m_pSourceVFS->beginRead( i );

		Helpers::print( std::string("Generating info page for spectrum\n")+a->getFileName()+std::string("\n"), &m_logFile );

		const int xpA = a->m_Index % m_gridSize;
		const int ypA = a->m_Index / m_gridSize;

		comparisonMap.clear();

		// ..also generate a local comparison map
		float maxErr = 0.0f;
		for (size_t c=0;c<m_gridSizeSqr;c++) 
		{
			pErrMap[c] = 0.0f;
		}


		for ( size_t j=0;j<m_numSpectra;j++ )
		{
			Spectra *b = m_pSourceVFS->beginRead( j );
			float err = a->compareSuperAdvanced( *b, compareInvariance ); 
			//float err = a->compare( *b ); 
			comparisonMap.insert( std::pair<float, size_t>(err, j) );	

			// fill comparison map
			if ( b->m_Index >= 0 && b->m_Index < m_gridSizeSqr )
			{
				assert( pErrMap[b->m_Index] == 0.0f );
				pErrMap[b->m_Index] = err;
				maxErr = MAX( maxErr, err );
			}

			m_pSourceVFS->endRead( j );
		}

		// create directory by plate
		int plate = a->getPlate();
		std::string sstrDir("export/");
		sstrDir += Spectra::plateToString(plate) + std::string("/");
		if ( plate != plateOld )
		{
			plateOld = plate;
			CreateDirectory( sstrDir.c_str(), NULL );
		}

		// filename for HTML page
		std::string sstrFilename(sstrDir);
		sstrFilename += a->getFileName();
		sstrFilename += ".html";

		// save local comparison map
		std::string sstrComprarisonMapFilename( sstrDir ) ;
		sstrComprarisonMapFilename += "LocalComparsion";
		sstrComprarisonMapFilename += a->getFileName();
		if ( maxErr > 0.0f )
		{
			for (size_t c=0;c<m_gridSizeSqr;c++) 
			{
				float scale  = log10f(pErrMap[c]+1.f)/log10f(maxErr);
				intensityToRGB( scale,  &pRGBMap[c*3] );
			}
		}
		// mark own position with red dot.
		{
			for ( int ty=-1;ty<=1;ty++)
			{
				for ( int tx=-1;tx<=1;tx++)
				{
					size_t o = (CALC_ADRESS_SAFE(xpA+tx,ypA+ty,m_gridSize,m_gridSize))*3;
					pRGBMap[o] = 1.0f;	pRGBMap[o+1] = 0.0f;	pRGBMap[o+2] = 0.0f;
				}
			}
		}
		SpectraHelpers::saveIntensityMap( pRGBMap, m_gridSize, m_gridSize, sstrComprarisonMapFilename );


		sstrMainHTMLDoc = sstrHTMLDocTemplate;

		// replace empty token.
		Helpers::insertString( HTMLExport::HTML_TOKEN_INFO, std::string(""), sstrMainHTMLDoc );

		std::string sstrInfo( "Summary page for spectrum " );
		sstrInfo += a->getFileName();
		Helpers::insertString( HTMLExport::HTML_TOKEN_TITLE, sstrInfo, sstrMainHTMLDoc );

		std::string sstrTable("");

		// insert icon and comparison map
		sstrTable += HTMLExport::beginTableRow();
		sstrTable += HTMLExport::beginTableCell();
		sstrTable += HTMLExport::image(a->getFileName()+ ".png"); 
		sstrTable += HTMLExport::endTableCell();
		sstrTable += HTMLExport::endTableRow();

		// insert search results
		size_t c=0;
		std::map<float,size_t>::iterator it( comparisonMap.begin() ); 
		while ( it != comparisonMap.end() && c < numHiScoresEntries)
		{
			Spectra *b = m_pSourceVFS->beginRead( it->second );

			const int xpB = b->m_Index % m_gridSize;
			const int ypB = b->m_Index / m_gridSize;

			const int xD = xpA-xpB;
			const int yD = ypA-ypB;

			const float error = it->first;

			sstrTable += HTMLExport::beginTableRow();

			sstrTable += HTMLExport::beginTableCell();
			sstrTable += HTMLExport::imageLink( std::string("http://cas.sdss.org/dr6/en/get/specById.asp?id=")+Helpers::numberToString<__int64>(b->m_SpecObjID), b->getURL() );
			sstrTable += HTMLExport::lineBreak();
			sstrTable += "err=";
			sstrTable += Helpers::numberToString<float>(error);
			sstrTable += "  z=";
			sstrTable += Helpers::numberToString<float>(b->m_Z);
			sstrTable += "  ";
			sstrTable += b->getFileName();
			sstrTable += "  ";

			// link to map if export sub pages = yes
			if ( m_params.exportSubPage ) 
			{
				sstrTable += HTMLExport::textLink(std::string("map"), std::string("../") + _sstrMapBaseName + Helpers::numberToString<int>(xpB/s_outputPlanSize) + "_" + Helpers::numberToString<int>(ypB/s_outputPlanSize) + ".html");
			}

			// distance
			if ( a->m_Index >= 0 && b->m_Index >= 0 )
			{
				sstrTable += "distance " + Helpers::numberToString<int>(xD) + "," + Helpers::numberToString<int>(yD);
			}
			sstrTable += HTMLExport::endTableCell();

			sstrTable += HTMLExport::beginTableCell();
			sstrTable += HTMLExport::image( "../empty.png" );
			sstrTable += HTMLExport::endTableCell();
			sstrTable += HTMLExport::beginTableCell();
			sstrTable += HTMLExport::image("../"+Spectra::plateToString(b->getPlate()) +"/LocalComparsion"+b->getFileName() + ".png");
			sstrTable += HTMLExport::endTableCell();
		
			{
				// sub table
				std::string sstrSubTable(HTMLExport::beginTable());

				const int dim = static_cast<int>(s_outputPlanSize/2);
				int xStart = MAX(xpB - dim, 0);
				int xEnd   = MIN(xpB + dim+1, m_gridSize);
				int yStart = MAX(ypB - dim, 0);
				int yEnd   = MIN(ypB + dim+1, m_gridSize);

				for ( int y=yStart;y<yEnd;y++)
				{
					sstrSubTable += HTMLExport::beginTableRow();
					sstrSubTable += HTMLExport::beginTableCell();
					sstrSubTable += HTMLExport::image( "../empty.png" );
					sstrSubTable += HTMLExport::endTableCell();
					for ( int x=xStart;x<xEnd;x++)
					{
						size_t nIndex = getIndex(x,y);
						Spectra *sp = m_pNet->beginRead( nIndex );
						sstrSubTable += HTMLExport::beginTableCell();
						// insert link
						if ( sp->m_Index>=0 && !sp->isEmpty() )
						{
							assert(sp->m_Index<static_cast<int>(m_numSpectra));
							std::string sstrPlanDirectory = "../"+Spectra::plateToString(sp->getPlate()) +"/";
							std::string sstrImagePlan = sstrPlanDirectory + sp->getFileName();
							sstrSubTable += HTMLExport::imageLink( sstrImagePlan+".png", sstrImagePlan+".html" );
						}
						else
						{
							// insert image
							sstrSubTable += HTMLExport::image( "../empty.png" );
						}

						m_pNet->endRead( nIndex );

						sstrSubTable += HTMLExport::endTableCell();
					}
					sstrSubTable += HTMLExport::endTableRow();
				}
				sstrSubTable += HTMLExport::endTable();
				sstrTable += HTMLExport::beginTableCell();
				sstrTable += sstrSubTable;
				sstrTable += HTMLExport::endTableCell();
			}


			sstrTable += HTMLExport::endTableRow();

			m_pSourceVFS->endRead( it->second );
			it++;
			c++;
		}

		Helpers::insertString( HTMLExport::HTML_TOKEN_TEMPLATE, sstrTable, sstrMainHTMLDoc );

		std::ofstream fon(sstrFilename.c_str());
		fon<<sstrMainHTMLDoc;

		m_pSourceVFS->endRead( i );
		return; // test.
	}

	delete[] pErrMap;
	delete[] pRGBMap;

}


void SOFMNetwork::exportToHTML( const std::string &_sstrFilename, bool _fullExport )
{
	std::string sstrName( FileHelpers::getFileName(_sstrFilename) );
	std::string sstrDirectory( FileHelpers::getFilePath(_sstrFilename) );

	std::string sstrLog;
	sstrLog = "Exporting results to ";
	sstrLog += _sstrFilename;
	sstrLog +="\n";

	Helpers::print( sstrLog, &m_logFile );

	std::string sstrHTMLDocTemplate = HTMLExport::loadHTMLTemplate();	
	std::string sstrHTMLDoc;
	std::string sstrMainHTMLDoc;
	std::string sstrMainTable;

	sstrMainHTMLDoc = sstrHTMLDocTemplate;

	std::string sstrInfo;

	const std::string sstrUMatrix =  "UMatrix_" + sstrName;
	const std::string sstrDifferenceMap = "DifferenceMap_" + sstrName;
	calcUMatrix( sstrDirectory+sstrUMatrix, true, false, false );
	calcDifferenceMap( sstrDirectory+sstrDifferenceMap, true, false);

	if ( _fullExport )
	{
		generateHTMLInfoPages( sstrName );
	}
	
	sstrInfo += std::string("creation date: ")+Helpers::getCurentDateTimeStampString()+HTMLExport::lineBreak();
	sstrInfo += std::string("step: ")+Helpers::numberToString( m_currentStep )+std::string(" / ")+Helpers::numberToString( m_params.numSteps )+HTMLExport::lineBreak();
	sstrInfo += std::string("grid size: ")+Helpers::numberToString( m_gridSize )+HTMLExport::lineBreak();
	sstrInfo += std::string("number of clustered spectra: ")+Helpers::numberToString( m_numSpectra )+HTMLExport::lineBreak();
	sstrInfo += std::string("random seed: ")+Helpers::numberToString( m_params.randomSeed )+HTMLExport::lineBreak();
	sstrInfo += std::string("learn rate begin: ")+Helpers::numberToString( m_params.lRateBegin )+HTMLExport::lineBreak();
	sstrInfo += std::string("learn rate end: ")+Helpers::numberToString( m_params.lRateEnd )+HTMLExport::lineBreak();
	sstrInfo += std::string("radius begin: ")+Helpers::numberToString( m_params.radiusBegin )+HTMLExport::lineBreak();
	sstrInfo += std::string("radius end: ")+Helpers::numberToString( m_params.radiusEnd )+HTMLExport::lineBreak();
	sstrInfo += std::string("spectrum size in bytes: ")+Helpers::numberToString( sizeof(Spectra) )+HTMLExport::lineBreak();
	sstrInfo += std::string("UMatrix:")+HTMLExport::lineBreak()+HTMLExport::image( sstrUMatrix+std::string(".png") )+HTMLExport::lineBreak();
	sstrInfo += std::string("Difference map:")+HTMLExport::lineBreak()+HTMLExport::image( sstrDifferenceMap+std::string(".png") )+HTMLExport::lineBreak();
	sstrInfo += std::string("Energy histogram:")+HTMLExport::lineBreak()+HTMLExport::image( std::string("energymap.png") )+HTMLExport::lineBreak();
	sstrInfo += std::string("Z histogram:")+HTMLExport::lineBreak()+HTMLExport::image( std::string("zmap.png") )+HTMLExport::lineBreak();

	Helpers::insertString( HTMLExport::HTML_TOKEN_INFO, sstrInfo, sstrMainHTMLDoc );
	Helpers::insertString( HTMLExport::HTML_TOKEN_TITLE, "", sstrMainHTMLDoc );
	const size_t OutputPlanSizeTemp = (m_params.exportSubPage) ? s_outputPlanSize : m_gridSize;

	size_t planXMax = 1 + m_gridSize / OutputPlanSizeTemp;
	size_t planYMax = 1 + m_gridSize / OutputPlanSizeTemp;
	if (!m_params.exportSubPage)
	{
		planXMax = 1;
		planYMax = 1;
	}
	for ( size_t planY = 0; planY<planYMax; planY++ )
	{
		sstrMainTable += HTMLExport::beginTableRow();
		for ( size_t planX = 0; planX<planXMax; planX++ )
		{
			sstrHTMLDoc = sstrHTMLDocTemplate;
			{
				std::string sstrTitle("Planquadrat: ");
				sstrTitle += Helpers::numberToString( planX );
				sstrTitle += " / ";
				sstrTitle += Helpers::numberToString( planY );

				Helpers::insertString( HTMLExport::HTML_TOKEN_INFO, "", sstrHTMLDoc );
				Helpers::insertString( HTMLExport::HTML_TOKEN_TITLE, sstrTitle, sstrHTMLDoc );
			}

			// generate table
			std::string sstrTable;
			size_t c = 0;

			// let planquadrate overlap
			size_t xStart = planX;
			size_t xS = 3;
			if ( planX > 0 ) {
				xStart--;
			} else {
				xS = 2;
			}
			xStart *= OutputPlanSizeTemp;
			size_t yStart = planY;
			size_t yS = 3;
			if ( planY > 0 ) {
				yStart--;
			} else {
				yS = 2;
			}
			yStart *= OutputPlanSizeTemp;
			size_t xEnd = MIN( xStart+OutputPlanSizeTemp*xS, m_gridSize );
			size_t yEnd = MIN( yStart+OutputPlanSizeTemp*yS, m_gridSize );

			bool bOut = false;

			std::string sstrLastImageInPlan("empty");

			for ( size_t y=yStart;y<yEnd;y++)
			{
				sstrTable += HTMLExport::beginTableRow();
				for ( size_t x=xStart;x<xEnd;x++)
				{
					bOut = true;

					size_t nIndex = getIndex(x,y);
					Spectra *sp = m_pNet->beginRead( nIndex );

					sstrTable += HTMLExport::beginTableCell();
					// insert link
					if ( sp->m_Index>=0 && !sp->isEmpty() )
					{
						assert(sp->m_Index<static_cast<int>(m_numSpectra));
						std::string sstrPlanDirectory = Spectra::plateToString(sp->getPlate()) + std::string("/");
						std::string sstrImagePlan = sstrPlanDirectory + sp->getFileName();

						if ( _fullExport )
						{
							sstrTable += HTMLExport::imageLink( sstrImagePlan + std::string(".png"), sstrImagePlan+".html" );
						}
						else
						{
							sstrTable += HTMLExport::imageLink( sstrImagePlan + std::string(".png"), sp->getURL() );
						}

						if (( (x>=(xStart+OutputPlanSizeTemp) && x<(xEnd-OutputPlanSizeTemp)) || 
							  (xS==2 && x<OutputPlanSizeTemp) ||
							  ((xEnd-xStart)<=OutputPlanSizeTemp*2 && xEnd==m_gridSize )) &&
							( (y>=(yStart+OutputPlanSizeTemp) && y<(yEnd-OutputPlanSizeTemp)) || 
							  (yS==2 && y<OutputPlanSizeTemp) ||
							  ((yEnd-yStart)<=OutputPlanSizeTemp*2 && yEnd==m_gridSize )) )
						{
							sstrLastImageInPlan = sstrImagePlan;
						}
					}
					else
					{
						// insert image
						sstrTable += HTMLExport::image( "empty.png" );
					}

					m_pNet->endRead( nIndex );

					sstrTable += HTMLExport::endTableCell();
					c++;
				}
				sstrTable += HTMLExport::endTableRow();
			}

			if ( bOut )
			{
				Helpers::insertString( HTMLExport::HTML_TOKEN_TEMPLATE, sstrTable, sstrHTMLDoc );

				std::string sstrFilename = _sstrFilename;
				sstrFilename += Helpers::numberToString( planX );
				sstrFilename += "_";
				sstrFilename += Helpers::numberToString( planY );
				sstrFilename += ".html";

				std::ofstream fon(sstrFilename.c_str());
				fon<<sstrHTMLDoc;

				sstrMainTable += HTMLExport::beginTableCell();
				sstrMainTable += HTMLExport::imageLink( sstrLastImageInPlan+".png", FileHelpers::getFileName(sstrFilename) );
				sstrMainTable += HTMLExport::endTableCell();
			}
		}
		sstrMainTable += HTMLExport::endTableRow();
	}

	if (!Helpers::insertString( HTMLExport::HTML_TOKEN_TEMPLATE, sstrMainTable, sstrMainHTMLDoc ) )
	{
		Helpers::print( std::string("export failed. Wrong template.html ?!?\n"), &m_logFile );
		return;
	}

	std::ofstream fon(std::string(_sstrFilename+".html").c_str());
	fon<<sstrMainHTMLDoc;

	writeSettings("settings.xml");

	if ( m_params.waitForUser )
	{
		Helpers::print( std::string("export finished. waiting for user input.\n"), &m_logFile );
		_getch();
	}
}



size_t SOFMNetwork::getIndex( size_t _cellX, size_t _cellY )
{
	return _cellX+_cellY*m_gridSize;
}