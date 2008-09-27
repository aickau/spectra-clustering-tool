#include "spectra.h"

#include <iostream>
#include <fstream>
#include <conio.h>
#include <math.h>
#include <float.h>
#include <assert.h>
#include "../cfitsio/fitsio.h"
#include "../cfitsio/longnam.h"

#include "random.h"
#include "helpers.h"
#include "defines.h"


// spectra read from SDSS FITS have a wavelength of 3800..9200 Angström
// full spectrum range should go from ~540 Amgström to 9200 assuming max redshifts of 6.
const float Spectra::waveBeginSrc = 3800.f;	
const float Spectra::waveEndSrc = 9200.f;
const float Spectra::waveBeginDst = 540.f;	
const float Spectra::waveEndDst = 9200.f;



Spectra::Spectra()
{
	// make sure data structure is aligned to 16 byte boundaries for SSE
	assert( sizeof(Spectra) % 16 == 0 ); 
	clear();
}

Spectra::Spectra( const Spectra &_source )
{
	set( _source );
}


Spectra::~Spectra()
{
}


void Spectra::clear()
{
	m_Min = 0.0f;
	m_Max = 1.f;
	m_SamplesRead = 0;
	m_Index = -1;
	m_SpecObjID = 0;
	m_Type = SpectraType::SPT_NOT_SET;
	m_Z = 0.0;

	for (size_t i=0;i<Spectra::numSamples;i++)
	{
		m_Amplitude[i] = 0.0f;
	}

	for (size_t i=0;i<Spectra::numSpectraLines;i++)
	{
		m_Lines[i].height = 0.0f;
		m_Lines[i].wave = 0.0f;
		m_Lines[i].waveMin = 0.0f;
		m_Lines[i].waveMax = 0.0f;
	}
}

void Spectra::randomize(float minrange, float maxrange )
{
	if ( minrange > maxrange )
	{
		float hui = maxrange;
		maxrange = minrange;
		minrange = hui;
	}

	float d = maxrange-minrange;

	for (size_t i=0;i<Spectra::numSamples;i++)
	{
		m_Amplitude[i] = static_cast<float>(mt_random_double())*d+minrange;
	}
	m_SamplesRead = Spectra::numSamples;
	calcMinMax();
}


void Spectra::set(const Spectra &_spectra)
{
	m_SamplesRead = _spectra.m_SamplesRead;
	m_Min = _spectra.m_Min;
	m_Max = _spectra.m_Max;
	m_Index = _spectra.m_Index;
	m_SpecObjID = _spectra.m_SpecObjID;
	m_Type = _spectra.m_Type;
	m_Z = _spectra.m_Z;

	for (size_t i=0;i<Spectra::numSamples;i++)
	{
		m_Amplitude[i] = _spectra.m_Amplitude[i];
	}

	for (size_t i=0;i<Spectra::numSpectraLines;i++)
	{
		m_Lines[i].height = _spectra.m_Lines[i].height;
		m_Lines[i].wave = _spectra.m_Lines[i].wave;
		m_Lines[i].waveMin = _spectra.m_Lines[i].waveMin;
		m_Lines[i].waveMax = _spectra.m_Lines[i].waveMax;
	}
}


void Spectra::set( size_t type, float noize )
{
	type = type %= 5;


	static size_t UIDCount = 1;
	m_SpecObjID =(UIDCount++)<<22;
/*
	if ( type == 0 )
	{
		m_strFileName = "sin";
	}

	if ( type == 1 )
	{
		m_strFileName = "cos";
	}

	if ( type == 2 )
	{
		m_strFileName = "lin";
	}

	if ( type == 3 )
	{
		m_strFileName = "lin inv";
	}

	if ( type == 4 )
	{
		m_strFileName = "sqr";
	}
*/

	for (size_t i=0;i<Spectra::numSamples;i++)
	{
		float x=static_cast<float>(i)*0.01f;

		if ( type == 0 )
		{
			m_Amplitude[i] = sinf(x)*20.f+(static_cast<float>(mt_random_double())-0.5f)*noize;
		}

		if ( type == 1 )
		{
			m_Amplitude[i] = cosf(x)*20.f+(static_cast<float>(mt_random_double())-0.5f)*noize;
		}

		if ( type == 2 )
		{
			m_Amplitude[i] = x+(static_cast<float>(mt_random_double())-0.5f)*noize;
		}

		if ( type == 3 )
		{
			m_Amplitude[i] = -x+(static_cast<float>(mt_random_double())-0.5f)*noize;
		}

		if ( type == 4 )
		{
			m_Amplitude[i] = x*x+(static_cast<float>(mt_random_double())-0.5f)*noize;
		}
	}



//	long peak_pos = mt_random_int() % Spectra::numSamples;
//	float x=static_cast<float>(peak_pos)*0.01f;
//	m_Amplitude[peak_pos] = 60.f;

	m_SamplesRead = Spectra::numSamples;

	calcMinMax();
}


