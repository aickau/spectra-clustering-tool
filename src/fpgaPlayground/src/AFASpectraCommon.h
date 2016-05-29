#ifndef AFA_SPECTRA_COMMON_H__
#define AFA_SPECTRA_COMMON_H__

#include "AFATypes.h"

#define SPT_DEFAULTFILTER                       ( 0x0ffffffff )

#define AFA_SPECTRA_SAMPLES_REDUCTION_FACTOR    ( 8 )             // reduce number of pixel by a factor of 8 (use only values of 2^n)

#define AFA_SPECTRA_NUM_SAMPLES_SDSS			(  3900 )         // number of samples in SDSS spectra
#define AFA_WAVE_LEN_START_SDSS		            (  3800 )		   // wavelength coverage (in Angström) for SDSS spectra (EDR..DR8)
#define AFA_WAVE_LEN_END_SDSS			        (  9200 )
#define AFA_SPECTRA_NUM_SAMPLES_PROCESS_SDSS_SW ( AFA_SPECTRA_NUM_SAMPLES_SDSS / AFA_SPECTRA_SAMPLES_REDUCTION_FACTOR )	// number of samples in reduced spectra for our processing
#define AFA_SDSS_SPECTRA_START                  (     0 )
#define AFA_SDSS_SPECTRA_END                    ( AFA_SPECTRA_NUM_SAMPLES_PROCESS_SDSS_SW - 1 )

#define AFA_SPECTRA_NUM_SAMPLES_BOSS			(  4700 )			// number of samples in BOSS spectra
#define AFA_WAVE_LEN_START_BOSS		            (  3650 )			// wavelength coverage (in Angström) for BOSS spectra (DR9 and upcoming)
#define AFA_WAVE_LEN_END_BOSS			        ( 10400 )						
#define AFA_SPECTRA_NUM_SAMPLES_PROCESS_BOSS_SW ( AFA_SPECTRA_NUM_SAMPLES_BOSS / AFA_SPECTRA_SAMPLES_REDUCTION_FACTOR )	// number of samples in reduced spectra
#define AFA_BOSS_WAVELEN_PER_PIXEL              (( float32_t )AFA_SPECTRA_SAMPLES_REDUCTION_FACTOR * ( float32_t )( AFA_WAVE_LEN_END_BOSS - AFA_WAVE_LEN_START_BOSS ) / ( float32_t )AFA_SPECTRA_NUM_SAMPLES_BOSS )
#define AFA_BOSS_SPECTRA_START                  (( uint32_t )(( float32_t )( AFA_WAVE_LEN_START_SDSS - AFA_WAVE_LEN_START_BOSS ) / AFA_BOSS_WAVELEN_PER_PIXEL ) - 1 )
#define AFA_BOSS_SPECTRA_END                    (( uint32_t )(( float32_t )( AFA_WAVE_LEN_END_SDSS   - AFA_WAVE_LEN_START_SDSS ) / AFA_BOSS_WAVELEN_PER_PIXEL ))

// =============================================
// select SDSS or BOSS here: start ...

#define AFA_SPECTRA_NUM_SAMPLES_PROCESS_SW      ( AFA_SPECTRA_NUM_SAMPLES_PROCESS_BOSS_SW )

// we currently only use the SDSS wavelength range to fit spectra into 2K chunks
// the entire BOSS wavelength range does not fit into 2K. 
#define AFA_PROCESS_PIXEL_START					( AFA_BOSS_SPECTRA_START )
#define AFA_PROCESS_PIXEL_END					( AFA_BOSS_SPECTRA_END )

// select SDSS or BOSS here: end ...
// =============================================

#define AFA_SPECTRA_NUM_SAMPLES_PROCESS_HW      ( AFA_PROCESS_PIXEL_END - AFA_PROCESS_PIXEL_START + 1 )


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
enum
{
    // indices describe index in float array describing the spectrum
    AFA_SPECTRA_INDEX_AMPLITUDE = 0,                                //< index 0: amplitude 0
                                                                    //< index n-1: amplitude n-1 (indices 0 .. n-1 are amplitudes)
    AFA_DEBUG_MAGIC = AFA_SPECTRA_NUM_SAMPLES_PROCESS_HW,
    AFA_SPECTRA_INDEX_INDEX = AFA_SPECTRA_NUM_SAMPLES_PROCESS_HW + 1,   //< index n: index-value
    AFA_SPECTRA_INDEX_SPEC_OBJ_ID_LOW,                              //< index n+1: ObjID - lower 32 bit part of object id
    AFA_SPECTRA_INDEX_SPEC_OBJ_ID_HIGH,						        //< index n+2: ObjID - higher 32 bit part of object id
    AFA_DEBUG_JSC,

