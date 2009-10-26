//! \verbatim
//! ###########################################################################
//! # SDSS sorting prototype - Stage I
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

/** @name SETTING Groups
*/
//@{


#define SOFMNET_SETTINGS std::string("SETTINGS")
#define SOFMNET_SETTINGS_STEP std::string("STEP")
#define SOFMNET_SETTINGS_GRIDSIZE std::string("GRIDSIZE")
#define SOFMNET_SETTINGS_RANDOMSEED std::string("RANDOMSEED")
#define SOFMNET_SETTINGS_LEARNRATE std::string("LEARNRATE")
#define SOFMNET_SETTINGS_RADIUS std::string("RADIUS")
#define SOFMNET_SETTINGS_SPECTRUM std::string("SPECTRUM")
#define SOFMNET_SETTINGS_EXPORT std::string("EXPORT")
#define SOFMNET_SETTINGS_EXPORT_SUBPAGES std::string("SUBPAGES")
#define SOFMNET_SETTINGS_EXPORT_WAITFORUSER std::string("WAITFORUSER")
#define SOFMNET_SETTINGS_SEARCHMODE std::string("SEARCHMODE")
#define SOFMNET_SETTINGS_NORMALIZATION std::string("NORMALIZATION")

//@}

/** @name SETTING values
*/
//@{

#define SOFMNET_SETTINGS_SEARCHMODE_localfast std::string("localfast")
#define SOFMNET_SETTINGS_SEARCHMODE_local std::string("local")
#define SOFMNET_SETTINGS_SEARCHMODE_global std::string("global")

#define SOFMNET_SETTINGS_NORMALIZATION_amplitude std::string("amplitude")
#define SOFMNET_SETTINGS_NORMALIZATION_flux std::string("flux")
#define SOFMNET_SETTINGS_NORMALIZATION_none std::string("none")


//@}	


#endif // _SOFMNETWORKSETTINGS_H
