#ifndef _SPECTRA_H
#define _SPECTRA_H

#include <vector>
#include <string>


// actually can hold only one spectrum so the name is a bit misleading here
class Spectra
{
public:
	static const int numSamples = 3900/4;		// number of samples in spectrum 3900/4
	static const float waveBegin;				// spectrum measurement start in Angström
	static const float waveEnd;					// spectrum measurement end in Angström

	enum SpectraType
	{
		SPEC_UNKNOWN = 0,
		SPEC_STAR    = 1,
		SPEC_GALAXY  = 2,
		SPEC_QSO     = 3,
		SPEC_HIZ_QSO = 4, // high redshift QSO, z>2.3, Ly-alpha finding code is triggered  
		SPEC_SKY     = 5, 
		STAR_LATE    = 6, // star dominated by molecular bands, Type M or later           
		GAL_EM       = 7, // emission line galaxy -- not set by the code                  
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

	// add signals from other spectra
	void add(const Spectra &_spectra);
	
	// scale signals by divisor
	void divide(float _divisor);

	// save to ASCII CSV
	bool saveToCSV(std::string &_filename);
	
	// load from SDSS .fit file
	bool loadFromFITS(std::string &_filename);

	// compare spectra and output accumulated quadratic error of all compared samples
	float compare(const Spectra &_spectra) const;

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

	static float indexToWaveLength( int _index );
	static int waveLengthToIndex( float _waveLength );

};

#endif