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
//! \file  spectra.h
//! \brief It should be better called spectrum because it contains only a single spectrum.


#include "spectra.h"



#include "random.h"
#include "helpers.h"
#include "fileHelpers.h"
#include "mathhelpers.h"
#include "defines.h"
#include "spectraBaseHelpers.h"
#include "CSVExport.h"
#include "spectraDB.h"

// disable SSE optimization (makes computations very slow)
//#define SSE_DISABLED

// temporary disable XML support under linux.
#ifndef _WIN32
#define _XMLIMPORT_DISABLED
#endif

#ifndef _XMLIMPORT_DISABLED
#include "XMLParser.h"
#endif


// temporary disable FITS support under linux.
#ifndef _WIN32
#define _FITSDISABLED
#endif


#ifndef _FITSDISABLED
#include "cfitsio/fitsio.h"
#include "cfitsio/longnam.h"
#endif 


#include <iostream>
#include <fstream>
#include <map>
#include <math.h>
#include <float.h>
#include <assert.h>
#include <string.h>


// spectra read from SDSS FITS have a wavelength of 3800..9200 Angström
// full spectrum range should range from ~540 Amgström to 9200 assuming max redshifts of 6.
const float Spectra::waveBeginDst	= 540.f;	
const float Spectra::waveEndDst		= 9200.f;
int Spectra::pixelStart				= 0;		
int Spectra::pixelEnd				= Spectra::numSamples;		

SpectraDB g_spectraDBDR12;
SpectraDB g_spectraDBDR10;
SpectraDB g_spectraDBDR9;

Spectra::Spectra()
{
	// make sure data structure is aligned to 16 byte boundaries for SSE
	assert( sizeof(Spectra) % 16 == 0 ); //< this doesn't require alignment to 16 byte boundaries
	clear();
}

Spectra::Spectra( const Spectra &_source )
{
	set( _source );
}


Spectra::~Spectra()
{
}


void Spectra::clear()
{
	m_Min = 0.0f;
	m_Max = 1.f;
	m_SamplesRead = 0;
	m_Index = -1;
	m_SpecObjID = 0;
	m_Type = 0;
	m_version = SP_ARTIFICIAL;
	m_Z = 0.0;
	m_flux = 0.0f;
	m_status = 0;

	for (size_t i=0;i<Spectra::numSamples;i++)
	{
		m_Amplitude[i] = 0.0f;
	}

}

void Spectra::randomize(float minrange, float maxrange )
{
	if ( minrange > maxrange )
	{
		float hui = maxrange;
		maxrange = minrange;
		minrange = hui;
	}

	float d = maxrange-minrange;

	Rnd r;

	for (size_t i=0;i<Spectra::numSamples;i++)
	{
		m_Amplitude[i] = r.randomFloat()*d+minrange;
	}
	m_SamplesRead = Spectra::numSamples;
	calcMinMax();
}


void Spectra::set(const Spectra &_spectra)
{
	m_SamplesRead		= _spectra.m_SamplesRead;
	m_Min				= _spectra.m_Min;
	m_Max				= _spectra.m_Max;
	m_Index				= _spectra.m_Index;
	m_SpecObjID			= _spectra.m_SpecObjID;
	m_version			= _spectra.m_version;
	m_Type				= _spectra.m_Type;
	m_Z					= _spectra.m_Z;
	m_flux				= _spectra.m_flux;
	m_status			= _spectra.m_status;

	for (size_t i=0;i<Spectra::numSamples;i++)
	{
		m_Amplitude[i] = _spectra.m_Amplitude[i];
	}
}


void Spectra::set( size_t type, float _noize )
{
	type = type %= 5;
	m_version = SP_ARTIFICIAL;

	static size_t UIDCount = 1;
	m_SpecObjID =(UIDCount++)<<22;
/*
	if ( type == 0 )
	{
		m_strFileName = "sin";
	}

	if ( type == 1 )
	{
		m_strFileName = "cos";
	}

	if ( type == 2 )
	{
		m_strFileName = "lin";
	}

	if ( type == 3 )
	{
		m_strFileName = "lin inv";
	}
 
	if ( type == 4 )
	{
		m_strFileName = "sqr";
	}
*/

	pad[0] = type;

	Rnd r;

	for (size_t i=0;i<Spectra::numSamples;i++)
	{
		float x=static_cast<float>(i)*0.02f;

		if ( type == 0 )
		{
			m_Amplitude[i] = sinf(x)*5.f+5.f+(r.randomFloat()-0.5f)*_noize;
		}

		if ( type == 1 )
		{
			m_Amplitude[i] = cosf(x)*5.f+5.f+(r.randomFloat()-0.5f)*_noize;
		}

		if ( type == 2 )
		{
			m_Amplitude[i] = 10.f*i/(float)Spectra::numSamples+(r.randomFloat()-0.5f)*_noize;
		}

		if ( type == 3 )
		{
			m_Amplitude[i] = 10.f+10.f*-(float)i/(float)Spectra::numSamples+(r.randomFloat()-0.5f)*_noize;
		}

		if ( type == 4 )
		{
			m_Amplitude[i] = 10.f*((float)i/(float)Spectra::numSamples)*((float)i/(float)Spectra::numSamples)+(r.randomFloat()-0.5f)*_noize;
		}
	}



//	long peak_pos = mt_random_int() % Spectra::numSamples;
//	float x=static_cast<float>(peak_pos)*0.01f;
//	m_Amplitude[peak_pos] = 60.f;

	m_SamplesRead = Spectra::numSamples;

	calcMinMax();
}


void Spectra::setSine( float _frequency, float _phase, float _amplitude, float _noize )
{
	static size_t UIDCount = 1;
	m_SpecObjID =(UIDCount++)<<22;
	m_version = SP_ARTIFICIAL;

	Rnd r;
	for (size_t i=0;i<Spectra::numSamples;i++)
	{
		float x=_phase+static_cast<float>(i)*_frequency;
		m_Amplitude[i] = sinf(x)*_amplitude+(r.randomFloat()-0.5f)*_noize;
	}
	m_SamplesRead = Spectra::numSamples;
	calcMinMax();
}

void Spectra::setRect( float _width, float _phase, float _amplitude )
{
	static size_t UIDCount = 1;
	m_SpecObjID =(UIDCount++)<<22;
	m_version = SP_ARTIFICIAL;
	
	float x = 0.0f;
	float xInc = 1.f/static_cast<float>(Spectra::numSamples);


	for (size_t i=0;i<Spectra::numSamples;i++)
	{
		if ( x>= _phase && x<= _phase+_width )
		{
			m_Amplitude[i] = _amplitude;
		}
		else
		{
			m_Amplitude[i] = 0.0f;
		}
		x += xInc;
	}


	m_SamplesRead = Spectra::numSamples;
	calcMinMax();
}



void Spectra::add(const Spectra &_spectra)
{
	for (size_t i=0;i<Spectra::numSamples;i++)
	{
		m_Amplitude[i] += _spectra.m_Amplitude[i];
	}
}


void Spectra::add(float _value)
{
	for (size_t i=0;i<Spectra::numSamples;i++)
	{
		m_Amplitude[i] += _value;
	}
}


void Spectra::subtract(const Spectra &_spectra)
{
	for (size_t i=0;i<Spectra::numSamples;i++)
	{
		m_Amplitude[i] -= _spectra.m_Amplitude[i];
	}
}


void Spectra::multiply( const Spectra &_spectra)
{
	for (size_t i=0;i<Spectra::numSamples;i++)
	{
		m_Amplitude[i] *= _spectra.m_Amplitude[i];
	}
}


void Spectra::multiply(float _multiplier)
{
	for (size_t i=0;i<Spectra::numSamples;i++)
	{
		m_Amplitude[i] *= _multiplier;
	}
}



bool Spectra::saveToCSV(const std::string &_filename)
{
	CSVExport cvsExporter(_filename, ", ");

	cvsExporter.writeTableEntry(std::string("Wavelength(A)"));
	cvsExporter.writeTableEntry(std::string("Flux"));
	cvsExporter.writeTableEntry(std::string("Error"));
	cvsExporter.writeTableEntry(std::string("Mask"));
	cvsExporter.newRow();

	const size_t numIterations = 3900/Spectra::numSamples;
	for (size_t i=0;i<Spectra::numSamples;i++)
	{
		for (size_t j=0;j<numIterations;j++ )
		{
			cvsExporter.writeTableEntry(0.f);
			cvsExporter.writeTableEntry(m_Amplitude[i]);
			cvsExporter.writeTableEntry(0.f);
			cvsExporter.writeTableEntry(0.f);
			cvsExporter.newRow();
		}
	}
	
	return true;
}

bool Spectra::loadFromCSV(const std::string &_filename, std::ofstream *_logStream)
{
	static size_t nCounter = 1;
	if ( FileHelpers::getFileExtension(_filename) != ".csv" )
	{
		return false;
	}
	clear();
	std::string sstrCSV;
	const bool bSuccess = FileHelpers::loadFileToString( _filename, sstrCSV );
	if ( !bSuccess )
	{
		Helpers::print("Could not load spectrum from CSV file.\n", _logStream );
		return false;
	}


	std::stringstream fin(sstrCSV.c_str());
	std::string sstrTemp;
	getline(fin, sstrTemp, '\n');

	// The first line can contain additional information:
	// specObjID, plate, mjd, fiber ID, type, z

	std::istringstream sstr;
	sstr.str(sstrTemp);
	int64_t specObjID=0;
	int type=-1;
	int mjd = -1;
	int plate = -1;
	int fiberID = -1;
	double z=-100000.0;

	sstr >> specObjID;
 	sstr >> mjd;
 	sstr >> plate;
 	sstr >> fiberID;
	sstr >> type;
	sstr >> z;

	m_Type = 0;
	m_version = SP_CSV;
	if ( type > 0 )
	{
		m_Type = type;
	}


	if ( specObjID > 0 )
	{
		// use specobj directly
		m_SpecObjID = specObjID;
	}
	else
	{

		// try to calculate it form the other data.
		if ( mjd > 0 && plate > 0 && fiberID >= 0 ) 
		{
			m_SpecObjID = Spectra::calcSpecObjID_DR7( plate, mjd, fiberID, 0 );
		} 
		else
		{
			// generate fake spec obj ID
			m_SpecObjID = Spectra::calcSpecObjID_DR7( nCounter/640, 0, nCounter%640, 0 );
			nCounter++;
		}
	}

	if ( z > -100000.0 )
	{
		m_Z = z;
	}


	float spectrum[numSamplesSDSS];


	//float x0,x2,x3;
	float x1;
	size_t c=0;

	while( fin >> sstrTemp && c < numSamplesSDSS ) 
	{	
		// row 0: Wavelength(A)
//		std::stringstream st0(sstrTemp.c_str() );
//		st0 >> x0;

		// row 1: Flux
//		fin >> sstrTemp;
		std::stringstream st1(sstrTemp.c_str() );
		st1 >> x1;

		// row: 2: Error
//		fin >> sstrTemp;
//		std::stringstream st2(sstrTemp.c_str() );
//		st2 >> x2;

		// row 3: Mask
//		fin >> sstrTemp;
//		std::stringstream st3(sstrTemp.c_str() );
//		st3 >> x3;

		spectrum[c] = x1;

		c++;
	}

	SpectraBaseHelpers::foldSpectrum( spectrum, c, m_Amplitude, numSamples, MathHelpers::log2(reductionFactor) );

	m_SamplesRead = c/reductionFactor;

	if ( m_SamplesRead < 3 )
	{
		Helpers::print("Could not load spectrum from CSV file. Reason: Not enough samples read from table.\n", _logStream );
		return false;
	}


	calcMinMax();

	const bool bConsistent = checkConsistency();
	if ( !bConsistent )
	{
		Helpers::print("Could not load spectrum from CSV file. Reason: Spectrum amplitudes contain NANs or INFs or just insane high numbers.\n", _logStream );
		return false;
	}

	return (m_SamplesRead>0 && bConsistent);	
}


