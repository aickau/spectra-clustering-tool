#include "AFADefines.h"
#include "AFATypes.h"

bool_t
AFATypesOK()
{
	bool_t rv = TRUE;
    rv &= ( 1 == sizeof( sint8_t  )) ? TRUE : FALSE;
    rv &= ( 2 == sizeof( sint16_t )) ? TRUE : FALSE;
    rv &= ( 4 == sizeof( sint32_t )) ? TRUE : FALSE;
    rv &= ( 8 == sizeof( sint64_t )) ? TRUE : FALSE;

    if ( !rv )
    {
    	printf( "Error: Types not ok!\n" );
		printf( "==> Cannot continue - please fix it in AFATypes.h !\n" );
    	exit( 1 );
    }

    return rv;
}
