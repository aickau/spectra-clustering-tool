#ifndef AFA_SPECTRA_COMMON_H__
#define AFA_SPECTRA_COMMON_H__

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
    /** @name MEMBERS
    */
    //@{


    float m_Amplitude[numSamples];		// amplitude in 10^(-17) erg/cm/s^2/Ang
    float m_Min;
    float m_Max;
    sint32_t m_Index;					// index to source spectrum [0..num src spectra-1], -1 = no src spectrum
//    sint16_t m_SamplesRead;
//    sint32_t m_JSCIndex;
#ifndef ASK_AICK_WHETHER_THIS_IS_OK
	uint32_t m_SpecObjID;				// spectra object identifier, encodes plate id, fiber id & MJD for SDSS spectra. Light curves with no SDSS association may use a simple hash)
//	uint32_t m_SpecObjIDLowerBits;
#else
	sint64_t m_SpecObjID;				// spectra object identifier, encodes plate id, fiber id & MJD for SDSS spectra. Light curves with no SDSS association may use a simple hash)
#endif
//    sint32_t m_Type;					// bits 0..2 SpectraClass
										// bits 3..9 SpectraSubClass
										// bit 10 BROADLINE flag
//	SpectraVersion m_version;
//    double m_Z_;
    float m_flux;
//    char m_status;						// 0=ok, 1=bad pixels
//    char pad[3];						// for padding to multiple of 16 byte boundaries


    //@}

} AFASpectra;

//typedef uint32_t [ ]


#endif
