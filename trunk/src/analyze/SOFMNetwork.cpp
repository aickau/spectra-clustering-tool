#include "analyze/SOFMNetwork.h"

#include <conio.h>
#include <math.h>
#include <float.h>
#include <assert.h>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <omp.h>

#include "sdsslib/random.h"
#include "sdsslib/defines.h"
#include "sdsslib/helpers.h"
#include "sdsslib/Timer.h"
#include "sdsslib/spectraVFS.h"
#include "sdsslib/spectraHelpers.h"
#include "sdsslib/mathhelpers.h"
#include "sdsslib/XMLExport.h"
#include "sdsslib/XMLParser.h"




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
	if ( !ReadSettings("settings.xml", sstrSOFMFileName) )
	{
		//error
		Helpers::Print( std::string("Error reading settings from settings.xml. Abortion.\n"), &m_logFile );
		exit(0);
	}

	// test begin
	//for ( size_t i=0;i<m_numSpectra;i++ )
	//{
	//	Spectra *a = m_pSourceVFS->beginWrite( i );

	//	a->set( .1f + (float) i*0.001f );
	//
	//	m_pSourceVFS->endWrite( i );
	//}
	// test end


	CalcMinMaxInputDS();


	if ( !bContinueComputation )
	{
		size_t gridSizeMin = static_cast<size_t>(ceilf(sqrtf(m_numSpectra+1))*1.2f);
		if ( m_gridSize < gridSizeMin )
		{
			std::string sstrString( std::string("Grid size ") );
			sstrString += Helpers::numberToString(m_gridSize);
			sstrString += std::string("  too small. Clamping to ");
			sstrString += Helpers::numberToString(gridSizeMin);
			sstrString += std::string(".\n\n");
			Helpers::Print(  sstrString, &m_logFile );
			m_gridSize = gridSizeMin;
		}
		m_gridSizeSqr = m_gridSize*m_gridSize;

		// generate random filled cluster and load it.
		std::string sstrNet("sofmnet.bin");
		SpectraVFS::write( m_gridSize, m_Min, m_Max*0.01f, sstrNet );
		m_pNet = new SpectraVFS( sstrNet, false );
		Reset(m_params);

		//ExportEnergyMap();
		RenderIcons();
	}
	else
	{
		m_pNet = new SpectraVFS( sstrSOFMFileName, false );

		m_gridSizeSqr = m_pNet->getNumSpectra();

		if ( m_gridSizeSqr == 0 || 
			m_gridSize*m_gridSize != m_gridSizeSqr )
		{
			// error
			Helpers::Print( std::string("Error reading ") + sstrSOFMFileName + std::string(". Abortion.\n"), &m_logFile );
			exit(0);
		}
	}
}


SOFMNetwork::~SOFMNetwork()
{
	delete m_pNet;
}


