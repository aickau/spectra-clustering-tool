//! \verbatim
//! ###########################################################################
//! # SDSS sorting prototype - Stage I
//! #
//! # Copyright (c) 2009 Aick in der Au
//! # All rights reserved.
//! ###########################################################################
//!
//!      created by : Aick in der Au <aick.inderau@gmail.com>
//!      created on : 5/17/2009
//! additional docs : none
//!  responsibility : 1. Aick in der Au
//!                   2. 
//! \endverbatim
//!
//! \file  compare.cpp
//! \brief 



#include "tclap/CmdLine.h"

#include "sdsslib/defines.h"
#include "sdsslib/spectraVFS.h"
#include "sdsslib/spectra.h"
#include "sdsslib/helpers.h"
#include "sdsslib/filehelpers.h"
#include "sdsslib/sdssSoftwareVersion.h"

#include <Windows.h>
#include <conio.h>
#include <fstream>


void main(int argc, char* argv[])
{
	std::ofstream logFile("compare_log.txt");

	Helpers::print("Welcome to SDSS Compare!\n\n\n", &logFile);
	Helpers::print("Does a global comparison, one FITS file with a spectrum with many spectra from a given SOM.\n", &logFile);

	std::string sstrSourceDumpFile("allSpectra.bin");
	std::string sstrSOMDumpFile("sofmnet.bin");
	std::string sstrFITSFile("compare.fit");
	std::string sstrOutFile("FITSfilesNUMBASrgb.txt");


	try 
	{  
		std::string sstrExamples("examples:\n");
		sstrExamples += std::string("compare.exe -s allSpectra.bin -m sofmnet.bin -f data/0266/1d/spSpec-51630-0266-637.fit -o rgbmap.txt\n");


		TCLAP::CmdLine cmd(sstrExamples, ' ', SDSSVERSIONSTRING);

		TCLAP::ValueArg<std::string> SOMdumpFile("m", "somdumpfile", "example: sofmnet.bin. ", false, sstrSOMDumpFile, "SOM dump file that contains clustered map.");
		TCLAP::ValueArg<std::string> sourceDumpFile("s", "sourcedumpfile", "example: allSpectra.bin. ", false, sstrSourceDumpFile, "Source dump file that contains original spectral data.");
		TCLAP::ValueArg<std::string> FITSFile("f", "fitsfile", "example: spSpec-51630-0266-637.fit ", false, sstrFITSFile, "FITS file to compare.");
		TCLAP::ValueArg<std::string> OutFile("o", "outfile", "example: compareRGB.txt ", false, sstrOutFile, "Output text file that contains the error values.");

		cmd.add( SOMdumpFile );
		cmd.add( sourceDumpFile );
		cmd.add( FITSFile );
		cmd.add( OutFile );
	
		cmd.parse( argc, argv );

		sstrSOMDumpFile = SOMdumpFile.getValue();
		sstrSourceDumpFile = sourceDumpFile.getValue();
		sstrFITSFile = FITSFile.getValue();
		sstrOutFile = OutFile.getValue();
	}
	catch (TCLAP::ArgException &e)  
	{ 
		Helpers::print( "error: "+e.error()+" for argument "+e.argId()+"\n", &logFile );
	}

	Helpers::print( "Comparing with following parameters:\n", &logFile);
	Helpers::print( "dumpfile input: "+sstrSourceDumpFile+"\n", &logFile );
	Helpers::print( "dumpfile SOM: "+sstrSOMDumpFile+"\n", &logFile );
	Helpers::print( "FITS file: "+sstrFITSFile+"\n", &logFile );
	Helpers::print( "out file: "+sstrOutFile+"\n", &logFile );

	ISSE_ALIGN Spectra a;
	if ( !a.loadFromFITS(sstrFITSFile) )
	{
		Helpers::print("Error: Could not load FITS file.\n", &logFile);
		return;
	}


	SpectraVFS vfsSOM(sstrSOMDumpFile);
	SpectraVFS vfsSrc(sstrSourceDumpFile);

	const size_t numSpectraSOM = vfsSOM.getNumSpectra();
	const size_t numSpectraSRC = vfsSrc.getNumSpectra();

	if ( numSpectraSOM == 0 ) 
	{
		Helpers::print("Error: Could not load spectra from SOM dump file.\n", &logFile);
		return;	
	}

	if ( numSpectraSRC == 0 ) 
	{
		Helpers::print("Error: Could not load spectra from source dump file.\n", &logFile);
		return;	
	}

	if ( numSpectraSRC >= numSpectraSOM ) 
	{
		Helpers::print("Warning: Source spectra greater than SOM map."+Helpers::numberToString<__int32>(numSpectraSRC)+" / "+Helpers::numberToString<size_t>(numSpectraSOM)+"\n", &logFile);
		return;	
	}

	std::string sstrOutput;
	static const std::string sstrEmptyLine("-1.0; -1.0; -1.0;\n");
	

	size_t c = 0;
	for ( size_t i=0;i<numSpectraSOM;i++ )
	{
		Spectra *spSOM = vfsSOM.beginRead(i);
		if( spSOM->isEmpty()  )
		{
			sstrOutput += sstrEmptyLine;
		}
		else
		{
			if (spSOM->m_Index>=numSpectraSRC)
			{
				Helpers::print("SOM index to source spectra out of range "+Helpers::numberToString<__int32>(spSOM->m_Index)+" / "+Helpers::numberToString<size_t>(numSpectraSRC)+"\n" );
				sstrOutput += sstrEmptyLine;
			}
			else
			{
				Spectra *spSRC = vfsSrc.beginRead(spSOM->m_Index);

				float err0 = 0.0f;
				float err1 = 0.0f;
				float err2 = 0.0f;
				
				size_t nRange = Spectra::numSamples/3;
				for (size_t j=0;j<nRange;j++)
				{
					float d0 = (a.m_Amplitude[i]-spSRC->m_Amplitude[i]);
					float d1 = (a.m_Amplitude[i+nRange]-spSRC->m_Amplitude[i+nRange]);
					float d2 = (a.m_Amplitude[i+nRange*2]-spSRC->m_Amplitude[i+nRange*2]);
					err0 += d0*d0;
					err1 += d1*d1;
					err2 += d2*d2;
				}

				sstrOutput += Helpers::numberToString<float>( err0 ) + "; " + Helpers::numberToString<float>( err1 ) + "; " + Helpers::numberToString<float>( err2 ) + ";\n";

				vfsSrc.endRead(spSOM->m_Index);
			}
			
		}
		vfsSOM.endRead(i);
		c += 3;
	}

	// write map to csv
	std::ofstream fon(sstrOutFile.c_str());
	fon<<sstrOutput;

	return;
}

