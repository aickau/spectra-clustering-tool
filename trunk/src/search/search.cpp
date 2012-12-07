//! \verbatim
//! ###########################################################################
//! # ASPECT: A spectra clustering tool for exploration of large spectral surveys - Stage I
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
//! \file  search.cpp
//! \brief perform search queries to find similar spectra for a list of given spectra

#include <Windows.h>

#include <map>
#include <sstream>
#include <iostream>
#include <fstream>
#include <conio.h>
#include <omp.h>

#include "tclap/CmdLine.h"

#include "sdsslib/defines.h"
#include "sdsslib/helpers.h"
#include "sdsslib/filehelpers.h"
#include "sdsslib/spectra.h"
#include "sdsslib/spectraVFS.h"
#include "sdsslib/memory.h"
#include "sdsslib/spectrahelpers.h"
#include "sdsslib/HTMLExport.h"
#include "sdsslib/sdssSoftwareVersion.h"

#define COMPAREDIR std::string("compare/")

std::vector<std::string> g_compareFileList;

#define DUMPFILE std::string("allSpectra.bin")

static
void writeTextTableEntry( const Spectra &_sp, float _error, std::string &_sstrOutString )
{
	_sstrOutString += Helpers::numberToString<double>(_sp.m_Z);
	_sstrOutString += " ";
	_sstrOutString += _sp.getFileName();
	_sstrOutString += " ";
	_sstrOutString += Helpers::numberToString<float>(_error);
	_sstrOutString += "\n";
}