bool Spectra::loadFromXML(const std::string &_filename, std::ofstream *_logStream)
{
#ifndef _XMLIMPORT_DISABLED
	clear();

	// reserve the last two samples to store ra & dec angles
	pixelEnd = numSamples-2;	


	XMLParser p;
	if (!p.loadXMLFromFile( _filename ))
	{
		Helpers::print("Could not load lightcurve from xml file. Reason: "+p.getParserErrorLog()+"\n", _logStream );
		return false;
	}

	bool bSuccess;


	std::string sstrType;
	size_t plateID=0;
	size_t fiberID=0;
	size_t mjd=0;
	float ra = -1000.0f;
	float dec = -1000.0f;
	float z = -1.0f;
	bSuccess = p.getChildValue("type", "value", sstrType );
	bSuccess = p.getChildValue("plate", "value", plateID );
	bSuccess = p.getChildValue("fiberId", "value", fiberID );
	bSuccess = p.getChildValue("mjd", "value", mjd );
	bSuccess = p.getChildValue("ra", "value", ra );
	bSuccess = p.getChildValue("dec", "value", dec );
	bSuccess = p.getChildValue("z", "value", z );

	m_Z = z;
	m_Amplitude[numSamples-2] = ra;
	m_Amplitude[numSamples-1] = dec;

	if ( mjd > 50000 && plateID > 0 && fiberID > 0 ) 
	{
		m_version = SP_LIGHTCURVE_SDSS;
		m_SpecObjID = calcSpecObjID_DR7( plateID, mjd, fiberID, 0);

		// recheck mjd, fiber and plate by calculating them from spec obj id.
		if ( getFiber() != fiberID || 
			getMJD() != mjd ||
			getPlate() != plateID )
		{
			// unique identifier mismatch
			Helpers::print("Could not load lightcurve from XML file. Reason: Unique identifier mismatch, something went wrong during spec Obj ID calculation.\n", _logStream );
			return false;
		}
	}
	else
	{
		if ( ra != -1000.f && dec != -1000.f ) {
			m_version = SP_LIGHTCURVE_RADEC;
		} else {
			m_version = SP_LIGHTCURVE_PLAIN;
		}
	}




	m_Type = 0;

	p.gotoChild();

	do
	{
		if (p.getCurrentTag()=="DATA")
		{
			break;
		}
	} while (p.gotoSibling());

	float xMin = 0.5f;
	float xMax = 4.0f;
	float yMin = -2.0f;
	float yMax = 0.0f;
	bool specifyYRange;
	p.getValue("xMin", xMin );
	p.getValue("xMax", xMax );
	specifyYRange = p.getValue("yMin", yMin );
	if (specifyYRange) 
		specifyYRange = p.getValue("yMax", yMax );

	float xDelta = xMax-xMin;
	if ( xDelta < 0.0f )
	{
		float temp = xMin;
		xMin = xMax;
		xMax = temp;
		xDelta *= 1.f;
	}
	if (xDelta == 0.0f )
	{
		Helpers::print("Could not load lightcurve from xml file. Reason: x-range = 0.0.\n", _logStream );
		return false;
	}

	float yDelta = yMax-yMin;
	if ( yDelta < 0.0f )
	{
		float temp = yMin;
		yMin = xMax;
		yMax = temp;
		yDelta *= 1.f;
	}
	if (yDelta == 0.0f )
	{
		Helpers::print("Could not load lightcurve from xml file. Reason: y-range = 0.0.\n", _logStream );
		return false;
	}

	float xRes = xDelta/(float)pixelEnd;

	if ( specifyYRange )
	{
		m_Min = yMin;
		m_Max = yMax;
	}
	else
	{
		calcMinMax();
	}


	std::string sstrData;
	p.getContent(sstrData);

	if ( sstrData.empty() ) 
	{
		Helpers::print("Could not load lightcurve from xml file. Reason: no data elements found.\n", _logStream );
		return false;
	}

	std::stringstream fin(sstrData.c_str());
	std::string sstrTemp;


	std::vector<float> values;
	while( fin >> sstrTemp  ) 
	{	
		float xp,yp;
		// x-value
		std::stringstream st(sstrTemp.c_str() );
		st >> xp;

		// separator (,;)
		char c;
		st >> c;

		// y-value
		st >> yp;

		values.push_back(xp);
		values.push_back(yp);
	}
	const int numPoints = values.size()/2;

	if ( numPoints < 1 ) {
		Helpers::print("Could not load lightcurve from xml file. Reason: no data elements found.\n", _logStream );
		return false;
	}

	// interpolate
	float xp = xMin;


	for (int i=0;i<pixelEnd;i++) 
	{

		int i1 = 0;
		for ( int k=0;k<numPoints;k++) {
			i1 = k-1;
			if ( values[k*2] >= xp  ) {
				break;
			}
		}

		int i0 = i1-1;
		int i2 = i1+1;
		int i3 = i1+2;

		i0 = CLAMP(i0,0,numPoints-1);
		i1 = CLAMP(i1,0,numPoints-1);
		i2 = CLAMP(i2,0,numPoints-1);
		i3 = CLAMP(i3,0,numPoints-1);

		float dx = values[i2*2]-values[i1*2]; 
		float x = ( dx>0.0f ) ? (xp-values[i1*2])/dx : 0.0f;
		x = CLAMP( x, 0.0f, 1.0f );

		m_Amplitude[i] = MathHelpers::lerp(values[i1*2+1], values[i2*2+1], x); //MathHelpers::interpolateCatmullRom( values[i0*2+1], values[i1*2+1], values[i2*2+1], values[i3*2+1], x );

		xp += xRes;
	}
	
	m_SamplesRead = pixelEnd;

	// if the imported light curve does not contain plate id, mjd and fiber id, where the specObj ID is calculated from
	// we hash the amplitudes and use this as identifier. 
	if ( m_version != SP_LIGHTCURVE_SDSS ) {
		m_SpecObjID = Helpers::hash( (const char*)&m_Amplitude[0], m_SamplesRead );
	}


	return true;
#else //_XMLIMPORT_DISABLED
	Helpers::print("Could not load lightcurve from xml file. Reason: Code disabled, undefine _XMLIMPORT_DISABLED.\n", _logStream );

	return false;
#endif // _XMLIMPORT_DISABLED
}


#ifndef _FITSDISABLED
Spectra::SpectraVersion checkVersion(fitsfile *f)
{
	int status = 0;

	// MJD, plate ID, fiber ID must be in every SDSS FITS file
	int mjd, plateID, fiber;
	fits_read_key( f, TINT, "FIBERID", &fiber, NULL, &status );
	if ( status != 0 )
	{
		return Spectra::SP_VERSION_INVALID;
	}

	fits_read_key( f, TINT, "MJD", &mjd, NULL, &status );
	fits_read_key( f, TINT, "PLATEID", &plateID, NULL, &status );
	if ( status != 0 )
	{
		// check for APOGEE infrared spectrum.
		status=0;
		fits_read_key( f, TINT, "MJD5", &mjd, NULL, &status );
		fits_read_key( f, TINT, "PLATE", &plateID, NULL, &status );
		if ( status != 0 )
		{
			return Spectra::SP_VERSION_INVALID;
		}

		// TODO: further APOGEE checking..

		return Spectra::SP_VERSION_APOGEE;
	}


	int bitpix = 0;
	fits_get_img_type(f, &bitpix, &status );
	if ( status != 0 )
		return Spectra::SP_VERSION_INVALID;
	
	double z=0.0;
	int specType = 0;
	int statusDR7 = 0;

	// if we can read the spectra type, z and bitpix==-32 we have DR7 and below spectra. 
	fits_read_key( f, TDOUBLE, "Z", &z, NULL, &statusDR7 );
	fits_read_key( f, TINT, "SPEC_CLN", &specType, NULL, &statusDR7 );
	if (statusDR7 == 0 && bitpix == -32 )
	{
		return Spectra::SP_VERSION_DR7;
	}

	// DR8, DR9 spectra have bitpix set to 8
	// DR10 spectra have bitpix set to 16
	if ( !(bitpix == 8 || bitpix == 16) )
		return Spectra::SP_VERSION_INVALID;
	char run2d[FLEN_VALUE];
	fits_read_key( f, TSTRING, "RUN2D", run2d, NULL, &status );

	// 	run2d can be an integer, like 26, 
	// 	or a string of the form 'vN_M_P', 
	if  (status == 0 && (run2d[0]=='v' || run2d[0]=='V'))
	{
		// RUN2D keyword found, must be DR9/DR10 spectra.
		return Spectra::SP_VERSION_BOSS;
	}

	return Spectra::SP_VERSION_DR8;
}


std::string fitsGetErrorMessage( int status )
{
	char err[128]={0};
	fits_get_errstatus( status, err );
	return std::string(err);
}

#endif // _FITSDISABLED


bool Spectra::loadFromFITS(const std::string &_filename, std::ofstream *_logStream)
{
#ifdef _FITSDISABLED
	assert(0); // temporary disabled.
	Helpers::print("Could not load spectrum from FITS file. Reason: Code disabled, undefine _FITSDISABLED.\n", _logStream );

	return false;
#else
	fitsfile *f=NULL;

	// According to FITS standard, to perform a specific action, status must be always 0 before.
	int status = 0;

	fits_open_file( &f, _filename.c_str(), READONLY, &status );
	if ( status != 0 ) 
	{
		Helpers::print("Could not load spectrum from FITS file. Reason: fits_open_file() failed with "+fitsGetErrorMessage(status)+"\n", _logStream );
		return false;
	}

	SpectraVersion version = checkVersion( f );
	fits_close_file(f, &status);
	
	if ( version == SP_VERSION_BOSS )
	{
		return loadFromFITS_BOSS(_filename, _logStream);
	}
	else if ( version == SP_VERSION_DR8 )
	{
		return loadFromFITS_DR8(_filename, _logStream);
	}
	else if ( version == SP_VERSION_DR7 )
	{
		return loadFromFITS_SDSS(_filename, _logStream);
	}
	else if ( version == SP_VERSION_APOGEE )
	{
		Helpers::print("Could not load DR10 and above APOGEE spectrum form FITS file. Reason: APOGEE infrared spectra not yet supported. \n", _logStream );
		return false;
	}

	Helpers::print("Could not load spectrum form FITS file. Reason: Invalid version. \n", _logStream );

	
	return false;
#endif // _FITSDISABLED
}


void Spectra::loadDataFromSpectraDB( std::ofstream *_logStream )
{
	SpectraDB::Info spectraInfo;
	// load spectra DB the first time and retrieve add. spectra params
	bool spectraInfoLoaded = g_spectraDBDR12.loadDB( SpectraDB::DR12 ) && g_spectraDBDR12.getInfo( m_SpecObjID, spectraInfo );

	if ( spectraInfoLoaded )
	{
		m_Z		= spectraInfo.z;
		m_Type	= spectraInfo.spClass;
	} 
	else {
		spectraInfoLoaded = g_spectraDBDR10.loadDB( SpectraDB::DR10 ) && g_spectraDBDR10.getInfo( m_SpecObjID, spectraInfo );
		if ( spectraInfoLoaded )
		{
			m_Z		= spectraInfo.z;
			m_Type	= spectraInfo.spClass;
		} 
		else {
			spectraInfoLoaded = g_spectraDBDR9.loadDB( SpectraDB::DR9 ) && g_spectraDBDR9.getInfo( m_SpecObjID, spectraInfo );
			if ( spectraInfoLoaded )
			{
				m_Z		= spectraInfo.z;
				m_Type	= spectraInfo.spClass;
			}
			else {
				Helpers::print("Could not load additional spectra info for specObjID="+Helpers::numberToString(m_SpecObjID)+" from spectra DB.\n", _logStream );
			}
		}
	}
}


