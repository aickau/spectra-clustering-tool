#include <Windows.h>

#include <map>
#include <sstream>
#include <iostream>
#include <fstream>
#include <conio.h>

#include "sdsslib/helpers.h"
#include "sdsslib/spectra.h"
#include "sdsslib/spectraVFS.h"
#include "sdsslib/memory.h"

#define COMPAREDIR std::string("compare/")

std::vector<std::string> g_compareFileList;


bool insertTag( std::string &sstrDoc, const std::string &sstrTag, const std::string &sstrInsertText )
{
	std::string sstrTagTitle("*TITLE*");
	size_t insertpos = sstrDoc.find(sstrTag);
	if (insertpos == std::string::npos )
	{
		return false;
	}

	sstrDoc = sstrDoc.erase( insertpos, sstrTag.size() );
	sstrDoc.insert( insertpos, sstrInsertText );

	return true;
}


void writeTableEntry( const Spectra &spectrum, float error, std::string &sstrOutTable )
{
	const Spectra *sp = &spectrum;
	sstrOutTable += "<tr>\n";
	sstrOutTable += "<td>";
	// insert link
	if ( !sp->getFileName().empty() )
	{
		sstrOutTable += "<a href=\"";
		sstrOutTable += sp->getURL();
		sstrOutTable += "\" target=\"_blank\">";

		sstrOutTable += "<img src=\"";
		sstrOutTable += "http://cas.sdss.org/dr6/en/get/specById.asp?id=";
		sstrOutTable += Helpers::numberToString<__int64>(sp->m_SpecObjID);
		sstrOutTable += "\"><br>err=";
		sstrOutTable += Helpers::numberToString<float>(error);
		sstrOutTable += "  z=";
		sstrOutTable += Helpers::numberToString<float>(sp->m_Z);
		sstrOutTable += "  ";
		sstrOutTable += sp->getFileName();
		sstrOutTable += "</td>";
	}
	else
	{
		// insert image
		sstrOutTable += "<img src=\"export/empty.png\"></td>";
	}

	sstrOutTable += "</td>\n";
	sstrOutTable += "</tr>\n";
}


#define DUMPFILE std::string("allSpectra.bin")


void main(int argc, char* argv[])
{
	_cprintf("Welcome to SDSS Search!\n\n\n");

	_cprintf("order spectra relative to a set of reference spectra by their absolute error.\n\n");

	if ( argc < 2 )
	{
		_cprintf( "No parameters specified. Using default settings.\n\n");
		_cprintf( "usage: search.exe <dumpfile.bin> [normalize] [reverse order]\n");
		_cprintf( "example: search.exe allSpectra.bin 1 0\n\n\n");
	}

	_cprintf( "each spectrum contains %i bytes.\n\n", sizeof(Spectra) );


	bool bNormalize = false;
	bool bReverseOrder = false;
	std::string sstrDumpFile(DUMPFILE);

	unsigned int spectraFilter = SPT_DEFAULTFILTER;

	if (argc > 1)
	{
		sstrDumpFile = std::string(argv[1]);
	}

	if (argc > 2)
	{
		bNormalize = (argv[2][0] == '1') ? true : false;
	}

	if (argc > 3)
	{
		bReverseOrder = (argv[3][0] == '1') ? true : false;
	}

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

	size_t numCompareSpectra = Helpers::getFileList( std::string(COMPAREDIR+"*.fit"), g_compareFileList );

	if ( numCompareSpectra == 0 )
	{
		_cprintf("Error: No comparison spectral data found.\n");
		return;
	}
	else
	{
		_cprintf("%i comparison spectra found.\n",numCompareSpectra);
	}

	Spectra *compareSpectra = static_cast<Spectra*>( Memory::MemAlignedAlloc( sizeof(Spectra) * numCompareSpectra ) ); //new Spectra[numCompareSpectra];
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
				float err = compareSpectra[j].compare( *a );
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
		// load template
		std::ifstream fin("template.html");
		std::string sstrTemp;
		std::string sstrHTMLDoc;
		std::string sstrCompareFilename(g_compareFileList.at(j));

		if( !fin ) 
		{
			printf("export failed. Missing template.html\n");
			return;
		}

		while( getline(fin,sstrTemp) ) 
		{
			sstrHTMLDoc += sstrTemp;
		}


		if (!insertTag( sstrHTMLDoc, std::string("*TITLE*"), sstrCompareFilename ) )
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
				writeTableEntry( *sp, it->first, sstrTable );
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
				writeTableEntry( *sp, it->first, sstrTable );
				vfs.endRead( it->second );
				it++;
				c++;
			}
		}
	
		if (!insertTag( sstrHTMLDoc, std::string("*TEMPLATE*"), sstrTable ) )
		{
			printf("export failed. Wrong template.html ?!?\n");
			return;
		}


		std::string sstrResultFilename("searchresults_");
		sstrResultFilename += Helpers::getFileName(sstrCompareFilename);
		sstrResultFilename += ".html";

		printf( "\n\nExporting results to %s \n", sstrResultFilename.c_str() );

		std::ofstream fon(sstrResultFilename.c_str());
		fon<<sstrHTMLDoc;
	}
	Memory::MemAlignedFree( compareSpectra );


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

