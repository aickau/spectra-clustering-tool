//! \verbatim
//! ###########################################################################
//! # ASPECT: A spectra clustering tool for exploration of large spectral surveys - Stage I
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
#include "sdsslib/sdssSoftwareVersion.h"

#include "analyze/SOFMNetworkSettings.h"
#include "analyze/ComputeCUDA.h"

#include <conio.h>
#include <math.h>
#include <float.h>
#include <assert.h>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <map>
#include <omp.h>

//#define SDSS_SINETEST

static 
void setBestMatch( Spectra &_networkSpectrum, size_t _networkIndex, Spectra &_bestMatchSpectrum, size_t _bestMatchIndex )
{
	assert( _networkSpectrum.isEmpty() );
	// set best matching related info.
	_networkSpectrum.m_SpecObjID	= _bestMatchSpectrum.m_SpecObjID;
	_networkSpectrum.m_Index		= _bestMatchIndex;
	_networkSpectrum.m_version		= _bestMatchSpectrum.m_version;

	// remember best match position to NW for faster search
	_bestMatchSpectrum.m_Index = _networkIndex;

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
,sstrSearchMode(SOFMNET_SETTINGS_SEARCHMODE_global)
,normaliziationType(Spectra::SN_FLUX)
,useBOSSWavelengthRange(false)
{
}


void SOFMNetwork::BestMatch::reset()
{
	error = FLT_MAX;
	index = 0;
}


std::string SOFMNetwork::spectraNormalizationToString( Spectra::SpectraNormalization _type )
{
	switch(_type)
	{
	case Spectra::SN_AMPLITUDE : return SOFMNET_SETTINGS_NORMALIZATION_amplitude;
	case Spectra::SN_FLUX : return SOFMNET_SETTINGS_NORMALIZATION_flux;
	default:;
	}
	return SOFMNET_SETTINGS_NORMALIZATION_none;
}


Spectra::SpectraNormalization SOFMNetwork::spectraNormalizationFromString( const std::string &_sstrSpectraNormalization )
{
	std::string sstrSpectraNormalization( Helpers::lowerCase( _sstrSpectraNormalization ) );
	if ( sstrSpectraNormalization == SOFMNET_SETTINGS_NORMALIZATION_amplitude ) {
		return Spectra::SN_AMPLITUDE;
	}
	else if ( sstrSpectraNormalization == SOFMNET_SETTINGS_NORMALIZATION_flux ) {
		return Spectra::SN_FLUX;
	} 

	return Spectra::SN_NONE;
}




SOFMNetwork::SOFMNetwork( SpectraVFS *_pSourceVFS, bool bContinueComputation, std::ofstream *_logStream )
:m_pSourceVFS(_pSourceVFS)
,m_numSpectra(_pSourceVFS->getNumSpectra())
,m_gridSize(0)
,m_gridSizeSqr(0)
,m_currentStep(0)
,m_params( Parameters::defaultParameters )
,m_Min(FLT_MAX)
,m_Max(-FLT_MAX)
,m_pLogStream(_logStream)
,m_pAvgDistanceToBMU(NULL)
,m_pCUDA(NULL)
{
	std::string sstrSOFMFileName("");
	if ( !readSettings("settings.xml", sstrSOFMFileName) )
	{
		//error
		Helpers::print( std::string("Error reading settings from settings.xml. Abortion.\n"), m_pLogStream );
		exit(0);
	}

	Spectra::setOperationRange( m_params.useBOSSWavelengthRange );

#ifdef SDSS_SINETEST
	const float freqMin = 0.002f;
	const float freqMax = 0.05f;
	const float freqStepSize = (freqMax-freqMin)/static_cast<float>(m_numSpectra);
	float freq = freqMin;

	for (size_t i=0;i<m_numSpectra;i++)
	{
		Spectra *a = _pSourceVFS->beginRead(i);
		a->setSine(freq);
		a->m_Z = static_cast<float>(i)/static_cast<float>(m_numSpectra);
		freq += freqStepSize;
		_pSourceVFS->endRead(i);
	}
#endif // SDSS_SINETEST

	calcFluxAndNormalizeInputDS( m_params.normaliziationType );
	calcMinMaxInputDS();

	if ( !bContinueComputation )
	{
		//
		// start new computation 
		//
		m_gridSize = static_cast<size_t>(ceilf(sqrtf((float)m_numSpectra+1))*1.1f); // gives a factor of 1.1^2=1.21 in total
		m_gridSizeSqr = m_gridSize*m_gridSize;

		Helpers::print( std::string("Start clustering using ")+Helpers::numberToString(m_numSpectra)+
			            std::string(" spectra. Grid size is ")+Helpers::numberToString(m_gridSize)+std::string(".\n"), m_pLogStream );

		// generate random filled cluster and load it.
		std::string sstrNet("sofmnet.bin");
		SpectraVFS::write( m_gridSize, m_Min, m_Max*0.01f, sstrNet );
		m_pNet = new SpectraVFS( sstrNet, false );
		reset(m_params);

		renderIcons();
		exportHistograms("export/");

		// initialize with input data
		Helpers::print( std::string("Initializing network with input data.\n"), m_pLogStream );

		// for p:
		SSE_ALIGN Spectra multiplier;
		bool bMult = multiplier.loadFromCSV("multiplier.csv");
		if ( bMult ) 
		{
			Helpers::print( std::string("Multiplying input data with multiplier.csv.\n"), m_pLogStream );
		}

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
				if ( bMult )
				{
					b->multiply(multiplier);
				}
				a->set( *b );
				m_pSourceVFS->endRead( spectraIndex );
				m_pNet->endWrite( i );
			}
		}

	}
	else
	{
		//
		// continue old computation 
		//

		m_pNet = new SpectraVFS( sstrSOFMFileName, false );

		m_gridSizeSqr = m_pNet->getNumSpectra();
		m_gridSize = sqrtf(m_gridSizeSqr);

		Helpers::print( std::string("Continue clustering at step ")+Helpers::numberToString(m_currentStep)+
						std::string(" using ")+Helpers::numberToString(m_numSpectra)+
						std::string(" spectra. Grid size is ")+Helpers::numberToString(m_gridSize)+std::string(".\n"), m_pLogStream );

		if ( m_gridSizeSqr == 0 )
		{
			// error
			Helpers::print( std::string("Error reading ") + sstrSOFMFileName + std::string(". Abortion.\n"), m_pLogStream );
			exit(0);
		}
	}

	m_pAvgDistanceToBMU = new float[m_params.numSteps];
	for ( size_t i=0;i<m_params.numSteps;i++ )
	{
		m_pAvgDistanceToBMU[i] = 0.0;
	}

	// write som info to our log

	const size_t memUsageBytes = m_numSpectra*sizeof(Spectra)+m_gridSizeSqr*sizeof(Spectra);
	const double memUsageGB = static_cast<double>(memUsageBytes)/(1024.0*1024.0*1024.0);
	Helpers::print( std::string("We need ") + Helpers::numberToString( static_cast<float>(memUsageGB)) + std::string(" GB of main memory for clustering.\n"), m_pLogStream );