bool Spectra::loadFromFITS_BOSS(const std::string &_filename, std::ofstream *_logStream)
{
#ifdef _FITSDISABLED
	assert(0); // temporary disabled.
	return false;
#else
	fitsfile *f=NULL;

	// According to FITS standard, to perform a specific action, status must be always 0 before.
	int status = 0;
	int statusclose = 0; // we always want to close a given FITs file, even if a previous operation failed


	fits_open_file( &f, _filename.c_str(), READONLY, &status );
	if ( status != 0 ) 
	{
		Helpers::print("Could not load BOSS spectrum from FITS file. Reason: fits_open_file() failed with: "+fitsGetErrorMessage(status)+"\n", _logStream );
		return false;
	}

	m_version = checkVersion( f );

	if ( m_version != SP_VERSION_BOSS )
	{
		// wrong version
		Helpers::print("Could not load BOSS spectrum from FITS file. Reason: Wrong version, could not identify a BOSS FITS file. \n", _logStream );
		fits_close_file(f, &statusclose);
		return false;
	}


	// read important info of spectrum 
	// 	run2d can be an integer, like 26, 
	// 	or a string of the form 'vN_M_P', 
	// 	where N, M and P are integers, 
	// 		with the restriction 5<=N<=6, 0<=M<=99, and 0<=P<=99. 
	// 		This is understood to be the run2d value for a spectrum. 
	int mjd, plateID, fiber, bitpix;
	char run2d[FLEN_VALUE];
	fits_read_key( f, TINT, "MJD", &mjd, NULL, &status );
	fits_read_key( f, TINT, "PLATEID", &plateID, NULL, &status );
	fits_read_key( f, TINT, "FIBERID", &fiber, NULL, &status );
	fits_read_key( f, TSTRING, "RUN2D", run2d, NULL, &status );
	fits_get_img_type(f, &bitpix, &status );

	if ( status != 0 )
	{
		// could not read important spectra info.
		Helpers::print("Could not load BOSS spectrum from FITS file. Reason: Could not read important identifiers (MJD, plate ID, fiber id, run2D), fits_read_key() failed with: "+fitsGetErrorMessage(status)+"\n", _logStream );
		fits_close_file(f, &statusclose);
		return false;
	}

	// encode run2d string to numbers n,m,p.
	char m[3];
	char p[3];
	m[0] = run2d[3]; m[1]=0; m[2] =0;
	p[1]=0; p[2] =0;
	
	int o=5;
	if ( run2d[4] != '_' ) {
		m[1] = run2d[4];
		o=6;
	}
	p[0] = run2d[o];
	if ( run2d[o+1] >= 48 && run2d[o+1] <= 57  ) 
		p[1] = run2d[o+1];


	const int run2d_n = run2d[1]-48;
	const int run2d_m = Helpers::stringToNumber<int>( m );
	const int run2d_p = Helpers::stringToNumber<int>( p );

	if ( run2d_n < 5 || mjd < 50000 )
	{
		// wrong mjd or run 2D version number.
		Helpers::print("Could not load BOSS spectrum from FITS file. Reason: Wrong MJD or run2D version number.\n", _logStream );
		fits_close_file(f, &statusclose);
		return false;
	}

	
	m_SpecObjID = calcSpecObjID_DR8( plateID, mjd, fiber, run2d_m, run2d_n, run2d_p);

	// recheck mjd, fiber and plate by calculating them from spec obj id.
	if ( getFiber() != fiber || 
		getMJD() != mjd ||
		getPlate() != plateID )
	{
		// unique identifier mismatch
		Helpers::print("Could not load BOSS spectrum from FITS file. Reason: Unique identifier mismatch, something went wrong during spec Obj ID calculation.\n", _logStream );
		fits_close_file(f, &statusclose);
		return false;
	}


	int numhdus = 0;
	int hdutype = 0; // should be BINARY_TBL
	long tblrows = 0; // should be numLines
	int tblcols = 0; // should be 23
	fits_get_num_hdus( f, &numhdus, &status );
	fits_movabs_hdu( f, 2, &hdutype, &status );
	fits_get_num_rows( f, &tblrows, &status );
	fits_get_num_cols( f, &tblcols, &status );
	if ( status != 0 || tblcols <= 0 || tblrows <= 0 || hdutype != BINARY_TBL )
	{
		// wrong table
		Helpers::print("Could not load BOSS spectrum from FITS file. Reason: Could not access binary spectrum data from HUD 2: "+fitsGetErrorMessage(status)+"\n", _logStream );
		fits_close_file(f, &statusclose);
		return false;
	}



	float spectrum[numSamplesBOSS];
	float invar[numSamplesBOSS];		//inverse variance of flux
	unsigned long maskArray[numSamplesBOSS];
	bool maskArrayBool[numSamplesBOSS];

	// number of elements should be 2 at least.
	size_t elementsToRead = tblrows;
	assert( elementsToRead <= numSamplesBOSS );
	elementsToRead = MIN( elementsToRead, numSamplesBOSS );
	if ( elementsToRead < 3 )
	{
		// to few spectrum samples/pixels to read.
		Helpers::print("Could not load BOSS spectrum from FITS file. Reason: To few spectrum samples/pixels to read.\n", _logStream );
		fits_close_file(f, &statusclose);
		return false;
	}

	// spec says:
	// 	Name 		Type 	Comment
	// 	flux 		float32	coadded calibrated flux [10-17 ergs/s/cm2/Å]
	// 	loglam 		float32	log10(wavelength [Å])
	// 	ivar 		float32	inverse variance of flux
	// 	and_mask	int32 	AND mask
	// 	or_mask 	int32 	OR mask
	// 	wdisp 		float32	wavelength dispersion in pixel=dloglam units
	// 	sky 		float32	subtracted sky flux [10-17 ergs/s/cm2/Å]
	// 	model 		float32	pipeline best model fit used for classification and redshift	
	fits_read_col( f, TFLOAT, 1, 1, 1, elementsToRead, NULL, spectrum, NULL, &status );
	fits_read_col( f, TFLOAT, 3, 1, 1, elementsToRead, NULL, invar, NULL, &status );
	fits_read_col( f, TINT, 4, 1, 1, elementsToRead, NULL, maskArray, NULL, &status );

	if ( status != 0 )
	{
		// could not read table
		Helpers::print("Could not load BOSS spectrum from FITS file. Reason: Could not read binary spectrum data from HUD 2, fits_read_col() failed with: "+fitsGetErrorMessage(status)+"\n", _logStream );
		fits_close_file(f, &statusclose);
		return false;
	}


	// count bad pixels 
	size_t badPixelCount = 0;
	for (size_t i=0;i<elementsToRead;i++)
	{
		// pixels are bad where inverse variance of flux is 0.0 or the mask array says "bright sky")
		if ( invar[i] == 0.0 || ((maskArray[i] & SP_MASK_BRIGHTSKY)>0) )
		{
			maskArrayBool[i] = true;
			// bad pixel detected.
			badPixelCount++;
		}
		else
		{
			maskArrayBool[i] = false;
		}
	}


	SpectraBaseHelpers::repairSpectra( spectrum, maskArrayBool, elementsToRead );
	// mark spectrum as bad if more than 5 % are bad pixels
	if ( badPixelCount > elementsToRead/20 ) 
	{
		m_status = 1;
	}



	//BRIGHTSKY

	SpectraBaseHelpers::foldSpectrum( spectrum, elementsToRead, m_Amplitude, numSamples, MathHelpers::log2(reductionFactor) );
	elementsToRead /= reductionFactor; 
	m_SamplesRead = static_cast<int16_t>(elementsToRead);

	fits_close_file(f, &statusclose);
	calcMinMax();

	// retrieve add. info.
	loadDataFromSpectraDB( _logStream );


	const bool bConsistent = checkConsistency();

	if ( !bConsistent )
	{
		Helpers::print("Could not load BOSS spectrum from FITS file. Reason: Spectrum amplitudes contain NANs or INFs or just insane high numbers.\n", _logStream );
		return false;
	}

	if ((m_SamplesRead <= numSamples/2))
	{
		Helpers::print("Could not load BOSS spectrum from FITS file. Reason: Not enough samples read.\n", _logStream );
		return false;
	}

	return true;	
#endif //_FITSDISABLED
}



bool Spectra::loadFromFITS_DR8(const std::string &_filename, std::ofstream *_logStream)
{
#ifdef _FITSDISABLED
	assert(0); // temporary disabled.
	return false;
#else
	fitsfile *f=NULL;

	// According to FITS standard, to perform a specific action, status must be always 0 before.
	int status = 0;
	int statusclose = 0; // we always want to close a given FITs file, even if a previous operation failed


	fits_open_file( &f, _filename.c_str(), READONLY, &status );
	if ( status != 0 ) 
	{
		// file not opened / file not found
		Helpers::print("Could not load DR8 spectrum from FITS file. Reason: fits_open_file() failed with: "+fitsGetErrorMessage(status)+"\n", _logStream );
		return false;
	}

	m_version = checkVersion( f );

	if ( m_version != SP_VERSION_DR8 )
	{
		// wrong version
		Helpers::print("Could not load DR8 spectrum from FITS file. Reason: Wrong version, could not identify a DR8 FITS file.\n", _logStream );
		fits_close_file(f, &statusclose);
		return false;
	}


	// read important info of spectrum 
	int mjd, plateID, fiber, bitpix;
	char specID[FLEN_VALUE]={0};
	fits_read_key( f, TINT, "MJD", &mjd, NULL, &status );
	fits_read_key( f, TINT, "PLATEID", &plateID, NULL, &status );
	fits_read_key( f, TINT, "FIBERID", &fiber, NULL, &status );
	fits_read_key( f, TSTRING, "SPEC_ID", specID, NULL, &status );
	fits_get_img_type(f, &bitpix, &status );

	if ( status != 0 )
	{
		// could not read important spectra info.
		Helpers::print("Could not load DR8 spectrum from FITS file. Reason: Could not read important identifiers (MJD, plate ID, fiber id, SPEC_ID) fits_read_key() failed with: "+fitsGetErrorMessage(status)+"\n", _logStream );
		fits_close_file(f, &statusclose);
		return false;
	}


	m_SpecObjID = Helpers::stringToNumber<int64_t>(specID);

	if ( getFiber() != fiber || 
		 getMJD() != mjd ||
		 getPlate() != plateID )
	{
		// unique identifier mismatch
		Helpers::print("Could not load DR8 spectrum from FITS file. Reason: SpecObjId mismatch.\n", _logStream );
		fits_close_file(f, &statusclose);
		return false;
	}

	int numhdus = 0;
	int hdutype = 0; // should be BINARY_TBL
	long tblrows = 0; // should be numLines
	int tblcols = 0; // should be 23
	fits_get_num_hdus( f, &numhdus, &status );
	fits_movabs_hdu( f, 2, &hdutype, &status );
	fits_get_num_rows( f, &tblrows, &status );
	fits_get_num_cols( f, &tblcols, &status );
	if ( status != 0 || tblcols <= 0 || tblrows <= 0 )
	{
		// wrong table
		Helpers::print("Could not load DR8 spectrum from FITS file. Reason: Could not access binary spectrum data from HUD 2: "+fitsGetErrorMessage(status)+"\n", _logStream );
		fits_close_file(f, &statusclose);
		return false;
	}



	float spectrum[numSamplesBOSS];
	float invar[numSamplesBOSS];		//inverse variance of flux
	unsigned long maskArray[numSamplesBOSS];
	bool maskArrayBool[numSamplesBOSS];

	// number of elements should be 2 at least.
	size_t elementsToRead = tblrows;
	assert( elementsToRead <= numSamplesBOSS );
	elementsToRead = MIN( elementsToRead, numSamplesBOSS );
	if ( elementsToRead < 3 )
	{
		// to few spectrum samples/pixels to read.
		Helpers::print("Could not load DR8 spectrum from FITS file. Reason: To few spectrum samples/pixels to read.\n", _logStream );
		fits_close_file(f, &statusclose);
		return false;
	}

	// spec says:
	// 	Name 		Type 	Comment
	// 	flux 		float32	coadded calibrated flux [10-17 ergs/s/cm2/Å]
	// 	loglam 		float32	log10(wavelength [Å])
	// 	ivar 		float32	inverse variance of flux
	// 	and_mask	int32 	AND mask
	// 	or_mask 	int32 	OR mask
	// 	wdisp 		float32	wavelength dispersion in pixel=dloglam units
	// 	sky 		float32	subtracted sky flux [10-17 ergs/s/cm2/Å]
	// 	model 		float32	pipeline best model fit used for classification and redshift	
	fits_read_col( f, TFLOAT, 1, 1, 1, elementsToRead, NULL, spectrum, NULL, &status );
	fits_read_col( f, TFLOAT, 3, 1, 1, elementsToRead, NULL, invar, NULL, &status );
	fits_read_col( f, TINT, 4, 1, 1, elementsToRead, NULL, maskArray, NULL, &status );

	if ( status != 0 )
	{
		// could not read table
		Helpers::print("Could not load DR8 spectrum from FITS file. Reason: Could not read binary spectrum data from HUD 2, fits_read_col() failed with: "+fitsGetErrorMessage(status)+"\n", _logStream );
		fits_close_file(f, &statusclose);
		return false;
	}

	// count bad pixels 
	size_t badPixelCount = 0;
	for (size_t i=0;i<elementsToRead;i++)
	{
		// pixels are bad where inverse variance of flux is 0.0 or the mask array says "bright sky")
		if ( invar[i] == 0.0 || ((maskArray[i] & SP_MASK_BRIGHTSKY)>0) )
		{
			maskArrayBool[i] = true;
			// bad pixel detected.
			badPixelCount++;
		}
		else
		{
			maskArrayBool[i] = false;
		}
	}


	SpectraBaseHelpers::repairSpectra( spectrum, maskArrayBool, elementsToRead );
	// mark spectrum as bad if more than 5 % are bad pixels
	if ( badPixelCount > elementsToRead/20 ) 
	{
		m_status = 1;
	}

	const int offset = getSDSSSpectraOffsetStart();
	SpectraBaseHelpers::foldSpectrum( spectrum, elementsToRead, &m_Amplitude[offset], numSamples-offset, MathHelpers::log2(reductionFactor) );
	elementsToRead /= reductionFactor; 
	m_SamplesRead = static_cast<int16_t>(elementsToRead);

	fits_close_file(f, &statusclose);
	calcMinMax();

	// retrieve add. info.
	loadDataFromSpectraDB( _logStream );

	const bool bConsistent = checkConsistency();

	if ( !bConsistent )
	{
		Helpers::print("Could not load DR8 spectrum from FITS file. Reason: Spectrum amplitudes contain NANs or INFs or just insane high numbers.\n", _logStream );
		return false;
	}

	if ((m_SamplesRead <= numSamples/2))
	{
		Helpers::print("Could not load DR8 spectrum from FITS file. Reason: Not enough samples read.\n", _logStream );
		return false;
	}


	return true;	
#endif //_WIN32
}



