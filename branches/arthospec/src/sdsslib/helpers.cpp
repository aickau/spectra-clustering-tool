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
//! \file  helpers.cpp
//! \brief 

#include "helpers.h"

#include <fstream>
#include <iostream>
#include <algorithm>

#include <conio.h>
#include <windows.h>
#include <fcntl.h>
#include <io.h>



bool Helpers::insertString( const std::string &sstrTag, const std::string &sstrInsertValue, std::string &sstrValue )
{
	size_t insertpos = sstrValue.find(sstrTag);
	if ( insertpos == std::string::npos )
	{
		return false;
	}
	sstrValue = sstrValue.erase(insertpos, sstrTag.size());
	sstrValue.insert( insertpos, sstrInsertValue );
	return true;
}


std::string Helpers::upperCase( const std::string &_sstrString )
{
	std::string temp( _sstrString );
	std::transform( temp.begin(), temp.end(), temp.begin(), toupper );
	return temp;
}

std::string Helpers::lowerCase( const std::string &_sstrString )
{
	std::string temp( _sstrString );
	std::transform( temp.begin(), temp.end(), temp.begin(), tolower );
	return temp;				
}


void Helpers::print( const std::string &_sstrText, std::ofstream *_logStream, bool _bPrintPrefix )
{
	std::string sstrPrefix;
	if ( _bPrintPrefix )
	{
		sstrPrefix = Helpers::getCurentDateTimeStampString();
		sstrPrefix += "> ";
		_cprintf( sstrPrefix.c_str() );
	}
	_cprintf( _sstrText.c_str() );
	if ( _logStream != NULL )
	{
		(*_logStream) << sstrPrefix;
		(*_logStream) << _sstrText;
		_logStream->flush();		// slower, but more secure for log outputs
	}
}


std::string Helpers::getCurentDateTimeStampString()
{
	SYSTEMTIME systime;
	GetSystemTime( &systime	);

	std::string sstrOutString("");
	sstrOutString += numberToString<WORD>(systime.wMonth);
	sstrOutString += "/";
	sstrOutString += numberToString<WORD>(systime.wDay);
	sstrOutString += "/";
	sstrOutString += numberToString<WORD>(systime.wYear);
	sstrOutString += " ";
	sstrOutString += Helpers::numberToString<WORD>(systime.wHour);
	sstrOutString += ":";
	sstrOutString += numberToString<WORD>(systime.wMinute);
	sstrOutString += ":";
	sstrOutString += numberToString<WORD>(systime.wSecond);
	//sstrOutString += ",";
	//sstrOutString += numberToString<WORD>(systime.wMilliseconds);

	return sstrOutString;
}


void Helpers::createConsole()
{
	AllocConsole();
	int outHandle;
	FILE *outFile;
	outHandle = _open_osfhandle( (long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
	outFile = _fdopen( outHandle, "w" );
	*stdout = *outFile;
	setvbuf( stdout, NULL, _IONBF, 0 );
	std::cout.sync_with_stdio();
}