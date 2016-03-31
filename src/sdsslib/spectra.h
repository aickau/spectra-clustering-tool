//! \verbatim
//! ###########################################################################
//! # ASPECT: A spectra clustering tool for exploration of large spectral surveys - Stage I
//! #
//! # Copyright (c) 2009 Aick in der Au
//! # All rights reserved.
//! ###########################################################################
//!
//!      created by : Aick in der Au <aick.inderau@gmail.com>
//!      created on : 1/19/2009
//! additional docs : none
//!  responsibility : 1. Aick in der Au
//!                   2. 
//! \endverbatim
//!
//! \file  spectra.h
//! \brief It should be better called spectrum because it contains only a single spectrum.

#ifndef _SPECTRA_H
#define _SPECTRA_H


#include "debug.h"
#include "defines.h"


#include <vector>
#include <string>


#define SPT_DEFAULTFILTER (0x0ffffffff)

// Can hold only one spectrum so the name is a bit misleading here.
//
// warning:
// changing order and / or size of this structure will invalidate all dumped files !
// 
class Spectra
{
public:
	static const int numSamplesSDSS			= 3900;								// number of samples in SDSS spectra
	static const int waveLenStartSDSS		= 3800;								// wavelength coverage (in Angström) for SDSS spectra (EDR..DR8)
	static const int waveLenEndSDSS			= 9200;									

	static const int numSamplesBOSS			= 4700;								// number of samples in BOSS spectra
	static const int waveLenStartBOSS		= 3650;								// wavelength coverage (in Angström) for BOSS spectra (DR9 and upcoming)
	static const int waveLenEndBOSS			= 10400;						

	static const int reductionFactor		= 8;								// reduce number of pixel by a factor of 8 (use only values of 2^n)
	static const int numSamples				= numSamplesBOSS/reductionFactor;	// number of samples in reduced spectra
	static const int numSpectraLines		= 44;								// number of stored emission and absorption lines
	static const float waveBeginDst;											// spectrum start in destination system 
	static const float waveEndDst;												// spectrum end in destination system 

	static int pixelStart;														// spectrum pixel range for compare and adaption processes [0..<pixelEnd], must be multiples of four.
	static int pixelEnd;														// spectrum pixel range for compare and adaption processes [0..numSamples]


	/** @name TYPES
	*/
	//@{


