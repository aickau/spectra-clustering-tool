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


#include "spectra.h"

#include <iostream>
#include <fstream>
#include <map>
#include <conio.h>
#include <math.h>
#include <float.h>
#include <assert.h>
#include "../cfitsio/fitsio.h"
#include "../cfitsio/longnam.h"

//#include "fftw/fftw3.h"

#include "sdsslib/random.h"
#include "sdsslib/helpers.h"
#include "sdsslib/mathhelpers.h"
#include "sdsslib/defines.h"


// spectra read from SDSS FITS have a wavelength of 3800..9200 Angstr�m
// full spectrum range should range from ~540 Amgstr�m to 9200 assuming max redshifts of 6.
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
	m_Type = SPT_NOT_SET;
	m_Z = 0.0;

	m_RealZ= 0.0;
	m_Mi=0.0;					
	m_coeff0=0.0;
	m_coeff1=0.0;

	for (size_t i=0;i<Spectra::numSamples;i++)
	{
		m_Amplitude[i] = 0.0f;
		m_stdDev[i] = 0.0f;		
		m_badPixels[i] = false;		
	}

#ifdef _USE_SPECTRALINES
	for (size_t i=0;i<Spectra::numSpectraLines;i++)
	{
		m_Lines[i].height = 0.0f;
		m_Lines[i].wave = 0.0f;
		m_Lines[i].waveMin = 0.0f;
		m_Lines[i].waveMax = 0.0f;
	}
#endif
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

	Rnd r;

	for (size_t i=0;i<Spectra::numSamples;i++)
	{
		m_Amplitude[i] = r.randomFloat()*d+minrange;
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
		m_stdDev[i] = _spectra.m_stdDev[i];		
		m_badPixels[i] = _spectra.m_badPixels[i];		

	}
#ifdef _USE_SPECTRALINES
	for (size_t i=0;i<Spectra::numSpectraLines;i++)
	{
		m_Lines[i].height = _spectra.m_Lines[i].height;
		m_Lines[i].wave = _spectra.m_Lines[i].wave;
		m_Lines[i].waveMin = _spectra.m_Lines[i].waveMin;
		m_Lines[i].waveMax = _spectra.m_Lines[i].waveMax;
	}
#endif
}


void Spectra::set( size_t type, float _noize )
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

	Rnd r;

	for (size_t i=0;i<Spectra::numSamples;i++)
	{
		float x=static_cast<float>(i)*0.01f;
		m_stdDev[i] = 0.0f;		
		m_badPixels[i] = false;		


		if ( type == 0 )
		{
			m_Amplitude[i] = sinf(x)*20.f+(r.randomFloat()-0.5f)*_noize;
		}

		if ( type == 1 )
		{
			m_Amplitude[i] = cosf(x)*20.f+(r.randomFloat()-0.5f)*_noize;
		}

		if ( type == 2 )
		{
			m_Amplitude[i] = x+(r.randomFloat()-0.5f)*_noize;
		}

		if ( type == 3 )
		{
			m_Amplitude[i] = -x+(r.randomFloat()-0.5f)*_noize;
		}

		if ( type == 4 )
		{
			m_Amplitude[i] = x*x+(r.randomFloat()-0.5f)*_noize;
		}
	}



//	long peak_pos = mt_random_int() % Spectra::numSamples;
//	float x=static_cast<float>(peak_pos)*0.01f;
//	m_Amplitude[peak_pos] = 60.f;

	m_SamplesRead = Spectra::numSamples;

	calcMinMax();
}


void Spectra::setSine( float _frequency, float _phase, float _amplitude, float _noize )
{
	static size_t UIDCount = 1;
	m_SpecObjID =(UIDCount++)<<22;

	Rnd r;
	for (size_t i=0;i<Spectra::numSamples;i++)
	{
		float x=_phase+static_cast<float>(i)*_frequency;
		m_Amplitude[i] = sinf(x)*_amplitude+(r.randomFloat()-0.5f)*_noize;
		m_stdDev[i] = 0.0f;		
		m_badPixels[i] = false;		
	}
	m_SamplesRead = Spectra::numSamples;
	calcMinMax();
}

