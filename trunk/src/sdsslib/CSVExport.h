//! \verbatim
//! ###########################################################################
//! # ASPECT: A spectra clustering tool for exploration of large spectral surveys - Stage I
//! #
//! # Copyright (c) 2009 Aick in der Au
//! # All rights reserved.
//! ###########################################################################
//!
//!      created by : Aick in der Au <aick.inderau@gmail.com>
//!      created on : 11/15/2009
//! additional docs : none
//!  responsibility : 1. Aick in der Au
//!                   2. 
//! \endverbatim
//!
//! \file  CSVExport.h
//! \brief Simpler saver class to write CSV table entries.


// Simpler saver class to write CSV table entries.
// See http://en.wikipedia.org/wiki/Comma-separated_values
#ifndef __CSVEXPORT_H__
#define __CSVEXPORT_H__

#include "defines.h"

#include <string>


class CSVExport
{
public:
	CSVExport(const std::string &_sstrFileName, const std::string &_sstrSeperator=";" );
	virtual ~CSVExport();

	void writeTableEntry( const char *_text );
	void writeTableEntry( const std::string &_sstrText );
	void writeTableEntry( float _number );

	// \param _width fill number with preceding zeros
	void writeTableEntry( int _number, unsigned int _width = -1 );
	void writeTableEntry( int64_t _number, unsigned int _width = -1 );

	void newRow();

	// flush table to disk.
	void flush();

private:
	const std::string m_sstrSeperator;
	const std::string m_sstrFilename;
	std::string m_sstrTable;
};


#endif // __CSVEXPORT_H__