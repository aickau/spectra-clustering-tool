#if 0
#include "AFASpectraHW.h"
#include "AFADefines.h"
#include "AFARandomHW.h"

#include <float.h>
#include <assert.h>
#include <math.h>

int pixelStart_HW				= 0;		
int pixelEnd_HW					= 0x12345678;		

// unique identifier count
unsigned int UIDCount_HW = 1;

void AFASpectraPixelStartEndSet_HW(
	int pStart,
	int pEnd )
{
	pixelStart_HW = pStart;
	pixelEnd_HW = pEnd;
}

bool_t AFASpectraIsEmpty_HW(volatile AFASpectra *sp) 
{
	return (sp->m_SpecObjID == 0);
}

float AFASpectraCompare_HW(volatile AFASpectra *sp1, volatile AFASpectra *sp2) 
{
	// c-version (slow)
	int i=0;
	float error=0.0f;
	for (i=pixelStart_HW;i<pixelEnd_HW;i++)
	{
		float d = sp2->m_Amplitude[i]-sp1->m_Amplitude[i];
		error += d*d;
	}

	return error;
}


void AFASpectraAdapt_HW( volatile AFASpectra *dst, volatile AFASpectra *src, float _adaptionRate ) 
{
	int i=0;
	// c-version (slow)
	for (i=pixelStart_HW;i<pixelEnd_HW;i++ )
	{
		dst->m_Amplitude[i] += _adaptionRate*(src->m_Amplitude[i]-dst->m_Amplitude[i]);
	}

}

#endif
