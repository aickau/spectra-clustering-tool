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
//! \file  CSVExport.cpp
#include "CSVExport.h"

#include "helpers.h"

#include <sstream>
#include <fstream>


CSVExport::CSVExport(const std::string &_sstrFileName, const std::string &_sstrSeperator )
:m_sstrSeperator(_sstrSeperator)
,m_sstrFilename(_sstrFileName)
,m_sstrTable("")
{
}

CSVExport::~CSVExport()
{
	flush();
}

void CSVExport::writeTableEntry( const char *_text )
{
	m_sstrTable += _text;
	m_sstrTable += m_sstrSeperator;
}


void CSVExport::writeTableEntry( const std::string &_sstrText )
{
	// todo, add double-quote characters if necessary
	m_sstrTable += _sstrText;
	m_sstrTable += m_sstrSeperator;
}


void CSVExport::writeTableEntry( float _number )
{
	m_sstrTable += Helpers::numberToString(_number);
	m_sstrTable += m_sstrSeperator;
}


void CSVExport::writeTableEntry( int _number, unsigned int _width )
{
	m_sstrTable += Helpers::numberToString(_number,_width);
	m_sstrTable += m_sstrSeperator;
}

void CSVExport::writeTableEntry( int64_t _number, unsigned int _width )
{
	m_sstrTable += Helpers::numberToString<int64_t>(_number,_width);
	m_sstrTable += m_sstrSeperator;
}


void CSVExport::newRow()
{
	m_sstrTable += "\n";
}


void CSVExport::flush()
{
	std::ofstream fon(std::string(m_sstrFilename).c_str());
	fon<<m_sstrTable;
}