void Spectra::setRect( float _width, float _phase, float _amplitude )
{
	static size_t UIDCount = 1;
	m_SpecObjID =(UIDCount++)<<22;
	
	float x = 0.0f;
	float xInc = 1.f/static_cast<float>(Spectra::numSamples);


	for (size_t i=0;i<Spectra::numSamples;i++)
	{
		if ( x>= _phase && x<= _phase+_width )
		{
			m_Amplitude[i] = _amplitude;
		}
		else
		{
			m_Amplitude[i] = 0.0f;
		}
		m_stdDev[i] = 0.0f;		
		m_badPixels[i] = false;		

		x += xInc;
	}


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
		// TODO: add m_stdDev, m_badPixels and wavelength
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
#define _ZBACKCALC

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

	fits_read_key( f, TDOUBLE, "COEFF0", &m_coeff0, NULL, &status );
	fits_read_key( f, TDOUBLE, "COEFF1", &m_coeff1, NULL, &status );

	// read spectral data
	fits_get_img_size(f, 2, size, &status );
	const size_t spectrumMaxSize = 3900; 
	float spectrum[spectrumMaxSize];
	float noize[spectrumMaxSize];
	unsigned long maskarray[spectrumMaxSize];
	
	size_t elementsToRead = size[0];
	assert( elementsToRead <= spectrumMaxSize );
	elementsToRead = MIN( elementsToRead, spectrumMaxSize );

	// spec says:
	// The first row is the spectrum,
	// the second row is the continuum subtracted spectrum, 
	// the third row is the noise in the spectrum (standard deviation, in the same units as the spectrum), 
	// the forth row is the mask array. The spectra are binned log-linear. Units are 10^(-17) erg/cm/s^2/Ang.
	
	// read spectrum
	long adress[2]={1,1};
	fits_read_pix( f, TFLOAT, adress, elementsToRead, NULL, (void*)spectrum, NULL, &status );

	// read noize
	adress[1] = 3;
	fits_read_pix( f, TFLOAT, adress, elementsToRead, NULL, (void*)noize, NULL, &status );

	// mask array
	adress[1] = 4;
	fits_read_pix( f, TLONG, adress, elementsToRead, NULL, (void*)maskarray, NULL, &status );


#ifdef _ZBACKCALC

	// Arjen says:
	// to convert pixel no. to wavelength (in the observers frame), use the following equation:
	// wavelenght = 10^(coeff0+coeff1*pixel)
	// where coeff0 and coeff1 are in the fits file. pixel is the pixel number.
	// use wavelenght_0 = (wavelenght/(1+z)) to shift spectra into rest frame for a few redshift bins (low, moderate, high), which are:
	//  0.5 < z < 0.6  : 2779 spectra, cut to 2535-5750 angstroems in the wavelenght_0 rest frame
	//	1.5 < z < 1.6  : 4662             "   1520-3535              "         
	//	3.6 < z < 3.8  : 771              "    830-1910              "           

	float pixelNum = 1.f;
	for (size_t i=0;i<elementsToRead;i++)
	{
		float wavelength = powf(10.f, m_coeff0+m_coeff1*pixelNum);
		float wavelength_0 = wavelength/(1.f+m_RealZ);

		float waveMin0, waveMax0;

		//spectrum[c] = 
		if ( m_RealZ >= 0.5f && m_RealZ <= 0.6f )
		{
			// low
			waveMin0 = 2535.f;
			waveMax0 = 5750.f;
		}
		else 
		if ( m_RealZ >= 1.5f && m_RealZ <= 1.6f )
		{
			// medium
			waveMin0 = 1520.f;
			waveMax0 = 3535.f;
		}
		else 
		if ( m_RealZ >= 3.6f && m_RealZ <= 3.8f )
		{
			// max
			waveMin0 = 830.f;
			waveMax0 = 1910.f;
		}
		else
		{
			continue;
		}

		wavelength_0 = CLAMP( wavelength_0, waveMin0, waveMax0);
		size_t i0 = Spectra::waveLengthToIndex( wavelength_0, waveMin0, waveMax0, numSamples );
		m_Amplitude[i0] = spectrum[i];
		m_stdDev[i0] = noize[i];
		const unsigned int mask = (!SpectraMask::SP_MASK_OK) && (!SpectraMask::SP_MASK_EMLINE);
		m_badPixels[i0] = (maskarray[i] & mask) != 0; // pixel ok only if SP_MASK_OK or SP_MASK_EMLINE is set.

		pixelNum+=1.f;
	}
	m_SamplesRead = numSamples;

	
/*
	old z-back calc:
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
	m_SamplesRead = numSamples;

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

	*/
#else // _ZBACKCALC
	const size_t sampleReductionRatio = 2;
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
#endif // _ZBACKCALC
#ifdef _USE_SPECTRALINES
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
				
				// uncomment to use spectra lines as spectrum
				//size_t indexBegin=Spectra::waveLengthToIndex(waveMin);
				//size_t indexEnd=Spectra::waveLengthToIndex(waveMax);
				//for (size_t j=indexBegin;j<indexEnd;j++) {
				//	m_Amplitude[j]=height;
				//}
			}
		}
	}
