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
//! \file  helpers.h
//! \brief 


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

	// returns current date and time as string.
	std::string getCurentDateTimeStampString();

	// find tag in value and replace by insert value
	bool insertString( const std::string &sstrTag, const std::string &sstrInsertValue, std::string &sstrValue );
	
	// print text to console, optional to a log file.
	void print( const std::string &_sstrText, std::ofstream *_logStream=NULL );

};


#endif