	enum SpectraVersion
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
	};



	enum SpectraClass 
	{
		SPC_NOT_SET				= 0,
		SPC_UNKNOWN				= 1,
		SPC_STAR				= 2,
		SPC_GALAXY				= 3,
		SPC_QSO					= 4
	};

	// details: https://www.sdss3.org/dr8/spectro/catalogs.php#objects
	enum SpectraSubClass 
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
	};

	// Older spectra type only for historic reasons here. 
	// Now use SpectraClass and SpectraSubClass
	// was called object type in SDSS II
	// is now called source type in SDSS III
	enum OldSpectraType
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
	};


	enum SpectraMask
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
	};

	enum SpectraNormalization
	{
		SN_NONE,
		SN_AMPLITUDE,
		SN_FLUX
	};

	enum SpectraZStatus
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
		SP_ZSTATUS_4000BREAK	= 11,		// x-corr redshift determined when EW(4000Ã… break) > 0.95.
		SP_ZSTATUS_ABLINE_CAII	= 12		// Redshift determined from average of CaII triplet fits.
	};

	enum SpectraZWarning
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
	};


	enum SpectraOperationRange
	{
		SP_OPERATION_SDSS,								// if all spectra are SDSS spectra only use the default SDSS wavelength range.
		SP_OPERATION_BOSS,								// if we should use the entire BOSS wavelength range during clustering
		SP_OPERATION_LIGHTCURVES						// for lightcurves the last two amplitude values encode ra and declination coordinates
	};

	//@}

	/** @name CONSTRUCTORS / DESTRUCTORS
	*/
	//@{


	Spectra();
	Spectra( const Spectra &_source );
	~Spectra();

	//@}

	/** @name ACCESSORS
	*/
	//@{

	// returns assembled filename
	std::string getFileName() const;

	// get modified Julian date of observation.
	// MJD starts at November 7.th 1858, see http://en.wikipedia.org/wiki/Julian_date
	int getMJD() const;

	// fiber ID for SDSS
	// 1..640 for SDSS I & II
	// 1..1000 for BOSS spectra
	// 1..300 for APOGEE spectra
	// note that on re-observations fibers may change
	// see http://spectro.princeton.edu/#plots
	//     https://www.sdss3.org/dr9/spectro/spectro_basics.php
	int getFiber() const;

	// plate id 
	int getPlate() const;

	// center point right ascension in J2000 decimal degrees
	// only for light curve import ATM.
	double getRa() const;

	// center point declination in J2000 decimal degrees
	// only for light curve import ATM.
	double getDec() const;

	// get SDSS URL to spectrum overview page
	std::string getURL() const;

	// get URL to SDSS image of spectrum
	std::string getImgURL() const;


	// returns true if marked as empty spectrum (specObjID is set to zero).
	bool isEmpty() const;

	// returns true if the number of bad pixels in spectrum are over a threshold of 5%
	bool hasBadPixels() const;

	// return true if spectrum does not contain any awkward numbers, e.g. NANs, infinite numbers.
	bool checkConsistency() const;

	// compare spectra and return accumulated quadratic error of all compared samples (euclidean style).
	float compare(const Spectra &_spectra) const;

	SpectraClass getClass() const;

	SpectraSubClass getSubClass() const;

	// if spectrum is of class GALAXY or QSO and 
	// have lines detected at the 10-sigma level with sigmas > 200 km/sec at the 5-sigma level
	// see: https://www.sdss3.org/dr8/spectro/catalogs.php#objects
	bool isBroadline() const;

	// returns true spectrum class and subclass matches corresponding filter, otherwise false
	bool matchesFilter( unsigned int spClassFilter, unsigned int spSubClassFilter=0x0ffffffff );

	//@}

	/** @name MODIFIERS
	*/
	//@{

	// clear data/reset spectrum
	void clear();

	void set(const Spectra &_spectra);

	// set experimental test spectra
	// type: 0=sin, 1=cos, 2=linear, 3=linear inverse, 4=squared
	void set( size_t _type, float _noize );

	// set rectangular impulse (for testing purposes)
	// _phase 0..1, _width 0..1
	void setRect( float _width=0.1, float _phase=0.5, float _amplitude=1.f );

	// set sine curve with a given frequency, phase and amplitude.
	void setSine( float _frequency = 1.f, float _phase = 0.f, float _amplitude=1.f, float _noize=0.f );

	// fill spectrum with noise
	void randomize( float _minrange, float _maxrange);

	// add signals from other spectra
	void add(const Spectra &_spectra);

	// add constant value to amplitudes
	void add(float _value);

	// subtract signals from other spectra
	void subtract(const Spectra &_spectra);

	// multiply signals with other spectra
	void multiply( const Spectra &_spectra);

	// scale signals
	void multiply(float _multiplier);

	// Load spectrum from comma separated values, we assume ~3900 samples.
	// spectrum should look like this:
	//
	//
	//	Wavelength(A),Flux,Error,Mask
	//	3830.01081226449, -1.97607004642487, 0, 83886080
	//	3830.01081226449, -1.97607004642487, 0, 83886080
	//  .. omitting next ~3900 lines.
	//	3833.54000759044, 1.56192994117737, 1.66190469264984, 0
	//
	// _logStream optional log stream for detailed logging output in case loading fails.
	bool loadFromCSV(const std::string &_filename, std::ofstream *_logStream=NULL );

	// Load spectrum or light curve from XML.
	// Here we do not read the number of samples directly, instead we read sparse data. 
	// A variable number of x,y value pairs are read and interpolated to the number of samples.
	//
	// Structure of the XML:
	//
	// 	<?xml version="1.0" encoding="UTF-8"?>
	// 	<LIGHTCURVE>
	// 		<type value="SDSS">										<!-- can be any type string, used to distinguish different datasets-->
	// 		<plate value="4096"/>									<!-- SDSS plate ID -->
	// 		<mjd value="55896"/>									<!-- SDSS modified Julian date of observation. -->
	// 		<fiberId value="4"/>									<!-- SDSS fiber ID 1..640 for SDSS, 1..1000 for BOSS -->
	// 		<ra value="0.01"/>										<!-- right ascension, J2000 -->
	// 		<dec value="0.056"/>									<!-- declination, J2000 -->
	// 		<z value="5.0"/>										<!-- red shift -->
	// 		<DATA xMin="0.5" xMax="4.0" yMin="-2.0" yMax="0.0" >
	// 		0.5,-1.2
	// 		0.55,-1.3
	// 		...
	// 		</DATA>
	// 	</LIGHTCURVE>
	//
	// _logStream optional log stream for detailed logging output in case loading fails.
	bool loadFromXML(const std::string &_filename, std::ofstream *_logStream=NULL);

	// automatically checks the version
	// _logStream optional log stream for detailed logging output in case loading fails.
	bool loadFromFITS(const std::string &_filename, std::ofstream *_logStream=NULL);

	// load from SDSS .fit file up to DR7
	// Better use loadFromFITS() which checks the version and selects the aprpriate loader method.
	// e.g. spSpec-51630-0266-633.fit
	// FITS file description see http://www.sdss.org/DR6/dm/flatFiles/spSpec.html
	// general info here: http://www.sdss.org/DR6/dm/flatFiles/FILES.html
	bool loadFromFITS_SDSS(const std::string &_filename, std::ofstream *_logStream=NULL);

	// load SDSS DR8 spectra
	// Better use loadFromFITS() which checks the version and selects the aprpriate loader method.
	bool loadFromFITS_DR8(const std::string &_filename, std::ofstream *_logStream=NULL);

	// BOSS Spectra from DR9/DR10. e.g. dr9spec-3588-55184-0511.fits
	// Better use loadFromFITS() which checks the version and selects the aprpriate loader method.
	bool loadFromFITS_BOSS(const std::string &_filename, std::ofstream *_logStream=NULL);

	// helper loader func to retrieve additional data from spectra DB.
	void loadDataFromSpectraDB( std::ofstream *_logStream=NULL );

	// save to ASCII CSV
	bool saveToCSV(const std::string &_filename);

	// calculate extrema
	void calcMinMax();

	// calculates the surface of the spectrum
	void calculateFlux();

	// normalize to range -1..1
	void normalize();

	// normalize by flux
	void normalizeByFlux();

	// adapt spectrum towards another spectrum by a given factor
	// _spectra spectrum to adapt to
	// _adaptionRate [0..1]
	void adapt( const Spectra &_spectra, float _adaptionRate );

	// set spectra type from class and subclass
	void setType( SpectraClass spClass, SpectraSubClass spSubClass, bool hasBroadline );
	void setType( OldSpectraType spType );

	//@}



	/** @name MEMBERS
	*/
	//@{


	float m_Amplitude[numSamples];		// amplitude in 10^(-17) erg/cm/s^2/Ang
	float m_Min;
	float m_Max;
	int32_t m_Index;					// index to source spectrum [0..num src spectra-1], -1 = no src spectrum
	int16_t m_SamplesRead;
	int64_t m_SpecObjID;				// spectra object identifier, encodes plate id, fiber id & MJD for SDSS spectra. Light curves with no SDSS association may use a simple hash)
	int32_t m_Type;						// bits 0..2 SpectraClass 
										// bits 3..9 SpectraSubClass
										// bit 10 BROADLINE flag
	SpectraVersion m_version;
	double m_Z;

	float m_flux;
	char m_status;						// 0=ok, 1=bad pixels
	char pad[3];						// for padding to multiple of 16 byte boundaries


	//@}

	/** @name HELPER FUNCTIONS
	*/
	//@{

	// calc unique photo object identifier from a bunch of parameters
	static uint64_t calcPhotoObjID( int _run, int _rerun, int _camcol, int _field, int _obj );

	// calc unique spec object identifier from a bunch of parameters (DR7 or below)
	static uint64_t calcSpecObjID_DR7( int _plate, int _mjd, int _fiber, int _type );

	// calc unique spec object identifier from a bunch of parameters for spectra (DR8 and above)
	static uint64_t calcSpecObjID_DR8( int _plate, int _mjd, int _fiber, int _run2d_m, int _run2d_n, int _run2d_p );

	// convert plate number to string, e.g 266 -> "0266"
	static std::string plateToString( int _plate );

	static float indexToWaveLength( size_t _index, float _waveBegin, float _waveEnd, int _numSamples );
	static size_t waveLengthToIndex( float _waveLength, float _waveBegin, float _waveEnd, int _numSamples );

	static float waveLenghtToRestFrame( float _waveLength, float _z );

	// returns any combination of SpectraTypes as filter
	static std::string spectraFilterToString( unsigned int _spectraFilter );
	static OldSpectraType spectraTypeFromString( const std::string &_spectraType );

	static SpectraClass spectraClassFromString( const std::string &_spectraClass );
	static SpectraSubClass spectraSubClassFromString( const std::string &_spectraSubClass );

	// returns SPSC_BROADLINE or SPSC_NOT_SET
	static bool spectraSubClassHasBroadlineFromString( const std::string &_spectraSubClass );

	// pack spectra class and subclass into 32bit integer:
    //  bits 0..2: SpectraClass 
	//  bits 3..9 SpectraSubClass
	//  bit 10 BROADLINE flag
	static unsigned int packSpectraClassAndSubclass( SpectraClass spClass, SpectraSubClass spSubClass, bool hasBroadline );


	static std::string spectraVersionToString( SpectraVersion _spectraVersion );


	// generate spectra filename from plate, MJD and fiber, e.g. spSpec-52203-0716-002.fit
	// MJD has always five digits
	// plate number has always four digits
	// fiber id has always three digits
	// dr8 set to true for data release 8 or above spectra, for DR 1..7 set to false.
	static std::string getSpecObjFileName( int _plate, int _mjd, int _fiberID, bool dr8 );

	// if we use BOSS and SDSS spectra combined calculate offset for SDSS spectra.
	// BOSS spectra start at 3650A, SDSS spectra at 3800A -> thus use offset ~13 so both types operate in equal wavelengths
	static int getSDSSSpectraOffsetStart();
	static int getSDSSSpectraOffsetEnd();

	// set operation range, i.e. number of used pixels from spectrum depending type of input spectra 
	static void setOperationRange( SpectraOperationRange _operationRange );

	//@}

};

STATIC_ASSERT((sizeof(Spectra)%16)==0);		// check if spectrum is multiple of 16 bytes

#endif