bool Spectra::loadFromFITS_SDSS(const std::string &_filename, std::ofstream *_logStream)
{
#ifdef _FITSDISABLED
	assert(0); // temporary disabled.
	return false;
#else
	fitsfile *f=NULL;

	// According to FITS standard, to perform a specific action, status must be always 0 before.
	int status = 0;
	int statusclose = 0; // we always want to close a given FITs file, even if a previous operation failed

	fits_open_file( &f, _filename.c_str(), READONLY, &status );
	if ( status != 0 ) 
	{
		// file not opened / file not found
		Helpers::print("Could not load SDSS I/II spectrum from FITS file. Reason: fits_open_file() failed with: "+fitsGetErrorMessage(status)+"\n", _logStream );
		return false;
	}

	m_version = checkVersion( f );

	if ( m_version != SP_VERSION_DR7 )
	{
		// wrong version
		Helpers::print("Could not load SDSS I/II spectrum from FITS file. Reason: Wrong version, could not identify a SDSS DR7 (or below) FITS file.\n", _logStream );
		fits_close_file(f, &statusclose);
		return false;
	}


	int naxis=0;
	long size[2];
	fits_get_img_dim(f, &naxis, &status );
	if (naxis!=2 || status != 0)
	{
		// no 2-dimensional image
		if ( status != 0 ) 
		{
			Helpers::print("Could not load SDSS I/II spectrum from FITS file. Reason: Wrong table dimension, fits_get_img_dim() failed with: "+fitsGetErrorMessage(status)+"\n", _logStream );
		}
		else
		{
			Helpers::print("Could not load SDSS I/II spectrum from FITS file. Reason: Wrong table dimension.\n", _logStream );
		}
		fits_close_file(f, &statusclose);
		return false;
	}

	// read important info of spectrum
	int mjd, plateID, fiber, type;
	fits_read_key( f, TINT, "MJD", &mjd, NULL, &status );
	fits_read_key( f, TINT, "PLATEID", &plateID, NULL, &status );
	fits_read_key( f, TINT, "FIBERID", &fiber, NULL, &status );
	fits_read_key( f, TDOUBLE, "Z", &m_Z, NULL, &status );
	fits_read_key( f, TINT, "SPEC_CLN", &type, NULL, &status );

	if ( status != 0 )
	{
		// could not read important spectra info.
		Helpers::print("Could not load SDSS I/II spectrum from FITS file. Reason: Could not read important identifiers (MJD, plate ID, fiber id, SPEC_CLN, z) fits_read_key() failed with: "+fitsGetErrorMessage(status)+"\n", _logStream );
		fits_close_file(f, &statusclose);
		return false;
	}




	setType( static_cast<OldSpectraType>(1<<type) );
	m_SpecObjID = Spectra::calcSpecObjID_DR7( plateID, mjd, fiber, 0 );

	// read spectral data
	fits_get_img_size(f, 2, size, &status );
	float spectrum[numSamplesSDSS];
	unsigned long maskArray[numSamplesSDSS];
	bool maskArrayBool[numSamplesSDSS];

	// number of elements should be 2 at least.
	size_t elementsToRead = size[0];
	assert( elementsToRead <= numSamplesSDSS );
	elementsToRead = MIN( elementsToRead, numSamplesSDSS );
	if ( elementsToRead < 3 )
	{
		// to few spectrum samples/pixels to read.
		Helpers::print("Could not load SDSS I/II spectrum from FITS file. Reason: To few spectrum samples/pixels to read.\n", _logStream );
		fits_close_file(f, &statusclose);
		return false;
	}

	// spec says:
	// The first row is the spectrum,
	// the second row is the continuum subtracted spectrum, 
	// the third row is the noise in the spectrum (standard deviation, in the same units as the spectrum), 
	// the forth row is the mask array. The spectra are binned log-linear. Units are 10^(-17) erg/cm/s^2/Ang.
	long adress[2]={1,1};
	fits_read_pix( f, TFLOAT, adress, elementsToRead, NULL, (void*)spectrum, NULL, &status );
	if ( status != 0  )
	{
		// table read error
		Helpers::print("Could not load SDSS I/II spectrum from FITS file. Reason: Could not read binary spectrum data. fits_read_pix() failed with: "+fitsGetErrorMessage(status)+"\n", _logStream );
		fits_close_file(f, &statusclose);
		return false;
	}


	// mask array
	const unsigned int maskErr = (~static_cast<unsigned int>(SP_MASK_OK)) & (~static_cast<unsigned int>(SP_MASK_EMLINE));
	adress[1] = 4;
	fits_read_pix( f, TLONG, adress, elementsToRead, NULL, (void*)maskArray, NULL, &status );
	if ( status != 0  )
	{
		// table read error
		Helpers::print("Could not load SDSS I/II spectrum from FITS file. Reason: Could not read mask array. fits_read_pix() failed with: "+fitsGetErrorMessage(status)+"\n", _logStream );
		fits_close_file(f, &statusclose);
		return false;
	}


	// count bad pixels 
	size_t badPixelCount = 0;
	for (size_t i=0;i<elementsToRead;i++)
	{
		if ( (maskArray[i] & maskErr) != 0 )
		{
			maskArrayBool[i] = true;
			// bad pixel detected.
			badPixelCount++;
		}
		else
		{
			maskArrayBool[i] = false;
		}
	}


	SpectraBaseHelpers::repairSpectra( spectrum, maskArrayBool, elementsToRead );


	// mark spectrum as bad if more than 5 % are bad pixels
	if ( badPixelCount > elementsToRead/20 ) 
	{
		m_status = 1;
	}


	const int offset = getSDSSSpectraOffsetStart();
	SpectraBaseHelpers::foldSpectrum( spectrum, elementsToRead, &m_Amplitude[offset], numSamples-offset, MathHelpers::log2(reductionFactor) );
	elementsToRead /= reductionFactor; 
	m_SamplesRead = static_cast<int16_t>(elementsToRead);
	fits_close_file(f, &statusclose);

	calcMinMax();

	const bool bConsistent = checkConsistency();

	if ( !bConsistent )
	{
		Helpers::print("Could not load SDSS I/II spectrum from FITS file. Reason: Spectrum amplitudes contain NANs or INFs or just insane high numbers.\n", _logStream );
		return false;
	}

	if ((m_SamplesRead <= numSamples/2))
	{
		Helpers::print("Could not load SDSS I/II spectrum from FITS file. Reason: Not enough samples read.\n", _logStream );
		return false;
	}


	return true;	
#endif //_FITSDISABLED
}


void Spectra::calcMinMax()
{
	m_Min = FLT_MAX;
	m_Max = -FLT_MAX;
	for (size_t i=pixelStart;i<pixelEnd;i++)
	{
		if ( m_Min > m_Amplitude[i])
			m_Min = m_Amplitude[i];
		if ( m_Max < m_Amplitude[i])
			m_Max = m_Amplitude[i];
	}
}


void Spectra::calculateFlux()
{
	double flux = 0.0;
	float offset = (m_Min < 0.0f) ? -m_Min : 0.0f;
	for (size_t i=pixelStart;i<pixelEnd;i++)
	{
		flux += static_cast<double>(m_Amplitude[i]+offset);
	}
	m_flux = static_cast<float>(flux);
}


void Spectra::normalize()
{
	calculateFlux();

	calcMinMax();

	float delta( m_Max-m_Min);

	if ( delta<=0.0f )
	{
		// uh, better not
		ASSERT(0);
		//_cprintf("warning spectrum %I64u has min=max=%f\n", m_SpecObjID, m_Min );
		return;
	}

	for (size_t i=pixelStart;i<pixelEnd;i++)
	{
		m_Amplitude[i] /= delta;
	}
}



void Spectra::normalizeByFlux()
{
	calculateFlux();

	if ( m_flux <= 0.0f )
		return;

	for (size_t i=pixelStart;i<pixelEnd;i++)
	{	
		m_Amplitude[i] /= (m_flux*0.001f);
	}
	calcMinMax();
}


extern "C" void spectraAdaptX64(const float *a0, const float *a1, float *adaptionRate, size_t numsamples);


void Spectra::adapt( const Spectra &_spectra, float _adaptionRate )
{
	//	million adaption / sec
	//	0,37  	c-version 
	//	0,46	sse version 

#ifdef SSE_DISABLED
	// c-version (slow)
	for ( size_t i=Spectra::pixelStart;i<Spectra::pixelEnd;i++ )
	{
		m_Amplitude[i] += _adaptionRate*(_spectra.m_Amplitude[i]-m_Amplitude[i]);
	}
#else //!SSE_DISABLED
#ifdef _WIN32

	const float *a0 = &m_Amplitude[Spectra::pixelStart];
	const float *a1 = &_spectra.m_Amplitude[Spectra::pixelStart];
	size_t numSamples4 = (Spectra::pixelEnd >> 3) << 3;
	SSE_ALIGN float adaptionRate4[4] = {_adaptionRate,_adaptionRate,_adaptionRate,_adaptionRate}; 

#ifdef X64
	spectraAdaptX64(a0,a1,adaptionRate4,numSamples4);
#else // X64
	_asm {
		mov edi, a0
		mov esi, a1
		mov ecx, numSamples4
		shr ecx, 3
		movaps xmm0, adaptionRate4

loop1:
		prefetchnta [esi+4*4*64]
		prefetchnta [edi+4*4*64]

		movaps xmm1, [edi+4*4*0]	// dst: m_Amplitude[c]
		movaps xmm2, [esi+4*4*0]	// src: _spectra.m_Amplitude[c]
		movaps xmm3, [edi+4*4*1]	// dst: m_Amplitude[c+4]
		movaps xmm4, [esi+4*4*1]	// src: _spectra.m_Amplitude[c+4]

		subps xmm2, xmm1			// _spectra.m_Amplitude[c]-m_Amplitude[c]
		subps xmm4, xmm3			// _spectra.m_Amplitude[c+4]-m_Amplitude[c+4]

		mulps xmm2, xmm0			// *=_adaptionRate
		mulps xmm4, xmm0			// *=_adaptionRate

		addps xmm1, xmm2			// add to dst
		addps xmm3, xmm4			// add to dst

		movaps [edi+4*4*0], xmm1	// this is the slowest bit
		movaps [edi+4*4*1], xmm3	// this is the slowest bit

		add edi, 4*4*2
		add esi, 4*4*2

		dec ecx
		jnz loop1
	}
#endif // X64

	int i=numSamples4;
	for (i;i<Spectra::pixelEnd;i++)
	{
		m_Amplitude[i] += _adaptionRate*(_spectra.m_Amplitude[i]-m_Amplitude[i]);
	}
#else // __linux

#ifdef X64

	const float *a0 = &m_Amplitude[Spectra::pixelStart];
	const float *a1 = &_spectra.m_Amplitude[Spectra::pixelStart];
	size_t numSamples4 = (Spectra::pixelEnd >> 3) << 3;
	SSE_ALIGN float adaptionRate4[4] = {_adaptionRate,_adaptionRate,_adaptionRate,_adaptionRate}; 

	spectraAdaptX64(a0,a1,adaptionRate4,numSamples4);

	int i=numSamples4;
	for (i;i<Spectra::pixelEnd;i++)
	{
		m_Amplitude[i] += _adaptionRate*(_spectra.m_Amplitude[i]-m_Amplitude[i]);
	}
#else // !X64
	// c-version (slow)
	for ( size_t i=Spectra::pixelStart;i<Spectra::pixelEnd;i++ )
	{
		m_Amplitude[i] += _adaptionRate*(_spectra.m_Amplitude[i]-m_Amplitude[i]);
	}
#endif // X64
#endif
#endif //! SSE_DISABLED

}



