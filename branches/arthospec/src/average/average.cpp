#include <Windows.h>
#include <tchar.h>
#include "spectra.h"
#include <string>
#include <conio.h>
#include <assert.h>
#include <sstream>
#include "helpers.h"

#define DATADIR std::string("data/")


int _tmain(int argc, _TCHAR* argv[])
{
	std::vector<std::string> g_FileList;
	Spectra g_Spectrum;

	size_t numSpectra = Helpers::getFileList( std::string(DATADIR+"*.csv"), g_FileList );


	g_Spectrum.clear();

	if ( numSpectra > 0 )
	{
		_cprintf("loading %i spectra\n", numSpectra );

		Spectra a;
		for ( size_t i=0;i<numSpectra;i++ )
		{
			a.loadFromCSV(g_FileList.at(i));
			g_Spectrum.add( a );

			_cprintf(".");
		}
		g_Spectrum.divide(float(numSpectra));
		g_Spectrum.saveToCSV( std::string("average.csv") );
		_cprintf("\n" );
	}


	return 0;
}

