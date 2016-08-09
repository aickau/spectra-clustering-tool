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
//! \file  XMLParser.h
//! \brief 

#ifndef __XMLPARSER_H__
#define __XMLPARSER_H__

#include <string>
#include "XMLParserAPI.h"

class XMLParserImpl;

//! A very simple XML parser which does not generate compile and link time dependencies to XERCES.
//! Use this parser class to parse simple XML files (e.g. for ini files) which do not require
//!	validation or other sophisitcated features of XERCES.
//!	The actual parsing process is done by the user by using the provided navigation functions to 
//! step through the XML file. 
//! This class loads a XML document ( /see loadXML() ). 
//! After loading the user can navigate through the document
//! by using one of the navigation functions:
//! /see gotoChild();
//! /see gotoSibling();
//! /see gotoParent();
//! Values can be retrieved by using the getValue() functions.
//! XML reminder: XML-element = <tag attribute="value"> XML-element [XML-element]* </tag>
//!                             <tag attribute="value"> content </tag>
class XMLParser
{
public:
	/** @name CONSTRUCTORS / DESTRUCTORS
	*/
	//@{
	XMLPARSERAPI XMLParser();
	XMLPARSERAPI virtual ~XMLParser();
	//@}

	/** @name ACCESSORS
	*/
	//@{

	//! get the current tag.
	//! remarks: It may happen that you will get a dummy tag, the form: "#text".
	//!          In this case, do not worry, use gotoSibling() and try again.
	//!          XERCES is responsible for that.  
	//! /return current tag or an empty string if a document is not loaded first.
	XMLPARSERAPI std::string getCurrentTag();

	//! get content to the current tag.
	//! /param sstrOutContent textual content to the current tag.
	//! /return true if content could retrieved successfully or false if:
	//! -the current tag has no content.
	//! -a XML document was not loaded in the first place.
	XMLPARSERAPI bool getContent( std::string &sstrOutContent );


	//! get content for a child tag of the current tag.
	//! /param sstrTag name of the child tag. 
	//! /param sstrOutContent textual content to the current tag.
	//! /return true if content could retrieved successfully or false if:
	//! -the tag could not be found in the current context.
	//! -the tag has no content.
	//! -a XML document was not loaded in the first place.
	//! XML structures with identical tag names on the same level should not use this convenience function, example:
	//! <CURRENT TAG>
	//!  <CHILD>content a</CHILD>
	//!  <CHILD>content b</CHILD>
	//! </CURRENT TAG>
	//! in this case only the content of the first first child is retrieved.
	XMLPARSERAPI bool getChildContent( const std::string &sstrTag, std::string &sstrOutContent );


	//! get the value to the current tag and a given attribute.
	//! /param sstrAttribute name of the attribute.
	//! /param sstrOutValue returns the value or an empty string if the value is not found.
	//! /return true if the value could retrieved successfully or false if:
	//! -the tag attribute combination could not be found in the current context.
	//! -a XML document was not loaded in the first place.
	XMLPARSERAPI bool getValue( const std::string &sstrAttribute, std::string &sstrOutValue );

	//! get the value to the current tag and a given attribute.
	//! /param sstrAttribute name of the attribute.
	//! /param nOutValue returns the value as unsigned integer. The output value is undefined if the function returns false.
	//! /return true if the value could retrieved successfully or false if:
	//! -the tag attribute combination could not be found in the current context.
	//! -a XML document was not loaded in the first place.
	XMLPARSERAPI bool getValue( const std::string &sstrAttribute, int &nOutValue );

	//! get the value to the current tag and a given attribute.
	//! /param sstrAttribute name of the attribute.
	//! /param fOutValue returns the value as float. The output value is undefined if the function returns false.
	//! /return true if the value could retrieved successfully or false if:
	//! -the tag attribute combination could not be found in the current context.
	//! -a XML document was not loaded in the first place.
	XMLPARSERAPI bool getValue( const std::string &sstrAttribute, float &fOutValue );


