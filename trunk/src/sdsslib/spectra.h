//! \verbatim
//! ###########################################################################
//! # SDSS sorting prototype - Stage I
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

#define SPT_DEFAULTFILTER (Spectra::SPT_SPEC_UNKNOWN|Spectra::SPT_SPEC_QSO|Spectra::SPT_SPEC_HIZ_QSO|Spectra::SPT_GAL_EM)

// actually can hold only one spectrum so the name is a bit misleading here
//
// warning:
// changing order and / or size of this structure will invalidate all dumped files !
// 
class Spectra
{
public:
	static const int numSamples = 3900/4;		// number of samples in spectrum 
	static const int numSpectraLines = 44;		// number of stored emission and absorption lines
	static const float waveBeginSrc;			// spectrum measurement start (in Angstr�m)
	static const float waveEndSrc;				// spectrum measurement end 
	static const float waveBeginDst;			// spectrum start in destination system 
	static const float waveEndDst;				// spectrum end in destination system 

	enum SpectraType
	{
		SPT_NOT_SET		 = 0,
		SPT_SPEC_UNKNOWN = 1,
		SPT_SPEC_STAR    = 2,
		SPT_SPEC_GALAXY  = 4,
		SPT_SPEC_QSO     = 8,
		SPT_SPEC_HIZ_QSO = 16, // high redshift QSO, z>2.3, Ly-alpha finding code is triggered  
		SPT_SPEC_SKY     = 32, 
		SPT_STAR_LATE    = 64, // star dominated by molecular bands, Type M or later           
		SPT_GAL_EM       = 128, // emission line galaxy -- not set by the code                  
	};

	// emission and absorption lines info
	struct SpectraLines
	{
		float wave;			// in Angstr�m
		float waveMin;		// in Angstr�m
		float waveMax;		// in Angstr�m
		float height;		// in 10^(-17) erg/cm/s^2/Ang
	};

	Spectra();
	Spectra( const Spectra &_source );
	~Spectra();

	// clear data
	void clear();

	// seed != 0
	void randomize( float _minrange, float _maxrange);

	void set(const Spectra &_spectra);
	
	// set test spectra
	// type: 0=sin, 1=cos, 2=lin, 3=lin inv, 4=sqr
	void set( size_t _type, float _noize );

	// set rect 
	// _phase 0..1, _width 0..1
	void setRect( float _width=0.1, float _phase=0.5, float _amplitude=1.f );

	// set sine curve with a given frequency, phase and amplitude.
	void setSine( float _frequency = 1.f, float _phase = 0.f, float _amplitude=1.f, float _noize=0.f );

	// add signals from other spectra
	void add(const Spectra &_spectra);
	
	// scale signals by divisor
	void divide(float _divisor);

	// save to ASCII CSV
	bool saveToCSV(std::string &_filename);
	
	// load from SDSS .fit file
	bool loadFromFITS(std::string &_filename);

	// compare spectra and return accumulated quadratic error of all compared samples (euclidean style).
	float compare(const Spectra &_spectra) const;

	// compare spectra using a more advanced error measure taking neighboring samples into account.
	// _width 0..1
	float compareAdvanced(const Spectra &_spectra, float _width) const;

	// super advanced compare using peak detectors and continuum spectra.
	// _width 0..1
	float compareSuperAdvanced(const Spectra &_spectra, float _width, bool _bOptimize=false) const;

	// calculate extrema
	void calcMinMax();

	// normalize to range -1..1
	void normalize();

	// transforms spectrum into frequency domain using a DFT.
	void dft();

	// generate continuum spectrum by heavy under sampling.
	// _continuumSamples should be an order of magnitude smaller than Spectra::numSamples. 32 for instance is a good fit.
	void generateContinuum( size_t _continuumSamples, std::vector<float> &_outContinuum ) const;

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

	// returns assembled filename
	std::string getFileName() const;

	// returns the squared sum of all samples
	float getTotalEnergy();

	int getMJD() const;

	// 1..640
	int getFiber() const;

	// plate id 
	int getPlate() const;

	std::string getURL() const;

	// returns true if marked as empty spectrum.
	bool isEmpty() const;

	float m_Amplitude[numSamples];		// amplitude in 10^(-17) erg/cm/s^2/Ang
	float m_Min;
	float m_Max;
	__int32 m_Index;
	__int16 m_SamplesRead;
	__int64 m_SpecObjID;	
	SpectraType m_Type;
	double m_Z;
#ifdef _USE_SPECTRALINES
	SpectraLines m_Lines[numSpectraLines];
#endif
	char pad[8];						// for padding to multiple of 16 byte boundaries

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// helper functions
	//////////////////////////////////////////////////////////////////////////////////////////////////

	// calc unique photo object identifier from a bunch of parameters
	static unsigned __int64 calcPhotoObjID( int _run, int _rerun, int _camcol, int _field, int _obj );

	// calc unique spec object identifier from a bunch of parameters
	static unsigned __int64 calcSpecObjID( int _plate, int _mjd, int _fiber, int _type );

	// convert plate number to string, e.g 266 -> "0266"
	static std::string plateToString( int _plate );

	static float indexToWaveLength( size_t _index, float _waveBegin, float _waveEnd, int _numSamples );
	static size_t waveLengthToIndex( float _waveLength, float _waveBegin, float _waveEnd, int _numSamples );

	// returns any ORed combination of SpectraTypes as filter
	static std::string spectraFilterToString( unsigned int spectraFilter );

};

#endif