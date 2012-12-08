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

#include <vector>
#include <string>

#include "sdsslib/debug.h"

#define SPT_DEFAULTFILTER (Spectra::SPT_SPEC_UNKNOWN|Spectra::SPT_SPEC_QSO|Spectra::SPT_SPEC_HIZ_QSO|Spectra::SPT_GAL_EM)

// actually can hold only one spectrum so the name is a bit misleading here
//
// warning:
// changing order and / or size of this structure will invalidate all dumped files !
// 
class Spectra
{
public:
	static const int numSamplesSDSS			= 3900;					// number of samples in SDSS spectra
	static const int waveLenStartSDSS		= 3800;					// wavelength coverage (in Angström) for SDSS spectra (EDR..DR8)
	static const int waveLenEndSDSS			= 9200;						

	static const int numSamplesBOSS			= 4700;					// number of samples in BOSS spectra
	static const int waveLenStartBOSS		= 3650;					// wavelength coverage (in Angström) for BOSS spectra (DR9 and upcoming)
	static const int waveLenEndBOSS			= 10400;						

	static const int numSamples				= numSamplesBOSS/8;		// number of samples in reduced spectra
	static const int numSpectraLines		= 44;					// number of stored emission and absorption lines
	static const float waveBeginDst;								// spectrum start in destination system 
	static const float waveEndDst;									// spectrum end in destination system 


	/** @name TYPES
	*/
	//@{


	enum SpectraVersion
	{
		SP_VERSION_INVALID,				//< invalid version or spectra
		SP_VERSION_DR7,					//<	Spectra DR1..DR7
		SP_VERSION_DR8,					
		SP_VERSION_DR9					//< BOSS spectra, new spectrograph, different wavelenght range
	};

	// was called objecttype in SDSS II
	// is now called source type in SDSS III
	enum SpectraType
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
		SPT_REDDEN_STD			= 0x010000000
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


	// emission and absorption lines info
	struct SpectraLines
	{
		float wave;			// in Angström
		float waveMin;		// in Angström
		float waveMax;		// in Angström
		float height;		// in 10^(-17) erg/cm/s^2/Ang
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
	// 1..640
	// note that on re-observations fibers may change
	// see http://spectro.princeton.edu/#plots
	int getFiber() const;

	// plate id 
	int getPlate() const;

	// get SDSS URL to spectrum overview page
	std::string getURL() const;

	// returns true if marked as empty spectrum (specObjID is set to zero).
	bool isEmpty() const;

	// returns true if the number of bad pixels in spectrum are over a threshold of 5%
	bool hasBadPixels() const;

	// return true if spectrum does not contain any awkward numbers, e.g. NANs, infinite numbers.
	bool checkConsistency() const;

	// compare spectra and return accumulated quadratic error of all compared samples (euclidean style).
	float compare(const Spectra &_spectra) const;

	// compare two spectra using the Bhattacharyya distance 
	// it is a statistical distance measure which works with probability distributions
	// make sure to normalize comparing spectra by flux!
	// see http://en.wikipedia.org/wiki/Bhattacharyya_distance
	float compareBhattacharyya(const Spectra &_spectra) const;

	// compare spectra using a more advanced error measure taking neighboring samples into account.
	// _width 0..1
	float compareAdvanced(const Spectra &_spectra, float _width) const;

	// super advanced compare using peak detectors and continuum spectrum.
	// _width 0..1
	float compareSuperAdvanced(const Spectra &_spectra, float _width, bool _bOptimize=false) const;

	// calculate continuum spectrum by heavy under sampling.
	// _continuumSamples should be an order of magnitude smaller than Spectra::numSamples. 32 for instance is a good fit.
	void getContinuum( size_t _continuumSamples, std::vector<float> &_outContinuum ) const;

	// generate continuum subtracted spectrum
	// _continuumSamples should be an order of magnitude smaller than Spectra::numSamples. 32 for instance is a good fit.
	// _outSpectrumMinusContinuum returns spectrum subtracted by the continuum
	// _outContinuum returns the continuum itself, has _continuumSamples in 
	void getSpectrumMinusContinuum( size_t _continuumSamples, std::vector<float> &_outSpectrumMinusContinuum, std::vector<float> &_outContinuum ) const;

	// get a list of detected peaks.
	// _spectrumMinusContinuum continuum subtracted spectrum of the used spectrum
	// _numPeaks maximum number of detected and returned peaks
	// _cutOffTreshold high value = many peaks, low value less peaks, [0.1..1000], reasonable range [1..10]
	// _outMinPeaks ordered pairs of minimum peaks below continuum. starts with lowest value first.
	//              first = amplitude, second = sample index
	// _outMinPeaks ordered pairs of maximum peaks above continuum. starts with lowest value first.
	//              first = amplitude, second = sample index
	void getPeaks( const std::vector<float> &_spectrumMinusContinuum, 
		size_t _numPeaks, 
		float _cutOffTreshold, 
		std::vector<float> &_outMinPeaks, 
		std::vector<float> &_outMaxPeak ) const;

