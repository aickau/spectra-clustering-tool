//! \verbatim
//! ###########################################################################
//! # ASPECT: A spectra clustering tool for exploration of large spectral surveys - Stage I
//! #
//! # Copyright (c) 2009 Aick in der Au
//! # All rights reserved.
//! ###########################################################################
//!
//!      created by : Aick in der Au <aick.inderau@gmail.com>
//!      created on : 1/21/2009
//! additional docs : none
//!  responsibility : 1. Aick in der Au
//!                   2. 
//! \endverbatim
//!
//! \file  HTMLExport.cpp
//! \brief helper functions for valid html export.

#include "HTMLExport.h"

#include <assert.h>
#include <fstream>


namespace HTMLExport
{


std::string loadHTMLTemplate()
{
	std::string sstrDefaultHTMLDocTemplate("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n");
	sstrDefaultHTMLDocTemplate += "<html>\n";
	sstrDefaultHTMLDocTemplate += "<head>\n";
	sstrDefaultHTMLDocTemplate += "<title>"+HTML_TOKEN_TITLE+"</title>\n";
	sstrDefaultHTMLDocTemplate += "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n";
	sstrDefaultHTMLDocTemplate += "</head>\n";
	sstrDefaultHTMLDocTemplate += "<body link=\"white\" alink=\"red\" vlink=\"#009010\">\n";
	sstrDefaultHTMLDocTemplate += HTML_TOKEN_INFO+"\n";
	sstrDefaultHTMLDocTemplate += "<table width=\"2\" cellspacing=\"0\" cellpadding=\"0\" border=\"0\" align=\"center\">\n";
	sstrDefaultHTMLDocTemplate += " <caption>*"+HTML_TOKEN_TITLE+"</caption>\n";
	sstrDefaultHTMLDocTemplate += "<tbody>\n";
	sstrDefaultHTMLDocTemplate += HTML_TOKEN_TEMPLATE+"\n";
	sstrDefaultHTMLDocTemplate += "</tbody>\n";
	sstrDefaultHTMLDocTemplate += "</table>\n";
	sstrDefaultHTMLDocTemplate += "</body>\n";
	sstrDefaultHTMLDocTemplate += "</html\n";

	const std::string sstrTemplateFileName("template.html");

	// load template
	std::string sstrTemp;
	std::string sstrHTMLDocTemplate;

	std::ifstream fin(sstrTemplateFileName.c_str());
	if( !fin ) 
	{
		assert(0); // missing template file, taking default template.
		return sstrDefaultHTMLDocTemplate;
	}
	while( getline(fin,sstrTemp) ) 
	{
		sstrHTMLDocTemplate += sstrTemp;
	}
	return sstrHTMLDocTemplate;
}


std::string image(const std::string &_sstrImage )
{
	std::string sstrImage( "<img src=\"" );
	sstrImage += _sstrImage;
	sstrImage += "\"/>\n"; 
	return sstrImage;
}


std::string textLink(const std::string &_sstrText, const std::string &_sstrLink )
{
	std::string sstrImageLink( "<a href=\"" );
	sstrImageLink += _sstrLink;
	sstrImageLink += "\" target=\"_blank\">";
	sstrImageLink += _sstrText;
	sstrImageLink += "</a>\n";

	return sstrImageLink;
}




std::string imageLink(const std::string &_sstrImage, const std::string &_sstrLink, bool _bSelf, int _borderSize )
{
	std::string sstrImageLink( "<a href=\"" );
	sstrImageLink += _sstrLink;
	if ( _bSelf )
	{
		sstrImageLink += "\" target=\"_self\">";
	}
	else
	{
		sstrImageLink += "\" target=\"_blank\">";
	}
	sstrImageLink += "<img ";
	if ( _borderSize > 0 )
		sstrImageLink += "border=\"4\" ";
	sstrImageLink += "src=\"";
	sstrImageLink += _sstrImage;
	sstrImageLink += "\"/>";
	sstrImageLink += "</a>\n";

	return sstrImageLink;
}

std::string beginTable()
{
	return  std::string("<table width=\"200\" height=\"200\" cellspacing=\"0\" cellpadding=\"0\" border=\"0\" align=\"center\">\n");
}

std::string endTable()
{
	return  std::string("</table>\n");
}

std::string beginTableRow()
{
	return  std::string("<tr>\n");
}

std::string endTableRow()
{
	return  std::string("</tr>\n");
}

std::string beginTableCell()
{
	return  std::string("<td>\n");
}

std::string endTableCell()
{
	return  std::string("</td>\n");
}

std::string lineBreak()
{
	return  std::string("<br>\n");
}

};


