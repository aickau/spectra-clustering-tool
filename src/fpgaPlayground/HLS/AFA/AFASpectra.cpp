#include "include/AFASpectra.h"
#include "include/AFADefines.h"
#include "include/AFARandom.h"

#include <float.h>
#include <assert.h>

int AFASpectra::pixelStart				= 0;		
int AFASpectra::pixelEnd				= AFASpectra::numSamples;		

AFASpectra::AFASpectra()
{
}

AFASpectra::AFASpectra( const AFASpectra &_source )
{
    set( _source );
}

AFASpectra::~AFASpectra()
{
}

void AFASpectra::clear()
{
    m_Min = 0.0f;
    m_Max = 1.f;
    m_SamplesRead = 0;
    m_Index = -1;
    m_SpecObjID = 0;
    m_Type = SPT_NOT_SET;
    m_version = SP_ARTIFICIAL;
    m_Z = 0.0;
    m_flux = 0.0f;
    m_status = 0;

    for (size_t i=0;i<AFASpectra::numSamples;i++)
    {
        m_Amplitude[i] = 0.0f;
    }

#ifdef _USE_SPECTRALINES
    for (size_t i=0;i<AFASpectra::numSpectraLines;i++)
    {
        m_Lines[i].height = 0.0f;
        m_Lines[i].wave = 0.0f;
        m_Lines[i].waveMin = 0.0f;
        m_Lines[i].waveMax = 0.0f;
    }
#endif
}

void AFASpectra::randomize(float minrange, float maxrange )
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

void AFASpectra::set(const AFASpectra &_spectra)
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

    for (size_t i=0;i<AFASpectra::numSamples;i++)
    {
        m_Amplitude[i] = _spectra.m_Amplitude[i];
    }
#ifdef _USE_SPECTRALINES
    for (size_t i=0;i<AFASpectra::numSpectraLines;i++)
    {
        m_Lines[i].height = _spectra.m_Lines[i].height;
        m_Lines[i].wave = _spectra.m_Lines[i].wave;
        m_Lines[i].waveMin = _spectra.m_Lines[i].waveMin;
        m_Lines[i].waveMax = _spectra.m_Lines[i].waveMax;
    }
#endif
}


void
AFASpectra::normalizeByFlux()
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

void
AFASpectra::calculateFlux()
{
    double flux = 0.0;
    float offset = (m_Min < 0.0f) ? -m_Min : 0.0f;
    for (size_t i=0;i<AFASpectra::numSamples;i++)
    {
        flux += static_cast<double>(m_Amplitude[i]+offset);
    }
    m_flux = static_cast<float>(flux);
}

void
AFASpectra::calcMinMax()
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

int
AFASpectra::getSDSSSpectraOffsetStart()
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


int
AFASpectra::getSDSSSpectraOffsetEnd()
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


void
AFASpectra::setOperationRange( bool _BOSSWavelengthRange )
{
    if ( _BOSSWavelengthRange )
    {
        pixelStart	= 0;
        pixelEnd	= numSamples;	
    }
    else
    {
        pixelStart	= (getSDSSSpectraOffsetStart()>>2)<<2;  // pixel start must be alligned to 16 byte boundaries for SSE operations.
        pixelEnd	= getSDSSSpectraOffsetEnd();	
    }
    assert( pixelStart<pixelEnd );
    assert( pixelEnd<=numSamples );
    assert( (pixelStart%4)==0 );
}

void
AFASpectra::write( volatile AFASpectra *spectraArray, size_t _gridSize, float _minPeak, float _maxPeak )
{
    size_t gridSizeSqr = _gridSize*_gridSize;
    float strengthScale = static_cast<float>(gridSizeSqr)*2.f;

    AFASpectra spec;
    spec.clear();
    for ( size_t i=0;i<gridSizeSqr;i++)
    {
        AFASpectra *spec = &spectraArray[ i ];
        spec->clear();

        size_t x = i%_gridSize;
        size_t y = i/_gridSize;
        float strength = (static_cast<float>(x*x+y*y)*0.25f)/strengthScale;
        spec->randomize( _minPeak*strength, _maxPeak*strength );

//        w.write(spec);
    }
}

