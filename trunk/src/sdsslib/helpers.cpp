#include "helpers.h"

#include <conio.h>
#include <windows.h>
#include <algorithm>
#include <fstream>

#include <float.h>



static
void lowercase( std::string &_sstr ) 
{
	for ( size_t i = 0; i < _sstr.size(); i++) 
	{
		_sstr[i] = tolower(_sstr[i]);
	}
}

size_t Helpers::getFileList( const std::string &_sstrSearchDir, std::vector<std::string> &_outFileNameList )
{
	WIN32_FIND_DATA fileInfo;
	HANDLE h = FindFirstFile( _sstrSearchDir.c_str(), &fileInfo );

	if ( h == INVALID_HANDLE_VALUE )
	{
		return 0;
	}

	do {
		std::string sstrFileName(fileInfo.cFileName);

		if ( fileInfo.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY )
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

bool Helpers::fileExists(const std::string &_sstrFilename)
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


std::string Helpers::getFileExtension(const std::string &_sstrFilename)
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


std::string Helpers::getFileName(const std::string &_sstrFilename)
{
	std::string sstrFileName(_sstrFilename);
	std::replace( sstrFileName.begin(), sstrFileName.end(), '\\', '/' );
	size_t nIndex( _sstrFilename.find_last_of('/') );
	if ( nIndex == std::string::npos )
	{
		return _sstrFilename;
	}
	return _sstrFilename.substr( nIndex+1, std::string::npos );
}




std::string Helpers::getFilePath(const std::string &_sstrFilename)
{
	std::string sstrFileName(_sstrFilename);
	std::replace( sstrFileName.begin(), sstrFileName.end(), '\\', '/' );
	size_t nIndex( _sstrFilename.find_last_of('/') );
	if ( nIndex == std::string::npos )
	{
		return _sstrFilename;
	}
	return _sstrFilename.substr( 0, nIndex+1 );
}


void Helpers::writefile(const std::string &_sstrFilename, char *_buf, int _size)
{
	FILE *f;
	f = fopen(_sstrFilename.c_str(),"wb");
	fwrite(_buf,_size,1,f);
	fclose(f);
}

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


void Helpers::Print( const std::string &_sstrText, std::ofstream *_logStream )
{
	_cprintf( _sstrText.c_str() );
	if ( _logStream != NULL )
	{
		(*_logStream) << _sstrText;
	}
}


void Helpers::getMinMax( float *_values, size_t _valueCount, size_t _strideInBytes, size_t _offsetInBytes, float &_outMin, float &_outMax )
{
	_outMin=FLT_MAX;
	_outMax=-FLT_MAX;

	if ( _strideInBytes == 0 )
	{
		_strideInBytes = 4;
	}
	char *p = reinterpret_cast<char*>(_values)+_offsetInBytes;
	for ( int i=0;i<_valueCount;i++)
	{
		float *v = reinterpret_cast<float*>( p );
		if ( *v < _outMin )
		{
			_outMin = *v;
		}
		if ( *v > _outMax )
		{
			_outMax = *v;
		}
		p += _strideInBytes;
	}
}


