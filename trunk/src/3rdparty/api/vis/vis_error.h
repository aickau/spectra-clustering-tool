#ifndef _VIS_ERROR_H
#define _VIS_ERROR_H

#include "viperror.h"

/*
use this with:  __FILE__ and __LINE__
				see winerror.h for HRESULT details
*/

/**
 usage: VISTHROW_MSG(ERROR_ID) where ERROR_ID is an id in the stringtable
 with a corresponding error message text.
*/
#define VISTHROW_MSG( errorCode )	{CString _errormsg; _errormsg.LoadString(errorCode); throw CVisError( (errorCode) , __LINE__ , __FILE__ , (_errormsg) );}

/// only wraps CVIPError
class CVisError:public CVIPError
{
public:
	CVisError();
	CVisError(HRESULT errorCode , int line , const char* file , const char* message );
	CVisError( const CVIPError& src );
	CVisError& operator = ( const CVisError& src );
};

#endif