#endif // _USE_SPECTRALINES
	fits_close_file(f, &status);

	calcMinMax();

	//return ((m_SamplesRead > numSamples/2) && (status == 0));	
	return (status == 0);
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
	/*
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
	*/
	// normalize by flux:
	float flux = 0.f;
	for (size_t i=0;i<Spectra::numSamples;i++)
	{
		if ( !m_badPixels[i] )
		{
			flux += m_Amplitude[i];
		}
	}

	if ( flux <= 0.0f )
		return;

	for (size_t i=0;i<Spectra::numSamples;i++)
	{
		if ( !m_badPixels[i] )
		{
			m_Amplitude[i] /= flux;
		}
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
	size_t numSamples4 = (Spectra::numSamples >> 3) << 3;

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

	int i=numSamples4;
	for (i;i<Spectra::numSamples;i++)
	{
		float d = m_Amplitude[i]-_spectra.m_Amplitude[i];
		error += d*d;
	}
	return error;
}



float Spectra::compareAdvanced(const Spectra &_spectra, float _width) const
{
	assert(_width>=0.0f);
	float error=0.0f;
	int width  = static_cast<int>((static_cast<float>(Spectra::numSamples)*_width));

	if (_width<=0||width==0)
	{
		// use default comparison if width=0
		return compare(_spectra);
	}

	for (int i=0;i<Spectra::numSamples;i++)
	{
		float d2 = FLT_MAX;
		int wBegin = MAX(static_cast<int>(i)-width, 0);
		int wEnd = MIN(i+width, Spectra::numSamples);

		for (int w=wBegin;w<wEnd;w++) {
			float d = m_Amplitude[i]-_spectra.m_Amplitude[w];
			d*=d;
			d2 = MIN(d2,d);
		}

		error += d2;
	}
	return error;
}

static
void genPeakSpectrum( const std::vector<float> &_peakArray, float _width, size_t _numSamples, std::vector<float> &_outPeakSpectrum )
{
	_outPeakSpectrum.resize( _numSamples );
	for ( size_t i=0;i<_numSamples;i++ ) {
		_outPeakSpectrum[i] = 0.0f;
	}
	//todo: calculate right bounds using a formula instead of this iterative shit.
	size_t w=1;
	while (w<_numSamples) {
		const float g =  MathHelpers::gauss1D( w, 1.f,0.f, _width );
		if ( g < 0.01f ) {
			break;
		}
		w++;
	}
	
	for ( size_t j=0;j<_peakArray.size();j+=2) {
		int beg = MAX(_peakArray[j+1]-w,0);
		int end = MIN(_peakArray[j+1]+w,_numSamples-1);
		for ( int i=beg;i<end;i++ ) {
			_outPeakSpectrum[i] = MAX( MathHelpers::gauss1D( static_cast<float>(i)-_peakArray[j+1], fabsf(_peakArray[j]), 0.f, _width ), _outPeakSpectrum[i] );
		}
	}
}

