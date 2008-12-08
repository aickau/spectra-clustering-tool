#ifndef H_HELPERS
#define H_HELPERS

#include <string>
#include <sstream>

namespace Helpers
{
	template<class T>
	std::string numberToString( T _number )
	{
		std::stringstream sstrStream;
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

	// find tag in value and replace by insert value
	bool insertString( const std::string &sstrTag, const std::string &sstrInsertValue, std::string &sstrValue );
	
	// print text to console, optional to a log file.
	void Print( const std::string &_sstrText, std::ofstream *_logStream=NULL );

	// get global bounds of a given float array.
	void getMinMax( float *_values, size_t _valueCount, size_t _strideInBytes, size_t _offsetInBytes, float &_outMin, float &_outMax );

	// returns current date and time as string.
	std::string getCurentDateTimeStampString();

};


#endif
