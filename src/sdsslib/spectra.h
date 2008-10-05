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
	static const int numSamples = 1732;			// number of samples in spectrum 
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

	// set sine test spectra with a given frequency.
	void set( float _freq );

	// add signals from other spectra
	void add(const Spectra &_spectra);
	
	// scale signals by divisor
	void divide(float _divisor);

	// save to ASCII CSV
	bool saveToCSV(std::string &_filename);
	
	// load from SDSS .fit file
	bool loadFromFITS(std::string &_filename);

	// compare spectra and return accumulated quadratic error of all compared samples
	float compare(const Spectra &_spectra) const;

	// compare spectra using a more advanced error measure (usese emission and absorption lines)
	float compareAdvanced(const Spectra &_spectra) const;

	// calculate extrema
	void calcMinMax();

	// normalize to range -1..1
	void normalize();

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
	SpectraLines m_Lines[numSpectraLines];

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