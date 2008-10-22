#include <Windows.h>
#include <conio.h>
#include "sdsslib/spectraVFS.h"
#include "sdsslib/spectra.h"
#include "sdsslib/helpers.h"

typedef char _TCHAR;

#define DATADIR std::string("c:/prj/sdss_svn2/trunk/data/*")
//#define DATADIR std::string("G:/SDSS_ANALYZE/fits/spectro/data/*")
#define DUMPFILE std::string("allSpectra.bin")
 


void main(int argc, char* argv[])
{
	_cprintf("Welcome to SDSS Dump!\n\n\n");
	_cprintf("reads sdss fits files from a given directory (and subdirectories) and dumps the data to a binary file.\n\n");

	if ( argc < 2 )
	{
		_cprintf( "No parameters specified. Using default settings.\n\n");
		_cprintf( "usage: dump.exe <datadir/*> [<outputfilename.bin>] [<filter>]\n");
		_cprintf( "where <filter> is any added combination of:\n");
		_cprintf( "  SPEC_UNKNOWN =   1\n");
		_cprintf( "  SPEC_STAR    =   2\n");
		_cprintf( "  SPEC_GALAXY  =   4\n");
		_cprintf( "  SPEC_QSO     =   8\n");
		_cprintf( "  SPEC_HIZ_QSO =  16\n");
		_cprintf( "  SPEC_SKY     =  32\n");
		_cprintf( "  STAR_LATE    =  64\n");
		_cprintf( "  GAL_EM       = 128\n\n\n");

		_cprintf( "example: dump.exe F:/SDSS_ANALYZE/fits/spectro/data/ allSpectra.bin 25\n\n\n");
	}

	std::string sstrDataDir = DATADIR;
	std::string sstrDumpFile = DUMPFILE;
	unsigned int spectraFilter = 0x0fffff;//SPT_DEFAULTFILTER;

	if ( argc > 1 )
	{
		sstrDataDir = std::string(argv[1]);
	}
	if ( argc > 2 )
	{
		sstrDumpFile = std::string(argv[2]);
	}

	if ( argc > 3 )
	{
		spectraFilter = Helpers::stringToNumber<unsigned int>( std::string(argv[3]) );
	}


	_cprintf( "datadir: %s\n", sstrDataDir.c_str() );
	_cprintf( "dumpfile: %s\n", sstrDumpFile.c_str() );
	_cprintf( "filter: %s\n", Spectra::spectraFilterToString(spectraFilter).c_str() );
	_cprintf( "each spectrum contains %i bytes.\n\n", sizeof(Spectra) );

	SpectraVFS::write( sstrDataDir, sstrDumpFile, spectraFilter );
}

