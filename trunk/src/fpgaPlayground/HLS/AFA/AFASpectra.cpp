#include "include/AFASpectra.h"
#include "include/AFADefines.h"
#include "include/AFARandom.h"

#include <float.h>
#include <assert.h>
#include <math.h>

int AFASpectra::pixelStart				= 0;		
int AFASpectra::pixelEnd				= AFASpectra::numSamples;		

AFASpectra::AFASpectra()
{
}

AFASpectra::AFASpectra( volatile AFASpectra *_source )
{
    set( _source );
}

AFASpectra::~AFASpectra()
{
}




int AFASpectra::getMJD() const
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

int AFASpectra::getFiber() const
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

int AFASpectra::getPlate() const
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



bool AFASpectra::isEmpty() volatile
{
	return (m_SpecObjID == 0);
}




AFASpectra::SpectraClass AFASpectra::getClass() const
{
	SpectraClass retVal = (SpectraClass) (m_Type & 0x07);
	return retVal;
}

AFASpectra::SpectraSubClass AFASpectra::getSubClass() const
{
	SpectraSubClass retVal = (SpectraSubClass) ((m_Type >> 3) & 0x7f);
	return retVal;
}


bool AFASpectra::isBroadline() const 
{
	bool retVal = (m_Type & 1024) > 0;
	return retVal;
}





void AFASpectra::clear() volatile
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

    for (size_t i=0;i<AFASpectra::numSamples;i++)
    {
        m_Amplitude[i] = 0.0f;
    }

}


void AFASpectra::setSine( float _frequency, float _phase, float _amplitude, float _noize ) volatile
{
	static size_t UIDCount = 1;
	m_SpecObjID =(UIDCount++)<<22;
	m_version = SP_ARTIFICIAL;

	AFARnd r;
	for (size_t i=0;i<AFASpectra::numSamples;i++)
	{
		float x=_phase+(float)(i)*_frequency;
		m_Amplitude[i] = sinf(x)*_amplitude+(r.randomFloat()-0.5f)*_noize;
	}
	m_SamplesRead = AFASpectra::numSamples;
	calcMinMax();
}

void AFASpectra::randomize(float minrange, float maxrange ) volatile
{
    if ( minrange > maxrange )
    {
        float hui = maxrange;
        maxrange = minrange;
        minrange = hui;
    }

    float d = maxrange-minrange;

    AFARnd r;

    for (size_t i=0;i<AFASpectra::numSamples;i++)
    {
        m_Amplitude[i] = r.randomFloat()*d+minrange;
    }
    m_SamplesRead = AFASpectra::numSamples;
    calcMinMax();
}

void AFASpectra::set( volatile AFASpectra *_spectra) volatile
{
    m_SamplesRead		= _spectra->m_SamplesRead;
    m_Min				= _spectra->m_Min;
    m_Max				= _spectra->m_Max;
    m_Index				= _spectra->m_Index;
    m_SpecObjID			= _spectra->m_SpecObjID;
    m_version			= _spectra->m_version;
    m_Type				= _spectra->m_Type;
    m_Z					= _spectra->m_Z;
    m_flux				= _spectra->m_flux;
    m_status			= _spectra->m_status;

    for (size_t i=0;i<AFASpectra::numSamples;i++)
    {
        m_Amplitude[i] = _spectra->m_Amplitude[i];
    }
}


void AFASpectra::normalizeByFlux() volatile
{
    calculateFlux();

    if ( m_flux <= 0.0f )
        return;

    for (int i=0;i<AFASpectra::numSamples;i++)
    {	
        m_Amplitude[i] /= (m_flux*0.001f);
    }
    calcMinMax();
}



float AFASpectra::compare(volatile AFASpectra *_spectra) volatile
{
	// c-version (slow)
	float error=0.0f;
	for (int i=AFASpectra::pixelStart;i<AFASpectra::pixelEnd;i++)
	{
		float d = m_Amplitude[i]-_spectra->m_Amplitude[i];
		error += d*d;
	}

	return error;
}



void AFASpectra::adapt( volatile AFASpectra *_spectra, float _adaptionRate ) volatile
{
	// c-version (slow)
	for ( int i=AFASpectra::pixelStart;i<AFASpectra::pixelEnd;i++ )
	{
		m_Amplitude[i] += _adaptionRate*(_spectra->m_Amplitude[i]-m_Amplitude[i]);
	}

}

void AFASpectra::calculateFlux() volatile
{
    double flux = 0.0;
    float offset = (m_Min < 0.0f) ? -m_Min : 0.0f;
    for (size_t i=0;i<AFASpectra::numSamples;i++)
    {
        flux += static_cast<double>(m_Amplitude[i]+offset);
    }
    m_flux = static_cast<float>(flux);
}

void AFASpectra::calcMinMax() volatile
{
    m_Min = FLT_MAX;
    m_Max = -FLT_MAX;
    for (size_t i=0;i<AFASpectra::numSamples;i++)
    {
        if ( m_Min > m_Amplitude[i])
            m_Min = m_Amplitude[i];
        if ( m_Max < m_Amplitude[i])
            m_Max = m_Amplitude[i];
    }
}

int AFASpectra::getSDSSSpectraOffsetStart()
{
    int offset = 0;
    // if we use BOSS and SDSS spectra combined add spectrum at right offset.
    // BOSS spectra start at 3650A, SDSS spectra at 3800A -> thus use offset ~13 so both types operate in equal wavelengths
    if ( numSamples == (numSamplesBOSS/reductionFactor) )
    {
        const float wavelenPerPixel = (float)reductionFactor*(float)(waveLenEndBOSS-waveLenStartBOSS)/(float)numSamplesBOSS;
        offset = (int)((float)(waveLenStartSDSS-waveLenStartBOSS)/wavelenPerPixel);
    }
    return offset;
}


int AFASpectra::getSDSSSpectraOffsetEnd()
{
    int offset = numSamples;
    // if we use BOSS and SDSS spectra combined add spectrum at right offset.
    // BOSS spectra start at 3650A, SDSS spectra at 3800A -> thus use offset ~13 so both types operate in equal wavelengths
    if ( numSamples == (numSamplesBOSS/reductionFactor) )
    {
        const float wavelenPerPixel = (float)reductionFactor*(float)(waveLenEndBOSS-waveLenStartBOSS)/(float)numSamplesBOSS;
        offset = (int)((float)(waveLenEndSDSS-waveLenStartSDSS)/wavelenPerPixel);
    }
    return offset;
}


void AFASpectra::setOperationRange( bool _BOSSWavelengthRange )
{
    if ( _BOSSWavelengthRange )
    {
        pixelStart	= 0;
        pixelEnd	= numSamples;	
    }
    else
    {
        pixelStart	= (getSDSSSpectraOffsetStart()>>2)<<2;  // pixel start must be aligned to 16 byte boundaries for SSE operations.
        pixelEnd	= getSDSSSpectraOffsetEnd();	
    }
    assert( pixelStart<pixelEnd );
    assert( pixelEnd<=numSamples );
    assert( (pixelStart%4)==0 );
}