static
void genGaussLUT(float *_pGaussLUT, int _GaussLUTSize, float _width)
{
	assert(_pGaussLUT != NULL);

	for ( int i=0;i<_GaussLUTSize;i++ ) {
		_pGaussLUT[i] = MathHelpers::gauss1D( i-_GaussLUTSize/2, 1.f, 0.f, _width );
	}
}

static
void genPeakSpectrum2( const std::vector<float> &_peakArray, float *_pGaussLUT, int _GaussLUTSize, size_t _numSamples, std::vector<float> &_outPeakSpectrum )
{
	assert(_pGaussLUT != NULL);
	_outPeakSpectrum.resize( _numSamples );
	for ( size_t i=0;i<_numSamples;i++ ) {
		_outPeakSpectrum[i] = 0.0f;
	}

	const size_t w=128;

	for ( size_t j=0;j<_peakArray.size();j+=2) {
		int beg = MAX(_peakArray[j+1]-w,0);
		int end = MIN(_peakArray[j+1]+w,_numSamples-1);
		for ( int i=beg;i<end;i++ ) {
			// calc gaussian peak uses lookup table
			int ind = CLAMP(i-_peakArray[j+1]+_GaussLUTSize/2,0,_GaussLUTSize);
			_outPeakSpectrum[i] = MAX( _pGaussLUT[ind]*fabsf(_peakArray[j]), _outPeakSpectrum[i] );
		}
	}
}



float Spectra::compareSuperAdvanced(const Spectra &_spectra, float _width, bool _bOptimize) const
{
	const size_t continuumMaxSize = 32;
	const size_t numPeaks = 20;
	const float peakCutOffTreshold = 2.f;

	// map _width (0..1] to right range.
	_width *= static_cast<float>(Spectra::numSamples/4);

	// (1) generate continuum spectrum and generate spectrum minus continuum 
	std::vector<float> continuum1;
	std::vector<float> continuum2;
	std::vector<float> spectrumMinusContinuum1;
	std::vector<float> spectrumMinusContinuum2;
	getSpectrumMinusContinuum( continuumMaxSize, spectrumMinusContinuum1, continuum1 );
	_spectra.getSpectrumMinusContinuum( continuumMaxSize, spectrumMinusContinuum2, continuum2 );


	// (2) detect peaks
	std::vector<float> minPeaks1;
	std::vector<float> maxPeaks1;
	std::vector<float> minPeaks2;
	std::vector<float> maxPeaks2;
	getPeaks( spectrumMinusContinuum1, numPeaks, peakCutOffTreshold, minPeaks1, maxPeaks1 );
	_spectra.getPeaks( spectrumMinusContinuum2, numPeaks, peakCutOffTreshold, minPeaks2, maxPeaks2 );

	// (3) generate artificial sprectra with smoothed peaks using gauss function
	std::vector<float> minPeaksS1;
	std::vector<float> maxPeaksS1;
	std::vector<float> minPeaksS2;
	std::vector<float> maxPeaksS2;


	// uses optimized version
	if ( _width > 24.f && _width < 25.f && _bOptimize)
	{
		static const int gaussLUTSize = 256;
		static float gaussLUT[gaussLUTSize];
		static bool bInitLUT = true;
		if (bInitLUT)
		{
			genGaussLUT(gaussLUT, gaussLUTSize, _width );
			bInitLUT = false;
		}

		genPeakSpectrum2( minPeaks1, gaussLUT, gaussLUTSize, Spectra::numSamples, minPeaksS1 );
		genPeakSpectrum2( maxPeaks1, gaussLUT, gaussLUTSize, Spectra::numSamples, maxPeaksS1 );
		genPeakSpectrum2( minPeaks2, gaussLUT, gaussLUTSize, Spectra::numSamples, minPeaksS2 );
		genPeakSpectrum2( maxPeaks2, gaussLUT, gaussLUTSize, Spectra::numSamples, maxPeaksS2 );
	}
	else
	{
		genPeakSpectrum( minPeaks1, _width, Spectra::numSamples, minPeaksS1 );
		genPeakSpectrum( maxPeaks1, _width, Spectra::numSamples, maxPeaksS1 );
		genPeakSpectrum( minPeaks2, _width, Spectra::numSamples, minPeaksS2 );
		genPeakSpectrum( maxPeaks2, _width, Spectra::numSamples, maxPeaksS2 );
	}

	float errMinPeaks = MathHelpers::getError( &minPeaksS1[0], &minPeaksS2[0], Spectra::numSamples );
	float errMaxPeaks = MathHelpers::getError( &maxPeaksS1[0], &maxPeaksS2[0], Spectra::numSamples );
	float errContinuum = MathHelpers::getError( &continuum1[0], &continuum2[0], continuum1.size() );

	// combined error, min+max peaks+continuum
	double errTotal = sqrtf((errMinPeaks+errMaxPeaks)*errContinuum);

	return static_cast<float>(errTotal);
}




