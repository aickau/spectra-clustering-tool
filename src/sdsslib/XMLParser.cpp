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
//! \file  XMLParser.cpp
//! \brief 

#include "XMLParser.h"
#include "XMLParserImpl.h"


XMLParser::XMLParser()
:m_pParserImpl(NULL)
{
}

XMLParser::~XMLParser()
{
	if ( m_pParserImpl != NULL )
	{
		delete m_pParserImpl;
	}
	m_pParserImpl = NULL;
}


bool XMLParser::loadXMLFromMemory( const std::string &sstrXML )
{
	if ( m_pParserImpl == NULL )
	{
		m_pParserImpl = new XMLParserImpl();
	}

	return m_pParserImpl->loadXMLFromMemory( sstrXML );
}


bool XMLParser::loadXMLFromFile( const std::string &sstrFilename )
{
	if ( m_pParserImpl == NULL )
	{
		m_pParserImpl = new XMLParserImpl();
	}

	return m_pParserImpl->loadXMLFromFile( sstrFilename );
}


std::string XMLParser::getParserErrorLog()
{
	if ( m_pParserImpl == NULL )
	{
		return "";
	}

	return m_pParserImpl->getParserErrorLog();
}


std::string XMLParser::getCurrentTag()
{
	if ( m_pParserImpl == NULL )
	{
		return "";
	}

	return m_pParserImpl->getCurrentTag();
}


bool XMLParser::getContent( std::string &sstrOutContent )
{
	if ( m_pParserImpl == NULL )
	{
		return false;
	}

	return m_pParserImpl->getContent( sstrOutContent );
}


bool XMLParser::getChildContent( const std::string &sstrTag, std::string &sstrOutContent )
{
	if ( m_pParserImpl == NULL )
	{
		return false;
	}

	return m_pParserImpl->getChildContent( sstrTag, sstrOutContent );
}


bool XMLParser::getValue( const std::string &sstrAttribute,
							std::string &sstrOutValue )
{
	if ( m_pParserImpl == NULL )
	{
		return false;
	}

	return m_pParserImpl->getValue( sstrAttribute, sstrOutValue );
}


bool XMLParser::getValue( const std::string &sstrAttribute,
							int &nOutValue )
{
	if ( m_pParserImpl == NULL )
	{
		return false;
	}

	return m_pParserImpl->getValue( sstrAttribute, nOutValue );
}


bool XMLParser::getValue( const std::string &sstrAttribute,
							float &fOutValue )
{
	if ( m_pParserImpl == NULL )
	{
		return false;
	}

	return m_pParserImpl->getValue( sstrAttribute, fOutValue );
}



bool XMLParser::getChildValue( const std::string &sstrTag, 
								const std::string &sstrAttribute,
								std::string &sstrOutValue )
{
	if ( m_pParserImpl == NULL )
	{
		return false;
	}

	return m_pParserImpl->getChildValue( sstrTag, sstrAttribute, sstrOutValue );
}


bool XMLParser::getChildValue( const std::string &sstrTag, 
								const std::string &sstrAttribute,
								size_t &nOutValue )
{
	if ( m_pParserImpl == NULL )
	{
		return false;
	}

	return m_pParserImpl->getChildValue( sstrTag, sstrAttribute, nOutValue );
}


bool XMLParser::getChildValue( const std::string &sstrTag, 
								const std::string &sstrAttribute,
								float &fOutValue )
{
	if ( m_pParserImpl == NULL )
	{
		return false;
	}

	return m_pParserImpl->getChildValue( sstrTag, sstrAttribute, fOutValue );
}


bool XMLParser::gotoChild()
{
	if ( m_pParserImpl == NULL )
	{
		return false;
	}

	return m_pParserImpl->gotoChild();
}

bool XMLParser::gotoParent()
{
	if ( m_pParserImpl == NULL )
	{
		return false;
	}

	return m_pParserImpl->gotoParent();
}

bool XMLParser::gotoSibling()
{
	if ( m_pParserImpl == NULL )
	{
		return false;
	}

	return m_pParserImpl->gotoSibling();
}





