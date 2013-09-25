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
//! \file  helpers.cpp
//! \brief 

#include "helpers.h"

#include <fstream>
#include <iostream>
#include <algorithm>

#include <conio.h>
#include <fcntl.h>
#include <io.h>

#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#include <DbgHelp.h>
#endif



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


int64_t Helpers::HiLowtoUInt64(uint32_t _nLowerPart, uint32_t _nHigherPart )
{
	return (static_cast<uint64_t>(_nHigherPart)<<32)+static_cast<uint64_t>(_nLowerPart);
}

void Helpers::UInt64toHiLow(uint64_t _nInNumber,uint32_t &_nOutLowerPart,uint32_t &_nOutHigherPart )
{
	_nOutLowerPart = static_cast<uint32_t>(_nInNumber & 0x0ffffffff);
	_nOutHigherPart = static_cast<uint32_t>(_nInNumber >> 32);
}

#ifdef _WIN32 

char **Helpers::getCommandLineFromString( const std::string &_sstrCommandLineString, int &_outArgC )
{
	_outArgC=0;
	size_t commandlineSize = _sstrCommandLineString.size();
	WCHAR *commandlinewc = new WCHAR[commandlineSize];
	char *commandline = new char[commandlineSize];
	mbstowcs( commandlinewc, GetCommandLine(), 16384 );
	LPWSTR *argvwc = CommandLineToArgvW( commandlinewc, &_outArgC );
	char **argv = new char*[_outArgC+1];
	size_t c=0;
	for (int i=0;i<_outArgC;i++) {
		argv[i] = commandline+c;
		size_t len =  wcslen(argvwc[i]);
		wcstombs(&commandline[c],argvwc[i], len);
		c+= len+1;
		commandline[c-1] = 0;
	}
	argv[_outArgC] = NULL; // terminate with zero.

	return argv;
}

typedef BOOL (WINAPI * MINIDUMP_WRITE_DUMP)(
    IN HANDLE           hProcess,
    IN DWORD            ProcessId,
    IN HANDLE           hFile,
    IN MINIDUMP_TYPE    DumpType,
    IN CONST PMINIDUMP_EXCEPTION_INFORMATION    ExceptionParam, OPTIONAL
    IN PMINIDUMP_USER_STREAM_INFORMATION        UserStreamParam, OPTIONAL
    IN PMINIDUMP_CALLBACK_INFORMATION           CallbackParam OPTIONAL
    );


void WINAPI Helpers::writeMiniDump( PEXCEPTION_POINTERS ep )
{
	HMODULE hDbgHelp = LoadLibrary("DBGHELP.DLL");
	MINIDUMP_WRITE_DUMP MiniDumpWriteDump = (MINIDUMP_WRITE_DUMP)GetProcAddress(hDbgHelp, "MiniDumpWriteDump");

	if (MiniDumpWriteDump)
	{
		MINIDUMP_EXCEPTION_INFORMATION  M;
		HANDLE  hDump_File;
		CHAR    Dump_Path[MAX_PATH];

		M.ThreadId = GetCurrentThreadId();
		M.ExceptionPointers = ep;

		M.ClientPointers = 0;

		GetModuleFileName(NULL, Dump_Path, sizeof(Dump_Path));
		lstrcpy(Dump_Path + lstrlen(Dump_Path) - 3, "dmp");

		hDump_File = CreateFile(Dump_Path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDump_File, MiniDumpNormal, (M.ExceptionPointers) ? &M : NULL, NULL, NULL);

		CloseHandle(hDump_File);
	}
}

#endif