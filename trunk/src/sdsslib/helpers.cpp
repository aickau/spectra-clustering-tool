#include "helpers.h"

#include <conio.h>
#include <windows.h>
#include <fstream>

#include <float.h>



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
	for ( size_t i=0;i<_valueCount;i++)
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


std::string Helpers::getCurentDateTimeStampString()
{
	SYSTEMTIME systime;
	GetSystemTime( &systime	);

	std::string sstrOutString("");
	sstrOutString += Helpers::numberToString<WORD>(systime.wHour);
	sstrOutString += ":";
	sstrOutString += numberToString<WORD>(systime.wMinute);
	sstrOutString += ":";
	sstrOutString += numberToString<WORD>(systime.wSecond);
	sstrOutString += ",";
	sstrOutString += numberToString<WORD>(systime.wMilliseconds);
	sstrOutString += " ";
	sstrOutString += numberToString<WORD>(systime.wMonth);
	sstrOutString += "/";
	sstrOutString += numberToString<WORD>(systime.wDay);
	sstrOutString += "/";
	sstrOutString += numberToString<WORD>(systime.wYear);

	return sstrOutString;
}