void Spectra::add(const Spectra &_spectra)
{
	for (size_t i=0;i<Spectra::numSamples;i++)
	{
		m_Amplitude[i] += _spectra.m_Amplitude[i];
	}
}


void Spectra::divide(float divisor)
{
	for (size_t i=0;i<Spectra::numSamples;i++)
	{
		m_Amplitude[i] /= divisor;
	}
}


bool Spectra::saveToCSV(std::string &filename)
{
	std::string sstrOutput("Wavelength(A),Flux,Error,Mask\n");

	for (size_t i=0;i<Spectra::numSamples;i++)
	{
		std::string sstrLine("0.0, ");
		sstrLine += Helpers::numberToString<float>( m_Amplitude[i] );
		sstrLine += ", 0.0, 0.0\n";
		sstrOutput += sstrLine;
		sstrOutput += sstrLine;
	}
	
	std::ofstream fon(filename.c_str());
	fon<<sstrOutput;

	return true;
}


bool Spectra::loadFromFITS(std::string &filename)
{
	fitsfile *f;
	int status = 0;

	fits_open_file( &f, filename.c_str(), READONLY, &status );
	if ( status != 0 )
		return false;

	int bitpix = 0;
	fits_get_img_type(f, &bitpix, &status );
	if ( bitpix != -32 )
	{
		// image has not float32 type
		fits_close_file(f, &status);
		return false;
	}

	int naxis=0;
	long size[2];
	fits_get_img_dim(f, &naxis, &status );
	if (naxis!=2)
	{
		// no 2-dimenional image
		fits_close_file(f, &status);
		return false;
	}

	// read important info of spectrum
	int mjd, plateID, fiber;
	fits_read_key( f, TINT, "MJD", &mjd, NULL, &status );
	fits_read_key( f, TINT, "PLATEID", &plateID, NULL, &status );
	fits_read_key( f, TINT, "FIBERID", &fiber, NULL, &status );
	fits_read_key( f, TDOUBLE, "Z", &m_Z, NULL, &status );
	fits_read_key( f, TINT, "SPEC_CLN", &m_Type, NULL, &status );
	m_Type = static_cast<SpectraType>(1<<m_Type);
	m_SpecObjID = Spectra::calcSpecObjID( plateID, mjd, fiber, 0 );

	// read spectral data
	fits_get_img_size(f, 2, size, &status );
	const size_t spectrumMaxSize = 3900; 
	float spectrum[spectrumMaxSize];
	
	size_t elementsToRead = size[0];
	assert( elementsToRead <= spectrumMaxSize );
	elementsToRead = MIN( elementsToRead, spectrumMaxSize );

	// spec says:
	// The first row is the spectrum,
	// the second row is the continuum subtracted spectrum, 
	// the third row is the noise in the spectrum (standard deviation, in the same units as the spectrum), 
	// the forth row is the mask array. The spectra are binned log-linear. Units are 10^(-17) erg/cm/s^2/Ang.
	long adress[2]={1,1};
	fits_read_pix( f, TFLOAT, adress, elementsToRead, NULL, (void*)spectrum, NULL, &status );

	// fold the spectrum to reduce noize
	const size_t sampleReductionRatio = 4;
	for ( size_t j=0;j<sampleReductionRatio;j++ )
	{
		size_t c=0;
		for (size_t i=0;i<elementsToRead-1;i+=2)
		{
			spectrum[c] = (spectrum[i]+spectrum[i+1]) * 0.5f;
			c++;
		}
		elementsToRead /= 2; 
	}

	// calculate redshift back
	float wBegin = waveBeginSrc / (1.f+m_Z);
	float wEnd = waveEndSrc / (1.f+m_Z);

	float d = (waveEndDst-waveBeginDst) / static_cast<float>(numSamples);
	float w = waveBeginDst;
	for (size_t i=0;i<numSamples;i++)
	{
		if ( w < wBegin || w > wEnd )
		{
			m_Amplitude[i] = m_Z;
		}
		else
		{
			size_t i0 = Spectra::waveLengthToIndex( w, wBegin, wEnd, elementsToRead );

			if (i0>=elementsToRead-2)
			{
				i0 = elementsToRead-2;
			}

			float w0 = Spectra::indexToWaveLength( i0, wBegin, wEnd, elementsToRead );
			float w1 = Spectra::indexToWaveLength( i0+1, wBegin, wEnd, elementsToRead );
			float frac = (w-w0)/(w1-w0); 
			float a0 = spectrum[i0];
			float a1 = spectrum[i0+1];
			m_Amplitude[i] = a0*(1.f-frac)+a1*frac;
		}

		w+=d; 
	}

	m_SamplesRead = numSamples;



/*
	for ( int j=0;j<sampleReductionRatio;j++ )
	{
		size_t c=0;
		for (size_t i=0;i<elementsToRead-1;i+=2)
		{
			spectrum[c] = (spectrum[i]+spectrum[i+1]) * 0.5f;
			c++;
		}
		elementsToRead /= 2; 
	}

	memcpy( m_Amplitude, spectrum, sizeof(float)*elementsToRead );

	// fill unread samples with 0.0
	for ( size_t i=elementsToRead;i<numSamples;i++)
	{
		m_Amplitude[i] = 0.0f;
	}
	m_SamplesRead = static_cast<__int16>(elementsToRead);
*/
	// read emission and absorption lines
	int numhdus = 0;
	int hdutype = 0; // should be BINARY_TBL
	long tblrows = 0; // should be numLines
	int tblcols = 0; // should be 23
	fits_get_num_hdus( f, &numhdus, &status );
	fits_movabs_hdu( f, 3, &hdutype, &status );

	fits_get_num_rows( f, &tblrows, &status );
	fits_get_num_cols( f, &tblcols, &status );

	float nullVal = 0.f;
	float wave, waveMin, waveMax, height = 0.f;
	size_t rowsToRead = MIN( tblrows, numSpectraLines ); 

	//for (size_t i=0;i<numSamples;i++) {
	//	m_Amplitude[i]=0.0f;
	//}

	if ( hdutype == BINARY_TBL && 
		 tblcols == 23 )
	{
		for ( size_t i=0;i<rowsToRead;i++ )
		{
			size_t i1 = i+1;
			fits_read_col( f, TFLOAT, 1, i1, 1, 1, &nullVal, &wave, NULL, &status );
			if (wave>0.f)
			{
				fits_read_col( f, TFLOAT, 3, i1, 1, 1, &nullVal, &waveMin, NULL, &status );
				fits_read_col( f, TFLOAT, 4, i1, 1, 1, &nullVal, &waveMax, NULL, &status );
				fits_read_col( f, TFLOAT, 9, i1, 1, 1, &nullVal, &height, NULL, &status );

				m_Lines[i].height = height;
				m_Lines[i].wave = wave;
				m_Lines[i].waveMin = waveMin;
				m_Lines[i].waveMax = waveMax;

				//size_t indexBegin=Spectra::waveLengthToIndex(waveMin);
				//size_t indexEnd=Spectra::waveLengthToIndex(waveMax);
				//for (size_t j=indexBegin;j<indexEnd;j++) {
				//	m_Amplitude[j]=height;
				//}
			}
		}
	}


	fits_close_file(f, &status);

	calcMinMax();

	return ((m_SamplesRead > numSamples/2) && (status == 0));	
}