    AFA_SPECTRA_INDEX_SIZE_IN_UINT32 = 512,                         //< size of data-record := n+3 elements (with n:=AFA_SPECTRA_NUM_SAMPLES_PROCESS_HW)
    AFA_SPECTRA_INDEX_SIZE_IN_BYTES = AFA_SPECTRA_INDEX_SIZE_IN_UINT32 * 4  //< size of data-record in bytes
};

#if 0 // old without "enough" padding
typedef struct AFASpectra_
{
    float32_t m_Amplitude[ AFA_SPECTRA_NUM_SAMPLES_PROCESS_SW ];		// amplitude in 10^(-17) erg/cm/s^2/Ang
    float32_t m_Min;
    float32_t m_Max;
    sint32_t m_Index;					// index to source spectrum [0..num src spectra-1], -1 = no src spectrum
    uint16_t m_SamplesRead;
    uint64_t m_SpecObjID;				// spectra object identifier, encodes plate id, fiber id & MJD for SDSS spectra. Light curves with no SDSS association may use a simple hash)
    uint32_t m_Type;						// bits 0..2 SpectraClass 
    uint32_t m_version;					//< SP_VERSION_INVALID=0,			invalid version or spectra
                                        //< SP_ARTIFICIAL,					artificial spectra, not loaded from any source.
                                        //< SP_CSV,							from comma separated values
                                        //< SP_VERSION_DR7,					Spectra DR1..DR7
                                        //< SP_VERSION_DR8,					DR8, DR9
                                        //< SP_VERSION_BOSS,				BOSS spectra from DR9/DR10, new spectrograph, different wavelenght range
                                        //< SP_VERSION_APOGEE,				Infrared Apogee spectrom from SDSS III
                                        //< SP_LIGHTCURVE_SDSS,				Lightcurve with associated SDSS info (plate id, MJD fiber id)
                                        //< SP_LIGHTCURVE_RADEC,			Lightcurve with associated ra/dec
                                        //< SP_LIGHTCURVE_PLAIN,			Lightcurve without any associated information
    float64_t m_Z;
    float32_t m_flux;
    char m_status;						// 0=ok, 1=bad pixels
    char pad[3];						// for padding to multiple of 16 byte boundaries

} AFASpectra_SW;
#else

#pragma pack(push)
#pragma pack ( 1 )
typedef struct AFASpectra_
{
    float32_t m_Amplitude[ AFA_SPECTRA_NUM_SAMPLES_PROCESS_SW ];        // amplitude in 10^(-17) erg/cm/s^2/Ang
    float32_t m_Min;
    float32_t m_Max;
    sint32_t m_Index;                    // index to source spectrum [0..num src spectra-1], -1 = no src spectrum
    uint16_t m_SamplesRead;
    uint16_t m_pad;
    uint32_t m_pad2;
    uint64_t m_SpecObjID;                // spectra object identifier, encodes plate id, fiber id & MJD for SDSS spectra. Light curves with no SDSS association may use a simple hash)
    uint32_t m_Type;                        // bits 0..2 SpectraClass
    uint32_t m_version;                    //< SP_VERSION_INVALID=0,            invalid version or spectra
                                        //< SP_ARTIFICIAL,                    artificial spectra, not loaded from any source.
                                        //< SP_CSV,                            from comma separated values
                                        //< SP_VERSION_DR7,                    Spectra DR1..DR7
                                        //< SP_VERSION_DR8,                    DR8, DR9
                                        //< SP_VERSION_BOSS,                BOSS spectra from DR9/DR10, new spectrograph, different wavelenght range
                                        //< SP_VERSION_APOGEE,                Infrared Apogee spectrom from SDSS III
                                        //< SP_LIGHTCURVE_SDSS,                Lightcurve with associated SDSS info (plate id, MJD fiber id)
                                        //< SP_LIGHTCURVE_RADEC,            Lightcurve with associated ra/dec
                                        //< SP_LIGHTCURVE_PLAIN,            Lightcurve without any associated information
    float64_t m_Z;
    float32_t m_flux;
    char m_status;                        // 0=ok, 1=bad pixels
    char pad[3];                        // for padding to multiple of 16 byte boundaries

} AFASpectra_SW;
#pragma pack(pop)
#endif

typedef float32_t
    AFASpectra_t;

#endif
