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
//! \file  XMLParserImpl.cpp
//! \brief 

#include "XMLParserImpl.h"
#include <sstream>

#ifdef __linux
#define DISABLE_XML_PARSER
#endif

#ifndef DISABLE_XML_PARSER

#define TIXML_USE_STL
#include "tinyxml/tinyxml.h"

#endif // DISABLE_XML_PARSER


XMLParserImpl::XMLParserImpl()
:m_pCurrentDocument(NULL)
,m_pCurrentNode(NULL)
,m_sstrParseErrLog("")
{	
}

XMLParserImpl::~XMLParserImpl()
{
}


bool XMLParserImpl::loadXMLFromMemory( const std::string &sstrXML )
{
#ifdef DISABLE_XML_PARSER
	return false;
#else
	m_pCurrentDocument = new TiXmlDocument( std::string("fromMemory") );
	m_pCurrentDocument->Parse( sstrXML.c_str() );

	m_sstrParseErrLog = m_pCurrentDocument->ErrorDesc();
	m_pCurrentNode = m_pCurrentDocument->RootElement();

	return !m_pCurrentDocument->Error();
#endif
}



bool XMLParserImpl::loadXMLFromFile( const std::string &sstrFilename )
{
#ifdef DISABLE_XML_PARSER
	return false;
#else	m_sstrParseErrLog="";

	if ( m_pCurrentDocument != NULL )
	{
		delete m_pCurrentDocument;
	}
	m_pCurrentDocument = new TiXmlDocument( sstrFilename );
	m_pCurrentDocument->LoadFile();

	m_sstrParseErrLog = m_pCurrentDocument->ErrorDesc();

	m_pCurrentNode = m_pCurrentDocument->RootElement();

	return !m_pCurrentDocument->Error();
#endif
}

std::string XMLParserImpl::getParserErrorLog()
{
	return m_sstrParseErrLog;
}




bool XMLParserImpl::gotoChild()
{
#ifdef DISABLE_XML_PARSER
	return false;
#else	
	if ( m_pCurrentNode == NULL )
	{
		m_sstrParseErrLog += "gotoChild(): No current node available.\n";
		return false;
	}

	TiXmlHandle hCurrentNode( m_pCurrentNode); 
	TiXmlElement *pChild = hCurrentNode.ChildElement(0).ToElement();

	if ( pChild != NULL )
	{
		m_pCurrentNode = pChild;
		return true;
	}
	
	return false;
#endif
}


bool XMLParserImpl::gotoParent()
{
#ifdef DISABLE_XML_PARSER
	return false;
#else	
	if ( m_pCurrentNode == NULL )
	{
		m_sstrParseErrLog += "gotoChild(): No current node available.\n";
		return false;
	}

	TiXmlNode *pParentNode = m_pCurrentNode->Parent();
	if ( pParentNode != NULL )
	{
		if (pParentNode->Type() == TiXmlNode::ELEMENT) 
		{
			m_pCurrentNode = static_cast<TiXmlElement*>(pParentNode);
			return true;
		}
	}
	return false;
#endif
}


bool XMLParserImpl::gotoSibling()
{
#ifdef DISABLE_XML_PARSER
	return false;
#else	
	if ( m_pCurrentNode == NULL )
	{
		m_sstrParseErrLog += "gotoSibling(): No current node available.\n";
		return false;
	}

	TiXmlElement *pSiblingNode = m_pCurrentNode->NextSiblingElement();
	if ( pSiblingNode != NULL )
	{
		m_pCurrentNode = pSiblingNode;
		return true;
	}

	return false;
#endif
}



std::string XMLParserImpl::getCurrentTag()
{
#ifdef DISABLE_XML_PARSER
	return std::string("");
#else		
	if ( m_pCurrentNode == NULL )
	{
		m_sstrParseErrLog += "getCurrentTag():  No current node available.\n";
		return "";
	}

	return m_pCurrentNode->Value();
#endif
}



bool XMLParserImpl::getContent( std::string &sstrOutContent )
{
#ifdef DISABLE_XML_PARSER
	return false;
#else	
	if ( m_pCurrentNode == NULL )
	{
		m_sstrParseErrLog += "getContent(): No current node available.\n";
		return false;
	}

	// migtht be wrong
	sstrOutContent = m_pCurrentNode->GetText();
	
	return true;
#endif
}


bool XMLParserImpl::getChildContent( const std::string &sstrTag, std::string &sstrOutContent )
{
#ifdef DISABLE_XML_PARSER
	return false;
#else	
	if ( m_pCurrentNode == NULL )
	{
		m_sstrParseErrLog += "getChildContent(): No current node available.\n";
		return false;
	}

	TiXmlHandle hCurrentNode( m_pCurrentNode); 
	TiXmlElement *pChild = hCurrentNode.FirstChildElement(sstrTag.c_str()).ToElement();

	if ( pChild == NULL )
	{
		m_sstrParseErrLog += "getChildContent(): XML tag:<" + sstrTag +"> not found in current context.\n";
		return false;
	}

	// migtht be wrong
	sstrOutContent = pChild->GetText();

	return true;
#endif
}



