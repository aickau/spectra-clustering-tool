#ifndef AFA_TYPES_H__
#define AFA_TYPES_H__

typedef signed char         sint8_t;
typedef unsigned char       uint8_t;
typedef signed short        sint16_t;
typedef unsigned short      uint16_t;
typedef signed long         sint32_t;
typedef unsigned long       uint32_t;
typedef signed long long    sint64_t;
typedef unsigned long long  uint64_t;

//typedef long unsigned int   size_t;

typedef float               float32_t;
typedef double              float64_t;
typedef int					bool_t;	



typedef enum SpectraVersion_
{
	SP_VERSION_INVALID=0,			//< invalid version or spectra
	SP_ARTIFICIAL,					//< artificial spectra, not loaded from any source.
	SP_CSV,							//< from comma seperated values
	SP_VERSION_DR7,					//<	Spectra DR1..DR7
	SP_VERSION_DR8,					//< DR8, DR9
	SP_VERSION_BOSS,				//< BOSS spectra from DR9/DR10, new spectrograph, different wavelenght range
	SP_VERSION_APOGEE,				//< Infrared Apogee spectrom from SDSS III
	SP_LIGHTCURVE_SDSS,				//< Lightcurve with associated SDSS info (plate id, MJD fiber id)
	SP_LIGHTCURVE_RADEC,			//< Lightcurve with associated ra/dec
	SP_LIGHTCURVE_PLAIN,			//< Lightcurve without any associated information
	SP_COUNT						//< must be the last entry here.
} SpectraVersion;



typedef enum SpectraClass_ 
{
	SPC_NOT_SET				= 0,
	SPC_UNKNOWN				= 1,
	SPC_STAR				= 2,
	SPC_GALAXY				= 3,
	SPC_QSO					= 4
} SpectraClass;

// details: https://www.sdss3.org/dr8/spectro/catalogs.php#objects
typedef enum  
{
	SPSC_NOT_SET			= 0,
	SPSC_STARFORMING		= 1,
	SPSC_STARBURST			= 2,
	SPSC_AGN				= 3,
	SPSC_O					= 4,
	SPSC_O8					= 5,
	SPSC_O9					= 6,
	SPSC_OB					= 7,
	SPSC_B0					= 8,
	SPSC_B1					= 9,
	SPSC_B2					= 10,
	SPSC_B3					= 11,
	SPSC_B4					= 12,
	SPSC_B5					= 13,
	SPSC_B6					= 14,
	SPSC_B7					= 15,
	SPSC_B8					= 16,
	SPSC_B9					= 17,
	SPSC_A0					= 18,
	SPSC_A0p				= 19,
	SPSC_A1					= 20,
	SPSC_A2					= 21,
	SPSC_A3					= 22,
	SPSC_A4					= 23,
	SPSC_A5					= 24,
	SPSC_A6					= 25,
	SPSC_A7					= 26,
	SPSC_A8					= 27,
	SPSC_A9					= 28,
	SPSC_F0					= 29,
	SPSC_F1					= 30, // not used
	SPSC_F2					= 31,
	SPSC_F3					= 32,
	SPSC_F4					= 33, // not used
	SPSC_F5					= 34,
	SPSC_F6					= 35,
	SPSC_F7					= 36, // not used
	SPSC_F8					= 37,
	SPSC_F9					= 38,
	SPSC_G0					= 39,
	SPSC_G1					= 40,
	SPSC_G2					= 41,
	SPSC_G3					= 42,
	SPSC_G4					= 43,
	SPSC_G5					= 44,
	SPSC_G6					= 45,
	SPSC_G7					= 46,
	SPSC_G8					= 47,
	SPSC_G9					= 48,
	SPSC_K0					= 49,
	SPSC_K1					= 50,
	SPSC_K2					= 51,
	SPSC_K3					= 52,
	SPSC_K4					= 53,
	SPSC_K5					= 54,
	SPSC_K6					= 55, // not used
	SPSC_K7					= 56, 
	SPSC_K8					= 57, // not used
	SPSC_K9					= 58, // not used
	SPSC_M0V				= 59,
	SPSC_M2V				= 60,
	SPSC_M1					= 61,
	SPSC_M2					= 62,
	SPSC_M3					= 63,
	SPSC_M4					= 64,
	SPSC_M5					= 65,
	SPSC_M6					= 66,
	SPSC_M7					= 67,
	SPSC_M8					= 68,
	SPSC_M9					= 69,
	SPSC_L0					= 70,
	SPSC_L1					= 71,
	SPSC_L2					= 72,
	SPSC_L3					= 73,
	SPSC_L4					= 74,
	SPSC_L5					= 75,
	SPSC_L55				= 76,	// 5.5
	SPSC_L6					= 77,
	SPSC_L7					= 78, // not used
	SPSC_L8					= 79, // not used
	SPSC_L9					= 80,
	SPSC_LDWARF				= 81,
	SPSC_T2					= 82,
	SPSC_CARBON				= 83,
	SPSC_CARBONWD			= 84,
	SPSC_CARBON_LINES		= 85,
	SPSC_CV					= 86,
	SPSC_CALCIUMWD			= 87, 
	SPSC_WD					= 88,
	SPSC_WDCOOLER			= 89,
	SPSC_WDHOTTER			= 90,
	SPSC_WDMAGNETIC			= 91,
	SPSC_BROADLINE			= 128	// this can be combined with all subclasses (hence needs highest bit) 
} SpectraSubClass;

