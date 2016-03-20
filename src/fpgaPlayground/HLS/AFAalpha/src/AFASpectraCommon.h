#ifndef AFA_SPECTRA_COMMON_H__
#define AFA_SPECTRA_COMMON_H__

#include "AFATypes.h"

#define SPT_DEFAULTFILTER (0x0ffffffff)


// convert defines
#define numSamplesSDSS			3900								// number of samples in SDSS spectra
#define waveLenStartSDSS		3800								// wavelength coverage (in Angström) for SDSS spectra (EDR..DR8)
#define waveLenEndSDSS			9200									

#define AFA_SPECTRA_NUM_SAMPLES_BOSS			4700								// number of samples in BOSS spectra
#define waveLenStartBOSS		3650								// wavelength coverage (in Angström) for BOSS spectra (DR9 and upcoming)
#define waveLenEndBOSS			10400						

#define AFA_SPECTRA_SAMPLES_REDUCTION_FACTOR			8									// reduce number of pixel by a factor of 8 (use only values of 2^n)
#define AFA_SPECTRA_NUM_SAMPLES				(AFA_SPECTRA_NUM_SAMPLES_BOSS / AFA_SPECTRA_SAMPLES_REDUCTION_FACTOR)	// number of samples in reduced spectra

//
//
// COMMENT for AICK:
// * Can we reduce the size of AFASpectra to 2048Bytes in all cases (SDSS & BOSS) ?
// * Can we omit all unnecessary data like m_Min, m_Max, m_SpecObjID ?
//

// Can hold only one spectrum so the name is a bit misleading here.
//
// warning:
// changing order and / or size of this structure will invalidate all dumped files !
// 

typedef struct AFASpectra_
{
    float32_t m_Amplitude[ AFA_SPECTRA_NUM_SAMPLES ];		// amplitude in 10^(-17) erg/cm/s^2/Ang
    float32_t m_Min;
    float32_t m_Max;
    float32_t m_Index;					// index to source spectrum [0..num src spectra-1], -1 = no src spectrum
    float32_t m_SpecObjID;				// spectra object identifier, encodes plate id, fiber id & MJD for SDSS spectra. Light curves with no SDSS association may use a simple hash)
    float32_t m_flux;
} AFASpectra;

typedef float32_t
    *AFASpectra_t;

#endif