	//@}

	/** @name MODIFIERS
	*/
	//@{

	// clear data/reset spectrum
	void clear();

	void set(const Spectra &_spectra);

	// set test spectra
	// type: 0=sin, 1=cos, 2=lin, 3=lin inv, 4=sqr
	void set( size_t _type, float _noize );

	// set rect 
	// _phase 0..1, _width 0..1
	void setRect( float _width=0.1, float _phase=0.5, float _amplitude=1.f );

	// set sine curve with a given frequency, phase and amplitude.
	void setSine( float _frequency = 1.f, float _phase = 0.f, float _amplitude=1.f, float _noize=0.f );

	// fill spectrum with noise
	// seed != 0
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

	// load spectrum from comma separated values, we aussume ~3900 samples.
	// spectrum should look like this:
	//
	//
	//	Wavelength(A),Flux,Error,Mask
	//	3830.01081226449, -1.97607004642487, 0, 83886080
	//	3830.01081226449, -1.97607004642487, 0, 83886080
	//  .. omitting next ~3900 lines.
	//	3833.54000759044, 1.56192994117737, 1.66190469264984, 0
	//
	bool loadFromCSV(const std::string &_filename);

	// load from SDSS .fit file up to DR7
	// e.g. spSpec-51630-0266-633.fit
	// FITS file description see http://www.sdss.org/DR6/dm/flatFiles/spSpec.html
	// general info here: http://www.sdss.org/DR6/dm/flatFiles/FILES.html
	bool loadFromFITS_SDSS(const std::string &_filename);

//	bool loadFromFITS_SDSS_DR8(const std::string &_filename);

	// BOSS Spectra from DR9. e.g. dr9spec-3588-55184-0511.fits
	bool loadFromFITS_BOSS(const std::string &_filename);

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

	// transforms spectrum into frequency domain using a DFT.
	//void dft();

	//@}



	/** @name MEMBERS
	*/
	//@{


	float m_Amplitude[numSamples];		// amplitude in 10^(-17) erg/cm/s^2/Ang
	float m_Min;
	float m_Max;
	__int32 m_Index;					// index to source spectrum [0..num src spectra-1], -1 = no src spectrum
	__int16 m_SamplesRead;
	__int64 m_SpecObjID;	
	SpectraType m_Type;
	double m_Z;
#ifdef _USE_SPECTRALINES
	SpectraLines m_Lines[numSpectraLines];
#endif
	float m_flux;
	char m_status;						// 0=ok, 1=bad pixels
	char pad[3];						// for padding to multiple of 16 byte boundaries


	//@}

	/** @name HELPER FUNCTIONS
	*/
	//@{

	// calc unique photo object identifier from a bunch of parameters
	static unsigned __int64 calcPhotoObjID( int _run, int _rerun, int _camcol, int _field, int _obj );

	// calc unique spec object identifier from a bunch of parameters (DR7 or below)
	static unsigned __int64 calcSpecObjID_DR7( int _plate, int _mjd, int _fiber, int _type );

	// calc unique spec object identifier from a bunch of parameters for spectra (DR8 and above)
	static unsigned __int64 calcSpecObjID_DR8( int _plate, int _mjd, int _fiber, int _run2d_m, int _run2d_n, int _run2d_p );

	// convert plate number to string, e.g 266 -> "0266"
	static std::string plateToString( int _plate );

	static float indexToWaveLength( size_t _index, float _waveBegin, float _waveEnd, int _numSamples );
	static size_t waveLengthToIndex( float _waveLength, float _waveBegin, float _waveEnd, int _numSamples );

	static float waveLenghtToRestFrame( float _waveLength, float _z );

	// returns any combination of SpectraTypes as filter
	static std::string spectraFilterToString( unsigned int _spectraFilter );
	static SpectraType spectraTypeFromString( const std::string &_spectraType );


	// generate spectra filename from plate, MJD and fiber, e.g. spSpec-52203-0716-002.fit
	// MJD has always five digits
	// plate number has always four digits
	// fiber id has always three digits
	static std::string getSpecObjFileName(int _plate, int _mjd, int _fiberID );

	//@}

};

STATIC_ASSERT((sizeof(Spectra)%16)==0);		// check if spectrum is multiple of 16 bytes

#endif