extern "C" void spectraCompareX64(const float *a0, const float *a1, float *errout, size_t numsamples);

float Spectra::compare(const Spectra &_spectra) const
{
#ifdef SSE_DISABLED
	float error=0.0f;
	for (size_t i=Spectra::pixelStart;i<Spectra::pixelEnd;i++)
	{
		float d = m_Amplitude[i]-_spectra.m_Amplitude[i];
		error += d*d;
	}
#else // !SSE_DISABLED
#ifdef _WIN32


	SSE_ALIGN float errorv[4];

	// optimized memory friendly version. make sure spectra is 16 bytes aligned
	// this is 10x faster than compiler generated SSE code!
	const float *a0 = &m_Amplitude[Spectra::pixelStart];
	const float *a1 = &_spectra.m_Amplitude[Spectra::pixelStart];
	size_t numSamples4 = (Spectra::pixelEnd >> 3) << 3;

#ifdef X64
	spectraCompareX64(a0,a1,errorv,numSamples4);
#else // X64

	_asm {
		mov edi, a0
		mov esi, a1
		mov ecx, numSamples4
		shr ecx, 3
		xorps xmm0, xmm0


loop1:
		prefetcht0 [esi+4*4*64]
		prefetcht0 [edi+4*4*64]

		movaps xmm1, [edi+4*4*0]
		movaps xmm2, [esi+4*4*0]
		movaps xmm3, [edi+4*4*1]
		movaps xmm4, [esi+4*4*1]

		subps xmm1, xmm2
		subps xmm3, xmm4

		mulps xmm1, xmm1
		mulps xmm3, xmm3

		addps xmm0, xmm1
		addps xmm0, xmm3

		add edi, 4*4*2
		add esi, 4*4*2

		dec ecx
		jnz loop1

		movaps errorv, xmm0
	}
#endif // X64

	float error=errorv[0]+errorv[1]+errorv[2]+errorv[3];

	int i=numSamples4;
	for (i;i<pixelEnd;i++)
	{
		float d = m_Amplitude[i]-_spectra.m_Amplitude[i];
		error += d*d;
	}

#else // ___linux

#ifdef X64
	SSE_ALIGN float errorv[4];

	// optimized memory friendly version. make sure spectra is 16 bytes aligned
	// this is 10x faster than compiler generated SSE code!
	const float *a0 = &m_Amplitude[Spectra::pixelStart];
	const float *a1 = &_spectra.m_Amplitude[Spectra::pixelStart];
	size_t numSamples4 = (Spectra::pixelEnd >> 3) << 3;


	spectraCompareX64(a0,a1,errorv,numSamples4);

	float error=errorv[0]+errorv[1]+errorv[2]+errorv[3];

	int i=numSamples4;
	for (i;i<pixelEnd;i++)
	{
		float d = m_Amplitude[i]-_spectra.m_Amplitude[i];
		error += d*d;
	}
#else // !X64

	float error=0.0f;
	for (size_t i=Spectra::pixelStart;i<Spectra::pixelEnd;i++)
	{
		float d = m_Amplitude[i]-_spectra.m_Amplitude[i];
		error += d*d;
	}
#endif // X64

#endif // !WIN32
#endif // !SSE_DISABLED
	
	return error;
}


void Spectra::setType( SpectraClass spClass, SpectraSubClass spSubClass, bool hasBroadline )
{
	m_Type = packSpectraClassAndSubclass( spClass, spSubClass, hasBroadline );
}

void Spectra::setType( OldSpectraType spType )
{
	switch (spType) {
	case SPT_STAR				: setType( SPC_STAR,	SPSC_NOT_SET, false );break;
	case SPT_GALAXY				: setType( SPC_GALAXY,	SPSC_NOT_SET, false );break;
	case SPT_QSO				: setType( SPC_QSO,		SPSC_NOT_SET, false );break;
	case SPT_HIZ_QSO			: setType( SPC_QSO,		SPSC_NOT_SET, false );break;
	case SPT_STAR_LATE			: setType( SPC_STAR,	SPSC_NOT_SET, false );break;
	case SPT_GAL_EM				: setType( SPC_GALAXY,	SPSC_NOT_SET, false );break;
	case SPT_QA					: setType( SPC_UNKNOWN, SPSC_NOT_SET, false );break;
	case SPT_STAR_PN			: setType( SPC_STAR,	SPSC_NOT_SET, false );break;
	case SPT_STAR_CARBON		: setType( SPC_STAR,	SPSC_NOT_SET, false );break;
	case SPT_STAR_BROWN_DWARF	: setType( SPC_STAR,	SPSC_NOT_SET, false );break;
	case SPT_STAR_SUB_DWARF		: setType( SPC_STAR,	SPSC_NOT_SET, false );break;
	case SPT_STAR_CATY_VAR		: setType( SPC_STAR,	SPSC_NOT_SET, false );break;
	case SPT_STAR_RED_DWARF		: setType( SPC_STAR,	SPSC_NOT_SET, false );break;
	case SPT_STAR_WHITE_DWARF	: setType( SPC_STAR,	SPSC_NOT_SET, false );break;
	case SPT_STAR_BHB			: setType( SPC_STAR,	SPSC_NOT_SET, false );break;
	case SPT_BLAZAR				: setType( SPC_STAR,	SPSC_NOT_SET, false );break;
	case SPT_QSO_BAL			: setType( SPC_QSO,		SPSC_NOT_SET, false );break;
	default : m_Type = 0;
	}
}



std::string Spectra::getFileName() const
{
	// e.g. spSpec-52203-0716-002.fit
	std::string sstrFileName( "spSpec-" );

	// for spectra with unassigned plate/MJD/fiber ids we use the hash value.
	// This could be the case for artificial or light curve spectra.
	if ( m_version == SP_LIGHTCURVE_RADEC || m_version == SP_LIGHTCURVE_PLAIN ) {
		sstrFileName += Helpers::numberToString<uint64_t>(m_SpecObjID);
	} else {
		sstrFileName = getSpecObjFileName( getPlate(), getMJD(), getFiber(), (m_version != SP_VERSION_DR7) );
	} 

	return sstrFileName;
}


int Spectra::getMJD() const
{
	int mjd;

	if ( m_version == SP_LIGHTCURVE_RADEC || 
		 m_version == SP_LIGHTCURVE_PLAIN || 
		 m_version == SP_ARTIFICIAL ) {
		 mjd = 0;
	} else if ( m_version == SP_VERSION_DR7 || m_version == SP_LIGHTCURVE_SDSS || m_version == SP_CSV ) {
		// light curve spectra use DR7 specObj ID encoding
		mjd = static_cast<int>((m_SpecObjID&(uint64_t)0x0000FFFF00000000)>>(uint64_t)32);
	} else {
		// light curve spectra use DR8 and above specObj ID encoding
		mjd = static_cast<int>((m_SpecObjID&(uint64_t)0x0000003FFFC00000)>>(uint64_t)24)+50000;
	}

	return mjd;
}

int Spectra::getFiber() const
{
	int fiber;

	if ( m_version == SP_LIGHTCURVE_RADEC || 
		m_version == SP_LIGHTCURVE_PLAIN || 
		m_version == SP_ARTIFICIAL ) {
			fiber = 0;
	} else if ( m_version == SP_VERSION_DR7 || m_version == SP_LIGHTCURVE_SDSS || m_version == SP_CSV ) {
		// light curve spectra use DR7 specObj ID encoding
		fiber = static_cast<int>((m_SpecObjID&(uint64_t)0x00000000FFC00000)>>(uint64_t)22);
	} else {
		// light curve spectra use DR8 and above specObj ID encoding
 		fiber = static_cast<int>((m_SpecObjID&(uint64_t)0x0003FFC000000000)>>(uint64_t)38);
	}
	return fiber;
}

int Spectra::getPlate() const
{
	int plate;
	if ( m_version == SP_LIGHTCURVE_RADEC || 
		m_version == SP_LIGHTCURVE_PLAIN || 
		m_version == SP_ARTIFICIAL ) {
			plate = 0;
	} else if ( m_version == SP_VERSION_DR7 || m_version == SP_LIGHTCURVE_SDSS || m_version == SP_CSV ) {
		// light curve spectra use DR7 specObj ID encoding
		plate = static_cast<int>((m_SpecObjID&(uint64_t)0xFFFF000000000000)>>(uint64_t)48);
	} else {
		// light curve spectra use DR8 and above specObj ID encoding
		plate = static_cast<int>((m_SpecObjID&(uint64_t)0xFFFC000000000000)>>(uint64_t)50);
	}

	return plate;
}

double Spectra::getRa() const
{
	if ( m_version == SP_LIGHTCURVE_RADEC )
		return (double) m_Amplitude[numSamples-2];
	
	return 0.0;
}

double Spectra::getDec() const
{
	if ( m_version == SP_LIGHTCURVE_RADEC )
		return (double) m_Amplitude[numSamples-1];
	return 0.0;
}

std::string Spectra::getURL()const
{
	if ( m_SpecObjID == 0 || m_version == SP_ARTIFICIAL || m_version == SP_LIGHTCURVE_PLAIN || m_version == SP_VERSION_INVALID ) {
		return "";
	}


	if ( m_version == SP_LIGHTCURVE_RADEC ) 
	{
		// object explorer link for light curves where plate, mjd and fiber ID are missing.
		
		// e.g.: http://skyserver.sdss3.org/dr9/en/tools/explore/obj.asp?ra=6.04858449&dec=-0.73003035
		std::string sstrUrlObjectExplorer("http://skyserver.sdss3.org/dr9/en/tools/explore/obj.asp?ra=");
		sstrUrlObjectExplorer += Helpers::numberToString( getRa() );
		sstrUrlObjectExplorer += "&dec=";
		sstrUrlObjectExplorer += Helpers::numberToString( getDec() );
	
		return sstrUrlObjectExplorer;
	}
/*

	// check for DR1..DR7 spectra, they get the old overview page.
	if ( m_version == SP_VERSION_DR7 ) {
		std::string sstrUrlDR7("http://cas.sdss.org/dr7/en/tools/explore/obj.asp?sid=");
		sstrUrlDR7 += Helpers::numberToString( m_SpecObjID );
		return sstrUrlDR7;
	}

	if ( m_version == SP_VERSION_DR8 ) {
		std::string strUrlDR9("http://skyserver.sdss3.org/dr9/en/tools/explore/obj.asp?sid=");
		strUrlDR9 += Helpers::numberToString( m_SpecObjID );
		return strUrlDR9;
	}
*/


	// e.g.: interactive spectrum http://dr10.sdss3.org/spectrumDetail?mjd=55280&fiber=1&plateid=3863
	//       object explorer      http://skyserver.sdss.org/dr12/en/tools/explore/summary.aspx?plate=4031&mjd=55604&fiber=420 
//	std::string sstrUrlDR12("http://dr12.sdss3.org/spectrumDetail?plateid=");
//	std::string sstrUrlDR10("http://dr10.sdss3.org/spectrumDetail?plateid=");
	std::string sstrUrlDR12("http://skyserver.sdss.org/dr12/en/tools/explore/summary.aspx?plate=");
	std::string sstrUrlDR10("http://skyserver.sdss.org/dr10/en/tools/explore/summary.aspx?plate=");


	std::string sstrUrl(sstrUrlDR12);

	sstrUrl += Helpers::numberToString( getPlate() );
	sstrUrl += "&mjd=";
	sstrUrl += Helpers::numberToString( getMJD() );
	sstrUrl += "&fiber=";
	sstrUrl += Helpers::numberToString( getFiber() );

	return sstrUrl;
}