// 	Helpers::print( std::string("Spectra VFS cache line size ") + Helpers::numberToString( SpectraVFS::CACHELINESIZE ) + " spectra.\n", m_pLogStream );
// 	Helpers::print( std::string("Spectra VFS number of cache lines ") + Helpers::numberToString( SpectraVFS::CACHELINES ) + ".\n", m_pLogStream );
// 	Helpers::print( std::string("That allows us to pack ") + Helpers::numberToString( SpectraVFS::CACHELINES*SpectraVFS::CACHELINESIZE ) + " spectra into main memory.\n", m_pLogStream );
// 	Helpers::print( std::string("We can eat up ") + Helpers::numberToString( static_cast<float>(2*SpectraVFS::CACHELINES*SpectraVFS::CACHELINESIZE*sizeof(Spectra))/(1024.f*1024.f*1024.f) ) + " GB of main memory for clustering .\n", m_pLogStream );
	Helpers::print( std::string("We are using ") + m_params.sstrSearchMode  + " search.\n", m_pLogStream );
	Helpers::print( std::string("Spectra normalization is set to ") +spectraNormalizationToString(m_params.normaliziationType) + ".\n", m_pLogStream );
	if ( m_params.useBOSSWavelengthRange )
	{
		Helpers::print( std::string("Using extended wavelength range of 3650..10400 Angstrom for BOSS spectra.\n"), m_pLogStream );
	}
	else
	{
		Helpers::print( std::string("Using standard wavelength range of 3800..9200 Angstrom for SDSS spectra.\n"), m_pLogStream );
	}

	const bool useCUDA = false;
	if (useCUDA)
	{
		m_pCUDA = new ComputeCUDA(*m_pSourceVFS,*m_pNet );
		if (!m_pCUDA->hasCUDADevice())
		{
			delete m_pCUDA;
			m_pCUDA = NULL;
		}
		else
		{
			bool success = m_pCUDA->uploadSpectra();
			if ( !success )
			{
				delete m_pCUDA;
				m_pCUDA = NULL;
			}
		}
	}


	Helpers::print( std::string("Initialization finished.\n"), m_pLogStream );
}


SOFMNetwork::~SOFMNetwork()
{
	delete m_pNet;
	delete[] m_pAvgDistanceToBMU;
}


void SOFMNetwork::writeSettings( const std::string &_sstrFileName )
{
	std::string sstrXML;
	XMLExport::xmlHeader(sstrXML);

	XMLExport::xmlSingleComment("Analyzer config file, ASPECT: A spectra clustering tool for exploration of large spectral surveys - Stage I, Copyright (c) 2009 "+sstrSDSSVersionString, sstrXML);

	XMLExport::xmlElementBegin( SOFMNET_SETTINGS, 0, sstrXML );
	XMLExport::xmlElementEndBegin( sstrXML );

	XMLExport::xmlSingleElementBegin( SOFMNET_SETTINGS_STEP, 1, sstrXML );
	XMLExport::xmlAddAttribute( "current", static_cast<unsigned int>(m_currentStep), sstrXML );
	XMLExport::xmlAddAttribute( "total", static_cast<unsigned int>(m_params.numSteps), sstrXML );
	XMLExport::xmlSingleElementEnd( sstrXML );

	XMLExport::xmlSingleElementBegin( SOFMNET_SETTINGS_GRIDSIZE, 1, sstrXML );
	XMLExport::xmlAddAttribute( "value", static_cast<unsigned int>(m_gridSize), sstrXML );
	XMLExport::xmlSingleElementEnd( sstrXML );

	XMLExport::xmlSingleElementBegin( SOFMNET_SETTINGS_RANDOMSEED, 1, sstrXML );
	XMLExport::xmlAddAttribute( "value", static_cast<unsigned int>(m_params.randomSeed), sstrXML );
	XMLExport::xmlSingleElementEnd( sstrXML );

	XMLExport::xmlSingleElementBegin( SOFMNET_SETTINGS_LEARNRATE, 1, sstrXML );
	XMLExport::xmlAddAttribute( "begin", m_params.lRateBegin, sstrXML );
	XMLExport::xmlAddAttribute( "end", m_params.lRateEnd, sstrXML );
	XMLExport::xmlSingleElementEnd( sstrXML );

	XMLExport::xmlSingleElementBegin( SOFMNET_SETTINGS_RADIUS, 1, sstrXML );
	XMLExport::xmlAddAttribute( "begin", m_params.radiusBegin, sstrXML );
	XMLExport::xmlAddAttribute( "end", m_params.radiusEnd, sstrXML );
	XMLExport::xmlSingleElementEnd( sstrXML );

	XMLExport::xmlSingleElementBegin( SOFMNET_SETTINGS_SPECTRUM, 1, sstrXML );
	XMLExport::xmlAddAttribute( "file", m_pNet->getFileName(), sstrXML );
	XMLExport::xmlSingleElementEnd( sstrXML );

	XMLExport::xmlElementBegin( SOFMNET_SETTINGS_EXPORT, 1, sstrXML );
	XMLExport::xmlElementEndBegin( sstrXML );

		XMLExport::xmlSingleElementBegin( SOFMNET_SETTINGS_EXPORT_SUBPAGES, 2, sstrXML );
		XMLExport::xmlAddAttribute( "value", (int)m_params.exportSubPage, sstrXML );
		XMLExport::xmlSingleElementEnd( sstrXML );

		XMLExport::xmlSingleElementBegin( SOFMNET_SETTINGS_EXPORT_WAITFORUSER, 2, sstrXML );
		XMLExport::xmlAddAttribute( "value", (int)m_params.waitForUser, sstrXML );
		XMLExport::xmlSingleElementEnd( sstrXML );

	XMLExport::xmlElementEnd( SOFMNET_SETTINGS_EXPORT, 1, sstrXML );
	
	XMLExport::xmlSingleElementBegin( SOFMNET_SETTINGS_SEARCHMODE, 1, sstrXML );
	XMLExport::xmlAddAttribute( "value", m_params.sstrSearchMode, sstrXML );
	XMLExport::xmlSingleElementEnd( sstrXML );
	XMLExport::xmlSingleComment("localfast / local / global", sstrXML);

	XMLExport::xmlSingleElementBegin( SOFMNET_SETTINGS_NORMALIZATION, 1, sstrXML );
	XMLExport::xmlAddAttribute( "value", spectraNormalizationToString(m_params.normaliziationType), sstrXML );
	XMLExport::xmlSingleElementEnd( sstrXML );
	XMLExport::xmlSingleComment("amplitude / flux / none", sstrXML);

	XMLExport::xmlSingleElementBegin( SOFMNET_SETTINGS_WAVELENGTHRANGE, 1, sstrXML );
	XMLExport::xmlAddAttribute( "value", (m_params.useBOSSWavelengthRange) ? SOFMNET_SETTINGS_WAVELENGTHRANGE_boss : SOFMNET_SETTINGS_WAVELENGTHRANGE_sdss, sstrXML );
	XMLExport::xmlSingleElementEnd( sstrXML );
	XMLExport::xmlSingleComment("sdss (3800..9200) / boss (3650..10400)", sstrXML);

		
	XMLExport::xmlElementEnd( "SETTINGS", 0, sstrXML );

	std::ofstream fon(_sstrFileName.c_str());
	fon<<sstrXML;
}