void SOFMNetwork::WriteSettings( const std::string &_sstrFileName )
{
	std::string sstrXML("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");

	XMLExport::xmlElementBegin( "SETTINGS", 0, sstrXML );
	XMLExport::xmlElementEndBegin( sstrXML );

	XMLExport::xmlSingleElementBegin( "STEP", 1, sstrXML );
	XMLExport::xmlAddAttribute( "current", m_currentStep, sstrXML );
	XMLExport::xmlAddAttribute( "total", m_params.numSteps, sstrXML );
	XMLExport::xmlSingleElementEnd( sstrXML );

	XMLExport::xmlSingleElementBegin( "GRIDSIZE", 1, sstrXML );
	XMLExport::xmlAddAttribute( "value", m_gridSize, sstrXML );
	XMLExport::xmlSingleElementEnd( sstrXML );

	XMLExport::xmlSingleElementBegin( "RANDOMSEED", 1, sstrXML );
	XMLExport::xmlAddAttribute( "value", m_params.randomSeed, sstrXML );
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

bool SOFMNetwork::ReadSettings( const std::string &_sstrFileName, std::string &_sstrSOFMFileName )
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
		Helpers::Print( std::string("Error: some setting value could not loaded.\n"), &m_logFile );


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


void SOFMNetwork::CalcMinMax( SpectraVFS &_vfs, float &_outMin, float &_outMax )
{
	_outMin = FLT_MAX;
	_outMax = -FLT_MAX;

	// calc min/max
	for ( size_t i=0;i<m_numSpectra;i++ )
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


void SOFMNetwork::CalcMinMaxInputDS()
{
	Helpers::Print( std::string("Calculating min/max of input.\n"), &m_logFile );
	CalcMinMax( *m_pSourceVFS, m_Min, m_Max );
	Helpers::Print( std::string("global min / max: ") + Helpers::numberToString(m_Min) + std::string(" / " ) + Helpers::numberToString(m_Max) + std::string("\n" ), &m_logFile );
}



void SOFMNetwork::ExportEnergyMap()
{
	Helpers::Print( std::string("Exporting energy map.\n"), &m_logFile );
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

	SpectraHelpers::RenderDiagramToDisk(&energymap[0], energymap.size(), 4, 0, 1200, 800, std::string("energymap.png") );
	SpectraHelpers::RenderDiagramToDisk(&toatalenergymap[0], toatalenergymap.size(), 4, 0, 1200, 800, std::string("toatalenergymap.png") );
	SpectraHelpers::RenderDiagramToDisk(&zmap[0], zmap.size(), 4, 0, 1200, 800, std::string("zmap.png") );
}


void SOFMNetwork::RenderIcons()
{
	Helpers::Print( std::string("Rendering Icons.\n"), &m_logFile );

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

		SpectraHelpers::RenderSpectraIconToDisk(*a, sstrFilename, 100, 100, localmax, redness );

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



void SOFMNetwork::Reset( const Parameters &_params )
{
	m_params = _params;
	m_currentStep = 0;

	m_Random.initRandom( m_params.randomSeed );
}

 

void SOFMNetwork::Process()
{
	if ( m_currentStep > m_params.numSteps )
	{
		Helpers::Print( std::string("Clustering finished (success).\n"), &m_logFile );
		exit(1);
	}

	if ( m_currentStep == m_params.numSteps )
	{
		Export("export/full");
	} else
	if ( m_currentStep == m_params.numSteps/2 )
	{
		Export("export/half");
	} else
	if ( m_currentStep == m_params.numSteps/4 )
	{
		Export("export/quarter");
	} else
	if ( m_currentStep == 1 )
	{
		Export("export/first");
	} else
	{
		Export("export/current");
	}

	std::string sstrLog("Calculating step ");
	sstrLog += Helpers::numberToString( m_currentStep ) + " / " + Helpers::numberToString( m_params.numSteps ) + "\n";
	Helpers::Print( sstrLog, &m_logFile );
	Helpers::Print( std::string("initializing.\n"), &m_logFile );

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
//		Helpers::Print( std::string("."), &m_logFile );

		// initialize best match batch
		BestMatch bestMatch[SpectraVFS::CACHELINESIZE];
		for ( int k=0;k<jInc;k++)
		{
			bestMatch[k].error = FLT_MAX;
			bestMatch[k].index = 0;
		}

		// retrieve best match neuron for a cache line batch of source spectra
		Timer t;
		for ( size_t i = 0;i < m_gridSizeSqr;i++)
		{
			Spectra *a = m_pNet->beginRead( i );

			Spectra *src[SpectraVFS::CACHELINESIZE];
			for ( int k=0;k<jInc;k++)
			{
				const size_t spectraIndex = spectraIndexList.at(j+k);
				src[k] = m_pSourceVFS->beginRead(spectraIndex);
			}


			#pragma omp parallel for
			for ( int k=0;k<jInc;k++)
			{
				BestMatch &currentBestMatch = bestMatch[k];
				Spectra &currentSpectra = *src[k];

				const float tmin = a->compare( currentSpectra );

				if (tmin < currentBestMatch.error && a->isEmpty() )
				{
					currentBestMatch.error = tmin;
					currentBestMatch.index = i;
				}
			}

			for ( int k=0;k<jInc;k++)
			{
				const size_t spectraIndex = spectraIndexList.at(j+k);
				m_pSourceVFS->endRead(spectraIndex);
			}

			m_pNet->endRead( i );
		}


		double search_t = t.GetElapsedSecs();
		_cprintf( "search time: %f\n", search_t ); 
		t.Start();

		for ( int k=0;k<jInc;k++)
		{
			BestMatch &currentBestMatch = bestMatch[k];
			const size_t spectraIndex = spectraIndexList.at(j+k);
			Spectra &currentSpectra = *m_pSourceVFS->beginRead(spectraIndex);

			Spectra *a = m_pNet->beginWrite( currentBestMatch.index );

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


		//	Helpers::Print( std::string(":"), &m_logFile );


			const float lPercent = static_cast<float>(m_currentStep)/static_cast<float>(m_params.numSteps);
			const float lrate = m_params.lRateBegin*pow(m_params.lRateEnd/m_params.lRateBegin,lPercent);
			const float lratehsxTreshhold = m_params.lRateEnd*0.01f;
			const float sigma = m_params.radiusBegin*pow(m_params.radiusEnd/m_params.radiusBegin,lPercent);
			const float tsigma = sigma*sigma;
			const float tsigma2 = tsigma*2.f;

			const size_t xpBestMatch = currentBestMatch.index % m_gridSize;
			const size_t ypBestMatch = currentBestMatch.index / m_gridSize;

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
					const float mexican_hat_term = 1.f-tdistall/tsigma;
					const float hxy = exp(-(tdistall)/tsigma2);
					const float lratehsx = lrate*hxy;

					if ( lratehsx > lratehsxTreshhold )
					{
						Spectra *a = m_pNet->beginWrite( c );
						for ( size_t i=0;i<Spectra::numSamples;i++ )
						{
							a->m_Amplitude[i] += lratehsx*(currentSpectra.m_Amplitude[i]-a->m_Amplitude[i]);
						}
						m_pNet->endWrite( c );
					}
					c++;
				}
			}

			m_pSourceVFS->endRead(spectraIndex);
		}
		double calc_t = t.GetElapsedSecs();
		_cprintf( "calc time %f\n", calc_t ); 

		j += jInc;
	}

//	Helpers::Print( std::string("="), &m_logFile );

	Timer t;
	// collision handling
	// for each collision spectra..
	for ( size_t j=0;j<spectraCollisionList.size();j++)
	{
		const size_t spectraIndex = spectraCollisionList.at(j);
		Spectra &currentSpectra = *m_pSourceVFS->beginRead(spectraIndex);

		// retrieve first best match neuron
		float min = FLT_MAX;
		size_t bestMatch = 0;
		
		for ( size_t i = 0;i < m_gridSizeSqr;i++)
		{
			Spectra *a = m_pNet->beginRead( i );
			float tmin =a->compare( currentSpectra );

			if ( tmin < min && a->isEmpty() )
			{
				min = tmin;
				bestMatch = i;
			}
			m_pNet->endRead( i );
		}

//		Helpers::Print( std::string(":"), &m_logFile );

		Spectra *a = m_pNet->beginWrite( bestMatch );
		a->m_SpecObjID = currentSpectra.m_SpecObjID;
		a->m_Index = spectraIndex;
		m_pNet->endWrite( bestMatch );

		const float lPercent = static_cast<float>(m_currentStep)/static_cast<float>(m_params.numSteps);
		const float lrate = m_params.lRateBegin*pow(m_params.lRateEnd/m_params.lRateBegin,lPercent);
		const float lratehsxTreshhold = m_params.lRateEnd*0.01f;
		const float sigma = m_params.radiusBegin*pow(m_params.radiusEnd/m_params.radiusBegin,lPercent);
		const float tsigma = sigma*sigma;
		const float tsigma2 = tsigma*2.f;

		const size_t xpBestMatch = bestMatch % m_gridSize;
		const size_t ypBestMatch = bestMatch / m_gridSize;

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
				const float mexican_hat_term = 1.f-tdistall/tsigma;
				const float hxy = exp(-(tdistall)/tsigma2);
				const float lratehsx = lrate*hxy;

				if ( lratehsx > lratehsxTreshhold )
				{
					Spectra *a = m_pNet->beginWrite( c );
					for ( size_t i=0;i<Spectra::numSamples;i++ )
					{
						a->m_Amplitude[i] += lratehsx*(currentSpectra.m_Amplitude[i]-a->m_Amplitude[i]);
					}
					m_pNet->endWrite( c );
				}
				c++;
			}
		}

		m_pSourceVFS->endRead(spectraIndex);
	//	Helpers::Print( std::string("."), &m_logFile );
	}


	double collision_t = t.GetElapsedSecs();
	_cprintf( "collision calc time %f\n", collision_t ); 


	Helpers::Print( std::string("\nFlushing cluster table to disk.\n"), &m_logFile );
	m_pNet->flush();

	m_currentStep++;
}