void Spectra::calcMinMax()
{
	m_Min = FLT_MAX;
	m_Max = -FLT_MAX;
	for (size_t i=0;i<Spectra::numSamples;i++)
	{
		if ( m_Min > m_Amplitude[i])
			m_Min = m_Amplitude[i];
		if ( m_Max < m_Amplitude[i])
			m_Max = m_Amplitude[i];
	}
}

void Spectra::normalize()
{
	calcMinMax();

	float delta( m_Max-m_Min);

	if ( delta<=0.0f )
	{
		// uh, better not
		_cprintf("warning spectrum %I64u has min=max=%f\n", m_SpecObjID, m_Min );
		return;
	}

	for (size_t i=0;i<Spectra::numSamples;i++)
	{
		m_Amplitude[i] /= delta;
	}
}

ISSE_ALIGN float errorv[4];
extern "C" void spectraCompareX64(const float *a0, const float *a1, float *errout, size_t numsamples);

float Spectra::compare(const Spectra &_spectra) const
{
/*
	float error=0.0f;
	for (size_t i=0;i<Spectra::numSamples;i++)
	{
		float d = m_Amplitude[i]-_spectra.m_Amplitude[i];
		error += d*d;
	}
*/
	
	// optimized memory friendly version. make sure spectra is 16 bytes aligned
	// this is 10x faster than compiler generated SSE code!
	const float *a0 = &m_Amplitude[0];
	const float *a1 = &_spectra.m_Amplitude[0];
	const size_t numSamples4 = (Spectra::numSamples >> 3) << 3;

#ifdef X64
	spectraCompareX64(a0,a1,errorv,numSamples4);
#else // X64

	_asm {
		mov edi, a0
		mov esi, a1
		mov ecx, Spectra::numSamples
		shr ecx, 3
		xorps xmm0, xmm0


loop1:
		prefetcht0 [esi+4*4*64]
		prefetcht0 [edi+4*4*64]

		movaps xmm1, [edi+4*4*0]
		movaps xmm2, [esi+4*4*0]
		movaps xmm3, [edi+4*4*1]
		movaps xmm4, [esi+4*4*1]

		subps xmm1, xmm2
		subps xmm3, xmm4

		mulps xmm1, xmm1
		mulps xmm3, xmm3

		addps xmm0, xmm1
		addps xmm0, xmm3

		add edi, 4*4*2
		add esi, 4*4*2

		dec ecx
		jnz loop1

		movaps errorv, xmm0
	}
#endif // X64

	float error=errorv[0]+errorv[1]+errorv[2]+errorv[3];

	size_t i=numSamples4;
	for (i;i<Spectra::numSamples;i++)
	{
		float d = m_Amplitude[i]-_spectra.m_Amplitude[i];
		error += d*d;
	}
	return error;
}


