#include "FileHelpers.h"

#include <conio.h>
#include <windows.h>
#include <algorithm>
#include <fstream>


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
	char dir[MAX_PATH+2];
	GetCurrentDirectory(MAX_PATH, dir);
	return std::string(dir);
}


bool FileHelpers::exitsDirectory(const std::string &_sstrFilename)
{
	// TODO.
	
	return true;
}

size_t FileHelpers::getFileList( const std::string &_sstrSearchDir, std::vector<std::string> &_outFileNameList )
{
	WIN32_FIND_DATA fileInfo;
	HANDLE h = FindFirstFile( _sstrSearchDir.c_str(), &fileInfo );

	if ( h == INVALID_HANDLE_VALUE )
	{
		return 0;
	}

	do {
		std::string sstrFileName(fileInfo.cFileName);

		if ( fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			if ( sstrFileName != "." && sstrFileName != ".." )
			{
				std::string sstrNewSearchDir( getFilePath(_sstrSearchDir) );
				sstrNewSearchDir += sstrFileName;
				sstrNewSearchDir += "/*";

				getFileList( sstrNewSearchDir, _outFileNameList );
			}
		}
		else
		{
			std::string sstrFullFileName( getFilePath(_sstrSearchDir) );
			sstrFullFileName += sstrFileName;
			_outFileNameList.push_back( sstrFullFileName );
		}
	} while( FindNextFile(h,&fileInfo) );

	return _outFileNameList.size();
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
	std::replace( sstrFileName.begin(), sstrFileName.end(), '\\', '/' );
	size_t nIndex( sstrFileName.find_last_of('/') );
	if ( nIndex == std::string::npos )
	{
		return sstrFileName;
	}
	return sstrFileName.substr( nIndex+1, std::string::npos );
}




std::string FileHelpers::getFilePath(const std::string &_sstrFilename)
{
	std::string sstrFileName(_sstrFilename);
	std::replace( sstrFileName.begin(), sstrFileName.end(), '\\', '/' );
	size_t nIndex( sstrFileName.find_last_of('/') );
	if ( nIndex == std::string::npos )
	{
		return sstrFileName;
	}
	return sstrFileName.substr( 0, nIndex+1 );
}


void FileHelpers::writefile(const std::string &_sstrFilename, char *_buf, int _size)
{
	FILE *f;
	f = fopen(_sstrFilename.c_str(),"wb");
	fwrite(_buf,_size,1,f);
	fclose(f);
}