// Older spectra type only for historic reasons here. 
// Now use SpectraClass and SpectraSubClass
// was called objecttype in SDSS II
// is now called source type in SDSS III
typedef enum
{
	SPT_NOT_SET				= 0,
	SPT_UNKNOWN				= 0x000000001,
	SPT_STAR				= 0x000000002,
	SPT_GALAXY				= 0x000000004,
	SPT_QSO					= 0x000000008,
	SPT_HIZ_QSO				= 0x000000010,					//< high redshift QSO, z>2.3, Ly-alpha finding code is triggered  
	SPT_SKY					= 0x000000020,					//< blank sky
	SPT_STAR_LATE			= 0x000000040,					//< star dominated by molecular bands, Type M or later           
	SPT_GAL_EM				= 0x000000080,					//< emission line galaxy -- not set by the code       
	// the following types are SDSS III only
	SPT_QA					= 0x000000100,					//<  Quality assurance (assigned to more than one tile)
	SPT_STAR_PN				= 0x000000200,	
	SPT_STAR_CARBON			= 0x000000400,	
	SPT_STAR_BROWN_DWARF	= 0x000000800,	
	SPT_STAR_SUB_DWARF		= 0x000001000,	
	SPT_STAR_CATY_VAR		= 0x000002000,	
	SPT_STAR_RED_DWARF		= 0x000004000,	
	SPT_STAR_WHITE_DWARF	= 0x000008000,	
	SPT_STAR_BHB			= 0x000010000,	
	SPT_ROSAT_A				= 0x000020000,
	SPT_ROSAT_B				= 0x000040000,
	SPT_ROSAT_C				= 0x000080000,
	SPT_ROSAT_D				= 0x000100000,
	SPT_SPECTROPHOTO_STD	= 0x000200000,	 
	SPT_HOT_STD				= 0x000400000,	 
	SPT_SERENDIPITY_BLUE	= 0x000800000,	 
	SPT_SERENDIPITY_FIRST	= 0x001000000,	 
	SPT_SERENDIPITY_RED		= 0x002000000, 
	SPT_SERENDIPITY_DISTANT	= 0x004000000, 	
	SPT_SERENDIPITY_MANUAL	= 0x008000000,	 
	SPT_REDDEN_STD			= 0x010000000,
	// the following types are from DR10:
	SPT_BLAZAR				= 0x020000000,
	SPT_QSO_BAL				= 0x040000000,
	SPT_EXOTIC				= 0x080000000				//< pheletora of different new source types from DR10
} OldSpectraType;


typedef enum
{
	SP_MASK_OK           =  0x000,      
	SP_MASK_NOPLUG       =  0x001,      //  Fiber not listed in plugmap file                     
	SP_MASK_BADTRACE     =  0x002,      //  Bad trace from routine TRACE320CRUDE                 
	SP_MASK_BADFLAT      =  0x004,      //  Low counts in fiberflat                              
	SP_MASK_BADARC       =  0x008,      //  Bad arc solution                                     
	SP_MASK_MANYBADCOL   =  0x010,      //  More than 10% pixels are bad columns                 
	SP_MASK_MANYREJECT   =  0x020,      //  More than 10% pixels are rejected in extraction      
	SP_MASK_LARGESHIFT   =  0x040,      //  Large spatial shift between flat and object position 
	SP_MASK_NEARBADPIX   =  0x10000,    //  Bad pixel within 3 pixels of trace                   
	SP_MASK_LOWFLAT      =  0x20000,    //  Flat field less than 0.5                             
	SP_MASK_FULLREJECT   =  0x40000,    //  Pixel fully rejected in extraction                   
	SP_MASK_PARTIALREJ   =  0x80000,    //  Some pixels rejected in extraction                   
	SP_MASK_SCATLIGHT    =  0x100000,   //  Scattered light significant                          
	SP_MASK_CROSSTALK    =  0x200000,   //  Cross-talk significant                               
	SP_MASK_NOSKY        =  0x400000,   //  Sky level unknown at this wavelength                 
	SP_MASK_BRIGHTSKY    =  0x800000,   //  Sky level > flux + 10*(flux error)                   
	SP_MASK_NODATA       =  0x1000000,  //  No data available in combine B-spline                
	SP_MASK_COMBINEREJ   =  0x2000000,  //  Rejected in combine B-spline                         
	SP_MASK_BADFLUXFACTOR=  0x4000000,  //  Low flux-calibration or flux-correction factor       
	SP_MASK_BADSKYCHI    =  0x8000000,  //  Chi^2 > 4 in sky residuals at this wavelength        
	SP_MASK_REDMONSTER   =  0x10000000, //  Contiguous region of bad chi^2 in sky residuals      
	SP_MASK_EMLINE       =  0x40000000  //  Emission line detected here                         
} SpectraMask;

