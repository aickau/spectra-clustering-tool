//! \verbatim
//! ###########################################################################
//! # ASPECT: A spectra clustering tool for exploration of large spectral surveys - Stage I
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
//! \file  XMLExport.cpp
//! \brief Simple XML export helper functions.


#include "XMLExport.h"

#include <sstream>
#include <assert.h>

static const std::string XMLHEADER("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");

namespace XMLExport
{
 
void appendTabToString( unsigned int n, std::string &sstrOut )
{
	for (unsigned int i=0; i < n; i++ )
	{
		sstrOut += "\t";
	}
}


void xmlElementBegin( const std::string &sstrTagName, unsigned int nLevel, std::string &sstrOut )
{
	sstrOut += "\n";
	appendTabToString( nLevel, sstrOut );
	sstrOut += "<";
	sstrOut += sstrTagName;
}


void xmlElementEndBegin( std::string &sstrOut )
{
	sstrOut += ">";
}

void xmlElementEnd( const std::string &sstrTagName, unsigned int nLevel, std::string &sstrOut )
{
	sstrOut += "\n";
	appendTabToString( nLevel, sstrOut );
	sstrOut += "</";
	sstrOut += sstrTagName;
	sstrOut += ">";
}



void xmlSingleElementBegin( const std::string &sstrTagName, unsigned int nLevel, std::string &sstrOut )
{
	xmlElementBegin( sstrTagName, nLevel, sstrOut );
}


void xmlSingleElementEnd( std::string &sstrOut )
{
	sstrOut += "/>";
}



void xmlAddAttribute( const std::string &sstrAttributeName, float value, std::string &sstrOut )
{
	std::stringstream strStream;
	strStream.clear();
	strStream << " " << sstrAttributeName << "=\"" << value << "\"";
	sstrOut += strStream.str();
}


void xmlAddAttribute( const std::string &sstrAttributeName, int value, std::string &sstrOut )
{
	std::stringstream strStream;
	strStream.clear();
	strStream << " " << sstrAttributeName << "=\"" << value << "\"";
	sstrOut += strStream.str();

}


void xmlAddAttribute( const std::string &sstrAttributeName, unsigned int value, std::string &sstrOut )
{
	std::stringstream strStream;
	strStream.clear();
	strStream << " " << sstrAttributeName << "=\"" << value << "\"";
	sstrOut += strStream.str();
}


void xmlAddAttribute( const std::string &sstrAttributeName, const std::string &sstrValue, std::string &sstrOut )
{
	std::stringstream strStream;
	strStream.clear();
	strStream << " " << sstrAttributeName << "=\"" << sstrValue << "\"";
	sstrOut += strStream.str();
}



void xmlAddAttributeArray( const std::string *psstrAttributeNames, const float *pValues, size_t nValues, std::string &sstrOut )
{
	std::stringstream strStream;
	strStream.clear();

	assert( psstrAttributeNames != NULL );
	assert( pValues != NULL );

	for ( size_t i=0; i < nValues;i++ )
	{
		strStream << " " << psstrAttributeNames[i] << "=\"" << pValues[i] << "\"";
	}
	sstrOut += strStream.str();
}


void xmlSingleComment( const std::string &sstrComment, std::string &sstrOut )
{
	sstrOut += "<!-- ";
	sstrOut += sstrComment;
	sstrOut += " -->";
}

void xmlHeader( std::string &sstrOut )
{
	sstrOut += XMLHEADER;
}

}