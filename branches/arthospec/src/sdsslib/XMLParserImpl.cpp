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
//! \file  XMLParserImpl.cpp
//! \brief 

#include "XMLParserImpl.h"
#include <sstream>

#define TIXML_USE_STL
#include "tinyxml/tinyxml.h"



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
	m_pCurrentDocument = new TiXmlDocument( std::string("fromMemory") );
	m_pCurrentDocument->Parse( sstrXML.c_str() );

	m_sstrParseErrLog = m_pCurrentDocument->ErrorDesc();
	m_pCurrentNode = m_pCurrentDocument->RootElement();

	return !m_pCurrentDocument->Error();
}



bool XMLParserImpl::loadXMLFromFile( const std::string &sstrFilename )
{
	m_sstrParseErrLog="";

	if ( m_pCurrentDocument != NULL )
	{
		delete m_pCurrentDocument;
	}
	m_pCurrentDocument = new TiXmlDocument( sstrFilename );
	m_pCurrentDocument->LoadFile();

	m_sstrParseErrLog = m_pCurrentDocument->ErrorDesc();

	m_pCurrentNode = m_pCurrentDocument->RootElement();

	return !m_pCurrentDocument->Error();
}

std::string XMLParserImpl::getParserErrorLog()
{
	return m_sstrParseErrLog;
}




bool XMLParserImpl::gotoChild()
{
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
}


bool XMLParserImpl::gotoParent()
{
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
}


bool XMLParserImpl::gotoSibling()
{
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
}



std::string XMLParserImpl::getCurrentTag()
{
	if ( m_pCurrentNode == NULL )
	{
		m_sstrParseErrLog += "getCurrentTag():  No current node available.\n";
		return "";
	}

	return m_pCurrentNode->Value();
}



bool XMLParserImpl::getContent( std::string &sstrOutContent )
{
	if ( m_pCurrentNode == NULL )
	{
		m_sstrParseErrLog += "getContent(): No current node available.\n";
		return false;
	}

	// migtht be wrong
	sstrOutContent = m_pCurrentNode->GetText();
	
	return true;
}


bool XMLParserImpl::getChildContent( const std::string &sstrTag, std::string &sstrOutContent )
{
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
}



bool XMLParserImpl::getValue( const std::string &sstrAttribute, std::string &sstrOutValue )
{
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
}


bool XMLParserImpl::getValue( const std::string &sstrAttribute, int &nOutValue )
{
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
}


bool XMLParserImpl::getValue( const std::string &sstrAttribute, float &fOutValue )
{
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
}


bool XMLParserImpl::getChildValue( const std::string &sstrTag, 
			                         const std::string &sstrAttribute,
		                             std::string &sstrOutValue )
{
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
}


bool XMLParserImpl::getChildValue( const std::string &sstrTag, 
                                     const std::string &sstrAttribute,
                                     size_t &nOutValue )
{
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
}


bool XMLParserImpl::getChildValue( const std::string &sstrTag, 
                                     const std::string &sstrAttribute,
                                     float &fOutValue )
{
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
}