std::string Spectra::getImgURL() const
{
	if ( m_SpecObjID == 0 ) {
		return "http://upload.wikimedia.org/wikipedia/en/d/d4/Mickey_Mouse.png";
	}



// 	TODO:
// 	We have some problems with image links and ids:
// 	what we get:
// 	http://skyserver.sdss.org/dr12/en/get/SpecById.ashx?id=4565553074336625664
// 	what it should be:
// 	http://skyserver.sdss.org/dr12/en/get/SpecById.ashx?id=4565553074336886784


	//http://dr12.sdss3.org/sas/dr12/sdss/spectro/redux/images/v5_7_2/7339-56783/spec-image-7339-56783-0176.png

	std::string sstrUrlDR12new("http://dr12.sdss3.org/sas/dr12/sdss/spectro/redux/images/");
	std::string sstrUrlDR12("http://skyserver.sdss3.org/dr12/en/get/SpecById.ashx?id=");
	std::string sstrUrlDR10("http://skyserver.sdss3.org/dr10/en/get/SpecById.ashx?id=");
	std::string sstrUrlDR9("http://skyserver.sdss3.org/dr9/en/get/specById.asp?id=");
	std::string sstrUrlDR7("http://cas.sdss.org/dr7/en/get/specById.asp?id=");
	std::string sstrUrl(sstrUrlDR12);

  	if ( m_version == SP_VERSION_DR7 )
	{
 		sstrUrl = sstrUrlDR7;
		sstrUrl += Helpers::numberToString( m_SpecObjID );
		return sstrUrl;
	}

	const int plateId = getPlate();
	const int mjd = getMJD();
	const int fiberId = getFiber();

	std::string reduxPipelineVersion;

	// here are some old versions
// 	if ( plateId >=1960 && plateId <= 2912 )
// 	{
// 		reduxPipelineVersion = "103";
// 	}
// 	else if ( plateId >=2640 && plateId <= 3501 )
// 	{
// 		reduxPipelineVersion = "104";
// 	}

	if ( plateId >= 266 && plateId <= 3006 )
	{
		reduxPipelineVersion = "26";
	}
	else if ( plateId >=3520 && plateId <= 7457 )
	{
		reduxPipelineVersion = "v5_7_0";
	}
	else
	{
		// plateId 7339 ... 7565 
		reduxPipelineVersion = "v5_7_2";
	}


	sstrUrl = sstrUrlDR12new+reduxPipelineVersion+"/";
	sstrUrl += Helpers::numberToString( plateId,4 );
	sstrUrl += "-"; 
	sstrUrl += Helpers::numberToString( mjd, 5);
	sstrUrl += "/spec-image-"; 
	sstrUrl += Helpers::numberToString( plateId,4 );
	sstrUrl += "-"; 
	sstrUrl += Helpers::numberToString( mjd, 5 );
	sstrUrl += "-"; 
	sstrUrl += Helpers::numberToString( fiberId, 4 );
	sstrUrl += ".png"; 

	return sstrUrl;
}


bool Spectra::isEmpty() const
{
	return (m_SpecObjID == 0);
}

bool Spectra::hasBadPixels() const
{
	return (m_status > 0);
}

bool Spectra::checkConsistency() const
{
	if (( isnan( m_Min ) || isinf(m_Min) ) ||
		( isnan( m_Max ) || isinf(m_Max) ) ||
		( isnan( m_Z ) || isinf(m_Z) ) ||
		( isnan( m_flux ) || isinf(m_flux) ))
	{
		return false;
	}
		
	for (size_t i=0;i<Spectra::numSamples;i++)
	{
		const double a = static_cast<double>(m_Amplitude[i]);
		if ( isnan( a ) || isinf(a) )
		{
			return false;
		}
		// check for insane high numbers
		if ( fabs(a) > 1000000000000.0 )
		{
			return false;
		}
	}
	return true;
}

Spectra::SpectraClass Spectra::getClass() const
{
	SpectraClass retVal = (SpectraClass) (m_Type & 0x07);
	return retVal;
}

Spectra::SpectraSubClass Spectra::getSubClass() const
{
	SpectraSubClass retVal = (SpectraSubClass) ((m_Type >> 3) & 0x7f);
	return retVal;
}


bool Spectra::isBroadline() const 
{
	bool retVal = (m_Type & 1024) > 0;
	return retVal;
}


bool Spectra::matchesFilter( unsigned int spClassFilter, unsigned int spSubClassFilter )
{
	unsigned int spClass = 1<<(m_Type & 0x07);
	bool retVal = ( (spClass & spClassFilter) > 0);

	// TODO: add sub class filter.

	return retVal;
}

uint64_t Spectra::calcPhotoObjID( int _run, int _rerun, int _camcol, int _field, int _obj )
{
	// taken from: http://cas.sdss.org/dr6/en/help/docs/algorithm.asp#O
	//Bits #of bits Mask				Assignment 	Description
	//0 		1 	0x8000000000000000 	empty 		unassigned
	//1-4 		4 	0x7800000000000000 	skyVersion 	resolved sky version (0=TARGET, 1=BEST, 2-15=RUNS)
	//5-15 		11 	0x07FF000000000000 	rerun 		number of pipeline rerun
	//16-31 	16 	0x0000FFFF00000000 	run 		run number
	//32-34 	3 	0x00000000E0000000 	camcol 		camera column (1-6)
	//35 		1 	0x0000000010000000 	firstField 	is this the first field in segment?
	//36-47 	12 	0x000000000FFF0000 	field 		field number within run
	//48-63 	16 	0x000000000000FFFF 	object 		object number within field}

	uint64_t photoObjID;
	photoObjID  = (uint64_t) 0x0800000000000000; // 1=best
	photoObjID |= (uint64_t) 0x07FF000000000000&((uint64_t)_rerun<<48);
	photoObjID |= (uint64_t) 0x0000FFFF00000000&((uint64_t)_run<<32);
	photoObjID |= (uint64_t) 0x00000000E0000000&((uint64_t)_camcol<<29);
	photoObjID |= (uint64_t) 0x000000000FFF0000&((uint64_t)_field<<16);
	photoObjID |= (uint64_t) 0x000000000000FFFF&((uint64_t)_obj);

	return photoObjID;
}

uint64_t Spectra::calcSpecObjID_DR7( int _plate, int _mjd, int _fiber, int _type )
{
	// taken from: http://cas.sdss.org/dr6/en/help/docs/algorithm.asp#O
	//Bits #of bits Mask				Assignment 			 Description
	//0-15 		16	0xFFFF000000000000 	plate 				 number of spectroscopic plate
	//16-31		16	0x0000FFFF00000000 	MJD 				 MJD (date) plate was observed
	//32-41		10	0x00000000FFC00000 	fiberID 			 number of spectroscopic fiber on plate (1-640)
	//42-47		6 	0x00000000003F0000 	type 				 type of targeted object
	//48-63		16	0x000000000000FFFF 	line/redshift/index  0 for SpecObj, else number of spectroscopic line (SpecLine) or index (SpecLineIndex) or redshift (ELRedhsift or XCRedshift)

	uint64_t specObjID;
	specObjID  = (uint64_t) 0xFFFF000000000000&((uint64_t)_plate<<48);
	specObjID |= (uint64_t) 0x0000FFFF00000000&((uint64_t)_mjd<<32);
	specObjID |= (uint64_t) 0x00000000FFC00000&((uint64_t)_fiber<<22);
	specObjID |= (uint64_t) 0x00000000003F0000&((uint64_t)_type<<16);
	specObjID |= (uint64_t) 0x000000000000FFFF&((uint64_t)0);

	return specObjID;
}


uint64_t Spectra::calcSpecObjID_DR8( int _plate, int _mjd, int _fiber, int _run2d_m, int _run2d_n, int _run2d_p )
{
	if ( _mjd < 50000 || _run2d_n < 5 )
		return 0;

	// taken from http://data.sdss3.org/datamodel/glossary.html
	// Note that this definition differs from that in DR7. Note that implicitly MJD>=50000.
	//  Bits   #of bits    Mask				   Assignment 			 Description
	//	0-9    10          0xFFC0000000000000  line/redshift index   0 all for SpecObj
	//	10-23  14                              run2d                 rerun number of pipeline reduction
	//	24-37  14                              MJD                   (date) of plugging minus 50000 
	//	38-49  12                              fiber id number 
	//	50-63  14                              plate id number 
	//
	// 	run2d can be an integer, like 26, 
	// 	or a string of the form 'vN_M_P', 
	// 	where N, M and P are integers, 
	// 		with the restriction 5<=N<=6, 0<=M<=99, and 0<=P<=99. 
	// 		This is understood to be the run2d value for a spectrum. 
	// 		In the latter case, the 14 bits corresponding to run2d are filled with (N-5)*10000+M*100+P.
	unsigned int run2d = (_run2d_n-5)*10000+_run2d_m*100+_run2d_p;

	uint64_t specObjID;
	
	_mjd -= 50000;
	                  
	specObjID  = (uint64_t) 0xFFFC000000000000&((uint64_t)_plate<<50);
	specObjID |= (uint64_t) 0x0003FFC000000000&((uint64_t)_fiber<<38);
	specObjID |= (uint64_t) 0x0000003FFFC00000&((uint64_t)_mjd<<24);
	specObjID |= (uint64_t) 0x00000000003FFC00&((uint64_t)run2d<<10);
	specObjID |= (uint64_t) 0x00000000000003FF&((uint64_t)0);


	return specObjID;
}


std::string Spectra::plateToString( int _plate )
{
	char buf[32]={0};
	sprintf( buf, "%04i", _plate );
	return std::string(buf);
}




float Spectra::indexToWaveLength( size_t _index, float _waveBegin, float _waveEnd, int _numSamples )
{
	float d = (_waveEnd-_waveBegin)/static_cast<float>(_numSamples);
	return _waveBegin+static_cast<float>(_index)*d;
}

size_t Spectra::waveLengthToIndex( float _waveLength, float _waveBegin, float _waveEnd, int _numSamples )
{
	float d = (_waveEnd-_waveBegin);
	d = (_waveLength-_waveBegin)/d;
	d = CLAMP( d, 0.f, 1.f );
	return static_cast<int>(floorf(d*static_cast<float>(_numSamples)));
}


float Spectra::waveLenghtToRestFrame( float _waveLength, float _z )
{
	if ( _z <= -1.0 ) {
		return FLT_MAX;
	}
	return _waveLength/(1.f+_z);
}



std::string Spectra::spectraFilterToString( unsigned int _spectraFilter )
{
	std::string sstrOutString;
	if ( _spectraFilter & (1 << Spectra::SPC_UNKNOWN) )
	{
		sstrOutString += "SPEC_UNKNOWN ";
	}
	if ( _spectraFilter & (1 << Spectra::SPC_STAR) )
	{
		sstrOutString += "SPEC_STAR ";
	}
	if ( _spectraFilter & (1 << Spectra::SPC_GALAXY) )
	{
		sstrOutString += "SPEC_GALAXY ";
	}
	if ( _spectraFilter & (1 << Spectra::SPC_QSO) )
	{
		sstrOutString += "SPEC_QSO ";
	}


	return sstrOutString;
}



