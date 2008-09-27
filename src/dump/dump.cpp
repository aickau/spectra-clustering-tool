#include <Windows.h>
#include <conio.h>
#include "sdsslib/spectraVFS.h"
#include "sdsslib/spectra.h"

typedef char _TCHAR;

#define DATADIR std::string("c:/prj/sdss_svn/data/*")
//#define DATADIR std::string("F:/SDSS_ANALYZE/fits/spectro/data/*")
#define DUMPFILE std::string("allSpectra.bin")


void main(int argc, char* argv[])
{
	_cprintf("Welcome to SDSS Dump!\n\n\n");
	_cprintf("reads sdss fits files from a given directory (and subdirectories) and dumps the data to a binary file.\n\n");

	if ( argc < 2 )
	{
		_cprintf( "No parameters specified. Using default settings.\n\n");
		_cprintf( "usage: dump.exe <datadir/*> [<outputfilename.bin>]\n");
		_cprintf( "example: dump.exe F:/SDSS_ANALYZE/fits/spectro/data/ allSpectra.bin\n\n\n");
	}

	std::string sstrDataDir = DATADIR;
	std::string sstrDumpFile = DUMPFILE;

	if ( argc > 1 )
	{
		sstrDataDir = std::string(argv[1]);
	}
	if ( argc > 2 )
	{
		sstrDumpFile = std::string(argv[2]);
	}


	_cprintf( "datadir: %s\n", sstrDataDir.c_str() );
	_cprintf( "dumpfile: %s\n", sstrDumpFile.c_str() );
	_cprintf( "each spectrum contains %i bytes.\n\n", sizeof(Spectra) );


	SpectraVFS::write( sstrDataDir, sstrDumpFile );
}

