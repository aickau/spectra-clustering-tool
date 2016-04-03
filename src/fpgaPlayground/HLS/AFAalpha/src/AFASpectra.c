#include "AFASpectra.h"
#include "AFADefines.h"
#include "AFARandom.h"

#include <float.h>
#include <assert.h>
#include <math.h>

uint32_t pixelStart				= 0;		
uint32_t pixelEnd				= AFA_SPECTRA_NUM_SAMPLES;		

// unique identifier count
unsigned int UIDCount = 1;

void
AFASpectraClear(
	AFASpectra_SW *sp)
{
    unsigned int i=0;

    sp->m_Min = 0.0f;
    sp->m_Max = 1.f;    //< shouldn't this be 0.0f ???
    sp->m_Index = -1;
    sp->m_SpecObjID = 0;
    sp->m_flux = 0.0f;

    for (i=0;i<AFA_SPECTRA_NUM_SAMPLES;i++)
    {
        sp->m_Amplitude[i] = 0.0f;
    }

}


void
AFASpectraSetSine(
	AFASpectra_SW *sp,
    float32_t _frequency,
    float32_t _phase,
    float32_t _amplitude,
    float32_t _noize )
{
    uint32_t i;
    uint32_t tmp;

    tmp = UIDCount++;
    sp->m_SpecObjID = tmp;

    for ( i = 0; i < AFA_SPECTRA_NUM_SAMPLES; i++ )
    {
        float32_t x =_phase + ( float32_t )( i ) * _frequency;
        sp->m_Amplitude[ i ] = sinf( x ) * _amplitude + ( AFARandomFloat() - 0.5f ) * _noize;
    }
    AFASpectraCalcMinMax( sp );
}

// calculates local m_Min and m_Max of spectra amplitude and stores it into record
void
AFASpectraCalcMinMax(
    AFASpectra_SW *sp )
{
    uint32_t i;
    sp->m_Min = FLT_MAX;
    sp->m_Max = -FLT_MAX;
    for ( i = 0; i < AFA_SPECTRA_NUM_SAMPLES; i++ )
    {
        if ( sp->m_Min > sp->m_Amplitude[ i ])
            sp->m_Min = sp->m_Amplitude[ i ];
        if ( sp->m_Max < sp->m_Amplitude[ i ])
            sp->m_Max = sp->m_Amplitude[ i ];
    }
}

// 1.) set for spectrum *sp the amplitudes to random values between minrange and maxrange.
// 2.) set real m_Min and m_Max values for this random numbers
void
AFASpectraRandomize(
	AFASpectra_SW *sp,
    float32_t minrange,
    float32_t maxrange )
{
    uint32_t i = 0;
    float32_t d = 0.0f;

    if ( minrange > maxrange )
    {
        float32_t hui = maxrange;
        maxrange = minrange;
        minrange = hui;
    }

    d = maxrange-minrange;

    for ( i = 0; i < AFA_SPECTRA_NUM_SAMPLES; i++ )
    {
        sp->m_Amplitude[ i ] = AFARandomFloat() * d + minrange;
    }

    AFASpectraCalcMinMax( sp );
}

void
AFASpectraSet(
	AFASpectra_SW *dst,
	AFASpectra_SW *src )
{
    uint32_t i = 0;
    dst->m_Min				= src->m_Min;
    dst->m_Max				= src->m_Max;
    dst->m_Index			= src->m_Index;
    dst->m_SpecObjID		= src->m_SpecObjID;
    dst->m_flux				= src->m_flux;

    for ( i = 0; i < AFA_SPECTRA_NUM_SAMPLES; i++ )
    {
        dst->m_Amplitude[ i ] = src->m_Amplitude[ i ];
    }
}

// sum up all amplitude values. if the minimum of values is below zero
// than offset the amplitudes to always be positive.
void AFASpectraCalculateFlux(
    AFASpectra_SW *sp )
{
    unsigned int i;
    double flux = 0.0;
    float offset = ( sp->m_Min < 0.0f ) ? -sp->m_Min : 0.0f; //< the offset in negative cases leads to wrong normalization
    for ( i = 0; i < AFA_SPECTRA_NUM_SAMPLES; i++ )
    {
        flux += ( double )( sp->m_Amplitude[ i ] + offset );
    }
    sp->m_flux = ( float )( flux );
}