Spectra::OldSpectraType Spectra::spectraTypeFromString( const std::string &_spectraType )
{
	std::string ssstrSpectraType( Helpers::upperCase(_spectraType) );

	if ( ssstrSpectraType == "GALAXY" )
		return SPT_GALAXY;

	if ( ssstrSpectraType == "STAR" )
		return SPT_STAR;

	if ( ssstrSpectraType == "QSO" )
		return SPT_QSO;

	if ( ssstrSpectraType == "HIZQSO" || ssstrSpectraType == "HIZ_QSO" )
		return SPT_HIZ_QSO;

	if ( ssstrSpectraType == "SKY" )
		return SPT_SKY;

	if ( ssstrSpectraType == "STAR_LATE" || ssstrSpectraType == "LATESTAR"  || ssstrSpectraType == "LATE_STAR" )
		return SPT_STAR_LATE;


	if ( ssstrSpectraType == "QA" )
		return SPT_QA;

	if ( ssstrSpectraType == "STAR_PN" )
		return SPT_STAR_PN;

	if ( ssstrSpectraType == "STAR_CARBON" )
		return SPT_STAR_CARBON;

	if ( ssstrSpectraType == "STAR_BROWN_DWARF" )
		return SPT_STAR_BROWN_DWARF;

	if ( ssstrSpectraType == "STAR_SUB_DWARF" )
		return SPT_STAR_SUB_DWARF;

	if ( ssstrSpectraType == "STAR_CATY_VAR" )
		return SPT_STAR_CATY_VAR;

	if ( ssstrSpectraType == "SPT_STAR_RED_DWARF" )
		return SPT_STAR_RED_DWARF;

	if ( ssstrSpectraType == "STAR_WHITE_DWARF" )
		return SPT_STAR_WHITE_DWARF;

	if ( ssstrSpectraType == "STAR_BHB" )
		return SPT_STAR_BHB;

	if ( ssstrSpectraType == "ROSAT_A" )
		return SPT_ROSAT_A;
	if ( ssstrSpectraType == "ROSAT_B" )
		return SPT_ROSAT_B;
	if ( ssstrSpectraType == "ROSAT_C" )
		return SPT_ROSAT_C;
	if ( ssstrSpectraType == "ROSAT_D" )
		return SPT_ROSAT_D;

	if ( ssstrSpectraType == "SPECTROPHOTO_STD" )
		return SPT_SPECTROPHOTO_STD;

	if ( ssstrSpectraType == "HOT_STD" )
		return SPT_HOT_STD;

	if ( ssstrSpectraType == "SERENDIPITY_BLUE" )
		return SPT_SERENDIPITY_BLUE;

	if ( ssstrSpectraType == "SERENDIPITY_FIRST" )
		return SPT_SERENDIPITY_FIRST;

	if ( ssstrSpectraType == "SERENDIPITY_RED" )
		return SPT_SERENDIPITY_RED;

	if ( ssstrSpectraType == "SERENDIPITY_DISTANT" )
		return SPT_SERENDIPITY_DISTANT;

	if ( ssstrSpectraType == "SERENDIPITY_MANUAL" )
		return SPT_SERENDIPITY_MANUAL;

	if ( ssstrSpectraType == "REDDEN_STD" )
		return SPT_REDDEN_STD;

	// new DR10 source types
	//////////////////////////////////////////////////////////

	if ( ssstrSpectraType == "BLAZGRFLAT" )
		return SPT_BLAZAR;
	if ( ssstrSpectraType == "BLAZGRQSO" )
		return SPT_BLAZAR;
	if ( ssstrSpectraType == "BLAZGVAR" )
		return SPT_BLAZAR;
	if ( ssstrSpectraType == "BLAZGX" )
		return SPT_BLAZAR;
	if ( ssstrSpectraType == "BLAZGXQSO" )
		return SPT_BLAZAR;
	if ( ssstrSpectraType == "BLAZGXR" )
		return SPT_BLAZAR;
	if ( ssstrSpectraType == "BLAZR" )
		return SPT_BLAZAR;
	if ( ssstrSpectraType == "BLAZXR" )
		return SPT_BLAZAR;
	if ( ssstrSpectraType == "BLAZXRSAM" )
		return SPT_BLAZAR;
	if ( ssstrSpectraType == "BLAZXRVAR" )
		return SPT_BLAZAR;

	
	if ( ssstrSpectraType == "LBQSBAL" )
		return SPT_QSO_BAL;
	if ( ssstrSpectraType == "FBQSBAL" )
		return SPT_QSO_BAL;
	if ( ssstrSpectraType == "ODDBAL" )
		return SPT_QSO_BAL;
	if ( ssstrSpectraType == "OTBAL" )
		return SPT_QSO_BAL;
	if ( ssstrSpectraType == "VARBAL" )
		return SPT_QSO_BAL;
	if ( ssstrSpectraType == "PREVBAL" )
		return SPT_QSO_BAL;
	
	

	if ( ssstrSpectraType == "HIZQSO82" )
		return SPT_HIZ_QSO;
	if ( ssstrSpectraType == "QSO_HIZ" )
		return SPT_HIZ_QSO;
	if ( ssstrSpectraType == "HIZQSOIR" )
		return SPT_HIZ_QSO;

	if ( ssstrSpectraType == "QSO_AAL" )
		return SPT_QSO;
	if ( ssstrSpectraType == "QSO_AALs" )
		return SPT_QSO;
	if ( ssstrSpectraType == "QSO_GRI" )
		return SPT_QSO;
	if ( ssstrSpectraType == "QSO_IAL" )
		return SPT_QSO;
	if ( ssstrSpectraType == "QSO_RADIO" )
		return SPT_QSO;
	if ( ssstrSpectraType == "QSO_RADIO_AAL" )
		return SPT_QSO;
	if ( ssstrSpectraType == "QSO_RADIO_IAL" )
		return SPT_QSO;
	if ( ssstrSpectraType == "QSO_RIZ" )
		return SPT_QSO;
	if ( ssstrSpectraType == "QSO_SUPPZ" )
		return SPT_QSO;
	if ( ssstrSpectraType == "QSO_VAR_FPG" )
		return SPT_QSO;
	if ( ssstrSpectraType == "QSO_VAR_SDSS" )
		return SPT_QSO;
	if ( ssstrSpectraType == "QSO_WISE_SUPP" )
		return SPT_QSO;
	if ( ssstrSpectraType == "QSO_noAALs" )
		return SPT_QSO;
	if ( ssstrSpectraType == "KQSO_BOSS" )
		return SPT_QSO;
	if ( ssstrSpectraType == "QSO_VAR" )
		return SPT_QSO;
	if ( ssstrSpectraType == "RADIO_2LOBE_QSO" )
		return SPT_QSO;
	if ( ssstrSpectraType == "RQSS_SF" )
		return SPT_QSO;
	if ( ssstrSpectraType == "RQSS_SFC" )
		return SPT_QSO;
	if ( ssstrSpectraType == "RQSS_STM" )
		return SPT_QSO;
	if ( ssstrSpectraType == "RQSS_STMC" )
		return SPT_QSO;
	
	if ( ssstrSpectraType == "GAL_NEAR_QSO" )
		return SPT_GALAXY;
	if ( ssstrSpectraType == "SN_GAL1" )
		return SPT_GALAXY;
	if ( ssstrSpectraType == "SN_GAL2" )
		return SPT_GALAXY;
	if ( ssstrSpectraType == "SN_GAL3" )
		return SPT_GALAXY;
	if ( ssstrSpectraType == "BRIGHTGAL" )
		return SPT_GALAXY;
	if ( ssstrSpectraType == "LRG" )
		return SPT_GALAXY;
	

	if ( ssstrSpectraType == "WHITEDWARF_NEW" )
		return SPT_STAR_WHITE_DWARF;
	if ( ssstrSpectraType == "WHITEDWARF_SDSS" )
		return SPT_STAR_WHITE_DWARF;

	if ( ssstrSpectraType == "AMC" )
		return SPT_EXOTIC;
	if ( ssstrSpectraType == "BLUE_RADIO" )
		return SPT_EXOTIC;
	if ( ssstrSpectraType == "CHANDRAv1" )
		return SPT_EXOTIC;
	if ( ssstrSpectraType == "CXOBRIGHT" )
		return SPT_EXOTIC;
	if ( ssstrSpectraType == "CXOGRIZ" )
		return SPT_EXOTIC;
	if ( ssstrSpectraType == "CXORED" )
		return SPT_EXOTIC;
	if ( ssstrSpectraType == "ELG" )
		return SPT_EXOTIC;
	if ( ssstrSpectraType == "FLARE1" )
		return SPT_EXOTIC;
	if ( ssstrSpectraType == "FLARE2" )
		return SPT_EXOTIC;
	if ( ssstrSpectraType == "HPM" )
		return SPT_EXOTIC;
	if ( ssstrSpectraType == "LOW_MET" )
		return SPT_EXOTIC;
	if ( ssstrSpectraType == "ELG" )
		return SPT_EXOTIC;
	if ( ssstrSpectraType == "NA" )
		return SPT_EXOTIC;
	if ( ssstrSpectraType == "MTEMP" )
		return SPT_EXOTIC;
	if ( ssstrSpectraType == "XMMBRIGHT" )
		return SPT_EXOTIC;
	if ( ssstrSpectraType == "XMMGRIZ" )
		return SPT_EXOTIC;
	if ( ssstrSpectraType == "XMMHR" )
		return SPT_EXOTIC;
	if ( ssstrSpectraType == "XMMRED" )
		return SPT_EXOTIC;
	if ( ssstrSpectraType == "RVTEST" )
		return SPT_EXOTIC;
	if ( ssstrSpectraType == "RED_KG" )
		return SPT_EXOTIC;

	if ( ssstrSpectraType == "SN_LOC" )
		return SPT_EXOTIC;
	if ( ssstrSpectraType == "SPEC_SN" )
		return SPT_EXOTIC;
	if ( ssstrSpectraType == "SPOKE" )
		return SPT_EXOTIC;
	if ( ssstrSpectraType == "STANDARD" )
		return SPT_EXOTIC;
	if ( ssstrSpectraType == "STD" )
		return SPT_EXOTIC;
	if ( ssstrSpectraType == "STRIPE82BCG" )
		return SPT_EXOTIC;
	if ( ssstrSpectraType == "VARS" )
		return SPT_EXOTIC;
	if ( ssstrSpectraType == "BRIGHTERL" )
		return SPT_EXOTIC;
	if ( ssstrSpectraType == "BRIGHTERM" )
		return SPT_EXOTIC;
	if ( ssstrSpectraType == "FAINTERL" )
		return SPT_EXOTIC;
	if ( ssstrSpectraType == "FAINTERM" )
		return SPT_EXOTIC;

	// i.e. NONLEGACY.
	return SPT_UNKNOWN;
}
		
	
Spectra::SpectraClass Spectra::spectraClassFromString( const std::string &_spectraClass )
{
	std::string ssstrSpectraClass( Helpers::upperCase(_spectraClass) );

	if  (ssstrSpectraClass == "STAR")
		return SPC_STAR;
	
	if  (ssstrSpectraClass == "GALAXY")
		return SPC_GALAXY;
	
	if  (ssstrSpectraClass == "QSO")
		return SPC_QSO;

	return SPC_UNKNOWN;
}

bool Spectra::spectraSubClassHasBroadlineFromString( const std::string &_spectraSubClass )
{
	std::string ssstrSpectraSubClass( Helpers::upperCase(_spectraSubClass) );

	if  (ssstrSpectraSubClass.find("BROADLINE")!=std::string::npos)
		return true;

	return false;

}

unsigned int Spectra::packSpectraClassAndSubclass( SpectraClass spClass, SpectraSubClass spSubClass, bool hasBroadline )
{
	int retVal = (spSubClass & 0x7f) << 3 | (spClass & 0x07);
	if ( hasBroadline ) {
		retVal |= 1024;
	}

	return retVal;
}


