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
//! \file  spectraHelpers.h
//! \brief 

#ifndef _SPECTRABASEHELPERS_H
#define _SPECTRABASEHELPERS_H


#include <string>
#include <vector>


class Spectra;


// Some low-level spectra helper functions 
// Additional, high-level helper functions can be found in SpectraHelpers.
namespace SpectraBaseHelpers
{
	// test compare and adpation performance in million compares/adaptions per second.
	// Writes temporary dump file perftest.bin in order to do the check.
	// \param [out] the millions of spectra compares (one to many) per second.
	// \param [out] the millions of spectra adaption (one to many) per second.
	// \param _skipIfFileExits skip if temporary binary already exits.
	void testSpectraPerformance( double &_outMioComparesPerSecond, double &_outMioAdaptionPerSecond, bool _skipIfFileExits=true );

	/** @name CALCULATIONS
	*/
	//@{

	// Fold a source spectrum by a given factor. For each iteration the spectrum size divides by two.
	// \param _pSrcSpectrum pointer to source spectrum. the data of the source spectrum is destroyed since we store intermediate information for folding in there.
	// \param _numSrcSamples number of samples/pixels of the source spectrum
	// \param _pDstSpectrum destination spectrum
	// \param _numDstSamples number of samples of the destination spectrum, must be at least the size of _numSrcSamples / (2^numFoldIterations)
	//        if the number of destination samples is greater than _numSrcSamples / (2^numFoldIterations) the rest is filled with zeros.
	// \param _numFoldIterations number of foldings. 
	void foldSpectrum( float *_pSrcSpectrum, size_t _numSrcSamples, float *_pDstSpectrum, size_t _numDstSamples, size_t _numFoldIterations=1 );

	// one to many spectra comparison.
	// _a source spectrum
	// _pB array of source spectra
	// _nCount number of Bs to compare with
	// _pOutErrors array of errors with _nCount elements
	void compareSpectra(const Spectra &_a, Spectra *_pB, size_t _nCount, float *_pOutErrors );

	// one to many spectra comparison.
	// _a source spectrum
	// _pB array of source spectra
	// _pOutErrors array of errors with _nCount elements
	void compareSpectra(const Spectra &_a, std::vector<Spectra*> &_pB, float *_pOutErrors );

	// repair isolated pixel errors of a given spectrum
	// _pixels pointer to spectrum data
	// _maskarray boolean array with flags set to true at pixel indicies where errors occur
	// _numsamples number of pixels 
	void repairSpectra( float *_pixels, bool *_maskarray, int _numsamples );
	//@}



};


#endif
