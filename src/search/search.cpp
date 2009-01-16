#include <Windows.h>

#include <map>
#include <sstream>
#include <iostream>
#include <fstream>
#include <conio.h>

#include "sdsslib/helpers.h"
#include "sdsslib/filehelpers.h"
#include "sdsslib/spectra.h"
#include "sdsslib/spectraVFS.h"
#include "sdsslib/memory.h"
#include "sdsslib/spectrahelpers.h"

#define COMPAREDIR std::string("compare/")

std::vector<std::string> g_compareFileList;



#define DUMPFILE std::string("allSpectra.bin")


void main(int argc, char* argv[])
{
	_cprintf("Welcome to SDSS Search!\n\n\n");

	_cprintf("order spectra relative to a set of reference spectra by their absolute error.\n\n");

	if ( argc < 2 )
	{
		_cprintf( "No parameters specified. Using default settings.\n\n");
		_cprintf( "usage: search.exe <dumpfile.bin> [spectra filter] [compare function] [comparison invariance] [normalize] [reverse order]\n");
		_cprintf( "spectra filter is defined as follows:\n" );
		_cprintf( "SPEC_UNKNOWN = 1\n" );
		_cprintf( "SPEC_STAR    = 2\n" );
		_cprintf( "SPEC_GALAXY  = 4\n" );
		_cprintf( "SPEC_QSO     = 8\n" );
		_cprintf( "SPEC_HIZ_QSO = 16\n" );
		_cprintf( "SPEC_SKY     = 32\n" );
		_cprintf( "STAR_LATE    = 64\n" );   
		_cprintf( "GAL_EM       = 128\n" );     
		_cprintf( "filters can be combined by adding the values, to disable filtering use 255.\n" );                 
		_cprintf( "if no filter is specified it uses SPEC_UNKNOWN|SPEC_QSO|SPEC_HIZ_QSO|GAL_EM as default filter\n\n" );  
		_cprintf( "compare function is defined as follows:\n" );
		_cprintf( "simple (euclidean squared over all samples) = 0\n" );
		_cprintf( "advanced (takes neighborhood into account)  = 1\n" );
		_cprintf( "super advanced (uses peak detection)        = 2\n\n" );
		_cprintf( "example: search.exe allSpectra.bin 255 2 0.1 1 0\n\n\n");

	}

	std::string sstrDumpFile(DUMPFILE);
	unsigned int spectraFilter = SPT_DEFAULTFILTER;
	unsigned int compareFunc = 2;
	float compareInvariance = 0.1f;
	bool bNormalize = false;
	bool bReverseOrder = false;

	if (argc > 1)
	{
		sstrDumpFile = std::string(argv[1]);
	}

	if (argc > 2)
	{
		int filter = Helpers::stringToNumber<int>(std::string(argv[2]));
		if ( filter > 0) 
		{
			spectraFilter = filter;
		}
	}

	if (argc > 3)
	{
		int compare = Helpers::stringToNumber<int>(std::string(argv[3]));
		if ( compare >= 0 && compare <= 2) 
		{
			compareFunc = compare;
		}
	}


	if (argc > 3)
	{
		float val = Helpers::stringToNumber<float>(std::string(argv[4]));
		if (val>=0.f || val<=1.f)
		{
			compareInvariance = val;
		}
	}

	if (argc > 5)
	{
		bNormalize = (argv[5][0] == '1') ? true : false;
	}

	if (argc > 6)
	{
		bReverseOrder = (argv[6][0] == '1') ? true : false;
	}

	std::string sstrCompareFunc;
	switch (compareFunc){
		case 1:sstrCompareFunc="advanced";break;
		case 2:sstrCompareFunc="super advanced";break;
		default: sstrCompareFunc="simple";break;
	}

	_cprintf( "dump file: %s\n", sstrDumpFile.c_str() );
	_cprintf( "filter: %s\n", Spectra::spectraFilterToString(spectraFilter).c_str() );
	_cprintf( "compare function: %s\n", sstrCompareFunc.c_str() );
	_cprintf( "comparison invariance: %f\n", compareInvariance );
	_cprintf( "normalize: %i\n", bNormalize );
	_cprintf( "reversed order: %i\n", bReverseOrder );
	_cprintf( "each spectrum contains %i bytes.\n\n", sizeof(Spectra) );

	std::string sstrBaseInfo;
	sstrBaseInfo += "creation date: ";
	sstrBaseInfo += Helpers::getCurentDateTimeStampString();
	sstrBaseInfo += "<br>\ndumpfile: ";
	sstrBaseInfo += sstrDumpFile;
	sstrBaseInfo += "<br>\nfilter: ";
	sstrBaseInfo += Spectra::spectraFilterToString(spectraFilter);
	sstrBaseInfo += "<br>\ncompare function: ";
	sstrBaseInfo += sstrCompareFunc;
	sstrBaseInfo += "<br>\ncomaprison invariance: ";
	sstrBaseInfo += Helpers::numberToString<float>(compareInvariance);
	sstrBaseInfo += "<br>\nnormalize: ";
	sstrBaseInfo += Helpers::numberToString<int>(bNormalize);
	sstrBaseInfo += "<br>\nreversed order: ";
	sstrBaseInfo += Helpers::numberToString<int>(bReverseOrder);
	sstrBaseInfo += "<br>\neach spectrum contains ";
	sstrBaseInfo += Helpers::numberToString<int>(sizeof(Spectra));
	sstrBaseInfo += " bytes.<br>\n\n";


	SpectraVFS vfs(sstrDumpFile);

	size_t numSpectra = vfs.getNumSpectra();

	if ( numSpectra == 0 )
	{
		_cprintf("Error: No spectral data found.\n");
		return;
	}
	else
	{
		_cprintf("%i spectra found.\n",numSpectra);
	}

	size_t numCompareSpectra = FileHelpers::getFileList( std::string(COMPAREDIR+"*.fit"), g_compareFileList );

	if ( numCompareSpectra == 0 )
	{
		_cprintf("Error: No comparison spectral data found.\n");
		return;
	}
	else
	{
		_cprintf("%i comparison spectra found.\n",numCompareSpectra);
	}

	Spectra *compareSpectra = static_cast<Spectra*>( Memory::memAlignedAlloc( sizeof(Spectra) * numCompareSpectra ) ); //new Spectra[numCompareSpectra];
	std::map<float,size_t> *comparisonMap = new std::map<float,size_t>[numCompareSpectra];

	if ( compareSpectra == NULL )
	{
		_cprintf("error: out of memory allocating %i comparison spectra.\n", numCompareSpectra);
	}

	for ( size_t j=0;j<numCompareSpectra;j++ )
	{

		bool bSuccess = compareSpectra[j].loadFromFITS( g_compareFileList.at(j) );
		if ( !bSuccess )
		{
			_cprintf("error: loading compare fits file %s .\n", g_compareFileList.at(j).c_str() );
		}
		if ( bNormalize )
		{
			compareSpectra[j].normalize();
		}
	}

	for ( size_t i=0;i<numSpectra;i++ )
	{
		Spectra *a = vfs.beginRead(i);
		if ((a->m_Type & spectraFilter) > 0)
		{
			if ( bNormalize )
			{
				a->normalize();
			}
			for ( size_t j=0;j<numCompareSpectra;j++ )
			{
				float err;
				switch (compareFunc)
				{
					case 1: err = compareSpectra[j].compareAdvanced( *a, compareInvariance ); break;
					case 2: err = compareSpectra[j].compareSuperAdvanced( *a, compareInvariance ); break;
					default: err = compareSpectra[j].compare( *a ); break;
				}
				comparisonMap[j].insert( std::pair<float, size_t>(err, i) );
			}
		}

		vfs.endRead(i);

		if ( i%SpectraVFS::CACHELINESIZE == 0 )
		{
			vfs.prefetch( i + SpectraVFS::CACHELINESIZE );
		}
	}


	for ( size_t j=0;j<numCompareSpectra;j++ )
	{
		std::string sstrHTMLDoc = SpectraHelpers::loadHTMLTemplate();
		std::string sstrCompareFilename(g_compareFileList.at(j));

		if (!Helpers::insertString( SpectraHelpers::HTML_TOKEN_TITLE, sstrCompareFilename, sstrHTMLDoc ) )
		{
			printf("export failed. Wrong template.html ?!?\n");
			return;
		}


		std::string sstrTable;

		size_t c=0;

		if ( !bReverseOrder )
		{
			std::map<float,size_t>::iterator it( comparisonMap[j].begin() ); 
			while ( it != comparisonMap[j].end() && c < 100)
			{
				Spectra *sp = vfs.beginRead( it->second );
				SpectraHelpers::writeTableEntry( *sp, it->first, sstrTable );
				vfs.endRead( it->second );
				it++;
				c++;
			}
		}
		else
		{	
			std::map<float,size_t>::reverse_iterator it( comparisonMap[j].rbegin() ); 
			while ( it != comparisonMap[j].rend() && c < 100)
			{
				Spectra *sp = vfs.beginRead( it->second );
				SpectraHelpers::writeTableEntry( *sp, it->first, sstrTable );
				vfs.endRead( it->second );
				it++;
				c++;
			}
		}
	
		if (!Helpers::insertString( SpectraHelpers::HTML_TOKEN_TEMPLATE, sstrTable, sstrHTMLDoc ) )
		{
			printf("export failed. Wrong template.html ?!?\n");
			return;
		}

		std::string sstrInfo(sstrBaseInfo);
		sstrInfo += "Compared spectrum: ";
		sstrInfo += sstrCompareFilename;
		sstrInfo += "<br>\nNumber of compared spectra: ";
		sstrInfo += Helpers::numberToString<size_t>(numSpectra);

		Helpers::insertString( SpectraHelpers::HTML_TOKEN_INFO, sstrInfo, sstrHTMLDoc );


		std::string sstrResultFilename("searchresults_");
		sstrResultFilename += FileHelpers::getFileName(sstrCompareFilename);
		sstrResultFilename += ".html";

		printf( "\n\nExporting results to %s \n", sstrResultFilename.c_str() );

		std::ofstream fon(sstrResultFilename.c_str());
		fon<<sstrHTMLDoc;
	}
	Memory::memAlignedFree( compareSpectra );


/*
	std::multimap<float,__int64> comparisonMap;


	for ( size_t i=0;i<numSpectra;i++ )
	{
		Spectra *a = vfs.beginRead(i);
		if ( a->m_Type & spectraFilter > 0 )
		{
			comparisonMap.insert( std::pair<float, __int64>(a->m_Z, a->m_SpecObjID) );
		}

		vfs.endRead(i);
	}

	std::string sstrOut;
	std::multimap<float,__int64>::iterator it( comparisonMap.begin() ); 
	while ( it != comparisonMap.end() )
	{
		sstrOut += Helpers::numberToString<__int64>((*it).second) + "  " + Helpers::numberToString<float>((*it).first) + "\n";
		it++;
	}

	std::ofstream fon("zorder.txt");
	fon<<sstrOut;
*/	
}