bool SOFMNetwork::readSettings( const std::string &_sstrFileName, std::string &_sstrSOFMFileName )
{
	XMLParser p;

	//what we read:
	//	<STEP current="1" total="100"/>
	//	<GRIDSIZE value="2"/>
	//	<RANDOMSEED value="15"/>
	//	<LEARNRATE begin="0.25" end="0.01"/>
	//	<RADIUS begin="1" end="0.5"/>
	//	<SPECTRUM file="sofmnet.bin"/>
	//	<EXPORT>
	//		<SUBPAGES value="0"/>
	//		<WAITFORUSER value="0"/>
	//	</EXPORT>
	//	<SEARCHMODE value="local"> 		<!-- localfast, local, global -->
	//	<NORMALIZATION value="flux">	<!-- none, amplitude, flux -->
	if (!p.loadXMLFromFile( _sstrFileName ))
	{
		return false;
	}

	bool bSuccess = true;
	size_t spectraSize = 0;

	std::string sstrSearchMode;
	std::string sstrNormalizationType;
	std::string sstrWavelengthRangeType;

	bSuccess &= p.getChildValue("STEP", "current", m_currentStep );
	bSuccess &= p.getChildValue(SOFMNET_SETTINGS_STEP, "total", m_params.numSteps );
//	bSuccess &= p.getChildValue(SOFMNET_SETTINGS_GRIDSIZE, "value", m_gridSize );
	bSuccess &= p.getChildValue(SOFMNET_SETTINGS_RANDOMSEED, "value", m_params.randomSeed );
	bSuccess &= p.getChildValue(SOFMNET_SETTINGS_LEARNRATE, "begin", m_params.lRateBegin );
	bSuccess &= p.getChildValue(SOFMNET_SETTINGS_LEARNRATE, "end", m_params.lRateEnd );
	bSuccess &= p.getChildValue(SOFMNET_SETTINGS_RADIUS, "begin", m_params.radiusBegin );
	bSuccess &= p.getChildValue(SOFMNET_SETTINGS_RADIUS, "end", m_params.radiusEnd );
//	bSuccess &= p.getChildValue(SOFMNET_SETTINGS_SPECTRUM, "size", spectraSize );
	bSuccess &= p.getChildValue(SOFMNET_SETTINGS_SPECTRUM, "file", _sstrSOFMFileName );
	bSuccess &= p.getChildValue(SOFMNET_SETTINGS_SEARCHMODE, "value", sstrSearchMode );
	bSuccess &= p.getChildValue(SOFMNET_SETTINGS_NORMALIZATION, "value", sstrNormalizationType );
	bSuccess &= p.getChildValue(SOFMNET_SETTINGS_WAVELENGTHRANGE, "value", sstrWavelengthRangeType );

	m_params.sstrSearchMode = Helpers::lowerCase( sstrSearchMode );
	m_params.normaliziationType = spectraNormalizationFromString( sstrNormalizationType );
	m_params.useBOSSWavelengthRange = ( Helpers::lowerCase(sstrWavelengthRangeType) == SOFMNET_SETTINGS_WAVELENGTHRANGE_boss);

	p.gotoChild();
	
	do
	{
		if (p.getCurrentTag()==SOFMNET_SETTINGS_EXPORT)
		{
			break;
		}
	} while (p.gotoSibling());

	size_t sp=0;
	bSuccess &= p.getChildValue(SOFMNET_SETTINGS_EXPORT_SUBPAGES, "value", sp );
	m_params.exportSubPage = (sp>0);
	bSuccess &= p.getChildValue(SOFMNET_SETTINGS_EXPORT_WAITFORUSER, "value", sp );
	m_params.waitForUser = (sp>0);

	if ( !bSuccess)
	{
		Helpers::print( std::string("Error: some setting value could not loaded.\n"), m_pLogStream );
		Helpers::print( p.getParserErrorLog() +"\n", m_pLogStream );
	}
	//bSuccess &= ( spectraSize == sizeof(Spectra) );
	//if ( !bSuccess)
	//	Helpers::Print( std::string("Error: Given spectrum size matches not with spectrum size from dump file.\n"), m_pLogStream );


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
	Helpers::print( std::string("Calculating min/max of input.\n"), m_pLogStream );
	calcMinMax( *m_pSourceVFS, m_Min, m_Max );
	Helpers::print( std::string("global min / max: ") + Helpers::numberToString(m_Min) + std::string(" / " ) + Helpers::numberToString(m_Max) + std::string("\n" ), m_pLogStream );
}

