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
#include "sdsslib/filehelpers.h"

typedef char _TCHAR;

#define DATADIR std::string("/data/*")
//#define DATADIR std::string("G:/SDSS_ANALYZE/fits/spectro/data/*")
#define DUMPFILE std::string("allSpectra.bin")
 


void main(int argc, char* argv[])
{
	_cprintf("Welcome to SDSS Dump!\n\n\n");
	_cprintf("reads SDSS fits files from a given directory (and subdirectories) and dumps the data to a binary file.\n");
	_cprintf("reads binary dump files and extracts text tables out of it.\n\n");

	if ( argc < 2 )
	{
		_cprintf( "No parameters specified. Using default settings.\n\n");
		_cprintf( "usage: dump.exe <datadir/*> [<outputfilename.bin>] [<filter>]\n");
		_cprintf( "usage: dump.exe <inputfilename.bin>\n");
		_cprintf( "where <filter> is any added combination of:\n");
		_cprintf( "  SPEC_UNKNOWN =   1\n");
		_cprintf( "  SPEC_STAR    =   2\n");
		_cprintf( "  SPEC_GALAXY  =   4\n");
		_cprintf( "  SPEC_QSO     =   8\n");
		_cprintf( "  SPEC_HIZ_QSO =  16\n");
		_cprintf( "  SPEC_SKY     =  32\n");
		_cprintf( "  STAR_LATE    =  64\n");
		_cprintf( "  GAL_EM       = 128\n\n\n");

		_cprintf( "example: dump.exe F:/SDSS_ANALYZE/fits/spectro/data/allSpectra.bin 25\n");
		_cprintf( "example: dump.exe sofmnet.bin\n\n\n");
	}

	std::string sstrDataDir = FileHelpers::getCurrentDirectory()+DATADIR;
	std::string sstrDumpFile = DUMPFILE;
	unsigned int spectraFilter = 0x0fffff;//SPT_DEFAULTFILTER;

	bool bReverseRead = false;

	if ( argc > 1 )
	{
		sstrDataDir = std::string(argv[1]);

		if ( FileHelpers::getFileExtension( sstrDataDir ) == ".bin" )
		{
			bReverseRead = true;
		}
	}
	if ( argc > 2 )
	{
		sstrDumpFile = std::string(argv[2]);
	}

	if ( argc > 3 )
	{
		spectraFilter = Helpers::stringToNumber<unsigned int>( std::string(argv[3]) );
	}

	if ( !bReverseRead )
	{
		_cprintf( "Creating binary dump with following parameters:\n");
		_cprintf( "datadir: %s\n", sstrDataDir.c_str() );
		_cprintf( "dumpfile: %s\n", sstrDumpFile.c_str() );
		_cprintf( "filter: %s\n", Spectra::spectraFilterToString(spectraFilter).c_str() );
		_cprintf( "each spectrum contains %i bytes.\n\n", sizeof(Spectra) );

		SpectraVFS::write( sstrDataDir, sstrDumpFile, spectraFilter );
	}
	else
	{
		sstrDumpFile = sstrDataDir;
		_cprintf( "Filename extraction from dumpfile with following parameters:\n");
		_cprintf( "dumpfile: %s\n", sstrDumpFile.c_str() );

		SpectraVFS vfs(sstrDumpFile, true);

		std::string sstrOutFileName(FileHelpers::getFileNameMinusExtension(sstrDumpFile));
		sstrOutFileName += ".txt";

		std::ofstream fon(sstrOutFileName.c_str());


		size_t numSpectra( vfs.getNumSpectra() );
		for (size_t i=0;i<numSpectra;i++)
		{
			Spectra *a = vfs.beginRead(i);
			fon << a->getFileName() + std::string("\n");			
			vfs.endRead(i);
		}
	}


}

