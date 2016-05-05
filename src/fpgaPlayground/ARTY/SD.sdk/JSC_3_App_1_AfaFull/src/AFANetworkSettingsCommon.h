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


#ifndef AFA_NETWORKSETTINGS_COMMON_H__
#define AFA_NETWORKSETTINGS_COMMON_H__

#include <stddef.h>
#include "AFATypes.h"

typedef enum
{
    AFANET_SETTINGS_SEARCHMODE_localfast,
    AFANET_SETTINGS_SEARCHMODE_local,
    AFANET_SETTINGS_SEARCHMODE_global,

    AFANET_SETTINGS_NORMALIZATION_amplitude,
    AFANET_SETTINGS_NORMALIZATION_flux,
    AFANET_SETTINGS_NORMALIZATION_none,

    AFANET_SETTINGS_WAVELENGTHRANGE_sdss,
    AFANET_SETTINGS_WAVELENGTHRANGE_boss
} AFANET_SETTINGS;


typedef struct AFAParameters_
{
    int numSteps;												// number of learn steps. reasonable: 10..500
    int randomSeed;												// zero is not allowed
    float lRateBegin;											// learn rate begin 0.0 .. 1.0
    float lRateEnd;												// learn rate end (0.0 .. 1.0)
    float radiusBegin;											// radius begin (0.0 .. grid size)
    float radiusEnd;											// radius end (0.0 .. grid size)
    AFANET_SETTINGS searchMode;									// search mode to control quality vs. computation time
                                                                // SOFMNET_SETTINGS_SEARCHMODE_global     < uses global search for BMUs, best quality, runtime is O(n²)
                                                                // SOFMNET_SETTINGS_SEARCHMODE_local      < uses local search for BMUs if possible.
                                                                // SOFMNET_SETTINGS_SEARCHMODE_localfast  < uses very fast local search for BMUs if possible, runtime is O(n)
    SpectraNormalization normaliziationType;					// type of normalization of spectra

    bool_t useBOSSWavelengthRange;								// process spectra in an extended wavelength window of 3650..10400 Angstrom
                                                                // instead of the 3800..92000 SDSS default window.
} AFAParameters;

#endif // _AFANETWORKSETTINGS_H
