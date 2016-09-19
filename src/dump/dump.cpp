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


#include <cstdio>
#include "tclap/CmdLine.h"

#include "sdsslib/spectraVFS.h"
#include "sdsslib/spectra.h"
#include "sdsslib/helpers.h"
#include "sdsslib/fileHelpers.h"
#include "sdsslib/SpectraHelpers.h"
#include "sdsslib/sdssSoftwareVersion.h"
#include "sdsslib/spectraDB.h"
#include "sdsslib/afaConnector.h"



#define DATADIR std::string("./data/dr12/*")
//#define DATADIR std::string("G:/SDSS_ANALYZE/fits/spectro/data/*")
#define DUMPFILE std::string("allSpectra.bin")
 

int main(int argc, char* argv[])
{
	std::ofstream logFile("dump_log.txt");

/*
	// output test spectra.
	double mioSpectraComparePerSecond;
	double mioSpectraAdaptionPerSecond;
	SpectraBaseHelpers::testSpectraPerformance( mioSpectraComparePerSecond, mioSpectraAdaptionPerSecond );

	Helpers::print( Helpers::numberToString<double>(mioSpectraComparePerSecond) +std::string(" million spectra compares per second.\n"), &logFile );
	Helpers::print( Helpers::numberToString<double>(mioSpectraAdaptionPerSecond) +std::string(" million spectra adaption per second.\n"), &logFile );

	return;
*/


	Helpers::print("Welcome to SDSS Dump "+sstrSDSSVersionString+" !\n\n\n", &logFile);
	Helpers::print("Dump can do the following:\n", &logFile);
	Helpers::print("(1) Read SDSS spectra as FITS files from a given directory (and subdirectories) and dumps the data to a single binary file. Use -d\n", &logFile);
	Helpers::print("    DR12 spectra can be downloaded here: http://data.sdss3.org/sas/dr12/sdss/spectro/redux/26/spectra \n\n", &logFile);
	Helpers::print("(2) Generation of n sine test spectra with increasing frequency. Use -t\n\n", &logFile);
	Helpers::print("(3) Reads binary dump files and extracts text tables out of it. Use -i\n\n", &logFile);
	Helpers::print("(4) Uploads spectra from binary dump files to ASPECT-FPGA-Accelerator (AFA). Use -a\n\n", &logFile);

	std::string sstrDataDir = FileHelpers::getCurrentDirectory()+DATADIR;
	std::string sstrDumpFile = DUMPFILE;
	int sineTestSpectra = 0;
	unsigned int spectraFilter = SPT_DEFAULTFILTER;
	std::string sstrInputDumpFile("");
	std::string sstrAfaDumpFile("");
	std::string sstrSelectionListFilename("");
	


	try {  

		std::string sstrExamples("examples:\n");
		sstrExamples += std::string("Write FITS files to binary dump file: \n    dump.exe -d F:/SDSS_ANALYZE/fits/spectro/data/* -o allSpectra.bin -f 25 -s selectionlist.txt\n");
		sstrExamples += std::string("Write 1000 sine test spectra: \n    dump.exe -t 1000 -o allSpectra.bin\n");
		sstrExamples += std::string("Outputs a linear list of the network: \n    dump.exe -i sofmnet.bin\n");
		sstrExamples += std::string("Uploads spectra dump to ASPECT-FPGA-Accelerator (AFA) \n    dump.exe -a allSpectra.bin\n");


		TCLAP::CmdLine cmd(sstrExamples, ' ', sstrSDSSVersionString);

		std::string sstrFilterDesc = std::string("where <filter> is any added combination of:\n");
		sstrFilterDesc +=  std::string( "  SPEC_UNKNOWN =   2\n");
		sstrFilterDesc +=  std::string( "  SPEC_STAR    =   4\n");
		sstrFilterDesc +=  std::string( "  SPEC_GALAXY  =   8\n");
		sstrFilterDesc +=  std::string( "  SPEC_QSO     =   16\n");
	

		TCLAP::ValueArg<std::string> dataDirArg("d", "datadir", "example: F:/SDSS_ANALYZE/fits/spectro/data/*", false, sstrDataDir, "datadir/*");
		TCLAP::ValueArg<unsigned int> sineTestArg("t", "sinetest", "generate n sine test spectra", false, 0, "Number of sine test spectra.");
		TCLAP::ValueArg<std::string> outputFilenameArg("o", "outputdumpfile", "example: allSpectra.bin", false, sstrDumpFile, "outputfilename.bin");
		TCLAP::ValueArg<unsigned int> filterArg("f", "filter", sstrFilterDesc, false, spectraFilter, "Dump only FITS files with the given filter type.");
		TCLAP::ValueArg<std::string> inputFilenameArg("i", "inputdumpfile", "example: sofmnet.bin. If input dump file is specified, then all other arguments are ignored. Outputs a linear list of the network.", false, sstrInputDumpFile, "Dumpfile for reverse reads.");
		TCLAP::ValueArg<std::string> selectionListFilenameArg("s", "selection", "Optional selection list of FITS files to dump a small subset of input spectra. File should contain plate-mjd-fiber pairs, e.g. 3586 55181 0001. First line in the file is the header and is ignored.", false, sstrSelectionListFilename, "selectionlist.txt");
		TCLAP::ValueArg<std::string> afaFilenameArg("a", "afaupload", "example: allSpectra.bin. If afa upload is specified, then upload spectra dump to ASPECT-FPGA-Accelerator (AFA). All other arguments are ignored.", false, sstrAfaDumpFile, "Dumpfile for AFA upload.");

		cmd.add( dataDirArg );
		cmd.add( outputFilenameArg );
		cmd.add( sineTestArg );
		cmd.add( filterArg );
		cmd.add( inputFilenameArg );
		cmd.add( selectionListFilenameArg );
		cmd.add( afaFilenameArg );

		cmd.parse( argc, argv );

		sstrDataDir					= dataDirArg.getValue();
		sstrDumpFile				= outputFilenameArg.getValue();
		sineTestSpectra				= sineTestArg.getValue();
		spectraFilter				= filterArg.getValue();
		sstrInputDumpFile			= inputFilenameArg.getValue();
		sstrAfaDumpFile				= afaFilenameArg.getValue();
		sstrSelectionListFilename	= selectionListFilenameArg.getValue();
	}
	catch (TCLAP::ArgException &e)  
	{ 
		Helpers::print( "error: "+e.error()+" for argument "+e.argId()+"\n", &logFile );
	}

	const bool bExtractFilenames = !sstrInputDumpFile.empty();
	const bool bAfaUpload = !sstrAfaDumpFile.empty();

	if ( sineTestSpectra > 0 )
	{
		// generate sine test spectra.
		///////////////////////////////////////////////////////////////////////////////////
		Helpers::print( "Generating : "+Helpers::numberToString<int>(sineTestSpectra)+" sine test spectra.\n", &logFile );
		Helpers::print( "dumpfile: "+sstrInputDumpFile+"\n", &logFile );
		SpectraVFS::write( sineTestSpectra, 0.0f, sstrDumpFile );
		Helpers::print( "fin.\n", &logFile );
		return 0;
	}
	else if ( bExtractFilenames )
	{
		// extract FITS filenames from a given binary dump.
		///////////////////////////////////////////////////////////////////////////////////
		Helpers::print( "Filename extraction from dumpfile with following parameters:\n", &logFile);
		Helpers::print( "dumpfile: "+sstrInputDumpFile+"\n", &logFile );

		SpectraVFS vfs(sstrInputDumpFile, true);

		size_t numSpectra( vfs.getNumSpectra() );

		if ( numSpectra == 0)
		{
			return 1;
		}

		std::string sstrOutFileName(FileHelpers::getFileNameMinusExtension(sstrDumpFile));
		sstrOutFileName += ".txt";

		std::ofstream fon(sstrOutFileName.c_str());


		for (size_t i=0;i<numSpectra;i++)
		{
			Spectra *a = vfs.beginRead(i);
			fon << a->getFileName() + std::string("\n");			
			vfs.endRead(i);
		}
		Helpers::print( "fin.\n", &logFile );
	
		return 0;
	}
	else if ( bAfaUpload )
	{
		// upload spectra dump file to AFA 
		///////////////////////////////////////////////////////////////////////////////////

		Helpers::print( "Uploading spectra to ASPECT-FPGA-Accelerator (AFA) with following parameters:\n", &logFile);
		Helpers::print( "dumpfile: "+sstrAfaDumpFile+"\n", &logFile );

		SpectraVFS vfs(sstrAfaDumpFile, true);
		size_t numSpectra( vfs.getNumSpectra() );

		if ( numSpectra == 0)
		{
			return 1;
		}


		AfaConnector afaConnector;

		if ( !afaConnector.isAFADeviceAvailable() )
		{
			Helpers::print( "Could not find ASPECT-FPGA-Accelerator:\n", &logFile );
			Helpers::print( afaConnector.getErrorMsg()+"\n", &logFile );
			return 1;
		}


		if ( !afaConnector.writeSpectra(vfs) )
		{
			Helpers::print( "Error transferring data to ASPECT-FPGA-Accelerator:\n", &logFile );
			Helpers::print( afaConnector.getErrorMsg(), &logFile );
			return 1;
		}
		Helpers::print( "Finished transfer of "+ Helpers::numberToString<int>(numSpectra) +" spectra to ASPECT-FPGA-Accelerator.\n", &logFile );
		return 0;
	}
	else
	{
		// load spectra and pack all necessary information into a single binary file.
		///////////////////////////////////////////////////////////////////////////////////

		Helpers::print( "Creating binary dump with following parameters:\n", &logFile);
		Helpers::print( "datadir: "+sstrDataDir+"\n", &logFile );
		Helpers::print( "dumpfile: "+sstrDumpFile+"\n", &logFile );
		Helpers::print( "filter: "+Spectra::spectraFilterToString(spectraFilter)+"\n", &logFile );
		Helpers::print( "selectionlist: "+sstrSelectionListFilename+"\n", &logFile );
		Helpers::print( "each spectrum contains " + Helpers::numberToString<size_t>(sizeof(Spectra)) + " bytes.\n\n", &logFile );


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

	return 0;
}
