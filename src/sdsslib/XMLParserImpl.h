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
//! \file  XMLParserImpl.h
//! \brief 

#ifndef __XMLPARSERIMPL_H__
#define __XMLPARSERIMPL_H__

#include <string>

class TiXmlDocument;
class TiXmlElement;


//! implementation class for class XMLParser.
//! Do not use this class directly. Use XMLParser instead.
class XMLParserImpl
{
public:

	/** @name CONSTRUCTORS / DESTRUCTORS
	*/
	//@{
	XMLParserImpl();
	virtual ~XMLParserImpl();
	//@}

	/** @name ACCESSORS
	*/
	//@{

	//! \see class XMLParser for description!

	std::string getCurrentTag();

	bool getContent( std::string &sstrOutContent );
	bool getChildContent( const std::string &sstrTag, std::string &sstrOutContent );

	bool getValue( const std::string &sstrAttribute, std::string &sstrOutValue );
	bool getValue( const std::string &sstrAttribute, int &nOutValue );
	bool getValue( const std::string &sstrAttribute, float &fOutValue );

	bool getChildValue( const std::string &sstrTag, 
						const std::string &sstrAttribute,
						std::string &sstrOutValue );

	bool getChildValue( const std::string &sstrTag, 
						const std::string &sstrAttribute,
						size_t &nOutValue );


	bool getChildValue( const std::string &sstrTag, 
						const std::string &sstrAttribute,
						float &fOutValue );
	
	std::string getParserErrorLog();

	//@}

	/** @name MODIFIERS
	*/
	//@{
	bool loadXMLFromMemory( const std::string &sstrXML );
	bool loadXMLFromFile( const std::string &sstrFilename );

	bool gotoChild();
	bool gotoSibling();
	bool gotoParent();
	//@}

private:

	//! Last accessed XML Tag for caching XML nodes.
	std::string m_sstrLastXMLTag;

	//! XML parser error log
	std::string m_sstrParseErrLog;

	//! our tiny XML document
	TiXmlDocument *m_pCurrentDocument;

	TiXmlElement *m_pCurrentNode;

};



#endif // __XMLPARSERIMPL_H__