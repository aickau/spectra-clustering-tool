#include "AFASpectra.h"
#include "AFADefines.h"
#include "AFARandom.h"

#include <float.h>
#include <assert.h>
#include <math.h>

int pixelStart				= 0;		
int pixelEnd				= numSamples;		

// unique identifier count
unsigned int UIDCount = 1;

bool_t AFASpectraIsEmpty(volatile AFASpectra *sp) 
{
	return (sp->m_SpecObjID == 0);
	}



void AFASpectraClear(volatile AFASpectra *sp)
{
	unsigned int i=0;

    sp->m_Min = 0.0f;
    sp->m_Max = 1.f;
    sp->m_SamplesRead = 0;
    sp->m_Index = -1;
    sp->m_SpecObjID = 0;
	sp->m_Type = 0;
    sp->m_version = SP_ARTIFICIAL;
    sp->m_Z = 0.0;
    sp->m_flux = 0.0f;
    sp->m_status = 0;

    for (i=0;i<numSamples;i++)
    {
        sp->m_Amplitude[i] = 0.0f;
    }

}


void AFASpectraSetSine( AFASpectra *sp, float _frequency, float _phase, float _amplitude, float _noize )
{
	unsigned int i=0;
	sp->m_SpecObjID =(UIDCount++)<<22;
	sp->m_version = SP_ARTIFICIAL;
	sp->m_Type = 0;

	for (i=0;i<numSamples;i++)
	{
		float x=_phase+(float)(i)*_frequency;
		sp->m_Amplitude[i] = sinf(x)*_amplitude+(AFARandomFloat()-0.5f)*_noize;
	}
	sp->m_SamplesRead = numSamples;
	AFASpectraCalcMinMax(sp);
}


void AFASpectraRandomize( volatile AFASpectra *sp, float minrange, float maxrange )
{
	unsigned int i=0;
	float d = 0.0f;

    if ( minrange > maxrange )
    {
        float hui = maxrange;
        maxrange = minrange;
        minrange = hui;
    }

    d = maxrange-minrange;

    for (i=0;i<numSamples;i++)
    {
        sp->m_Amplitude[i] = AFARandomFloat()*d+minrange;
    }
    sp->m_SamplesRead = numSamples;
    AFASpectraCalcMinMax(sp);
}

void AFASpectraSet( volatile AFASpectra *dst, volatile AFASpectra *src)
{
	unsigned int i=0;
    dst->m_SamplesRead		= src->m_SamplesRead;
    dst->m_Min				= src->m_Min;
    dst->m_Max				= src->m_Max;
    dst->m_Index			= src->m_Index;
    dst->m_SpecObjID		= src->m_SpecObjID;
    dst->m_version			= src->m_version;
    dst->m_Type				= src->m_Type;
    dst->m_Z				= src->m_Z;
    dst->m_flux				= src->m_flux;
    dst->m_status			= src->m_status;

    for ( i=0;i<numSamples;i++)
    {
        dst->m_Amplitude[i] = src->m_Amplitude[i];
    }
}


void AFASpectraNormalizeByFlux(volatile AFASpectra *sp) 
{
	int i;

    AFASpectraCalculateFlux(sp);

    if ( sp->m_flux <= 0.0f )
        return;

    for (i=0;i<numSamples;i++)
    {	
        sp->m_Amplitude[i] /= (sp->m_flux*0.001f);
    }
    AFASpectraCalcMinMax(sp);
}



float AFASpectraCompare(volatile AFASpectra *sp1, volatile AFASpectra *sp2) 
{
	// c-version (slow)
	int i=0;
	float error=0.0f;
	for (i=pixelStart;i<pixelEnd;i++)
	{
		float d = sp2->m_Amplitude[i]-sp1->m_Amplitude[i];
		error += d*d;
	}

	return error;
}



void AFASpectraAdapt( volatile AFASpectra *dst, volatile AFASpectra *src, float _adaptionRate ) 
{
	int i=0;
	// c-version (slow)
	for (i=pixelStart;i<pixelEnd;i++ )
	{
		dst->m_Amplitude[i] += _adaptionRate*(src->m_Amplitude[i]-dst->m_Amplitude[i]);
	}

}

void AFASpectraCalculateFlux(volatile AFASpectra *sp) 
{
	unsigned int i;
    double flux = 0.0;
    float offset = (sp->m_Min < 0.0f) ? -sp->m_Min : 0.0f;
    for (i=0;i<numSamples;i++)
    {
        flux += (double)(sp->m_Amplitude[i]+offset);
    }
    sp->m_flux = (float)(flux);
}

void AFASpectraCalcMinMax(volatile AFASpectra *sp) 
{
	unsigned int i;
    sp->m_Min = FLT_MAX;
    sp->m_Max = -FLT_MAX;
    for (i=0;i<numSamples;i++)
    {
        if ( sp->m_Min > sp->m_Amplitude[i])
            sp->m_Min = sp->m_Amplitude[i];
        if ( sp->m_Max < sp->m_Amplitude[i])
           sp->m_Max = sp->m_Amplitude[i];
    }
}

int AFASpectraGetSDSSSpectraOffsetStart()
{
	float wavelenPerPixel;
    int offset = 0;
    // if we use BOSS and SDSS spectra combined add spectrum at right offset.
    // BOSS spectra start at 3650A, SDSS spectra at 3800A -> thus use offset ~13 so both types operate in equal wavelengths
    if ( numSamples == (numSamplesBOSS/reductionFactor) )
    {
        wavelenPerPixel = (float)reductionFactor*(float)(waveLenEndBOSS-waveLenStartBOSS)/(float)numSamplesBOSS;
        offset = (int)((float)(waveLenStartSDSS-waveLenStartBOSS)/wavelenPerPixel);
    }
    return offset;
}


int AFASpectraGetSDSSSpectraOffsetEnd()
{
	float wavelenPerPixel = 0.0f;
    int offset = numSamples;
    // if we use BOSS and SDSS spectra combined add spectrum at right offset.
    // BOSS spectra start at 3650A, SDSS spectra at 3800A -> thus use offset ~13 so both types operate in equal wavelengths
    if ( numSamples == (numSamplesBOSS/reductionFactor) )
    {
        wavelenPerPixel = (float)reductionFactor*(float)(waveLenEndBOSS-waveLenStartBOSS)/(float)numSamplesBOSS;
        offset = (int)((float)(waveLenEndSDSS-waveLenStartSDSS)/wavelenPerPixel);
    }
    return offset;
}

void
AFASpectraPixelStartEndGet(
	int *pStart,
	int *pEnd )
{
	*pStart = pixelStart;
	*pEnd = pixelEnd;
}

void AFASpectraSetOperationRange( bool_t _BOSSWavelengthRange )
{
    if ( _BOSSWavelengthRange )
    {
        pixelStart	= 0;
        pixelEnd	= numSamples;	
    }
    else
    {
        pixelStart	= (AFASpectraGetSDSSSpectraOffsetStart()>>2)<<2;  // pixel start must be aligned to 16 byte boundaries for SSE operations.
        pixelEnd	= AFASpectraGetSDSSSpectraOffsetEnd();	
    }
    assert( pixelStart<pixelEnd );
    assert( pixelEnd<=numSamples );
    assert( (pixelStart%4)==0 );
}


