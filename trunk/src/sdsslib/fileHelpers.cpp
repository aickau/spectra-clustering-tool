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
//! \file  fileHelpers.cpp
//! \brief 

#include "fileHelpers.h"

#include "defines.h"
#include "debug.h"

#include <algorithm>
#include <fstream>
#include <assert.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#else
#include <unistd.h>
#include <sys/types.h>
#endif

#ifndef MAX_PATH
#define MAX_PATH 4096
#endif




#ifdef WIN32
const std::string sstrPathSeparator = "\\";
const std::string sstrInvalidSeparator = "/";
#else
const std::string sstrPathSeparator = "/";
const std::string sstrInvalidSeparator = "\\";
#endif




static
void lowercase( std::string &_sstr ) 
{
	for ( size_t i = 0; i < _sstr.size(); i++) 
	{
		_sstr[i] = tolower(_sstr[i]);
	}
}

void FileHelpers::convertSeperators( std::string &_sstrFilePath )
{
	std::replace( _sstrFilePath.begin(), _sstrFilePath.end(), '\\', '/' );
}


bool FileHelpers::isFilePathValid(const std::string &_sstrFilePath )
{
	size_t filePathSize(_sstrFilePath.size());
	if( filePathSize > MAX_PATH  || filePathSize == 0 )
	{
		return false;
	}

	if ( _sstrFilePath.find_first_of("|?<>&\"*") != std::string::npos)
	{
		return false;
	}

	for ( size_t i=0;i<filePathSize;i++ )
	{
		// allow extended ASCII characters, e.g. 'ä', .. 
		if ( static_cast<unsigned char>(_sstrFilePath[i]) <= 0x1f )
		{
			return false;
		}
	}

	std::string sstrFileName( getFileName(_sstrFilePath) );

	size_t fileNameSize(sstrFileName.size());
	if( fileNameSize >= 256 )
	{
		return false;
	}

	return true;
}




std::string FileHelpers::getCurrentDirectory()
{
#ifdef WIN32
	char dir[MAX_PATH+2];
	GetCurrentDirectory(MAX_PATH, dir);
	std::string sstrDir(dir);
	convertSeperators(sstrDir);

	return sstrDir;
#else
	char pwd[FILENAME_MAX];
	if( !getcwd( pwd, FILENAME_MAX ) ) 
	{
		ASSERT(0);
		return "";
	}
	return std::string( pwd );
#endif
}


bool FileHelpers::exitsDirectory(const std::string &_sstrDir)
{
	std::string fullPath("");
	if ( _sstrDir.size() > 0 && (_sstrDir[0] == '\\' || _sstrDir[0] == '/') ) {
		char cwd[MAX_PATH];
#ifdef _WIN32
		_getcwd(cwd, sizeof(cwd));
#else
		getcwd(cwd, sizeof(cwd));
#endif
		fullPath += cwd;
	}
	fullPath += _sstrDir;

	struct stat st;
	if( stat( fullPath.c_str(), &st) != 0) {
		return false;
	}
	return true; 	
}

size_t FileHelpers::getFileList( const std::string &_sstrSearchDir, std::vector<std::string> &_outFileNameList )
{
#ifdef _WIN32
	WIN32_FIND_DATA fileInfo;
	HANDLE h = FindFirstFile( _sstrSearchDir.c_str(), &fileInfo );

	if ( h == INVALID_HANDLE_VALUE )
	{
		return 0;
	}

	size_t c = 0;
	do {
		std::string sstrFileName(fileInfo.cFileName);

		if ( fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			if ( sstrFileName != "." && sstrFileName != ".." && sstrFileName != ".svn" )
			{
				std::string sstrNewSearchDir( getFilePath(_sstrSearchDir) );
				sstrNewSearchDir += sstrFileName;
				sstrNewSearchDir += "/*";

				c += getFileList( sstrNewSearchDir, _outFileNameList );
			}
		}
		else
		{
			std::string sstrFullFileName( getFilePath(_sstrSearchDir) );
			sstrFullFileName += sstrFileName;
			_outFileNameList.push_back( sstrFullFileName );
			c++;
		}
	} while( FindNextFile(h,&fileInfo) );

	return c;

#else
	assert(0);
	return 0;
#endif
}

