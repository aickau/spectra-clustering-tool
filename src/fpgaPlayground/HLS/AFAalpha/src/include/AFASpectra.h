#ifndef AFA_SPECTRA_H__
#define AFA_SPECTRA_H__

#include <stddef.h>

#include "AFATypes.h"

#define SPT_DEFAULTFILTER (0x0ffffffff)


// convert defines
#define numSamplesSDSS			3900								// number of samples in SDSS spectra
#define waveLenStartSDSS		3800								// wavelength coverage (in Angström) for SDSS spectra (EDR..DR8)
#define waveLenEndSDSS			9200									

#define numSamplesBOSS			4700								// number of samples in BOSS spectra
#define waveLenStartBOSS		3650								// wavelength coverage (in Angström) for BOSS spectra (DR9 and upcoming)
#define waveLenEndBOSS			10400						

#define reductionFactor			8									// reduce number of pixel by a factor of 8 (use only values of 2^n)
#define numSamples				(numSamplesBOSS/reductionFactor)	// number of samples in reduced spectra

extern int pixelStart;												// spectrum pixel range for compare and adaption processes [0..<pixelEnd], must be multiples of four.
extern int pixelEnd;												// spectrum pixel range for compare and adaption processes [0..numSamples]



// Can hold only one spectrum so the name is a bit misleading here.
//
// warning:
// changing order and / or size of this structure will invalidate all dumped files !
// 
typedef struct AFASpectra_
{
    /** @name MEMBERS
    */
    //@{


    float m_Amplitude[numSamples];		// amplitude in 10^(-17) erg/cm/s^2/Ang
    float m_Min;
    float m_Max;
    sint32_t m_Index;					// index to source spectrum [0..num src spectra-1], -1 = no src spectrum
    sint16_t m_SamplesRead;
    sint64_t m_SpecObjID;				// spectra object identifier, encodes plate id, fiber id & MJD for SDSS spectra. Light curves with no SDSS association may use a simple hash)
    sint32_t m_Type;					// bits 0..2 SpectraClass 
										// bits 3..9 SpectraSubClass
										// bit 10 BROADLINE flag
    SpectraVersion m_version;
    double m_Z;
    float m_flux;
    char m_status;						// 0=ok, 1=bad pixels
    char pad[3];						// for padding to multiple of 16 byte boundaries


    //@}

} AFASpectra;



/** @name ACCESSORS
*/
//@{


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