float Spectra::compareAdvanced(const Spectra &_spectra) const
{
	float err1 = compare(_spectra) / static_cast<float>(numSamples);

	float err2 = 0.0f;
	for (size_t i=0;i<numSpectraLines;i++)
	{
		// todo add weights
		float d = m_Lines[i].height-_spectra.m_Lines[i].height;
		err2 += d*d;
	}
	err2 /= static_cast<float>(numSpectraLines);

	float err3 = m_Z-_spectra.m_Z;
	err3 *= err3;

	return err1*1.0f + err2*0.25f + err3*0.1f;
}


std::string Spectra::getFileName() const
{
	// e.g. spSpec-52203-0716-002.fit
	std::string sstrFileName( "spSpec-" );

	char buf[64];
	sprintf_s( buf, "%05i", getMJD() );
	sstrFileName += buf;
	sstrFileName += "-";

	sprintf_s( buf, "%04i", getPlate() );
	sstrFileName += buf;
	sstrFileName += "-";

	sprintf_s( buf, "%03i", getFiber() );
	sstrFileName += buf;
	sstrFileName += ".fit";

	return sstrFileName;
}

float Spectra::getTotalEnergy()
{
	float e = 0.f;
	for (size_t i=0;i<Spectra::numSamples;i++)
	{
		e += m_Amplitude[i]*m_Amplitude[i];
	}
	return e;
}


int Spectra::getMJD() const
{
	return static_cast<int>((m_SpecObjID&(unsigned __int64)0x0000FFFF00000000)>>(unsigned __int64)32);
}

int Spectra::getFiber() const
{
	return static_cast<int>((m_SpecObjID&(unsigned __int64)0x00000000FFC00000)>>(unsigned __int64)22);
}

int Spectra::getPlate() const
{
	return static_cast<int>((m_SpecObjID&(unsigned __int64)0xFFFF000000000000)>>(unsigned __int64)48);
}

std::string Spectra::getURL()const
{
	std::string sstrUrl("http://cas.sdss.org/dr6/en/tools/explore/obj.asp?sid=");
	sstrUrl += Helpers::numberToString( m_SpecObjID );
	return sstrUrl;
}

bool Spectra::isEmpty() const
{
	return (m_SpecObjID == 0);
}