void SOFMNetwork::calcFluxAndNormalizeInputDS( Spectra::SpectraNormalization _normalizationType )
{
	Helpers::print( std::string("Calculating flux of input.\n"), m_pLogStream );
	
	// normalize input spectra and calculate overall flux
	if ( _normalizationType == Spectra::SN_FLUX )
	{
		Helpers::print( std::string("..and normalize by flux.\n"), m_pLogStream );
		m_flux = 0.0f;
		for ( size_t i=0;i<m_numSpectra;i++ )
		{
			Spectra *a = m_pSourceVFS->beginWrite( i );
			a->	normalizeByFlux();
			m_flux = MAX(a->m_flux, m_flux);
			m_pSourceVFS->endWrite( i );
		}
	}
	else if ( _normalizationType == Spectra::SN_AMPLITUDE )
	{
		Helpers::print( std::string("..and normalize by amplitude.\n"), m_pLogStream );
		for ( size_t i=0;i<m_numSpectra;i++ )
		{
			Spectra *a = m_pSourceVFS->beginWrite( i );
			a->normalize();
			m_flux = MAX(a->m_flux, m_flux);
			m_pSourceVFS->endWrite( i );
		}
	}
	else
	{
		for ( size_t i=0;i<m_numSpectra;i++ )
		{
			Spectra *a = m_pSourceVFS->beginWrite( i );
			a->calculateFlux();
			m_flux = MAX(a->m_flux, m_flux);
			m_pSourceVFS->endWrite( i );
		}
	}
	Helpers::print( std::string("global flux is: ") + Helpers::numberToString(m_flux) + std::string("\n" ), m_pLogStream );
}




void SOFMNetwork::exportHistograms( const std::string &_sstrExportDirectory )
{
	std::string sstrDir( FileHelpers::getFilePath(_sstrExportDirectory) );

	Helpers::print( std::string("Exporting energy maps.\n"), m_pLogStream );
	std::vector<float> energymap;
	std::vector<float> peakmap;
	std::vector<float> zmap;

	for ( size_t i=0;i<m_numSpectra;i++ )
	{
		Spectra *a = m_pSourceVFS->beginRead( i );
		energymap.push_back(a->m_flux);
		peakmap.push_back(a->m_Max);
		zmap.push_back(a->m_Z);
		m_pSourceVFS->endRead( i );
	}

	std::sort( energymap.begin(), energymap.end() );
	std::sort( peakmap.begin(), peakmap.end() );
	std::sort( zmap.begin(), zmap.end() );

	const size_t width = 800;
	const size_t height = 533;

	SpectraHelpers::renderDiagramToDisk(&energymap[0], energymap.size(), 1, 4, 0, width, height, sstrDir+std::string("energymap.png") );
	SpectraHelpers::renderDiagramToDisk(&peakmap[0], peakmap.size(), 1, 4, 0, width, height, sstrDir+std::string("peakmap.png") );
	SpectraHelpers::renderDiagramToDisk(&zmap[0], zmap.size(), 1, 4, 0, width, height, sstrDir+std::string("zmap.png") );
	exportNeighbourHoodFunction( sstrDir+std::string("neighborhoodfunc.png") );
}


void SOFMNetwork::renderIcons()
{
	Helpers::print( std::string("Rendering Icons.\n"), m_pLogStream );

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
		float localmax = fabs(a->m_flux);
		float redness = localmax;
		if (redness != 0.f)
		{
			redness = MathHelpers::logf(localmax+1.f,m_flux+1.f);
			redness *= redness*2.f;
		}
#ifdef SDSS_SINETEST
		redness = (float)i*2.f/(float)m_numSpectra;
#endif // SDSS_SINETEST
		SpectraHelpers::renderSpectraIconToDisk(*a, sstrFilename, s_iconSize, s_iconSize, redness );

		m_pSourceVFS->endRead( i );
	}
}

void SOFMNetwork::getSOFMSpectra( size_t _cellX, size_t _cellY, Spectra &_outSpectrum )
{
	assert( m_pNet != NULL );
	assert( _cellX < m_gridSize );
	assert( _cellY < m_gridSize );
	size_t nIndex( _cellY*m_gridSize+_cellX );
	_outSpectrum =	*m_pNet->beginRead( nIndex );
	m_pNet->endRead( nIndex );
}


bool SOFMNetwork::getInputSpectrum( size_t _cellX, size_t _cellY, Spectra &_outSpectrum )
{
	assert( m_pNet != NULL );
	assert( _cellX < m_gridSize );
	assert( _cellY < m_gridSize );
	size_t nIndex( _cellY*m_gridSize+_cellX );
	bool bRetVal = false;

	Spectra *spNet = m_pNet->beginRead( nIndex );
	if ( spNet != NULL && !spNet->isEmpty() ) 
	{
		Spectra *spSource = m_pSourceVFS->beginRead( spNet->m_Index );
		_outSpectrum = *spSource;
		m_pSourceVFS->endRead( spNet->m_Index );
		bRetVal = true;
	}
	m_pNet->endRead( nIndex );

	return bRetVal;
}

SpectraVFS &SOFMNetwork::getNetwork()
{
	return *m_pNet;
}



