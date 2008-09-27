
#ifndef __XMLEXPORT_H__
#define __XMLEXPORT_H__

#include <string>
#include <fstream>
#include "XMLParserAPI.h"

//! static helper functions to build and export XML documents.
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
class XMLExport
{
public:


	/** @name XML Export Helper Routines
	*/
	//@{

	//! begin a new XML element.
	//! /param sstrTagName tag name of the XML element
	//! /param nLevel tab level to improve readability for humans.
	//! /param sstrOut output XML document string
	XMLPARSERAPI static void xmlElementBegin( const std::string &sstrTagName, unsigned int nLevel, std::string &sstrOut );
	
	//! close the header of a new XML element.
	//! /param sstrOut output XML document string
	XMLPARSERAPI static void xmlElementEndBegin( std::string &sstrOut );
	
	//! close the XML element.
	//! /param sstrTagName tag name of the XML element
	//! /param nLevel tab level to improve readability for humans.
	//! /param sstrOut output XML document string
	XMLPARSERAPI static void xmlElementEnd( const std::string &sstrTagName, unsigned int nLevel, std::string &sstrOut );

	//! begin a new XML element within a single line.
	//! /param sstrTagName tag name of the XML element
	//! /param nLevel tab level to improve readability for humans.
	//! /param sstrOut output XML document string
	XMLPARSERAPI static void xmlSingleElementBegin( const std::string &sstrTagName, unsigned int nLevel, std::string &sstrOut );

	//! close the single line XML element.
	//! /param sstrOut output XML document string
	XMLPARSERAPI static void xmlSingleElementEnd( std::string &sstrOut );

	//! add a float attribute to the current XML element.
	//! call xmlElementBegin() or xmlSingleElementBegin() before calling this function.
	//! /param sstrAttributeName attribute name
	//! /param value 
	//! /param sstrOut output XML document string
	XMLPARSERAPI static void xmlAddAttribute( const std::string &sstrAttributeName, float value, std::string &sstrOut );

	//! add a signed integer attribute to the current XML element.
	//! call xmlElementBegin() or xmlSingleElementBegin() before calling this function.
	//! /param sstrAttributeName attribute name
	//! /param value 
	//! /param sstrOut output XML document string
	XMLPARSERAPI static void xmlAddAttribute( const std::string &sstrAttributeName, int value, std::string &sstrOut );

	//! add an unsigned integer attribute to the current XML element.
	//! call xmlElementBegin() or xmlSingleElementBegin() before calling this function.
	//! /param sstrAttributeName attribute name
	//! /param value 
	//! /param sstrOut output XML document string
	XMLPARSERAPI static void xmlAddAttribute( const std::string &sstrAttributeName, unsigned int value, std::string &sstrOut );

	//! add an string attribute to the current XML element.
	//! call xmlElementBegin() or xmlSingleElementBegin() before calling this function.
	//! /param sstrAttributeName attribute name
	//! /param sstrValue value
	//! /param sstrOut output XML document string
	XMLPARSERAPI static void xmlAddAttribute( const std::string &sstrAttributeName, const std::string &sstrValue, std::string &sstrOut );

	//! add an array of float attributes to the current XML element.
	//! call xmlElementBegin() or xmlSingleElementBegin() before calling this function.
	//! /param psstrAttributeNames pointer to an array of attribute names. Must have the size of nValues.
	//! /param pValues pointer to a float array. Must have the size of nValues.
	//! /param nValues size of the arrays.
	//! /param sstrOut output XML document string
	XMLPARSERAPI static void xmlAddAttributeArray( const std::string *psstrAttributeNames, const float *pValues, size_t nValues, std::string &sstrOut );

	//@}	
};


#endif // __XMLEXPORT_H__