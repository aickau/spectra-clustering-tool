//! \verbatim
//! ###########################################################################
//! # ASPECT: A spectra clustering tool for exploration of large spectral surveys - Stage I
//! #
//! # Copyright (c) 2009 Aick in der Au
//! # All rights reserved.
//! ###########################################################################
//!
//!      created by : Aick in der Au <aick.inderau@gmail.com>
//!      created on : 7/19/2009
//! additional docs : none
//!  responsibility : 1. Aick in der Au
//!                   2. 
//! \endverbatim
//!
//! \file  SOFMNetworkSettings.h
//! \brief Settings defines


#ifndef _SOFMNETWORKSETTINGS_H
#define _SOFMNETWORKSETTINGS_H

#include <string>

#include "sdsslib/spectra.h"


class SOFMParameters
{
public:
	SOFMParameters( size_t _numSteps, size_t _randomSeed, float _lRateBegin, float _lRateEnd, float _radiusBegin, float _radiusEnd);

	size_t numSteps;											// number of learn steps. reasonable: 10..500
	size_t randomSeed;											// zero is not allowed
	float lRateBegin;											// learn rate begin 0.0 .. 1.0
	float lRateEnd;												// learn rate end (0.0 .. 1.0)
	float radiusBegin;											// radius begin (0.0 .. grid size)
	float radiusEnd;											// radius end (0.0 .. grid size)
	bool exportSubPage;											// if true add sub pages to HTML output
	bool waitForUser;											// if true wait for user input after each calculation step
	bool fullExport;											// if true create a detailed export for each spectrum after final calculation step (online useful for small maps)
	int imageBoderSize;											// to draw colored frames around the image icons, to distinguish between visited and not visited images
	int iconSize;												// icon width/height in pixels for all spectra icons
	std::string sstrSearchMode;									// search mode to control quality vs. computation time
	// SOFMNET_SETTINGS_SEARCHMODE_global     < uses global search for BMUs, best quality, runtime is O(n²)
	// SOFMNET_SETTINGS_SEARCHMODE_local      < uses local search for BMUs if possible.
	// SOFMNET_SETTINGS_SEARCHMODE_localfast  < uses very fast local search for BMUs if possible, runtime is O(n) 
	Spectra::SpectraNormalization normaliziationType;			// type of normalization of spectra

	bool useBOSSWavelengthRange;								// process spectra in an extended wavelength window of 3650..10400 Angstrom 
	// instead of the 3800..92000 SDSS default window.




	static SOFMParameters defaultParameters;
};


/** @name SETTING Groups
*/
//@{


#define SOFMNET_SETTINGS						std::string("SETTINGS")
#define SOFMNET_SETTINGS_STEP					std::string("STEP")
#define SOFMNET_SETTINGS_GRIDSIZE				std::string("GRIDSIZE")
#define SOFMNET_SETTINGS_RANDOMSEED				std::string("RANDOMSEED")
#define SOFMNET_SETTINGS_LEARNRATE				std::string("LEARNRATE")
#define SOFMNET_SETTINGS_RADIUS					std::string("RADIUS")
#define SOFMNET_SETTINGS_SPECTRUM				std::string("SPECTRUM")
#define SOFMNET_SETTINGS_EXPORT					std::string("EXPORT")
#define SOFMNET_SETTINGS_EXPORT_SUBPAGES		std::string("SUBPAGES")
#define SOFMNET_SETTINGS_EXPORT_WAITFORUSER		std::string("WAITFORUSER")
#define SOFMNET_SETTINGS_EXPORT_FULLEXPORT		std::string("FULLEXPORT")
#define SOFMNET_SETTINGS_EXPORT_IMGBORDERSIZE	std::string("IMGBORDERSIZE")
#define SOFMNET_SETTINGS_EXPORT_ICONSIZE		std::string("ICONSIZE")
#define SOFMNET_SETTINGS_SEARCHMODE				std::string("SEARCHMODE")
#define SOFMNET_SETTINGS_NORMALIZATION			std::string("NORMALIZATION")
#define SOFMNET_SETTINGS_WAVELENGTHRANGE		std::string("WAVELENGTHRANGE")

//@}

/** @name SETTING values
*/
//@{

#define SOFMNET_SETTINGS_SEARCHMODE_localfast		std::string("localfast")
#define SOFMNET_SETTINGS_SEARCHMODE_local			std::string("local")
#define SOFMNET_SETTINGS_SEARCHMODE_global			std::string("global")

#define SOFMNET_SETTINGS_NORMALIZATION_amplitude	std::string("amplitude")
#define SOFMNET_SETTINGS_NORMALIZATION_flux			std::string("flux")
#define SOFMNET_SETTINGS_NORMALIZATION_none			std::string("none")

#define SOFMNET_SETTINGS_WAVELENGTHRANGE_sdss		std::string("sdss")
#define SOFMNET_SETTINGS_WAVELENGTHRANGE_boss		std::string("boss")

//@}	


#endif // _SOFMNETWORKSETTINGS_H