typedef enum
{
	SN_NONE,
	SN_AMPLITUDE,
	SN_FLUX
} SpectraNormalization;

typedef enum
{
	SP_ZSTATUS_NOT_MEASURED	= 0,		// Redshift not yet measured.
	SP_ZSTATUS_FAILED		= 1,		// Redshift measurement failed.
	SP_ZSTATUS_INCONSISTENT	= 2,		// Redshift cross-correlation and emz both high-confidence but inconsistent.
	SP_ZSTATUS_XCORR_EMLINE	= 3,		// Redshift determined from cross-correlation and emz are consistent.
	SP_ZSTATUS_XCORR_HIC	= 4,		// Redshift determined from x-corr with high confidence.
	SP_ZSTATUS_XCORR_LOC	= 5,		// Redshift determined from cross-correlation with low confidence.
	SP_ZSTATUS_EMLINE_XCORR	= 6,		// Redshift from emz plus consistent xcorr redshift measurement.
	SP_ZSTATUS_EMLINE_HIC	= 7,		// Redshift determined from em-lines with high confidence.
	SP_ZSTATUS_EMLINE_LOC	= 8,		// Redshift determined from em-lines with low confidence.
	SP_ZSTATUS_MANUAL_HIC	= 9,		// Redshift determined "by hand" with high confidence.
	SP_ZSTATUS_MANUAL_LOC	= 10,		// Redshift determined "by hand" with low confidence.
	SP_ZSTATUS_4000BREAK	= 11,		// x-corr redshift determined when EW(4000Å break) > 0.95.
	SP_ZSTATUS_ABLINE_CAII	= 12		// Redshift determined from average of CaII triplet fits.
} SpectraZStatus;

typedef enum
{
	SP_ZWARNING_OK				= 0x00000000,		//	No warnings.
	SP_ZWARNING_NO_SPEC			= 0x00000001,		//	No spectrum.
	SP_ZWARNING_NO_BLUE			= 0x00000004,		//	No blue side spectrum.
	SP_ZWARNING_NO_RED			= 0x00000008,		//	No red side spectrum.
	SP_ZWARNING_NOT_GAL			= 0x00000010,		//	Classification does not match galaxy target.
	SP_ZWARNING_NOT_QSO			= 0x00000020,		//	Classification does not match quasar target.
	SP_ZWARNING_NOT_STAR		= 0x00000040,		//	Classification does not match star target.
	SP_ZWARNING_GAL_COEF		= 0x00000080,		//	Galaxy PCA coefficients far off locus (placeholder).
	SP_ZWARNING_EMAB_INC		= 0x00000100,		//	Emission and absorption z's inconsistent.
	SP_ZWARNING_AB_INC			= 0x00000200,		//	Absorption redshifts inconsistent.
	SP_ZWARNING_EM_INC			= 0x00000400,		//	Emission redshifts inconsistent.
	SP_ZWARNING_HIZ				= 0x00000800,		//	Redshift is high.
	SP_ZWARNING_LOC				= 0x00001000,		//	Confidence is low.
	SP_ZWARNING_LOW_SNG			= 0x00002000,		//	Signal to noise is low in g.
	SP_ZWARNING_LOW_SNR			= 0x00004000,		//	Signal to noise is low in r .
	SP_ZWARNING_LOW_SNI			= 0x00008000,		//	Signal to noise is low in i.
	SP_ZWARNING_4000BREAK		= 0x00010000,		//	EW(4000A break) > 0.95.
	SP_ZWARNING_NOT_MAPPED		= 0x00020000,		//	Fiber was not mapped
	SP_ZWARNING_MANUAL_MAPPED	= 0x00040000,		//	Fiber mapping was set manually
	SP_ZWARNING_LOADER_MAPPED	= 0x00080000		//	Single unmapped fiber re-mapped by DB loader
} SpectraZWarning;

typedef struct BestMatch_
{
	unsigned int index;		//< index in the map
	float error;			//< euclidean distance
} BestMatch;

#endif
