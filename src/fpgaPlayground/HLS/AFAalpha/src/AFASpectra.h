#ifndef AFA_SPECTRA_H__
#define AFA_SPECTRA_H__

#include <stddef.h>

#include "AFATypes.h"
#include "AFASpectraCommon.h"

extern int pixelStart;												// spectrum pixel range for compare and adaption processes [0..<pixelEnd], must be multiples of four.
extern int pixelEnd;												// spectrum pixel range for compare and adaption processes [0..numSamples]


/** @name ACCESSORS
*/
//@{
void
AFASpectraPixelStartEndGet(
	int *pStart,
	int *pEnd );


// returns true if marked as empty spectrum (specObjID is set to zero).
bool_t AFASpectraIsEmpty( volatile AFASpectra *sp);


// compare spectra and return accumulated quadratic error of all compared samples (euclidean style).
float AFASpectraCompare(volatile AFASpectra *sp1,volatile AFASpectra *sp2);


//@}

/** @name MODIFIERS
*/
//@{

// clear data/reset spectrum
void AFASpectraClear( volatile AFASpectra *sp );

void AFASpectraSet( volatile AFASpectra *dst, volatile AFASpectra *src );

// set sine curve with a given frequency, phase and amplitude.
// Good to model test spectra
void AFASpectraSetSine( AFASpectra *sp, float _frequency, float _phase, float _amplitude, float _noize );

// fill spectrum with noise
void AFASpectraRandomize( volatile AFASpectra *sp, float _minrange, float _maxrange);


// calculate extrema
void AFASpectraCalcMinMax( volatile AFASpectra *sp );

// calculates the surface of the spectrum
void AFASpectraCalculateFlux(volatile AFASpectra *sp );

// normalize by flux
void AFASpectraNormalizeByFlux(volatile AFASpectra *sp );


// adapt spectrum towards another spectrum by a given factor
// src spectrum to adapt to
// _adaptionRate [0..1]
void AFASpectraAdapt( volatile AFASpectra *dst, volatile AFASpectra *src, float _adaptionRate );

//@}


/** @name HELPER FUNCTIONS
*/
//@{

// if we use BOSS and SDSS spectra combined calculate offset for SDSS spectra.
// BOSS spectra start at 3650A, SDSS spectra at 3800A -> thus use offset ~13 so both types operate in equal wavelengths
int AFASpectraGetSDSSSpectraOffsetStart();
int AFASpectraGetSDSSSpectraOffsetEnd();

// set to true if we should use the entire BOSS wavelength range during clustering or false if only default SDSS wavelength range should be used.
void AFASpectraSetOperationRange( bool_t _BOSSWavelengthRange );

//@}

#endif
