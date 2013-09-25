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
//! \file  debug.cpp
//! \brief 

#include "debug.h"


#include <stdio.h>

#ifdef _WIN32
#include <conio.h>
#endif 

#ifdef _DEBUG
#ifdef _WIN32
int DoAssert( const char *szcond, int line, const char *szfile, const char *msg ) {
  char temp[4096];
  if ( msg ) wsprintf ( temp, "%s\n\nCondition: %s\nFile: %s\nLine: %i\n\nAbort: Quit, Retry: Break", msg, szcond, szfile, line );
  else wsprintf ( temp, "Condition: %s\nFile: %s\nLine: %i\n\nAbort: Quit, Retry: Break", szcond, szfile, line );

  int r = MessageBox ( NULL, temp, "Assert failed", MB_ABORTRETRYIGNORE | MB_ICONERROR );

  if (r==IDABORT) {
    ExitProcess(1);
    return ASSERT_QUIT;
  }
  if (r==IDRETRY) 
    return ASSERT_BREAK;  

  return ASSERT_IGNORE;
}
#endif
#endif



 
void Debug::PrintConsole(const char *text , ...) 
{
#ifdef _WIN32
	if ( text == NULL )
		return;

	// fake goodness test but buffer overflow is still possible
	ASSERT( strlen(text)< 4096 );

	char		text2[8192];						
	va_list		ap;										

	va_start(ap, text);								
	vsprintf(text2, text, ap);						
	va_end(ap);	
	_cprintf(text2);
#endif
}
