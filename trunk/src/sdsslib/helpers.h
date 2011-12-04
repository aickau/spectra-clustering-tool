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
//! \file  helpers.h
//! \brief 


#ifndef H_HELPERS
#define H_HELPERS

#include <string>
#include <sstream>
#include <Windows.h>

namespace Helpers
{
	// \param _width fill number with preceding zeros
	template<class T>
	std::string numberToString( T _number, int _width =-1 )
	{
		std::stringstream sstrStream;
		if( _width!= -1 )
		{
			sstrStream.fill('0');
			sstrStream.width(_width);
		}
		sstrStream << _number;
		return sstrStream.str();
	}

	template<class T>
	T stringToNumber( const std::string &_sstrString )
	{
		std::istringstream sstrStream(_sstrString);
		T num;
		if (sstrStream) {
			sstrStream >> num;	
		}
		return num;
	}

	// make string upper case
	std::string upperCase( const std::string &_sstrString );

	// make string lower case
	std::string lowerCase( const std::string &_sstrString );

	// returns current date and time as string.
	std::string getCurentDateTimeStampString();

	// find tag in value and replace by insert value
	bool insertString( const std::string &sstrTag, const std::string &sstrInsertValue, std::string &sstrValue );
	
	// print text to console, optional to a log file.
	// _bPrintPrefix print out date and time stamp
	void print( const std::string &_sstrText, std::ofstream *_logStream=NULL, bool _bPrintPrefix = true );

	// allocate console window, set standard output to this console.
	void createConsole();

	__int64 HiLowtoUInt64( unsigned __int32 _nLowerPart, unsigned __int32 _nHigherPart );
	void UInt64toHiLow( unsigned __int64 _nInNumber, unsigned __int32 &_nOutLowerPart, unsigned __int32 &_nOutHigherPart );

	// convert command line string to command line structure (argc, char* argv[])
	// returns char** argv, even if command line string was empty.
	// make sure to proper deallocate the resulting pointer via delete[].
	char **getCommandLineFromString( const std::string &sstrCommandLineString, int &outArgC );

	// usage:
	//
	//  __try {
	//		[..]
	// }
	// __except( writeMiniDump( GetExceptionInformation() ) { }
	//
	void WINAPI writeMiniDump( PEXCEPTION_POINTERS ep );

};


#endif