unsigned __int64 Spectra::calcPhotoObjID( int _run, int _rerun, int _camcol, int _field, int _obj )
{
	// taken from: http://cas.sdss.org/dr6/en/help/docs/algorithm.asp#O
	//Bits #of bits Mask				Assignment 	Description
	//0 		1 	0x8000000000000000 	empty 		unassigned
	//1-4 		4 	0x7800000000000000 	skyVersion 	resolved sky version (0=TARGET, 1=BEST, 2-15=RUNS)
	//5-15 		11 	0x07FF000000000000 	rerun 		number of pipeline rerun
	//16-31 	16 	0x0000FFFF00000000 	run 		run number
	//32-34 	3 	0x00000000E0000000 	camcol 		camera column (1-6)
	//35 		1 	0x0000000010000000 	firstField 	is this the first field in segment?
	//36-47 	12 	0x000000000FFF0000 	field 		field number within run
	//48-63 	16 	0x000000000000FFFF 	object 		object number within field}

	unsigned __int64 photoObjID;
	photoObjID  = (unsigned __int64) 0x0800000000000000; // 1=best
	photoObjID |= (unsigned __int64) 0x07FF000000000000&((unsigned __int64)_rerun<<48);
	photoObjID |= (unsigned __int64) 0x0000FFFF00000000&((unsigned __int64)_run<<32);
	photoObjID |= (unsigned __int64) 0x00000000E0000000&((unsigned __int64)_camcol<<29);
	photoObjID |= (unsigned __int64) 0x000000000FFF0000&((unsigned __int64)_field<<16);
	photoObjID |= (unsigned __int64) 0x000000000000FFFF&((unsigned __int64)_obj);

	return photoObjID;
}

unsigned __int64 Spectra::calcSpecObjID( int _plate, int _mjd, int _fiber, int _type )
{
	// taken from: http://cas.sdss.org/dr6/en/help/docs/algorithm.asp#O
	//Bits #of bits Mask				Assignment 			Description
	//0-15 		16	0xFFFF000000000000 	plate 				number of spectroscopic plate
	//16-31		16	0x0000FFFF00000000 	MJD 				MJD (date) plate was observed
	//32-41		10	0x00000000FFC00000 	fiberID 			number of spectroscopic fiber on plate (1-640)
	//42-47		6 	0x00000000003F0000 	type 				type of targeted object
	//48-63		16	0x000000000000FFFF 	line/redshift/index 0 for SpecObj, else number of spectroscopic line (SpecLine) or index (SpecLineIndex) or redshift (ELRedhsift or XCRedshift)

	unsigned __int64 specObjID;
	specObjID  = (unsigned __int64) 0xFFFF000000000000&((unsigned __int64)_plate<<48);
	specObjID |= (unsigned __int64) 0x0000FFFF00000000&((unsigned __int64)_mjd<<32);
	specObjID |= (unsigned __int64) 0x00000000FFC00000&((unsigned __int64)_fiber<<22);
	specObjID |= (unsigned __int64) 0x00000000003F0000&((unsigned __int64)_type<<16);
	specObjID |= (unsigned __int64) 0x000000000000FFFF&((unsigned __int64)0);

	return specObjID;
}


std::string Spectra::plateToString( int _plate )
{
	char buf[32]={0};
	sprintf( buf, "%04i", _plate );
	return std::string(buf);
}


float Spectra::indexToWaveLength( size_t _index, float _waveBegin, float _waveEnd, int _numSamples )
{
	float d = (_waveEnd-_waveBegin)/static_cast<float>(_numSamples);
	return _waveBegin+static_cast<float>(_index)*d;
}

size_t Spectra::waveLengthToIndex( float _waveLength, float _waveBegin, float _waveEnd, int _numSamples )
{
	float d = (_waveEnd-_waveBegin);
	d = (_waveLength-_waveBegin)/d;
	d = CLAMP( d, 0.f, 1.f );
	return static_cast<int>(floorf(d*static_cast<float>(_numSamples)));
}


std::string Spectra::spectraFilterToString( unsigned int spectraFilter )
{
	std::string sstrOutString;
	if ( spectraFilter & Spectra::SPT_SPEC_UNKNOWN )
	{
		sstrOutString += "SPEC_UNKNOWN ";
	}
	if ( spectraFilter & Spectra::SPT_SPEC_STAR )
	{
		sstrOutString += "SPEC_STAR ";
	}
	if ( spectraFilter & Spectra::SPT_SPEC_GALAXY )
	{
		sstrOutString += "SPEC_GALAXY ";
	}
	if ( spectraFilter & Spectra::SPT_SPEC_QSO )
	{
		sstrOutString += "SPEC_QSO ";
	}
	if ( spectraFilter & Spectra::SPT_SPEC_HIZ_QSO )
	{
		sstrOutString += "SPEC_HIZ_QSO ";
	}
	if ( spectraFilter & Spectra::SPT_SPEC_SKY )
	{
		sstrOutString += "SPEC_SKY ";
	}
	if ( spectraFilter & Spectra::SPT_STAR_LATE )
	{
		sstrOutString += "STAR_LATE ";
	}
	if ( spectraFilter & Spectra::SPT_GAL_EM )
	{
		sstrOutString += "GAL_EM ";
	}
	return sstrOutString;
}