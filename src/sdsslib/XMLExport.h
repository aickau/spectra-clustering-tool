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
//! \file  XMLExport.h
//! \brief Simple XML export helper functions.

#ifndef __XMLEXPORT_H__
#define __XMLEXPORT_H__

#include <string>
#include <fstream>
#include "XMLParserAPI.h"

//! helper functions to build and export XML documents.
//!
//! example: 
//! xmlElementBegin( "XMLTAG0", 0, sstrXMLDoc );
//! xmlAddAttribute( "attribute1", "StringValue", sstrXMLDoc );
//! xmlElementEndBegin( sstrXMLDoc );
//!     xmlSingleElementBegin( "XMLTAG1", 1, sstrXMLDoc );
//!     xmlAddAttribute( "attribute2", "42", sstrXMLDoc );
//!     xmlSingleElementEnd( sstrXMLDoc );
//! xmlElementEnd( "XMLTAG0", 0, sstrXMLDoc );
//! 
//! gives you:
//! <XMLTAG0 attribute1="StringValue">
//!     <XMLTAG1 attribute2="42"/>
//! </XMLTAG0>
//!
//! XML reminder: XML-element = <tag attribute="value"> XML-element [XML-element]* </tag>
//!                             <tag attribute="value"> content </tag>
namespace XMLExport
{

	/** @name XML Export Helper Routines
	*/
	//@{

	//! begin a new XML element.
	//! /param sstrTagName tag name of the XML element
	//! /param nLevel tab level to improve readability for humans.
	//! /param sstrOut output XML document string
	XMLPARSERAPI void xmlElementBegin( const std::string &sstrTagName, unsigned int nLevel, std::string &sstrOut );
	
	//! close the header of a new XML element.
	//! /param sstrOut output XML document string
	XMLPARSERAPI void xmlElementEndBegin( std::string &sstrOut );
	
	//! close the XML element.
	//! /param sstrTagName tag name of the XML element
	//! /param nLevel tab level to improve readability for humans.
	//! /param sstrOut output XML document string
	XMLPARSERAPI void xmlElementEnd( const std::string &sstrTagName, unsigned int nLevel, std::string &sstrOut );

	//! begin a new XML element within a single line.
	//! /param sstrTagName tag name of the XML element
	//! /param nLevel tab level to improve readability for humans.
	//! /param sstrOut output XML document string
	XMLPARSERAPI void xmlSingleElementBegin( const std::string &sstrTagName, unsigned int nLevel, std::string &sstrOut );

	//! close the single line XML element.
	//! /param sstrOut output XML document string
	XMLPARSERAPI void xmlSingleElementEnd( std::string &sstrOut );

	//! add a float attribute to the current XML element.
	//! call xmlElementBegin() or xmlSingleElementBegin() before calling this function.
	//! /param sstrAttributeName attribute name
	//! /param value 
	//! /param sstrOut output XML document string
	XMLPARSERAPI void xmlAddAttribute( const std::string &sstrAttributeName, float value, std::string &sstrOut );

	//! add a signed integer attribute to the current XML element.
	//! call xmlElementBegin() or xmlSingleElementBegin() before calling this function.
	//! /param sstrAttributeName attribute name
	//! /param value 
	//! /param sstrOut output XML document string
	XMLPARSERAPI void xmlAddAttribute( const std::string &sstrAttributeName, int value, std::string &sstrOut );

	//! add an unsigned integer attribute to the current XML element.
	//! call xmlElementBegin() or xmlSingleElementBegin() before calling this function.
	//! /param sstrAttributeName attribute name
	//! /param value 
	//! /param sstrOut output XML document string
	XMLPARSERAPI void xmlAddAttribute( const std::string &sstrAttributeName, unsigned int value, std::string &sstrOut );

	//! add an string attribute to the current XML element.
	//! call xmlElementBegin() or xmlSingleElementBegin() before calling this function.
	//! /param sstrAttributeName attribute name
	//! /param sstrValue value
	//! /param sstrOut output XML document string
	XMLPARSERAPI void xmlAddAttribute( const std::string &sstrAttributeName, const std::string &sstrValue, std::string &sstrOut );

	//! add an array of float attributes to the current XML element.
	//! call xmlElementBegin() or xmlSingleElementBegin() before calling this function.
	//! /param psstrAttributeNames pointer to an array of attribute names. Must have the size of nValues.
	//! /param pValues pointer to a float array. Must have the size of nValues.
	//! /param nValues size of the arrays.
	//! /param sstrOut output XML document string
	XMLPARSERAPI void xmlAddAttributeArray( const std::string *psstrAttributeNames, const float *pValues, size_t nValues, std::string &sstrOut );

	//! add comment line.
	//! /param sstrComment text.
	//! /param sstrOut output XML document string
	XMLPARSERAPI void xmlSingleComment( const std::string &sstrComment, std::string &sstrOut );

	//! add xml header.
	//! /param sstrOut output XML document string, should be empty, since header should be in the first line.
	XMLPARSERAPI void xmlHeader( std::string &sstrOut );


	//@}	
};


#endif // __XMLEXPORT_H__