	//! get the value for a child tag of the current tag.
	//! Values are retrieved for the first child element with the given tag name.
	//! /param sstrTag name of the child tag. 
	//! /param sstrAttribute name of the attribute.
	//! /param sstrOutValue returns the value or an empty string if the value is not found.
	//! /return true if the value could retrieved successfully or false if:
	//! -the tag attribute combination could not be found in the current context.
	//! -a XML document was not loaded in the first place.
	//! XML structures with identical tag names on the same level should not use this convenience function, example:
	//! <CURRENT TAG>
	//!  <CHILD AttribName1="Value1"/>
	//!  <CHILD AttribName2="Value2"/>
	//! </CURRENT TAG>
	//! in this case only the first child is considered.
	XMLPARSERAPI bool getChildValue( const std::string &sstrTag, 
										const std::string &sstrAttribute,
										std::string &sstrOutValue );



	//! get the value as unsigned integer for a child tag of the current tag.
	//! Values are retrieved for the first child element with the given tag name.
	//! /param sstrTag name of the child tag. 
	//! /param sstrAttribute name of the attribute.
	//! /param nOutValue returns the value as unsigned integer. The output value is undefined if the function returns false.
	//! /return true if the value could retrieved successfully or false if:
	//! -the tag attribute combination could not be found in the current context.
	//! -a XML document was not loaded in the first place.
	//! XML structures with identical tag names on the same level should not use this convenience function, example:
	//! <CURRENT TAG>
	//!  <CHILD AttribName1="Value1"/>
	//!  <CHILD AttribName2="Value2"/>
	//! </CURRENT TAG>
	//! in this case only the first child is considered.
	XMLPARSERAPI bool getChildValue( const std::string &sstrTag, 
										const std::string &sstrAttribute,
										size_t &nOutValue );



	//! get the value as float for a child tag of the current tag.
	//! Values are retrieved for the first child element with the given tag name.
	//! /param sstrTag name of the child tag. 
	//! /param sstrAttribute name of the attribute.
	//! /param fOutValue returns the value as float. The output value is undefined if the function returns false.
	//! /return true if the value could retrieved successfully or false if:
	//! -the tag attribute combination could not be found in the current context.
	//! -a XML document was not loaded in the first place.
	//! XML structures with identical tag names on the same level should not use this convenience function, example:
	//! <CURRENT TAG>
	//!  <CHILD AttribName1="Value1"/>
	//!  <CHILD AttribName2="Value2"/>
	//! </CURRENT TAG>
	//! in this case only the first child is considered.
	XMLPARSERAPI bool getChildValue( const std::string &sstrTag, 
										const std::string &sstrAttribute,
										float &fOutValue );

	//! Get the parser error log. 
	//! This log contains information which attributes or values could not be found for a tag.
	XMLPARSERAPI std::string getParserErrorLog();
	//@}

	/** @name MODIFIERS
	*/
	//@{

	//! go to child element.
	//! \return true if a child element exists, otherwise false.
	XMLPARSERAPI bool gotoChild();

	//! go to the next sibling.
	//! \return true if a sibling exists, otherwise false.
	XMLPARSERAPI bool gotoSibling();

	//! go to the parent element.
	//! returns false if no parent exists or an XML was not loaded.
	XMLPARSERAPI bool gotoParent();

	//! loads a XML document from memory
	//! /param sstrXML a XML document
	//! /return true if the document was successfully loaded, otherwise false.
	XMLPARSERAPI bool loadXMLFromMemory( const std::string &sstrXML );

	//! loads a XML document from file.
	//! /param sstrFilename filename to a XML file.
	//! /return true if the document was successfully loaded, otherwise false.
	XMLPARSERAPI bool loadXMLFromFile( const std::string &sstrFilename );

	//@}

protected:
private:

	//! instance of the parser implementation.
	//! will be created on the first load.
	XMLParserImpl *m_pParserImpl;
};


#endif // __XMLPARSER_H__