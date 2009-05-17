//! \verbatim
//! ###########################################################################
//! # SDSS sorting prototype - Stage I
//! #
//! # Copyright (c) 2009 Aick in der Au
//! # All rights reserved.
//! ###########################################################################
//!
//!      created by : Aick in der Au <aick.inderau@gmail.com>
//!      created on : 5/17/2009
//! additional docs : none
//!  responsibility : 1. Aick in der Au
//!                   2. 
//! \endverbatim
//!
//! \file  spectraWrite.h
//! \brief 


#ifndef _SPECTRAWRITE_H
#define _SPECTRAWRITE_H

#include <Windows.h>
#include <string>

class Spectra;


// helper class for scope based writing spectra to disk.
// Note: the file is closed on destruction of the object.
class SpectraWrite
{
public:

	// creates a file with the given filename.
	SpectraWrite( const std::string &_sstrFileName );
	virtual ~SpectraWrite();

	// writes the given spectrum to file
	// returns true on success, otherwise false.
	bool write( const Spectra &_spectra );
private:
	HANDLE m_fileHandle;
};

#endif