void Spectra::dft()
{
	// test code
	//fftwf_complex complex[numSamples/2+1];
	//fftwf_plan p;

	//size_t numSamplesHalf = numSamples/2;

	//bool bReverse = (pad[0] > 0); 

	//if ( bReverse )
	//{
	//	for ( size_t i=0;i<numSamplesHalf;i++) {
	//		complex[i][0] = m_Amplitude[i];
	//		complex[i][1] = m_Amplitude[numSamplesHalf+i];
	//	}

	//	p = fftwf_plan_dft_c2r_1d( numSamples-2, complex, &m_Amplitude[0], FFTW_ESTIMATE ); //FFTW_MEASURE
	//	fftwf_execute(p); 
	//	pad[0] = 0;
	//}
	//else
	//{
	//	p = fftwf_plan_dft_r2c_1d( numSamples-2, &m_Amplitude[0], complex, FFTW_ESTIMATE ); //FFTW_MEASURE
	//	fftwf_execute(p); 

	//	for ( size_t i=0;i<numSamplesHalf;i++) {
	//		m_Amplitude[i] = complex[i][0];
	//		m_Amplitude[i+numSamplesHalf] = complex[i][1];
	//	}
	//	pad[0] = 1;
	//}

	//calcMinMax();
}


void Spectra::generateContinuum( size_t _continuumSamples, std::vector<float> &_outContinuum ) const
{
	float continuum[Spectra::numSamples];
	memcpy( continuum, m_Amplitude, Spectra::numSamples*sizeof(float) );

	size_t continuumSize = Spectra::numSamples;
	do {
		continuumSize = MathHelpers::fold1D( continuum, continuumSize );
	} while ( continuumSize >= _continuumSamples );

	_outContinuum.resize(continuumSize);
	for ( size_t i=0;i<continuumSize;i++ ) {
		_outContinuum[i] = continuum[i] ;
	}
}



void Spectra::getSpectrumMinusContinuum( size_t _continuumSamples, std::vector<float> &_outSpectrumMinusContinuum, std::vector<float> &_outContinuum ) const
{
	_outSpectrumMinusContinuum.resize(Spectra::m_SamplesRead);
	generateContinuum( _continuumSamples, _outContinuum );

	const float continuumSizef = static_cast<float>(_outContinuum.size());
	float c=0.0f;
	const float cInc=(continuumSizef-1.f)/static_cast<float>(Spectra::numSamples);
	for ( size_t i=0;i<static_cast<size_t>(Spectra::m_SamplesRead);i++ )
	{
		const float c0 = floorf(c); 
		const size_t i0 = static_cast<size_t>(c0);
		const float c1 = c - c0;
		_outSpectrumMinusContinuum[i] = m_Amplitude[i] - MathHelpers::lerp( _outContinuum[i0], _outContinuum[i0+1], c1 );
		c+=cInc;
	}
}

