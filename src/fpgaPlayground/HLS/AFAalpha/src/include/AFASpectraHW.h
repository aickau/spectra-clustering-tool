#ifndef AFA_SPECTRA_HW_H__
#define AFA_SPECTRA_HW_H__

//#include <stddef.h>

#include "AFATypes.h"
#include "include/AFASpectraCommon.h"

extern int pixelStart_HW;												// spectrum pixel range for compare and adaption processes [0..<pixelEnd], must be multiples of four.
extern int pixelEnd_HW;												// spectrum pixel range for compare and adaption processes [0..numSamples]


/** @name ACCESSORS
*/
//@{

// returns true if marked as empty spectrum (specObjID is set to zero).
bool_t AFASpectraIsEmpty_HW( volatile AFASpectra *sp);

// compare spectra and return accumulated quadratic error of all compared samples (euclidean style).
float AFASpectraCompare_HW(volatile AFASpectra *sp1,volatile AFASpectra *sp2);

//@}

/** @name MODIFIERS
*/
//@{

// adapt spectrum towards another spectrum by a given factor
// src spectrum to adapt to
// _adaptionRate [0..1]
void AFASpectraAdapt_HW( volatile AFASpectra *dst, volatile AFASpectra *src, float _adaptionRate );

//@}



#endif
