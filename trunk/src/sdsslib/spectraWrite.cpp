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
#include "sdsslib/Helpers.h"

#include <fstream>

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





SpectroLisWriter::SpectroLisWriter(const std::string &_sstrFilename )
:m_sstrFilename(_sstrFilename)
,m_sstrOutTable("")
,m_currentPlate(-1)
{
	m_sstrOutTable += "+ ./\n";
	m_sstrOutTable += "+ /spectro\n";
	m_sstrOutTable += "+ /spectro/1d_25\n";
}

SpectroLisWriter::~SpectroLisWriter()
{
	m_sstrOutTable += "- *\n";
	std::ofstream fon(m_sstrFilename.c_str());
	fon<<m_sstrOutTable;
}

void SpectroLisWriter::writeEntry( int _MJD, int _plate, int _fiberID )
{
	std::string sstrMJD = Helpers::numberToString<int>(_MJD, 5 );
	std::string sstrPlate = Helpers::numberToString<int>(_plate, 4 );
	std::string sstrFiberID = Helpers::numberToString<int>(_fiberID, 3 );

	if ( m_currentPlate != _plate )
	{
		m_sstrOutTable += "+ /spectro/1d_25/"+sstrPlate+"\n";
		m_sstrOutTable += "+ /spectro/1d_25/"+sstrPlate+"/1d\n";
		m_currentPlate = _plate;
	}
	m_sstrOutTable += "+ /spectro/1d_25/"+sstrPlate+"/1d/spSpec-"+sstrMJD+"-"+sstrPlate+"-"+sstrFiberID+".fit\n";
}