void SOFMNetwork::reset( const Parameters &_params )
{
	m_params = _params;
	m_currentStep = 0;
	m_Random.initRandom( m_params.randomSeed );
}


void SOFMNetwork::exportNeighbourHoodFunction( const std::string &_sstrFilenName )
{
	float lPercent = 0.f;
	std::vector<float> nbFunction;
	nbFunction.resize(m_gridSize*5);

	for ( int j=0;j<5;j++)
	{
		const float lRate = m_params.lRateBegin*pow(m_params.lRateEnd/m_params.lRateBegin,lPercent);
		const float sigma = m_params.radiusBegin*pow(m_params.radiusEnd/m_params.radiusBegin, lPercent);
		const float sigmaSqr = sigma*sigma;
		const float sigmaSqr2 = sigmaSqr*(1.f/EULER);

		for (size_t i=0;i<m_gridSize;i++)
		{
			const float tdistall = static_cast<float>(i)/static_cast<float>(m_gridSize);
			const float hxy = exp(-tdistall/sigmaSqr2);	
			nbFunction[i+j*m_gridSize] = hxy*lRate;
		}

		lPercent += 0.25f;
	}


	const size_t width = 800;
	const size_t height = 533;

	SpectraHelpers::renderDiagramToDisk(&nbFunction[0], m_gridSize, 5, 4, 0, width, height, _sstrFilenName );
}



void SOFMNetwork::adaptNetwork( const Spectra &_spectrum, size_t _bestMatchIndex, float _adaptionThreshold, float _sigmaSqr, float _lRate )
{
	const size_t xpBestMatch = _bestMatchIndex % m_gridSize;
	const size_t ypBestMatch = _bestMatchIndex / m_gridSize;
	const float sigmaSqr2 = _sigmaSqr*(1.f/EULER);
	const float fGridSizeSqr = static_cast<float>(m_gridSizeSqr);

	const int gridSize = static_cast<int>(m_gridSize);


	// adjust weights of the whole network
#pragma omp parallel for schedule (dynamic)
	for ( int y=0;y<gridSize;y++)
	{
		const float distY1 = static_cast<float>(y)-static_cast<float>(ypBestMatch);
		//const float distY2 = (static_cast<float>(gridSize)-static_cast<float>(y))-static_cast<float>(ypBestMatch);
		const float distY1Sqr = distY1*distY1;
		//const float distY2Sqr = distY2*distY2;
		const float distYSqr = distY1Sqr; //MIN(distY1Sqr, distY2Sqr);

		for ( int x=0;x<gridSize;x++)
		{
			const float distX1 = static_cast<float>(x)-static_cast<float>(xpBestMatch);
			//const float distX2 = (static_cast<float>(gridSize)-static_cast<float>(x))-static_cast<float>(xpBestMatch);
			const float distX1Sqr = distX1*distX1;
			//const float distX2Sqr = distX2*distX2;
			const float distXSqr = distX1Sqr;//MIN(distX1Sqr, distX2Sqr);
			const float distSqr = (distXSqr+distYSqr)/fGridSizeSqr;					// normalize squared distance with gridsize

			// calculate neighborhood function
			//const float mexican_hat_term = 1.f-distSqr/_sigmaSqr;
			//const float hxy = exp(-(distSqr)/sigmaSqr2);							// original
			//const float hxy = exp(-(distSqr)/sigmaSqr2)*mexican_hat_term;		// Mexican hat
			const float hxy = exp(-sqrtf(distSqr)/sigmaSqr2);						// spike
			const float lratehsx = _lRate*hxy;

			if ( lratehsx > _adaptionThreshold )
			{
				const size_t spectraAdress = y*m_gridSize+x;
				Spectra *a = m_pNet->beginWrite( spectraAdress );
				a->adapt( _spectrum, lratehsx );
				m_pNet->endWrite( spectraAdress );
			}
		}
	}
}


SOFMNetwork::BestMatch SOFMNetwork::searchBestMatchComplete( const Spectra &_src )
{
	float err[SpectraVFS::CACHELINESIZE];

	BestMatch bestMatch;
	bestMatch.reset();

	size_t j=0;
	while (j<m_gridSizeSqr)
	{
		const int jInc = MIN( SpectraVFS::CACHELINESIZE, (MIN(m_gridSizeSqr, j+SpectraVFS::CACHELINESIZE)-j));

		Spectra *a = m_pNet->beginRead( j );
		SpectraHelpers::compareSpectra( _src, a, jInc, err );
		m_pNet->endRead( j );

		for ( size_t k=0;k<jInc;k++ )
		{
			if (err[k] < bestMatch.error )
			{
				bestMatch.error = err[k];
				bestMatch.index = j+k;
			}
		}

		j += jInc;
	}

	return bestMatch;
}


 
SOFMNetwork::BestMatch SOFMNetwork::searchBestMatchLocal( const Spectra &_src, const int _searchRadius )
{
	assert(_searchRadius > 0);
	BestMatch bestMatch;
	bestMatch.reset();
	bool bOnFrame = false;
	bool bFound = false;

	if ( _src.m_Index < 0 )
	{
		// no old position, due to continue computation, we have to use complete search!
		bestMatch = searchBestMatchComplete( _src );
		return bestMatch;
	}

	const int xpBestMatchOld = _src.m_Index % m_gridSize;
	const int ypBestMatchOld = _src.m_Index / m_gridSize;

	// calc boundaries
	const int xMin = MAX( xpBestMatchOld-_searchRadius, 0 );
	const int yMin = MAX( ypBestMatchOld-_searchRadius, 0 );
	const int xMax = MIN( xpBestMatchOld+_searchRadius+1, m_gridSize );
	const int yMax = MIN( ypBestMatchOld+_searchRadius+1, m_gridSize );

	const size_t numSpectraToSearch( (xMax-xMin)*(yMax-yMin) );

	// setup arrays
	std::vector<Spectra*> searchSpectraVec;
	std::vector<size_t> indexVec;
	std::vector<float> errorVec;
	searchSpectraVec.resize( numSpectraToSearch );
	indexVec.resize( numSpectraToSearch );
	errorVec.resize( numSpectraToSearch );

	// read spectra from vfs
	size_t c=0;
	for ( int y=yMin;y<yMax;y++ )
	{
		for ( int x=xMin;x<xMax;x++ )
		{
			const size_t spectraIndex = getIndex( x, y );
			indexVec[c] = spectraIndex;
			searchSpectraVec[c] = m_pNet->beginRead( spectraIndex );
			c++;
		}
	}

	// calculate errors/distances
	SpectraHelpers::compareSpectra( _src, searchSpectraVec, &errorVec[0] );

	//end read, find bmu from error list
	for ( size_t i=0;i<numSpectraToSearch;i++ )
	{
		const size_t spectraIndex = indexVec[i];
		const float err = errorVec[i];

		if (err < bestMatch.error )
		{
			bestMatch.error = err;
			bestMatch.index = spectraIndex;
			bFound = true;
		}

		m_pNet->endRead(spectraIndex);
	}


	if ( !bFound )
	{
		// all spectra where used in the given search radius, use global search.
		//Helpers::print(".",NULL,false);
		bestMatch = searchBestMatchComplete( _src );
	}

	return bestMatch;
}


