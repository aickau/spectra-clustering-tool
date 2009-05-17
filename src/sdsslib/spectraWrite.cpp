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
//! \file  spectraWrite.cpp
//! \brief 



#include "sdsslib/spectraWrite.h"
#include "sdsslib/spectra.h"


SpectraWrite::SpectraWrite( const std::string &_sstrFileName )
:m_fileHandle(0)
{
	m_fileHandle = CreateFile( _sstrFileName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL );
}


SpectraWrite::~SpectraWrite()
{
	if ( m_fileHandle != NULL )
	{
		CloseHandle(m_fileHandle);
	}
}


bool SpectraWrite::write( const Spectra &_spectra )
{
	if ( m_fileHandle == 0 )
	{
		return false;
	}

	DWORD bytesWritten = 0;
	bool bResult = (WriteFile( m_fileHandle, &_spectra, sizeof(Spectra), &bytesWritten, NULL ) > 0);
	if (bytesWritten != sizeof(Spectra))
	{
		bResult = false;
	}

	return bResult;
}