void SOFMNetwork::Export( const std::string &_sstrFilename )
{
	std::string sstrLog;
	sstrLog = "Exporting results to ";
	sstrLog += _sstrFilename;
	sstrLog +="\n";

	Helpers::Print( sstrLog, &m_logFile );

	// load template
	std::ifstream fin("template.html");
	if( !fin ) 
	{
		Helpers::Print( std::string("export failed. Missing template.html\n"), &m_logFile );
		return;
	}

	std::string sstrTemp;
	std::string sstrHTMLDoc;
	std::string sstrMainHTMLDoc;
	std::string sstrHTMLDocTemplate;
	std::string sstrMainTable;

	while( getline(fin,sstrTemp) ) 
	{
		sstrHTMLDocTemplate += sstrTemp;
	}

	sstrMainHTMLDoc = sstrHTMLDocTemplate;

	std::string sstrInfo;
	
	sstrInfo += std::string("step ")+Helpers::numberToString( m_currentStep )+std::string(" / ")+Helpers::numberToString( m_params.numSteps )+std::string("<br>\n");
	sstrInfo += std::string("gridsize ")+Helpers::numberToString( m_gridSize )+std::string("<br>\n");;
	sstrInfo += std::string("number of clustered spectra ")+Helpers::numberToString( m_numSpectra )+std::string("<br>\n");
	sstrInfo += std::string("random seed ")+Helpers::numberToString( m_params.randomSeed )+std::string("<br>\n");
	sstrInfo += std::string("learn rate begin ")+Helpers::numberToString( m_params.lRateBegin )+std::string("<br>\n");
	sstrInfo += std::string("learn rate end ")+Helpers::numberToString( m_params.lRateEnd )+std::string("<br>\n");
	sstrInfo += std::string("radius begin ")+Helpers::numberToString( m_params.radiusBegin )+std::string("<br>\n");
	sstrInfo += std::string("radius end ")+Helpers::numberToString( m_params.radiusEnd )+std::string("<br>\n");
	sstrInfo += std::string("spectrum size in bytes ")+Helpers::numberToString( sizeof(Spectra) )+std::string("<br>\n");

	Helpers::insertString( std::string("*INFO*"), sstrInfo, sstrMainHTMLDoc );
	const size_t OutputPlanSizeTemp = (m_params.exportSubPage) ? OutputPlanSize : m_gridSize;

	size_t planXMax = 1 + m_gridSize / OutputPlanSizeTemp;
	size_t planYMax = 1 + m_gridSize / OutputPlanSizeTemp;
	if (!m_params.exportSubPage)
	{
		planXMax = 1;
		planYMax = 1;
	}
	for ( size_t planY = 0; planY<planYMax; planY++ )
	{
		sstrMainTable += "<tr>\n";
		for ( size_t planX = 0; planX<planXMax; planX++ )
		{
			sstrHTMLDoc = sstrHTMLDocTemplate;
			{
				std::string sstrTag("*TITLE*");
				size_t insertpos = sstrHTMLDoc.find(sstrTag);
				if (insertpos == std::string::npos )
				{
					Helpers::Print( std::string("export failed. Wrong template.html ?!?\n"), &m_logFile );
					return;
				}
				std::string sstrTitle("Planquadrat: ");
				sstrTitle += Helpers::numberToString( planX );
				sstrTitle += " / ";
				sstrTitle += Helpers::numberToString( planY );
				sstrHTMLDoc = sstrHTMLDoc.erase(insertpos, sstrTag.size());
				sstrHTMLDoc.insert(insertpos, sstrTitle );
			}


			// insertion pos for table
			std::string sstrTag("*TEMPLATE*");
			size_t insertpos = sstrHTMLDoc.find(sstrTag);
			sstrHTMLDoc = sstrHTMLDoc.erase(insertpos, sstrTag.size());

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
				sstrTable += "<tr>\n";
				for ( size_t x=xStart;x<xEnd;x++)
				{
					bOut = true;

					size_t nIndex = getIndex(x,y);
					Spectra *sp = m_pNet->beginRead( nIndex );

					sstrTable += "<td>";
					// insert link
					if ( sp->m_Index>=0 && !sp->isEmpty() )
					{
						assert(sp->m_Index<m_numSpectra);
						sstrTable += "<a href=\"";
						sstrTable += sp->getURL();
						sstrTable += "\" target=\"_blank\">";
						std::string sstrImagePlan = Spectra::plateToString(sp->getPlate()) + std::string("/") + sp->getFileName();
						sstrTable += "<img src=\"";
						sstrTable += sstrImagePlan;
						sstrTable += ".png\"></td>";

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
						sstrTable += "<img src=\"empty.png\"></td>";
					}

					m_pNet->endRead( nIndex );

					sstrTable += "</td>\n";
					c++;
				}
				sstrTable += "</tr>\n";
			}

			if ( bOut )
			{
				sstrHTMLDoc.insert(insertpos, sstrTable);

				std::string sstrFilename = _sstrFilename;
				sstrFilename += Helpers::numberToString( planX );
				sstrFilename += "_";
				sstrFilename += Helpers::numberToString( planY );
				sstrFilename += ".html";

				std::ofstream fon(sstrFilename.c_str());
				fon<<sstrHTMLDoc;

				sstrMainTable += "<td>";
				sstrMainTable += "<a href=\"";
				sstrMainTable += Helpers::getFileName(sstrFilename);
				sstrMainTable += "\" target=\"_blank\">";
				sstrMainTable += "<img src=\"";
				sstrMainTable += sstrLastImageInPlan;
				sstrMainTable += ".png\"></td>";
				sstrMainTable += "</td>\n";
			}
		}
		sstrMainTable += "</tr>\n";
	}

	if (!Helpers::insertString( std::string("*TEMPLATE*"), sstrMainTable, sstrMainHTMLDoc ) )
	{
		Helpers::Print( std::string("export failed. Wrong template.html ?!?\n"), &m_logFile );
		return;
	}

	std::ofstream fon(std::string(_sstrFilename+".html").c_str());
	fon<<sstrMainHTMLDoc;

	WriteSettings("settings.xml");

	if ( m_params.waitForUser )
	{
		Helpers::Print( std::string("export finished. waiting for user input.\n"), &m_logFile );
		_getch();
	}
}



size_t SOFMNetwork::getIndex( size_t _cellX, size_t _cellY )
{
	return _cellX+_cellY*m_gridSize;
}