void SOFMNetwork::process()
{
	if ( m_currentStep > 1 )
	{
		m_pNet->dumpToFile(m_pNet->getFileName()+"old");
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
	if ( m_currentStep == m_params.numSteps*3/4 )
	{
		exportToHTML("export/threequarter", false);
	} else
	if ( m_currentStep == 1 )
	{
		exportToHTML("export/first", false);
	} else
	if ( m_currentStep > 1 ) 
	{
		exportToHTML("export/current", false);
	}

	if ( m_currentStep > m_params.numSteps )
	{
		Helpers::print( std::string("Clustering finished (success).\n"), m_pLogStream );
		exit(1);
	}



	std::string sstrLog("Calculating step ");
	sstrLog += Helpers::numberToString( m_currentStep ) + " / " + Helpers::numberToString( m_params.numSteps ) + "\n";
	Helpers::print( sstrLog, m_pLogStream );

	const float lPercent = static_cast<float>(m_currentStep)/static_cast<float>(m_params.numSteps);
	const float lRate = m_params.lRateBegin*pow(m_params.lRateEnd/m_params.lRateBegin,lPercent);
	const float adaptionThreshold = m_params.lRateEnd*0.01f;
	const float sigma = m_params.radiusBegin*pow(m_params.radiusEnd/m_params.radiusBegin,lPercent);
	const float sigmaSqr = sigma*sigma;
	bool bFullSearch = true;
	size_t searchRadius = 1;

	// determine search strategy for BMUs for the current learning step
	if ( m_params.sstrSearchMode == SOFMNET_SETTINGS_SEARCHMODE_localfast )
	{
		// always use a constant search radius, never do a global search
		bFullSearch = (m_currentStep<1);
		searchRadius = 2;
	}
	else if ( m_params.sstrSearchMode == SOFMNET_SETTINGS_SEARCHMODE_local )
	{
		// global search for the first 5 steps, decreasing search radius for increasing number of learings teps
		bFullSearch = (m_currentStep<5);
		searchRadius = static_cast<size_t>(((1.f-lPercent)*0.5f*static_cast<float>(m_gridSize)))+2;
	}
	else // SOFMNET_SETTINGS_SEARCHMODE_global
	{
		// always use global search, never go local.
		bFullSearch = true;
	}


	if (bFullSearch) 
	{
		Helpers::print( "using complete search.\n", m_pLogStream );
	}
	else
	{
		Helpers::print( "using local search - search radius = "+Helpers::numberToString(searchRadius)+ "\n", m_pLogStream );
	}


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
		a->m_Index = -1;
		m_pNet->endWrite( i );
	}

	// use GPU version
	if ( m_pCUDA )
	{
		m_pCUDA->process( &spectraIndexList[0], adaptionThreshold, sigmaSqr, lRate );
		m_pCUDA->downloadNetwork();
	}
	else
	{
		// CPU version
		// for each training spectra..
		double avgDist = 0.0;
		for ( size_t j=0;j<m_numSpectra;j++ )
		{
			// initialize best match batch
			BestMatch bmu;

			const size_t spectraIndex = spectraIndexList.at(j);
			Spectra &currentSourceSpectra = *m_pSourceVFS->beginWrite(spectraIndex);

			//		Timer t;

			// retrieve best match neuron for a source spectra
			if (bFullSearch) 
			{
				bmu = searchBestMatchComplete( currentSourceSpectra );
			}
			else
			{
				bmu = searchBestMatchLocal( currentSourceSpectra, searchRadius );
			}

			//		double dt = t.getElapsedSecs();
			//		Helpers::print( Helpers::numberToString<double>(dt) +std::string("search.\n") );

			//		t.start();

			// mark best match neuron
			Spectra *bmuSpectrum = m_pNet->beginWrite( bmu.index );
			setBestMatch( *bmuSpectrum, bmu.index, currentSourceSpectra, spectraIndex );
			avgDist += currentSourceSpectra.compare( *bmuSpectrum );
			m_pNet->endWrite( bmu.index );

			// adapt neighborhood
			adaptNetwork( currentSourceSpectra, bmu.index, adaptionThreshold, sigmaSqr, lRate );

			//	dt = t.getElapsedSecs();
			//		Helpers::print( Helpers::numberToString<double>(dt) +std::string("adapt.\n") );


			m_pSourceVFS->endWrite(spectraIndex);
		}

		m_pAvgDistanceToBMU[m_currentStep] = avgDist/(double)m_numSpectra;
	}


	Helpers::print( "writing network to disk.\n", m_pLogStream );
	m_pNet->flush();
	writeIndicesFromNetwork();
	Helpers::print( "finished writing network to disk.\n", m_pLogStream );
	m_currentStep++;
}