bool FileHelpers::fileExists(const std::string &_sstrFilename)
{
	FILE *f;
	f = fopen(_sstrFilename.c_str(),"r");
	if ( f == NULL )
	{
		return false;
	}

	fclose(f);
	return true;
}


size_t FileHelpers::getFileSize(const std::string &_sstrFilename)
{
	FILE *f = fopen( _sstrFilename.c_str(), "rb" );

	if (f == NULL )
	{
		return 0;
	}

	_fseeki64( f, 0, SEEK_END );
	int64_t fileSize = _ftelli64( f );
	fclose( f );

	// check if we reach 32 bit limits.
	assert( static_cast<size_t>(fileSize) == fileSize );

	return static_cast<size_t>(fileSize);
}


std::string FileHelpers::getFileExtension(const std::string &_sstrFilename)
{
	size_t nIndex( _sstrFilename.find_last_of('.') );
	if ( nIndex == std::string::npos )
	{
		return std::string("");
	}
	std::string sstrRetVal( _sstrFilename.substr( nIndex ) );
	lowercase( sstrRetVal );
	return sstrRetVal;
}


std::string FileHelpers::getFileName(const std::string &_sstrFilename)
{
	std::string sstrFileName(_sstrFilename);
	convertSeperators(sstrFileName); 
	size_t nIndex( sstrFileName.find_last_of('/') );
	if ( nIndex == std::string::npos )
	{
		return sstrFileName;
	}
	return sstrFileName.substr( nIndex+1, std::string::npos );
}

std::string FileHelpers::getFileNameMinusExtension(const std::string &_sstrFilename)
{
	// remove path
	std::string sstrFileName(getFileName(_sstrFilename));

	size_t nIndex( sstrFileName.find_last_of('.') );
	if ( nIndex == std::string::npos )
	{
		return sstrFileName;
	}
	std::string sstrRetVal( _sstrFilename.substr( 0, nIndex ) );

	return sstrRetVal;
}




std::string FileHelpers::getFilePath(const std::string &_sstrFilename)
{
	std::string sstrFileName(_sstrFilename);
	convertSeperators(sstrFileName); 
	size_t nIndex( sstrFileName.find_last_of('/') );
	if ( nIndex == std::string::npos )
	{
		return "";//sstrFileName;
	}
	return sstrFileName.substr( 0, nIndex+1 );
}


void FileHelpers::writeFile(const std::string &_sstrFilename, void *_buf, int _size, bool _bWiteAsBinary )
{
	FILE *f;

	std::string sstrMode;
	if ( _bWiteAsBinary )
	{
		sstrMode = "wb";
	}
	else
	{
		sstrMode = "wt";
	}

	f = fopen( _sstrFilename.c_str(), sstrMode.c_str() );
	if ( f ) 
	{
		fwrite(_buf,_size,1,f);
		fclose(f);
	}
}

bool FileHelpers::renameFile( const std::string &_sstrFilename, const std::string &_sstrNewFilename, bool _bOverwriteExisting )
{
#ifdef _WIN32
	std::string sstrPath = getFilePath(_sstrFilename);
	std::string sstrFullNewFileName;

	if ( !sstrPath.empty() )
	{
		sstrFullNewFileName = sstrPath + "/";
	}
	sstrFullNewFileName += _sstrNewFilename;

	DWORD dwFlags = (_bOverwriteExisting) ? MOVEFILE_REPLACE_EXISTING : 0;
	bool bRetVal = (MoveFileEx(_sstrFilename.c_str(), sstrFullNewFileName.c_str(), dwFlags )==TRUE);
	return bRetVal;
#else
	assert(0);
	return false;
#endif

}



bool FileHelpers::loadFileToString(const std::string &_sstrFilename, std::string &_sstrOutString )
{
	_sstrOutString = "";
	std::ifstream fin(_sstrFilename.c_str());

	if( !fin ) 
	{
		// could not open file
		return false;
	}

	// read selection list
	std::string sstrLine;
	while( getline(fin, sstrLine) ) 
	{
		_sstrOutString += sstrLine + "\n";
	}
	return true;
}
