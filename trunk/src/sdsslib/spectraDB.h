//! \verbatim
//! ###########################################################################
//! # ASPECT: A spectra clustering tool for exploration of large spectral surveys - Stage I
//! #
//! # Copyright (c) 2012 Aick in der Au
//! # All rights reserved.
//! ###########################################################################
//!
//!      created by : Aick in der Au <aick.inderau@gmail.com>
//!      created on : 8/12/2012
//! additional docs : none
//!  responsibility : 1. Aick in der Au
//!                   2. 
//! \endverbatim
//!
//! \file  spectraDB.h
//! \brief Read additional spectra info from FITs file.

#ifndef _SPECTRADB_H
#define _SPECTRADB_H

#include "spectra.h"

#include <map>

class SpectraDB
{
public:
	struct Info
	{
		bool						success;
		double						z;
		Spectra::SpectraType		type;
	};

	bool loadDR9DB();

	Info getInfo( __int64 _specObjID );

private:

	std::map<__int64,Info> m_spectraDB;
};


#endif