void SOFMNetwork::generateHTMLInfoPages( const std::string &_sstrMapBaseName )
{
	Helpers::print( "Generating info pages for spectra\n", m_pLogStream );

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

		Helpers::print( std::string("Generating info page for spectrum\n")+a->getFileName()+std::string("\n"), m_pLogStream );

		const int xpA = a->m_Index % m_gridSize;
		const int ypA = a->m_Index / m_gridSize;

		comparisonMap.clear();

		// ..also generate a local comparison map
		float maxErr = 0.0f;
		for (size_t c=0;c<m_gridSizeSqr;c++) 
		{
			pErrMap[c] = -1.0f;
		}


		for ( size_t j=0;j<m_numSpectra;j+=SpectraVFS::CACHELINESIZE )
		{
			Spectra *src[SpectraVFS::CACHELINESIZE];
			float err[SpectraVFS::CACHELINESIZE];

			const int numBatchElements = MIN( SpectraVFS::CACHELINESIZE, (MIN(m_numSpectra, j+SpectraVFS::CACHELINESIZE)-j));

			// begin read for all src spectra
			for ( int k=0;k<numBatchElements;k++)
			{
				src[k] = m_pSourceVFS->beginRead(j+k);
			}
			
			#pragma omp parallel for
			for ( int k=0;k<numBatchElements;k++)
			{
				err[k] = a->compareSuperAdvanced( *src[k], compareInvariance, true ); 
				//float err[k] = a->compare( *src[k] ); 
			}

			// end read for all src spectra, add error to comparison map
			for ( int k=0;k<numBatchElements;k++)
			{
				comparisonMap.insert( std::pair<float, size_t>(err[k], j+k) );	

				// fill comparison map
				if ( src[k]->m_Index >= 0 && src[k]->m_Index < m_gridSizeSqr )
				{
					pErrMap[src[k]->m_Index] = err[k];
					maxErr = MAX( maxErr, err[k] );
				}

				m_pSourceVFS->endRead( j+k );
			}
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
			const float fMaxErrLog10 = log10f(maxErr);
			const int iGridSizeSqr = static_cast<int>(m_gridSizeSqr);
			#pragma omp parallel for
			for (int c=0;c<iGridSizeSqr;c++) 
			{
				// mark empty cells gray
				if ( pErrMap[c] < 0.0f )
				{
					pRGBMap[c*3+0] = pRGBMap[c*3+1] = pRGBMap[c*3+2] = 0.5f;
				}
				else
				{
					const float scale = log10f(pErrMap[c]+1.f)/fMaxErrLog10;
					SpectraHelpers::intensityToRGB( scale,  &pRGBMap[c*3] );
				}
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
		if ( a->hasBadPixels() )
		{
			sstrTable += "Bad pixels detected (over 5% out of "+Helpers::numberToString<int>(a->m_SamplesRead)+" pixels).";
			sstrTable += HTMLExport::lineBreak();
		}
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
			sstrTable += HTMLExport::imageLink( b->getImgURL(), b->getURL() );
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
		//return; // for testing.
	}

	delete[] pErrMap;
	delete[] pRGBMap;
}


std::string linkToPlan( const std::string &_sstrFilename, size_t _planX, size_t _planY )
{
	std::string sstrFilename = _sstrFilename;
	sstrFilename += Helpers::numberToString( _planX );
	sstrFilename += "_";
	sstrFilename += Helpers::numberToString( _planY );
	sstrFilename += ".html";
	return FileHelpers::getFileName(sstrFilename) ;
}

void SOFMNetwork::exportToHTML( const std::string &_sstrFilename, bool _fullExport )
{
	std::string sstrName( FileHelpers::getFileName(_sstrFilename) );
	std::string sstrDirectory( FileHelpers::getFilePath(_sstrFilename) );

	std::string sstrLog;
	sstrLog = "Exporting results to ";
	sstrLog += _sstrFilename;
	sstrLog +="\n";

	Helpers::print( sstrLog, m_pLogStream );



	std::string sstrHTMLDocTemplate = HTMLExport::loadHTMLTemplate();	
	std::string sstrHTMLDoc;
	std::string sstrMainHTMLDoc;
	std::string sstrMainTable;

	sstrMainHTMLDoc = sstrHTMLDocTemplate;

	std::string sstrInfo;

	const std::string sstrStep( Helpers::numberToString<unsigned int>(m_currentStep, 3) );

	const std::string sstrUMatrix =  "UMatrix_" + sstrStep;
	const std::string sstrDifferenceMap = "DifferenceMap_" + sstrStep;
	const std::string sstrZMap = "ZMap_" + sstrStep;
	const std::string sstrObjectTypeMap = "objectTypeMap" + sstrStep;
	SpectraHelpers::calcUMatrix( *m_pNet, sstrDirectory+sstrUMatrix, true, true, false, _fullExport, s_outputPlanSize );
	SpectraHelpers::calcDifferenceMap( *m_pSourceVFS, *m_pNet, sstrDirectory+sstrDifferenceMap, true, false, true, _fullExport, s_outputPlanSize);
	SpectraHelpers::calcZMap( *m_pSourceVFS, *m_pNet, sstrDirectory+sstrZMap, true, _fullExport, s_outputPlanSize );
	SpectraHelpers::writeSpectraTypes( *m_pSourceVFS, *m_pNet, sstrDirectory+sstrObjectTypeMap );


	SpectraHelpers::renderDiagramToDisk( m_pAvgDistanceToBMU, m_params.numSteps, 1, 4, 0, 800, 533, sstrDirectory+std::string("avgDistanceBMU.png") );
	SpectraHelpers::writeFloatList( m_pAvgDistanceToBMU, m_params.numSteps, std::string("avgDistanceBMU.txt"));

	const std::string sstrTableFilename = sstrDirectory + "spectraTbl_" + sstrZMap + ".csv";
	SpectraHelpers::writeSpectraInfoToTable( *m_pSourceVFS, *m_pNet, sstrTableFilename, m_pLogStream);


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
	sstrInfo += std::string("Z-map:")+HTMLExport::lineBreak()+HTMLExport::image( sstrZMap+std::string(".png") )+HTMLExport::lineBreak();
	sstrInfo += std::string("ObjectType map:")+HTMLExport::lineBreak()+HTMLExport::image( sstrObjectTypeMap+std::string(".png") )+HTMLExport::lineBreak();
	sstrInfo += std::string("Energy histogram:")+HTMLExport::lineBreak()+HTMLExport::image( std::string("energymap.png") )+HTMLExport::lineBreak();
	sstrInfo += std::string("Peak histogram:")+HTMLExport::lineBreak()+HTMLExport::image( std::string("peakmap.png") )+HTMLExport::lineBreak();
	sstrInfo += std::string("Z histogram:")+HTMLExport::lineBreak()+HTMLExport::image( std::string("zmap.png") )+HTMLExport::lineBreak();
	sstrInfo += std::string("Neighborhood function:")+HTMLExport::lineBreak()+HTMLExport::image( std::string("neighborhoodfunc.png") )+HTMLExport::lineBreak();
	sstrInfo += std::string("Average distance to BMU:")+HTMLExport::lineBreak()+HTMLExport::image( std::string("avgDistanceBMU.png") )+HTMLExport::lineBreak();

	// todo: also export images using this PHP func:
	// http://casjobs.sdss.org/ImgCutoutDR6/getjpeg.aspx?ra=147.30828866&dec=0.15117002&scale=0.2&width=200&height=200&opt=SGP
	// or see: http://cas.sdss.org/dr6/en/tools/chart/chartinfo.asp


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

						Spectra *spSRC = m_pSourceVFS->beginRead(sp->m_Index);

						std::string sstrPlanDirectory = Spectra::plateToString(spSRC->getPlate()) + std::string("/");
						std::string sstrImagePlan = sstrPlanDirectory + spSRC->getFileName();

						if ( _fullExport )
						{
							sstrTable += HTMLExport::imageLink( sstrImagePlan + std::string(".png"), sstrImagePlan+".html" );
						}
						else
						{
							sstrTable += HTMLExport::imageLink( sstrImagePlan + std::string(".png"), spSRC->getURL() );
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

						m_pSourceVFS->endRead(sp->m_Index);
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
			if ( planX > 0 )
			{
				sstrTable += HTMLExport::imageLink( std::string("left.png"), linkToPlan(_sstrFilename, planX-1, planY), true );
			}
			if ( planX < planXMax-1 )
			{
				sstrTable += HTMLExport::imageLink( std::string("right.png"), linkToPlan(_sstrFilename, planX+1, planY), true );
			}
			if ( planY > 0 )
			{
				sstrTable += HTMLExport::imageLink( std::string("up.png"), linkToPlan(_sstrFilename, planX, planY-1), true );
			}
			if ( planY < planYMax-1 )
			{
				sstrTable += HTMLExport::imageLink( std::string("down.png"), linkToPlan(_sstrFilename, planX, planY+1), true );
			}
			std::string sstrPlan("");
			if (_fullExport)
			{
				sstrPlan = "_"+Helpers::numberToString(planX,4)+"_"+Helpers::numberToString(planY,4);
			}
			sstrTable += HTMLExport::lineBreak();
			sstrTable += HTMLExport::image( sstrUMatrix+sstrPlan+std::string(".png") );
			sstrTable += HTMLExport::image( sstrDifferenceMap+sstrPlan+std::string(".png") );
			sstrTable += HTMLExport::image( sstrZMap+sstrPlan+std::string(".png") );

			if ( bOut )
			{
				// link to sub pages
				Helpers::insertString( HTMLExport::HTML_TOKEN_TEMPLATE, sstrTable, sstrHTMLDoc );

				std::string sstrFilename = _sstrFilename;
				sstrFilename += Helpers::numberToString( planX );
				sstrFilename += "_";
				sstrFilename += Helpers::numberToString( planY );
				sstrFilename += ".html";

				std::ofstream fon(sstrFilename.c_str());
				fon<<sstrHTMLDoc;

				sstrMainTable += HTMLExport::beginTableCell();
				sstrMainTable += HTMLExport::imageLink( sstrLastImageInPlan+".png", linkToPlan(_sstrFilename, planX, planY ));
				sstrMainTable += HTMLExport::endTableCell();
			}
		}
		sstrMainTable += HTMLExport::endTableRow();
	}

	if (!Helpers::insertString( HTMLExport::HTML_TOKEN_TEMPLATE, sstrMainTable, sstrMainHTMLDoc ) )
	{
		Helpers::print( std::string("export failed. Wrong template.html ?!?\n"), m_pLogStream );
		return;
	}

	{
		std::ofstream fon(std::string(_sstrFilename+".html").c_str());
		fon<<sstrMainHTMLDoc;
	}

	if ( _fullExport )
	{
		Helpers::print( "Export nearly complete, now creating info pages (You may abort here if you do not need them)...\n", m_pLogStream );
		generateHTMLInfoPages( sstrName );
	}


	writeSettings("settings.xml");

	if ( m_params.waitForUser )
	{
		Helpers::print( std::string("export finished. waiting for user input.\n"), m_pLogStream );
		_getch();
	}
}



size_t SOFMNetwork::getIndex( size_t _cellX, size_t _cellY )
{
	return _cellX+_cellY*m_gridSize;
}



void SOFMNetwork::writeIndicesFromNetwork()
{
	int *pIndexBuffer = new int[m_gridSizeSqr];
	
	for ( size_t i=0;i<m_gridSizeSqr;i++ )
	{
		Spectra *a = m_pNet->beginRead( i );
		pIndexBuffer[i] = a->m_Index;
		m_pNet->endRead( i );
	}

	std::string sstrFilename("export/indexList");
	sstrFilename += Helpers::numberToString( m_currentStep,4 );
	sstrFilename += ".bin";

	FileHelpers::writeFile( sstrFilename, static_cast<void*>(pIndexBuffer), m_gridSizeSqr*sizeof(int), true );

	delete[] pIndexBuffer;
}