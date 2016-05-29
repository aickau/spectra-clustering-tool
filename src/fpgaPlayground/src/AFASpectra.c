#include "AFASpectra.h"
#include "AFADefines.h"
#include "AFARandom.h"

#include <float.h>
#include <assert.h>
#include <math.h>

// unique identifier count
unsigned int UIDCount = 1;

void
AFASpectraClear(
	AFASpectra_SW *sp )
{
    uint16_t i = 0;
    sp->m_Min = 0.0f;
    sp->m_Max = 1.f;    //< shouldn't this be 0.0f ???
    sp->m_Index = -1;
    sp->m_SpecObjID = 0;
    sp->m_flux = 0.0f;

    for ( i = AFA_PROCESS_PIXEL_START; i < AFA_PROCESS_PIXEL_END; i++ )
    {
        sp->m_Amplitude[ i ] = 0.0f;
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

    // create individual object ID
    tmp = UIDCount++;
    sp->m_SpecObjID = tmp;

    // reset index
    sp->m_Index = 0;

    for ( i = 0; i < AFA_SPECTRA_NUM_SAMPLES_PROCESS_SW; i++ )
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
    for ( i = AFA_PROCESS_PIXEL_START; i < AFA_PROCESS_PIXEL_END; i++ )
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

    for ( i = AFA_PROCESS_PIXEL_START; i < AFA_PROCESS_PIXEL_END; i++ )
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

    for ( i = AFA_PROCESS_PIXEL_START; i < AFA_PROCESS_PIXEL_END; i++ )
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
    for ( i = AFA_PROCESS_PIXEL_START; i < AFA_PROCESS_PIXEL_END; i++ )
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
    for ( i = AFA_PROCESS_PIXEL_START; i < AFA_PROCESS_PIXEL_END; i++ )
    {	
        sp->m_Amplitude[ i ] /= ( sp->m_flux * 0.001f );  //< JSC: is this 0.001 to escape from far too small flux values?
    }
    AFASpectraCalcMinMax( sp );
}