Spectra::SpectraSubClass Spectra::spectraSubClassFromString( const std::string &_spectraSubClass )
{
	std::string ssstrSpectraSubClass( Helpers::upperCase(_spectraSubClass) );

	if  (ssstrSpectraSubClass.find("AGN")==0)
		return SPSC_AGN;
	if  (ssstrSpectraSubClass.find("STARBURST")==0)
		return SPSC_STARBURST;
	if  (ssstrSpectraSubClass.find("STARFORMING")==0)
		return SPSC_STARFORMING;

	if  (ssstrSpectraSubClass.find("A0p")==0)
		return SPSC_A0p;
	if  (ssstrSpectraSubClass.find("A0")==0)
		return SPSC_A0;
	if  (ssstrSpectraSubClass.find("A1")==0)
		return SPSC_A1;
	if  (ssstrSpectraSubClass.find("A2")==0)
		return SPSC_A2;
	if  (ssstrSpectraSubClass.find("A3")==0)
		return SPSC_A3;
	if  (ssstrSpectraSubClass.find("A4")==0)
		return SPSC_A4;
	if  (ssstrSpectraSubClass.find("A5")==0)
		return SPSC_A5;
	if  (ssstrSpectraSubClass.find("A6")==0)
		return SPSC_A6;
	if  (ssstrSpectraSubClass.find("A7")==0)
		return SPSC_A7;
	if  (ssstrSpectraSubClass.find("A8")==0)
		return SPSC_A8;
	if  (ssstrSpectraSubClass.find("A9")==0)
		return SPSC_A9;

	if  (ssstrSpectraSubClass.find("B0")==0)
		return SPSC_B0;
	if  (ssstrSpectraSubClass.find("B1")==0)
		return SPSC_B1;
	if  (ssstrSpectraSubClass.find("B2")==0)
		return SPSC_B2;
	if  (ssstrSpectraSubClass.find("B3")==0)
		return SPSC_B3;
	if  (ssstrSpectraSubClass.find("B4")==0)
		return SPSC_B4;
	if  (ssstrSpectraSubClass.find("B5")==0)
		return SPSC_B5;
	if  (ssstrSpectraSubClass.find("B6")==0)
		return SPSC_B6;
	if  (ssstrSpectraSubClass.find("B7")==0)
		return SPSC_B7;
	if  (ssstrSpectraSubClass.find("B8")==0)
		return SPSC_B8;
	if  (ssstrSpectraSubClass.find("B9")==0)
		return SPSC_B9;


	if  (ssstrSpectraSubClass.find("OB")==0)
		return SPSC_OB;
	if  (ssstrSpectraSubClass.find("O8")==0)
		return SPSC_O8;
	if  (ssstrSpectraSubClass.find("O9")==0)
		return SPSC_O9;
	if  (ssstrSpectraSubClass.find("O")==0)
		return SPSC_O;

	if  (ssstrSpectraSubClass.find("F0")==0)
		return SPSC_F0;
	if  (ssstrSpectraSubClass.find("F1")==0)
		return SPSC_F1;
	if  (ssstrSpectraSubClass.find("F2")==0)
		return SPSC_F2;
	if  (ssstrSpectraSubClass.find("F3")==0)
		return SPSC_F3;
	if  (ssstrSpectraSubClass.find("F4")==0)
		return SPSC_F4;
	if  (ssstrSpectraSubClass.find("F5")==0)
		return SPSC_F5;
	if  (ssstrSpectraSubClass.find("F6")==0)
		return SPSC_F6;
	if  (ssstrSpectraSubClass.find("F7")==0)
		return SPSC_F7;
	if  (ssstrSpectraSubClass.find("F8")==0)
		return SPSC_F8;
	if  (ssstrSpectraSubClass.find("F9")==0)
		return SPSC_F9;

	if  (ssstrSpectraSubClass.find("G0")==0)
		return SPSC_G0;
	if  (ssstrSpectraSubClass.find("G1")==0)
		return SPSC_G2;
	if  (ssstrSpectraSubClass.find("G2")==0)
		return SPSC_G2;
	if  (ssstrSpectraSubClass.find("G3")==0)
		return SPSC_G3;
	if  (ssstrSpectraSubClass.find("G4")==0)
		return SPSC_G4;
	if  (ssstrSpectraSubClass.find("G5")==0)
		return SPSC_G5;
	if  (ssstrSpectraSubClass.find("G6")==0)
		return SPSC_G6;
	if  (ssstrSpectraSubClass.find("G7")==0)
		return SPSC_G7;
	if  (ssstrSpectraSubClass.find("G8")==0)
		return SPSC_G8;
	if  (ssstrSpectraSubClass.find("G9")==0)
		return SPSC_G9;

	if  (ssstrSpectraSubClass.find("K0")==0)
		return SPSC_K0;
	if  (ssstrSpectraSubClass.find("K1")==0)
		return SPSC_K1;
	if  (ssstrSpectraSubClass.find("K2")==0)
		return SPSC_K2;
	if  (ssstrSpectraSubClass.find("K3")==0)
		return SPSC_K3;
	if  (ssstrSpectraSubClass.find("K4")==0)
		return SPSC_K4;
	if  (ssstrSpectraSubClass.find("K5")==0)
		return SPSC_K5;
	if  (ssstrSpectraSubClass.find("K6")==0)
		return SPSC_K6;
	if  (ssstrSpectraSubClass.find("K7")==0)
		return SPSC_K7;
	if  (ssstrSpectraSubClass.find("K8")==0)
		return SPSC_K8;
	if  (ssstrSpectraSubClass.find("K9")==0)
		return SPSC_K9;


	if  (ssstrSpectraSubClass.find("M0V")==0)
		return SPSC_M0V;
	if  (ssstrSpectraSubClass.find("M2V")==0)
		return SPSC_M2V;
	if  (ssstrSpectraSubClass.find("M1")==0)
		return SPSC_M1;
	if  (ssstrSpectraSubClass.find("M2")==0)
		return SPSC_M2;
	if  (ssstrSpectraSubClass.find("M3")==0)
		return SPSC_M3;
	if  (ssstrSpectraSubClass.find("M4")==0)
		return SPSC_M4;
	if  (ssstrSpectraSubClass.find("M5")==0)
		return SPSC_M5;
	if  (ssstrSpectraSubClass.find("M6")==0)
		return SPSC_M6;
	if  (ssstrSpectraSubClass.find("M7")==0)
		return SPSC_M7;
	if  (ssstrSpectraSubClass.find("M8")==0)
		return SPSC_M8;
	if  (ssstrSpectraSubClass.find("M9")==0)
		return SPSC_M9;

	
	if  (ssstrSpectraSubClass.find("LDWARF")==0)
		return SPSC_LDWARF;
	if  (ssstrSpectraSubClass.find("L5.5")==0)
		return SPSC_L55;
	if  (ssstrSpectraSubClass.find("L0")==0)
		return SPSC_L0;
	if  (ssstrSpectraSubClass.find("L1")==0)
		return SPSC_L1;
	if  (ssstrSpectraSubClass.find("L2")==0)
		return SPSC_L2;
	if  (ssstrSpectraSubClass.find("L3")==0)
		return SPSC_L3;
	if  (ssstrSpectraSubClass.find("L4")==0)
		return SPSC_L4;
	if  (ssstrSpectraSubClass.find("L5")==0)
		return SPSC_L5;
	if  (ssstrSpectraSubClass.find("L6")==0)
		return SPSC_L6;
	if  (ssstrSpectraSubClass.find("L7")==0)
		return SPSC_L7;
	if  (ssstrSpectraSubClass.find("L8")==0)
		return SPSC_L8;
	if  (ssstrSpectraSubClass.find("L9")==0)
		return SPSC_L9;

	if  (ssstrSpectraSubClass.find("T2")==0)
		return SPSC_T2;

	if  (ssstrSpectraSubClass.find("CARBON_LINES")==0)
		return SPSC_CARBON_LINES;
	if  (ssstrSpectraSubClass.find("CARBONWD")==0)
		return SPSC_CARBONWD;
	if  (ssstrSpectraSubClass.find("CARBON")==0)
		return SPSC_CARBON;

	if  (ssstrSpectraSubClass.find("CV")==0)
		return SPSC_CV;

	if  (ssstrSpectraSubClass.find("WDCOOLER")==0)
		return SPSC_WDCOOLER;
	if  (ssstrSpectraSubClass.find("WDHOTTER")==0)
		return SPSC_WDHOTTER;
	if  (ssstrSpectraSubClass.find("WDMAGNETIC")==0)
		return SPSC_WDMAGNETIC;
	if  (ssstrSpectraSubClass.find("WD")==0)
		return SPSC_WD;

	return SPSC_NOT_SET;
}


std::string Spectra::spectraVersionToString( SpectraVersion _spectraVersion )
{
	switch ( _spectraVersion )
	{
	case SP_VERSION_INVALID		: return "Invalid";
	case SP_ARTIFICIAL			: return "Artificial";
	case SP_CSV					: return "Comma Separated Values (CSV)";
	case SP_VERSION_DR7			: return "DR7 and below";
	case SP_VERSION_DR8			: return "DR8/DR9";
	case SP_VERSION_BOSS		: return "DR9/DR10 BOSS";
	case SP_VERSION_APOGEE		: return "DR10 APOGEE";
	case SP_LIGHTCURVE_SDSS		: return "Light Curve / XML with SDSS plate, MJD and fiber ID";
	case SP_LIGHTCURVE_RADEC	: return "Light Curve / XML with ra/dec";
	case SP_LIGHTCURVE_PLAIN	: return "Light Curve / XML without any associated info";
	}
	return "Spectra version that should not exist";
}



std::string Spectra::getSpecObjFileName( int _plate, int _mjd, int _fiberID, bool dr8 )
{
	std::string sstrFileName;
	if ( dr8 )
	{
		// dr8 and above
		// spec-plate-MJD-fiber.fits
		// e.g. spec-0281-51614-0579.fits
		sstrFileName = "spec-";

		char buf[64];

		sprintf_s( buf, "%04i", _plate );
		sstrFileName += buf;
		sstrFileName += "-";

		sprintf_s( buf, "%05i", _mjd );
		sstrFileName += buf;
		sstrFileName += "-";

		sprintf_s( buf, "%04i", _fiberID );
		sstrFileName += buf;
		sstrFileName += ".fits";

	}
	else
	{
		// dr7 and below
		// spSpec-MJD-plate-fiber.fit
		// e.g. spSpec-52203-0716-002.fit
		sstrFileName = "spSpec-";

		char buf[64];
		sprintf_s( buf, "%05i", _mjd );
		sstrFileName += buf;
		sstrFileName += "-";

		sprintf_s( buf, "%04i", _plate );
		sstrFileName += buf;
		sstrFileName += "-";

		sprintf_s( buf, "%03i", _fiberID );
		sstrFileName += buf;
		sstrFileName += ".fit";
	}
	
	return sstrFileName;
}





int Spectra::getSDSSSpectraOffsetStart()
{
	int offset = 0;
	// if we use BOSS and SDSS spectra combined add spectrum at right offset.
	// BOSS spectra start at 3650A, SDSS spectra at 3800A -> thus use offset ~13 so both types operate in equal wavelengths
	if ( numSamples == (numSamplesBOSS/reductionFactor) )
	{
		const float wavelenPerPixel = (float)reductionFactor*(float)(waveLenEndBOSS-waveLenStartBOSS)/(float)numSamplesBOSS;
		offset = (float)(waveLenStartSDSS-waveLenStartBOSS)/wavelenPerPixel;
	}
	return offset;
}


int Spectra::getSDSSSpectraOffsetEnd()
{
	int offset = numSamples;
	// if we use BOSS and SDSS spectra combined add spectrum at right offset.
	// BOSS spectra start at 3650A, SDSS spectra at 3800A -> thus use offset ~13 so both types operate in equal wavelengths
	if ( numSamples == (numSamplesBOSS/reductionFactor) )
	{
		const float wavelenPerPixel = (float)reductionFactor*(float)(waveLenEndBOSS-waveLenStartBOSS)/(float)numSamplesBOSS;
		offset = (float)(waveLenEndSDSS-waveLenStartSDSS)/wavelenPerPixel;
	}
	return offset;
}


void Spectra::setOperationRange( SpectraOperationRange _operationRange )
{
	if ( _operationRange == SP_OPERATION_BOSS )
	{
		pixelStart	= 0;
		pixelEnd	= numSamples;	
	}
	else if ( _operationRange == SP_OPERATION_LIGHTCURVES )
	{
		pixelStart = 0;
		pixelEnd = numSamples-2;
	}
	else
	{
		// SDSS
		// pixel start must be aligned to 16 byte boundaries for SSE operations.
		pixelStart	= (getSDSSSpectraOffsetStart()>>2)<<2;  
		pixelEnd	= getSDSSSpectraOffsetEnd();	
	}
	assert( pixelStart<pixelEnd );
	assert( pixelEnd<=numSamples );
	assert( (pixelStart%4)==0 );
}