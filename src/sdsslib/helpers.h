#ifndef H_HELPERS
#define H_HELPERS

#include <string>
#include <vector>
#include <sstream>

namespace Helpers
{
	std::string getCurrentDir();

	// get a list of files from a given directory.
	// e.g. _sstrSearchDir = d:/hui/* - find all files in directory "d:/hui" and all subdirectories.
	//      _sstrSearchDir = d:/hui/*.exe find all executables in in directory "d:/hui" and all subdirectories.
	//      _sstrSearchDir = d:/hui/ find all files in in directory "d:/hui" not in subdirectories.
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

};


#endif
