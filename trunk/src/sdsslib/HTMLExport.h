//! \verbatim
//! ###########################################################################
//! # SDSS sorting prototype - Stage I
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
//! \file  HTMLExport.h
//! \brief helper functions for valid html export.

#ifndef __HTMLEXPORT_H__
#define __HTMLEXPORT_H__

#include <string>


namespace HTMLExport
{
	// tokens for html template
	static const std::string HTML_TOKEN_INFO("*INFO*");
	static const std::string HTML_TOKEN_TITLE("*TITLE*");
	static const std::string HTML_TOKEN_TEMPLATE("*TEMPLATE*");

	// load HTML template containing tokens. see HTML_TOKEN_*
	std::string loadHTMLTemplate();

	std::string image(const std::string &_sstrImage );
	std::string textLink(const std::string &_sstrText, const std::string &_sstrLink );
	std::string imageLink(const std::string &_sstrImage, const std::string &_sstrLink );

	std::string beginTable();
	std::string endTable();
	std::string beginTableRow();
	std::string endTableRow();
	std::string beginTableCell();
	std::string endTableCell();

	std::string lineBreak();

};

#endif // __HTMLEXPORT_H__