bool XMLParserImpl::getValue( const std::string &sstrAttribute, std::string &sstrOutValue )
{
#ifdef DISABLE_XML_PARSER
	return false;
#else	
	if ( m_pCurrentNode == NULL )
	{
		m_sstrParseErrLog += "getValue(): No current node available.\n";
		return false;
	}

	const std::string *psstrValue = m_pCurrentNode->Attribute(sstrAttribute);
	if ( psstrValue != NULL )
	{
		sstrOutValue = *psstrValue;
		return true;
	}

	return false;
#endif
}


bool XMLParserImpl::getValue( const std::string &sstrAttribute, int &nOutValue )
{
#ifdef DISABLE_XML_PARSER
	return false;
#else	
	if ( m_pCurrentNode == NULL )
	{
		m_sstrParseErrLog += "getValue(): No current node available.\n";
		return false;
	}

	int tempValue=0;
	int retVal = m_pCurrentNode->QueryIntAttribute(sstrAttribute, &tempValue);
	if ( retVal == TIXML_SUCCESS )
	{
		nOutValue = tempValue;
		return true;
	}

	return false;
#endif
}


bool XMLParserImpl::getValue( const std::string &sstrAttribute, float &fOutValue )
{
#ifdef DISABLE_XML_PARSER
	return false;
#else	
	if ( m_pCurrentNode == NULL )
	{
		m_sstrParseErrLog += "getValue(): No current node available.\n";
		return false;
	}

	float tempValue=0;
	int retVal = m_pCurrentNode->QueryFloatAttribute(sstrAttribute.c_str(), &tempValue);
	if ( retVal == TIXML_SUCCESS )
	{
		fOutValue = tempValue;
		return true;
	}

	return false;
#endif
}


bool XMLParserImpl::getChildValue( const std::string &sstrTag, 
			                         const std::string &sstrAttribute,
		                             std::string &sstrOutValue )
{
#ifdef DISABLE_XML_PARSER
	return false;
#else	
	if ( m_pCurrentNode == NULL )
	{
		m_sstrParseErrLog += "getValue(): No current node available.\n";
		return false;
	}

	TiXmlHandle hCurrentNode( m_pCurrentNode); 
	TiXmlElement *pChild = hCurrentNode.FirstChildElement(sstrTag.c_str()).ToElement();
	if ( pChild == NULL )
	{
		m_sstrParseErrLog += "getChildValue(): XML tag:<" + sstrTag +"> not found in current context.\n";
		return false;
	}

	const std::string *psstrValue = pChild->Attribute(sstrAttribute);
	if ( psstrValue != NULL )
	{
		sstrOutValue = *psstrValue;
		return true;
	}

	return false;
#endif
}


bool XMLParserImpl::getChildValue( const std::string &sstrTag, 
                                     const std::string &sstrAttribute,
                                     size_t &nOutValue )
{
#ifdef DISABLE_XML_PARSER
	return false;
#else	
	if ( m_pCurrentNode == NULL )
	{
		m_sstrParseErrLog += "getValue(): No current node available.\n";
		return false;
	}

	TiXmlHandle hCurrentNode( m_pCurrentNode); 
	TiXmlElement *pChild = hCurrentNode.FirstChildElement(sstrTag.c_str()).ToElement();
	if ( pChild == NULL )
	{
		m_sstrParseErrLog += "getChildValue(): XML tag:<" + sstrTag +"> not found in current context.\n";
		return false;
	}

	int tempValue=0;
	int retVal = pChild->QueryIntAttribute(sstrAttribute, &tempValue);
	if ( retVal == TIXML_SUCCESS )
	{
		nOutValue = tempValue;
		return true;
	}

	return false;
#endif
}


bool XMLParserImpl::getChildValue( const std::string &sstrTag, 
                                     const std::string &sstrAttribute,
                                     float &fOutValue )
{
#ifdef DISABLE_XML_PARSER
	return false;
#else	
	if ( m_pCurrentNode == NULL )
	{
		m_sstrParseErrLog += "getValue(): No current node available.\n";
		return false;
	}

	TiXmlHandle hCurrentNode( m_pCurrentNode); 
	TiXmlElement *pChild = hCurrentNode.FirstChildElement(sstrTag.c_str()).ToElement();

	if ( pChild == NULL )
	{
		m_sstrParseErrLog += "getChildValue(): XML tag:<" + sstrTag +"> not found in current context.\n";
		return false;
	}

	float tempValue=0;
	int retVal = pChild->QueryFloatAttribute(sstrAttribute.c_str(), &tempValue);
	if ( retVal == TIXML_SUCCESS )
	{
		fOutValue = tempValue;
		return true;
	}

	return false;
#endif
}