void
AFASpectraNormalizeByFlux(
    AFASpectra_SW *sp )
{
    int i;

    AFASpectraCalculateFlux( sp );

    // flux is already been zero or positive; zero is not allowed.
    // maybe better to check against little epsilon barrier !?
    if ( sp->m_flux <= 0.0f )   
        return;
    // here the amplitudes are normalized. In the end they together should sum up to one.
    for ( i = 0; i < AFA_SPECTRA_NUM_SAMPLES; i++ )
    {	
        sp->m_Amplitude[ i ] /= ( sp->m_flux * 0.001f );  //< JSC: is this 0.001 to escape from far too small flux values?
    }
    AFASpectraCalcMinMax( sp );
}



float32_t
AFASpectraCompare(
	AFASpectra_SW *sp1,
	AFASpectra_SW *sp2)
{
    // c-version (slow)
    uint32_t i=0;
    float32_t error=0.0f;
    for (i=pixelStart;i<pixelEnd;i++)
    {
        float32_t d = sp2->m_Amplitude[i]-sp1->m_Amplitude[i];
        error += d*d;
    }

    return error;
}



void
AFASpectraAdapt(
	AFASpectra_SW *dst,
	AFASpectra_SW *src,
    float32_t  _adaptionRate ) 
{
    uint32_t i=0;
    // c-version (slow)
    for (i=pixelStart;i<pixelEnd;i++ )
    {
        dst->m_Amplitude[i] += _adaptionRate*(src->m_Amplitude[i]-dst->m_Amplitude[i]);
    }

}

int
AFASpectraGetSDSSSpectraOffsetStart()
{
    float wavelenPerPixel;
    int offset = 0;
    // if we use BOSS and SDSS spectra combined add spectrum at right offset.
    // BOSS spectra start at 3650A, SDSS spectra at 3800A -> thus use offset ~13 so both types operate in equal wavelengths
    if ( AFA_SPECTRA_NUM_SAMPLES == (AFA_SPECTRA_NUM_SAMPLES_BOSS/AFA_SPECTRA_SAMPLES_REDUCTION_FACTOR) )
    {
        wavelenPerPixel = (float)AFA_SPECTRA_SAMPLES_REDUCTION_FACTOR*(float)(waveLenEndBOSS-waveLenStartBOSS)/(float)AFA_SPECTRA_NUM_SAMPLES_BOSS;
        offset = (int)((float)(waveLenStartSDSS-waveLenStartBOSS)/wavelenPerPixel);
    }
    return offset;
}


int AFASpectraGetSDSSSpectraOffsetEnd()
{
    float wavelenPerPixel = 0.0f;
    int offset = AFA_SPECTRA_NUM_SAMPLES;
    // if we use BOSS and SDSS spectra combined add spectrum at right offset.
    // BOSS spectra start at 3650A, SDSS spectra at 3800A -> thus use offset ~13 so both types operate in equal wavelengths
    if ( AFA_SPECTRA_NUM_SAMPLES == (AFA_SPECTRA_NUM_SAMPLES_BOSS/AFA_SPECTRA_SAMPLES_REDUCTION_FACTOR) )
    {
        wavelenPerPixel = (float)AFA_SPECTRA_SAMPLES_REDUCTION_FACTOR*(float)(waveLenEndBOSS-waveLenStartBOSS)/(float)AFA_SPECTRA_NUM_SAMPLES_BOSS;
        offset = (int)((float)(waveLenEndSDSS-waveLenStartSDSS)/wavelenPerPixel);
    }
    return offset;
}

void
AFASpectraPixelStartEndGet(
    uint32_t *pStart,
    uint32_t *pEnd )
{
    *pStart = pixelStart;
    *pEnd = pixelEnd;
}

void AFASpectraSetOperationRange( bool_t _BOSSWavelengthRange )
{
    if ( _BOSSWavelengthRange )
    {
        pixelStart	= 0;
        pixelEnd	= AFA_SPECTRA_NUM_SAMPLES;	
    }
    else
    {
        pixelStart	= (AFASpectraGetSDSSSpectraOffsetStart()>>2)<<2;  // pixel start must be aligned to 16 byte boundaries for SSE operations.
        pixelEnd	= AFASpectraGetSDSSSpectraOffsetEnd();	
    }
    assert( pixelStart<pixelEnd );
    assert( pixelEnd<=AFA_SPECTRA_NUM_SAMPLES );
    assert( (pixelStart%4)==0 );
}


