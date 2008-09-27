#ifndef H_HELPERS
#define H_HELPERS

#include <string>
#include <vector>
#include <sstream>

namespace Helpers
{

	size_t getFileList( const std::string &_sstrSearchDir, std::vector<std::string> &_outFileNameList );

	// extract extension
	// e.g. hui.blah.TXT -> .txt (always lowercase)
	std::string getFileExtension(const std::string &_sstrFilename);

	// extract filename and extension (minus path)
	std::string getFileName(const std::string &_sstrFilename);

	// extract path, e.g c:\hui\blah.txt -> c:/hui/ or c:/hui/blah -> c:/hui/
	std::string getFilePath(const std::string &_sstrFilename);

	bool fileExists(const std::string &_sstrFilename);
	void writefile(const std::string &_sstrFilename, char *_buf, int _size);

	template<class T>
	std::string numberToString( T _number )
	{
		std::stringstream sstrStream;
		sstrStream << _number;
		return sstrStream.str();
	}

	// find tag in value and replace by insert value
	bool insertString( const std::string &sstrTag, const std::string &sstrInsertValue, std::string &sstrValue );
	
	// print text to console, optional to a log file.
	void Print( const std::string &_sstrText, std::ofstream *_logStream=NULL );

	// get global bounds of a given float array.
	void getMinMax( float *_values, size_t _valueCount, size_t _strideInBytes, size_t _offsetInBytes, float &_outMin, float &_outMax );

};


#endif