void Spectra::getPeaks( const std::vector<float> &_spectrumMinusContinuum, size_t _numPeaks, float _cutOffTreshold, std::vector<float> &_outMinPeaks, std::vector<float> &_outMaxPeaks ) const
{
	assert(_cutOffTreshold > 0.0f);
	assert( m_SamplesRead <= static_cast<int>(_spectrumMinusContinuum.size()) );

	typedef std::map<float, int> PeakMap;

	PeakMap minPeaksMap;
	PeakMap maxPeaksMap;

	if ( m_SamplesRead <= 1 && _spectrumMinusContinuum.size() <= 1) {
		assert(0);
		return;
	}

	const int lastSample = m_SamplesRead-1;

	if ( _spectrumMinusContinuum[0] > _spectrumMinusContinuum[1] ) {
		maxPeaksMap.insert( std::make_pair<float,int>(_spectrumMinusContinuum[0],0) );
	}

	if ( _spectrumMinusContinuum[lastSample] < _spectrumMinusContinuum[lastSample-1] ) {
		minPeaksMap.insert( std::make_pair<float,int>(_spectrumMinusContinuum[lastSample],lastSample) );
	}

	for ( int i=1;i<lastSample;i++ )
	{
		// process maximum peaks
		/////////////////////////////////////////////////

		// do we have a peak ?
		const bool bHaveMaxPeak = _spectrumMinusContinuum[i-1] < _spectrumMinusContinuum[i] && 
			                      _spectrumMinusContinuum[i+1] < _spectrumMinusContinuum[i];

		if ( bHaveMaxPeak )
		{
			if (maxPeaksMap.size()>=_numPeaks ) 
			{
				// check if existing smallest max peak is greater than current peak.
				PeakMap::iterator itFirst = maxPeaksMap.begin();
				if ( itFirst->first < _spectrumMinusContinuum[i] ) 
				{
					//  if not, erase.
					maxPeaksMap.erase( itFirst );
				}
			}
			// add new peak to list if last peak in map was erased
			if ( maxPeaksMap.size() < _numPeaks ) 
			{
				maxPeaksMap.insert( std::make_pair<float,int>(_spectrumMinusContinuum[i],i) );
			}
		}
	


		// process minimum peaks
		/////////////////////////////////////////////////

		// do we have a peak ?
		const bool bHaveMinPeak = _spectrumMinusContinuum[i-1] > _spectrumMinusContinuum[i] && 
								  _spectrumMinusContinuum[i+1] > _spectrumMinusContinuum[i];

		if ( bHaveMinPeak )
		{
			if (minPeaksMap.size()>=_numPeaks) 
			{
				PeakMap::iterator itLast = minPeaksMap.end();
				itLast--;

				if ( itLast->first > _spectrumMinusContinuum[i] ) 
				{
					minPeaksMap.erase( itLast );
				}
			}
			if ( minPeaksMap.size() < _numPeaks )
			{
				minPeaksMap.insert( std::make_pair<float,int>(_spectrumMinusContinuum[i],i) );
			}
		}
	}

	float globalMinPeak = 1.f;
	float globalMaxPeak = 1.f;

	if ( maxPeaksMap.size() > 0 ) {
		PeakMap::iterator itLast = maxPeaksMap.end();
		itLast--;
		globalMaxPeak =  fabsf(itLast->first);
		if ( globalMaxPeak == 0.0f ) {
			globalMaxPeak = 1.f;
		}
	}

	if ( minPeaksMap.size() > 0 )
	{
		globalMinPeak =  fabsf(minPeaksMap.begin()->first);
		if ( globalMinPeak == 0.0f ) {
			globalMinPeak = 1.f;
		}
	}

	const float globalPeak = MAX( globalMaxPeak, globalMinPeak );
	std::vector<float> maxPeaks;
	std::vector<float> minPeaks;


	// normalize min peaks
	{
		PeakMap::iterator it = minPeaksMap.begin();
		while (it != minPeaksMap.end() ) {
			const float p = it->first/globalPeak;
			minPeaks.push_back( p );
			minPeaks.push_back( static_cast<float>(it->second) );
			it++;
		}
	}

	// normalize max peaks
	{
		PeakMap::iterator it = maxPeaksMap.begin();
		while (it != maxPeaksMap.end() ) {
			const float p = it->first/globalPeak;
			maxPeaks.push_back( p );
			maxPeaks.push_back( static_cast<float>(it->second) );
			it++;
		}
	}


	// generate gradient for peaks
	std::vector<float> maxPeaksG(maxPeaks);
	std::vector<float> minPeaksG(minPeaks);
	if ( maxPeaksG.size() > 0 )
	{
		MathHelpers::gradient1D( &maxPeaksG[0], maxPeaksG.size()/2, 8, 0 );
		const float gradientTresholdMaxPeaks = MIN(2.f/(static_cast<float>(maxPeaksG.size())*_cutOffTreshold),1.f);

		for (size_t i=0;i<maxPeaksG.size();i+=2) {
			if ( maxPeaksG[i]<gradientTresholdMaxPeaks) {
				maxPeaks[i] = 0.0f;
				maxPeaks[i+1] = 0.0f;
			}
			else
			{
				break;
			}
		}
	}


	if ( minPeaksG.size() > 0 )
	{
		MathHelpers::gradient1D( &minPeaksG[0], minPeaksG.size()/2, 8, 0 );
		const float gradientTresholdMinPeaks = MIN(2.f/(static_cast<float>(minPeaksG.size())*_cutOffTreshold),1.f);

		for (size_t i=minPeaksG.size()-2;i>0;i-=2) {
			if ( minPeaksG[i]<gradientTresholdMinPeaks) {
				minPeaks[i] = 0.0f;
				minPeaks[i+1] = 0.0f;
			}
			else
			{
				break;
			}
		}
	}

	for ( size_t i=0;i<maxPeaks.size();i+=2 ) {
		if ( maxPeaks[i] != 0.0f ) {
			_outMaxPeaks.push_back( maxPeaks[i] );
			_outMaxPeaks.push_back( maxPeaks[i+1] );
		}
	}

	for ( size_t i=0;i<minPeaks.size();i+=2 ) {
		if ( minPeaks[i] != 0.0f ) {
			_outMinPeaks.push_back( minPeaks[i] );
			_outMinPeaks.push_back( minPeaks[i+1] );
		}
	}

	/*
	treshold calculation
	const float k=0.5;
	float m,d;
	float mAbs,dAbs;
	float mPos,dPos;
	float mNeg,dNeg;
	float gmin, gmax;
	MathHelpers::getMinMax( &spectrumMinusContinuum[0], spectrumMinusContinuum.size(), 4, 0, gmin, gmax );
	MathHelpers::getMeanDeviation( &spectrumMinusContinuum[0], spectrumMinusContinuum.size(), 4, 0, MathHelpers::MEAN_STANDARD, m, d );
	MathHelpers::getMeanDeviation( &spectrumMinusContinuum[0], spectrumMinusContinuum.size(), 4, 0, MathHelpers::MEAN_ABSOLUTE,mAbs, dAbs );
	MathHelpers::getMeanDeviation( &spectrumMinusContinuum[0], spectrumMinusContinuum.size(), 4, 0, MathHelpers::MEAN_POSITIVE, mPos, dPos );
	MathHelpers::getMeanDeviation( &spectrumMinusContinuum[0], spectrumMinusContinuum.size(), 4, 0, MathHelpers::MEAN_NEGATIVE, mNeg, dNeg );
	const float treshholdMax = (gmax+mPos)*0.5f+k*dPos;
	const float treshholdMin = (gmin+mNeg)*0.5f-k*dNeg;
	*/
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