void main(int argc, char* argv[])
{
	std::ofstream logFile("search_log.txt");
	Helpers::print("Welcome to SDSS Search "+sstrSDSSVersionString+" !\n\n\n", &logFile);
	Helpers::print("Order spectra relative to a set of reference spectra by their absolute error.\nCompare spectra should be located as FITS files in subdirectory /compare\n\n", &logFile);

	std::string sstrDumpFile(DUMPFILE);
	unsigned int spectraFilter = 0;//SPT_DEFAULTFILTER;
	unsigned int compareFunc = 2;
	float compareInvariance = 0.1f;
	bool bNormalize = false;
	bool bReverseOrder = false;
	unsigned int outputListLength = 100;


	try {  

		std::string sstrExamples("examples:\n");
		sstrExamples += std::string("search.exe -i allSpectra.bin -n\n");
		sstrExamples += std::string("search.exe -i allSpectra.bin -f 255 -c 2 -v 0.1\n");

		TCLAP::CmdLine cmd(sstrExamples, ' ', sstrSDSSVersionString);

		std::string sstrFilterDesc("Spectra filter is any added combination of:\n");
		sstrFilterDesc +=  std::string( "  SPEC_UNKNOWN =   1\n");
		sstrFilterDesc +=  std::string( "  SPEC_STAR    =   2\n");
		sstrFilterDesc +=  std::string( "  SPEC_GALAXY  =   4\n");
		sstrFilterDesc +=  std::string( "  SPEC_QSO     =   8\n");
		sstrFilterDesc +=  std::string( "  SPEC_HIZ_QSO =  16\n");
		sstrFilterDesc +=  std::string( "  SPEC_SKY     =  32\n");
		sstrFilterDesc +=  std::string( "  STAR_LATE    =  64\n");
		sstrFilterDesc +=  std::string( "  GAL_EM       = 128\n\n\n");

		std::string sstrCompareFuncDesc( "compare function is defined as follows:\n" );
		sstrCompareFuncDesc +=  std::string( "simple (euclidean squared over all samples) = 0\n" );
		sstrCompareFuncDesc +=  std::string( "advanced (takes neighborhood into account)  = 1\n" );
		sstrCompareFuncDesc +=  std::string( "super advanced (uses peak detection)        = 2\n\n" );


		TCLAP::ValueArg<std::string> dumpFilenameArg("i", "inputdumpfile", "example: allSpectra.bin", false, sstrDumpFile, "input dump file that contains all spectra to compare with.");
		TCLAP::ValueArg<unsigned int> filterArg("f", "filter", sstrFilterDesc, false, spectraFilter, "use only spectra for comparison with the given filter type");
		TCLAP::ValueArg<unsigned int> compareFuncArg("c", "comparefunc", sstrCompareFuncDesc, false, compareFunc, "compare function to use (0=simple, 1=advanced, 2=superadvanced)");
		TCLAP::ValueArg<float> compareInvarianceArg("v", "compareinvariance", "Percentage to take the neighboring samples of spectra for comparison into account. 0=no neighboring samples are taken into account, 1=all samples are taken into account. Only used if compare function is not set to simple", false, compareInvariance, "compare invariance (0..1)");
		TCLAP::SwitchArg normalizeArg("n","normalize","Normalize spectra", false);
		TCLAP::SwitchArg reverseArg("r","reverse","Reverse order", false);
		TCLAP::ValueArg<unsigned int> lengthArg("l", "length", "maximum number of spectra to output (default is 100)", false, outputListLength, "output list length" );

		cmd.add( dumpFilenameArg );
		cmd.add( filterArg );
		cmd.add( compareFuncArg );
		cmd.add( compareInvarianceArg );
		cmd.add( normalizeArg );
		cmd.add( reverseArg );
		cmd.add( lengthArg );

		cmd.parse( argc, argv );

		sstrDumpFile = dumpFilenameArg.getValue();
		spectraFilter = (filterArg.getValue() > 0) ? filterArg.getValue() : 255;
		compareFunc = CLAMP( compareFuncArg.getValue(), 0, 2 );
		compareInvariance = CLAMP(compareInvarianceArg.getValue(),0.f,1.f);
		bNormalize = normalizeArg.getValue();
		bReverseOrder = reverseArg.getValue();
		outputListLength = lengthArg.getValue();
	}
	catch (TCLAP::ArgException &e)  
	{ 
		Helpers::print( "error: "+e.error()+" for argument "+e.argId()+"\n", &logFile );
	}


	std::string sstrCompareFunc;
	switch (compareFunc){
		case 1:sstrCompareFunc="advanced";break;
		case 2:sstrCompareFunc="super advanced";break;
		default: sstrCompareFunc="simple";break;
	}

	Helpers::print( "dump file: "+sstrDumpFile+"\n", &logFile );
	Helpers::print( "filter: "+Spectra::spectraFilterToString(spectraFilter)+"\n", &logFile );
	Helpers::print( "compare function: "+sstrCompareFunc+"\n", &logFile );
	Helpers::print( "comparison invariance: "+Helpers::numberToString<float>(compareInvariance)+"\n", &logFile );
	Helpers::print( "normalize: "+Helpers::numberToString<bool>(bNormalize)+"\n", &logFile );
	Helpers::print( "reversed order: "+Helpers::numberToString<bool>(bReverseOrder)+"\n" , &logFile );
	Helpers::print( "output list length: "+Helpers::numberToString<unsigned int>(outputListLength)+"\n" , &logFile );
	Helpers::print( "each spectrum contains "+Helpers::numberToString<size_t>(sizeof(Spectra))+" bytes.\n\n", &logFile );

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
	sstrBaseInfo += "<br>\noutput list length: ";
	sstrBaseInfo += Helpers::numberToString<unsigned int>(outputListLength);
	sstrBaseInfo += "<br>\neach spectrum contains ";
	sstrBaseInfo += Helpers::numberToString<int>(sizeof(Spectra));
	sstrBaseInfo += " bytes.<br>\n\n";


	SpectraVFS vfs(sstrDumpFile);

	size_t numSpectra = vfs.getNumSpectra();

	if ( numSpectra == 0 )
	{
		Helpers::print("Error: No spectral data found.\n", &logFile);
		return;
	}
	else
	{
		Helpers::print(Helpers::numberToString<size_t>(numSpectra)+" spectra found.\n", &logFile);
	}

	const size_t numCompareSpectra = FileHelpers::getFileList( std::string(COMPAREDIR+"*.fit"), g_compareFileList );

	if ( numCompareSpectra == 0 )
	{
		Helpers::print("Error: No comparison spectral data found.\n", &logFile);
		return;
	}
	else
	{
		Helpers::print(Helpers::numberToString<size_t>(numCompareSpectra)+" comparison spectra found.\n", &logFile);
	}

	Spectra *compareSpectra = static_cast<Spectra*>( Memory::memAlignedAlloc( sizeof(Spectra) * numCompareSpectra ) ); //new Spectra[numCompareSpectra];
	std::map<float,size_t> *comparisonMap = new std::map<float,size_t>[numCompareSpectra];

	if ( compareSpectra == NULL )
	{
		Helpers::print("error: out of memory allocating "+Helpers::numberToString<size_t>(numCompareSpectra)+" comparison spectra.\n", &logFile);
	}

	for ( size_t i=0;i<numCompareSpectra;i++ )
	{

		bool bSuccess = compareSpectra[i].loadFromFITS_SDSS( g_compareFileList.at(i) );
		if ( !bSuccess )
		{
			Helpers::print("error: loading compare fits file "+g_compareFileList.at(i)+".\n", &logFile );
		}
		if ( bNormalize )
		{
			compareSpectra[i].normalizeByFlux();
		}
	}

	Spectra *src[SpectraVFS::CACHELINESIZE];
	float (*err)[SpectraVFS::CACHELINESIZE];
	err = new float[numCompareSpectra][SpectraVFS::CACHELINESIZE];

	int j=0;
	while (j<numSpectra)
	{
		const int jInc = MIN( SpectraVFS::CACHELINESIZE, (MIN(numSpectra, j+SpectraVFS::CACHELINESIZE)-j));

		// begin read for all src spectra
		for ( int k=0;k<jInc;k++)
		{
			src[k] = vfs.beginRead(j+k);
		}
 
#pragma omp parallel for
		for ( int k=0;k<jInc;k++)
		{
			if ((src[k]->m_Type & spectraFilter) > 0)
			{
				if ( bNormalize )
				{
					src[k]->normalizeByFlux();
				}
				for ( size_t l=0;l<numCompareSpectra;l++ )
				{
					switch (compareFunc)
					{
					case 1: err[l][k] = compareSpectra[l].compareAdvanced( *src[k], compareInvariance ); break;
					case 2: err[l][k] = compareSpectra[l].compareSuperAdvanced( *src[k], compareInvariance ); break;
					default: err[l][k] = compareSpectra[l].compare( *src[k] ); break;
					}
				}
			}
			else
			{
				// spectra not used, flag error as not used
				for ( size_t l=0;l<numCompareSpectra;l++ )
				{
					err[l][k] = -1.f;
				}
			}
		}


		// end read for all src spectra
		for ( int k=0;k<jInc;k++)
		{
			for ( size_t l=0;l<numCompareSpectra;l++ )
			{
				// only insert if used
				if ( err[l][k] >= 0.0f )
				{
					comparisonMap[l].insert( std::pair<float, size_t>(err[l][k], j+k) );
				}
			}
			vfs.endRead(j+k);
		}



		j+= jInc;
	}

	delete[] err;

	// for each compare spectrum
	for ( size_t j=0;j<numCompareSpectra;j++ )
	{
		// html doc
		std::string sstrHTMLDoc = HTMLExport::loadHTMLTemplate();
		std::string sstrCompareFilename(g_compareFileList.at(j));

		// additional text output for easier parsing and processing of results
		std::string sstrOutTextTable;


		if (!Helpers::insertString( HTMLExport::HTML_TOKEN_TITLE, sstrCompareFilename, sstrHTMLDoc ) )
		{
			Helpers::print("export failed. Wrong template.html ?!?\n", &logFile);
			return;
		}


		std::string sstrTable;

		size_t c=0;

		if ( !bReverseOrder )
		{
			std::map<float,size_t>::iterator it( comparisonMap[j].begin() ); 
			while ( it != comparisonMap[j].end() && c < outputListLength)
			{
				Spectra *sp = vfs.beginRead( it->second );
				SpectraHelpers::writeTableEntry( *sp, it->first, sstrTable );
				writeTextTableEntry( *sp, it->first, sstrOutTextTable );
				vfs.endRead( it->second );
				it++;
				c++;
			}
		}
		else
		{	
			std::map<float,size_t>::reverse_iterator it( comparisonMap[j].rbegin() ); 
			while ( it != comparisonMap[j].rend() && c < outputListLength)
			{
				Spectra *sp = vfs.beginRead( it->second );
				SpectraHelpers::writeTableEntry( *sp, it->first, sstrTable );
				writeTextTableEntry( *sp, it->first, sstrOutTextTable );
				vfs.endRead( it->second );
				it++;
				c++;
			}
		}
	
		if (!Helpers::insertString( HTMLExport::HTML_TOKEN_TEMPLATE, sstrTable, sstrHTMLDoc ) )
		{
			Helpers::print("export failed. Wrong template.html ?!?\n", &logFile);
			return;
		}

		std::string sstrInfo(sstrBaseInfo);
		sstrInfo += "Compared spectrum: ";
		sstrInfo += sstrCompareFilename;
		sstrInfo += "<br>\nNumber of compared spectra: ";
		sstrInfo += Helpers::numberToString<size_t>(numSpectra);

		Helpers::insertString( HTMLExport::HTML_TOKEN_INFO, sstrInfo, sstrHTMLDoc );


		std::string sstrResultFilename("searchresults_");
		sstrResultFilename += FileHelpers::getFileName(sstrCompareFilename);
		std::string sstrResultFilenameTXT(sstrResultFilename+".txt");
		std::string sstrResultFilenameHTML(sstrResultFilename+".html");

		sstrResultFilename += ".html";

		Helpers::print( "\n\nExporting results to "+sstrResultFilename+"\n", &logFile );

		std::ofstream fonHTML(sstrResultFilenameHTML.c_str());
		fonHTML<<sstrHTMLDoc;

		std::ofstream fonTXT(sstrResultFilenameTXT.c_str());
		fonTXT<<sstrOutTextTable;

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

