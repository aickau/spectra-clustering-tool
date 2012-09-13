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
//! \file  XMLParserAPI.h
//! \brief 

#ifndef __XMLPARSERAPI_H__
#define __XMLPARSERAPI_H__

#ifdef _XMLDLL

#ifdef _WIN32

// microsoft specific storage-class defines
# ifdef XMLPARSER_EXPORTS
#  define XMLPARSERAPI __declspec(dllexport)
#  define EXPIMP_TEMPLATE_XMLPARSERAPI
# else
#  define XMLPARSERAPI __declspec(dllimport)
#  define EXPIMP_TEMPLATE_XMLPARSERAPI extern
# endif

#else

# define XMLPARSERAPI
# define EXPIMP_TEMPLATE_XMLPARSERAPI
#endif

#else
# define XMLPARSERAPI
#endif


#endif // __XMLPARSERAPI_H__