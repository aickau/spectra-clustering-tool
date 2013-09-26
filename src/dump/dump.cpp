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
//! \file  dump.cpp
//! \brief program to generate binary dumps from a directory of FITS files.


#include <Windows.h>
#include <conio.h>

#include "tclap/CmdLine.h"

#include "sdsslib/spectraVFS.h"
#include "sdsslib/spectra.h"
#include "sdsslib/helpers.h"
#include "sdsslib/fileHelpers.h"
#include "sdsslib/SpectraHelpers.h"
#include "sdsslib/sdssSoftwareVersion.h"
#include "sdsslib/spectraDB.h"


typedef char _TCHAR;

#define DATADIR std::string("./data/*")
//#define DATADIR std::string("G:/SDSS_ANALYZE/fits/spectro/data/*")
#define DUMPFILE std::string("allSpectra.bin")
 


void main(int argc, char* argv[])
{
 
	// outputs sine test spectra.
//  	SpectraVFS::write( 10000, 0.0f, "allSpectra.bin" );
//  	return;
	std::ofstream logFile("dump_log.txt");

/*
	double mioSpectraComparePerSecond;
	double mioSpectraAdaptionPerSecond;
	SpectraHelpers::testSpectraPerformance( mioSpectraComparePerSecond, mioSpectraAdaptionPerSecond );

	Helpers::print( Helpers::numberToString<double>(mioSpectraComparePerSecond) +std::string(" million spectra compares per second.\n"), &logFile );
	Helpers::print( Helpers::numberToString<double>(mioSpectraAdaptionPerSecond) +std::string(" million spectra adaption per second.\n"), &logFile );

	return;
*/


	Helpers::print("Welcome to SDSS Dump "+sstrSDSSVersionString+" !\n\n\n", &logFile);
	Helpers::print("Reads SDSS fits files from a given directory (and subdirectories) and dumps the data to a binary file.\n", &logFile);
	Helpers::print("Reads binary dump files and extracts text tables out of it.\n\n", &logFile);

	std::string sstrDataDir = FileHelpers::getCurrentDirectory()+DATADIR;
	std::string sstrDumpFile = DUMPFILE;
	unsigned int spectraFilter = SPT_DEFAULTFILTER;
	std::string sstrInputDumpFile("");
	std::string sstrSelectionListFilename("");



	try {  

		std::string sstrExamples("examples:\n");
		sstrExamples += std::string("dump.exe -d F:/SDSS_ANALYZE/fits/spectro/data/* -o allSpectra.bin -f 25 -s selectionlist.txt\n");
		sstrExamples += std::string("dump.exe -i sofmnet.bin\n");


		TCLAP::CmdLine cmd(sstrExamples, ' ', sstrSDSSVersionString);

		std::string sstrFilterDesc = std::string("where <filter> is any added combination of:\n");
		sstrFilterDesc +=  std::string( "  SPEC_UNKNOWN =   1\n");
		sstrFilterDesc +=  std::string( "  SPEC_STAR    =   2\n");
		sstrFilterDesc +=  std::string( "  SPEC_GALAXY  =   4\n");
		sstrFilterDesc +=  std::string( "  SPEC_QSO     =   8\n");
		sstrFilterDesc +=  std::string( "  SPEC_HIZ_QSO =  16\n");
		sstrFilterDesc +=  std::string( "  SPEC_SKY     =  32\n");
		sstrFilterDesc +=  std::string( "  STAR_LATE    =  64\n\n");
	
		sstrFilterDesc +=  std::string( "for BOSS spectra (SDSS DR9 and above) the additional following types can be used:\n");
		sstrFilterDesc +=  std::string( "  QualityAssuarance    =   0x000000100\n");
		sstrFilterDesc +=  std::string( "  STAR_PN              =   0x000000200\n");
		sstrFilterDesc +=  std::string( "  STAR_CARBON          =   0x000000400\n");
		sstrFilterDesc +=  std::string( "  STAR_BROWN_DWARF     =   0x000000800\n");
		sstrFilterDesc +=  std::string( "  STAR_SUB_DWARF       =   0x000001000\n");
		sstrFilterDesc +=  std::string( "  STAR_CATY_VAR        =   0x000002000\n");
		sstrFilterDesc +=  std::string( "  STAR_RED_DWARF       =   0x000004000\n");
		sstrFilterDesc +=  std::string( "  STAR_WHITE_DWARF     =   0x000008000\n");
		sstrFilterDesc +=  std::string( "  STAR_BHB             =   0x000010000\n");
		sstrFilterDesc +=  std::string( "  ROSAT_A              =   0x000020000\n");
		sstrFilterDesc +=  std::string( "  ROSAT_B              =   0x000040000\n");
		sstrFilterDesc +=  std::string( "  ROSAT_C              =   0x000080000\n");
		sstrFilterDesc +=  std::string( "  ROSAT_D              =   0x000100000\n");
		sstrFilterDesc +=  std::string( "  SPECTROPHOTO_STD     =   0x000200000\n");
		sstrFilterDesc +=  std::string( "  HOT_STD              =   0x000400000\n");
		sstrFilterDesc +=  std::string( "  SERENDIPITY_BLUE     =   0x000800000\n");
		sstrFilterDesc +=  std::string( "  SERENDIPITY_FIRST    =   0x001000000\n");
		sstrFilterDesc +=  std::string( "  SERENDIPITY_RED      =   0x002000000\n");
		sstrFilterDesc +=  std::string( "  SERENDIPITY_DISTANT  =   0x004000000\n");
		sstrFilterDesc +=  std::string( "  SERENDIPITY_MANUAL   =   0x008000000\n");
		sstrFilterDesc +=  std::string( "  REDDEN_STD           =   0x010000000\n\n");
		sstrFilterDesc +=  std::string( "for DR10 spectra the additional following types can be used:\n");
		sstrFilterDesc +=  std::string( "  BLAZAR				=   0x020000000\n");
		sstrFilterDesc +=  std::string( "  SPT_QSO_BAL          =   0x040000000\n");
		sstrFilterDesc +=  std::string( "  SPT_EXOTIC           =   0x080000000\n");


		TCLAP::ValueArg<std::string> dataDirArg("d", "datadir", "example: F:/SDSS_ANALYZE/fits/spectro/data/*", false, sstrDataDir, "datadir/*");
		TCLAP::ValueArg<std::string> outputFilenameArg("o", "outputdumpfile", "example: allSpectra.bin", false, sstrDumpFile, "outputfilename.bin");
		TCLAP::ValueArg<unsigned int> filterArg("f", "filter", sstrFilterDesc, false, spectraFilter, "Dump only FITS files with the given filter type.");
		TCLAP::ValueArg<std::string> inputFilenameArg("i", "inputdumpfile", "example: sofmnet.bin. If inputdumpfile is specified, then all other arguments are ignored. Outputs a linear list of the network.", false, sstrInputDumpFile, "Dumpfile for reverse reads.");
		TCLAP::ValueArg<std::string> selectionListFilenameArg("s", "selection", "Optional selection list of FITS files to dump a small subset of input spectra. File should contain plate-mjd-fiber pairs, e.g. 3586 55181 0001. First line in the file is the header and is ignored.", false, sstrSelectionListFilename, "selectionlist.txt");

		cmd.add( dataDirArg );
		cmd.add( outputFilenameArg );
		cmd.add( filterArg );
		cmd.add( inputFilenameArg );
		cmd.add( selectionListFilenameArg );
 
		cmd.parse( argc, argv );

		sstrDataDir = dataDirArg.getValue();
		sstrDumpFile = outputFilenameArg.getValue();
		spectraFilter = filterArg.getValue();
		sstrInputDumpFile = inputFilenameArg.getValue();
		sstrSelectionListFilename = selectionListFilenameArg.getValue();
	}
	catch (TCLAP::ArgException &e)  
	{ 
		Helpers::print( "error: "+e.error()+" for argument "+e.argId()+"\n", &logFile );
	}

	bool bReverseRead = !sstrInputDumpFile.empty();

	if ( !bReverseRead )
	{
		Helpers::print( "Creating binary dump with following parameters:\n", &logFile);
		Helpers::print( "datadir: "+sstrDataDir+"\n", &logFile );
		Helpers::print( "dumpfile: "+sstrDumpFile+"\n", &logFile );
		Helpers::print( "filter: "+Spectra::spectraFilterToString(spectraFilter)+"\n", &logFile );
		Helpers::print( "selectionlist: "+sstrSelectionListFilename+"\n", &logFile );
		Helpers::print( "each spectrum contains " + Helpers::numberToString<size_t>(sizeof(Spectra)) + " bytes.\n\n", &logFile );

		SpectraDB spectraDBDR9;
		const bool DR9DBAvailable = spectraDBDR9.loadDB(SpectraDB::DR9);
		if (!DR9DBAvailable)
		{
			Helpers::print( "Warning, spectraParamDR9.bin missing. DR8, DR9 spectra cannot be loaded.\n" , &logFile );
		}
		SpectraDB spectraDBDR10;
		const bool DR10DBAvailable = spectraDBDR10.loadDB(SpectraDB::DR10);
		if (!DR10DBAvailable)
		{
			Helpers::print( "Warning, spectraParamDR10.bin missing. DR10 spectra cannot be loaded.\n" , &logFile );
		}

		std::set<std::string> FITSFilenameSet;
		if ( !sstrSelectionListFilename.empty() )
		{
			bool bSuccess = SpectraHelpers::readSelectionList(sstrSelectionListFilename, FITSFilenameSet);

			if ( !bSuccess || FITSFilenameSet.empty() )
			{
				Helpers::print( "Selection list not found or empty.\n", &logFile );
			}
		}

		Helpers::print( "starting dump...\n", &logFile );
		size_t writtenSpectra = SpectraVFS::write( sstrDataDir, sstrDumpFile, spectraFilter, &logFile, &FITSFilenameSet );
		Helpers::print( "...finished writing "+ Helpers::numberToString<size_t>(writtenSpectra) +" spectra.\n", &logFile );
	}
	else
	{
		Helpers::print( "Filename extraction from dumpfile with following parameters:\n", &logFile);
		Helpers::print( "dumpfile: "+sstrInputDumpFile+"\n", &logFile );

		SpectraVFS vfs(sstrInputDumpFile, true);

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
