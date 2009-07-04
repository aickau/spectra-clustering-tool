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
//! \file  fileHelpers.h
//! \brief 

#ifndef H_FILEHELPERS
#define H_FILEHELPERS

#include <string>
#include <vector>
#include <sstream>

namespace FileHelpers
{
	// convert file path to forward slashes
	// read the entire history here: http://en.wikipedia.org/wiki/Backslash
	// e.g. converts c:\hui\ to c:/hui/
	void convertSeperators( std::string &_sstrFilePath );

	// check if a given file path (directory plus optional file name) 
	// is syntactically correct (it is not necessary that the given path exists).
	bool isFilePathValid(const std::string &_sstrFilePath);

	// get currently set directory without a trailing seperator.
	// e.g. c:/hui
	std::string getCurrentDirectory();

	// check if directory exits.
	bool exitsDirectory(const std::string &_sstrFilename);

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

	// extract filename without extension, without path.
	std::string getFileNameMinusExtension(const std::string &_sstrFilename);

	// extract path, e.g c:\hui\blah.txt -> c:/hui or c:/hui/blah/ -> c:/hui/blah
	std::string getFilePath(const std::string &_sstrFilename);

	bool fileExists(const std::string &_sstrFilename);

	void writeFile(const std::string &_sstrFilename, char *_buf, int _size);

	bool loadFileToString(const std::string &_sstrFilename, std::string &_sstrOutString );
}

#endif