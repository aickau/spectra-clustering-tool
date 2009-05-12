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
//! \file  dump.cpp
//! \brief program to generate binary dumps from a directory of FITS files.


#include <Windows.h>
#include <conio.h>
#include "sdsslib/spectraVFS.h"
#include "sdsslib/spectra.h"
#include "sdsslib/helpers.h"

typedef char _TCHAR;

#define DATADIR std::string("c:/prj/sdss_svn2/trunk/data/*")
//#define DATADIR std::string("G:/SDSS_ANALYZE/fits/spectro/data/*")
 


void main(int argc, char* argv[])
{
	_cprintf("Welcome to SDSS Dump (Arjen's custom build version)!\n\n\n");
	_cprintf("reads sdss fits files from a given directory (and subdirectories) and dumps the data to a binary file.\n\n");

	if ( argc < 2 )
	{
		_cprintf( "No parameters specified. Using default settings.\n\n");
		_cprintf( "usage: dump.exe <datadir/*>\n");
	
		_cprintf( "example: dump.exe F:/SDSS_ANALYZE/fits/spectro/data/* \n\n\n");
	}

	std::string sstrDataDir = DATADIR;


	if ( argc > 1 )
	{
		sstrDataDir = std::string(argv[1]);
	}

	_cprintf( "datadir: %s\n", sstrDataDir.c_str() );
	_cprintf( "each spectrum contains %i bytes.\n\n", sizeof(Spectra) );

	SpectraVFS::write( sstrDataDir);
}

