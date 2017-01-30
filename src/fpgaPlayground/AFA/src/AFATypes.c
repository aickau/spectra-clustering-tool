#include "AFADefines.h"
#include "AFATypes.h"

#include <stdio.h>
#include <stdlib.h>

bool_t
AFATypesOK()
{
    bool_t rv = TRUE;
    uint32_t sizeType = 1;

    do
    {
    	switch ( sizeType )
    	{
			case 1:
				rv = ( 1 == sizeof( sint8_t  )) ? TRUE : FALSE;
				break;
			case 2:
				rv = ( 2 == sizeof( sint16_t )) ? TRUE : FALSE;
				break;
			case 4:
				rv = ( 4 == sizeof( sint32_t )) ? TRUE : FALSE;
				break;
			case 8:
				rv = ( 8 == sizeof( sint64_t )) ? TRUE : FALSE;
				break;
    		default:
    			rv = FALSE;
    			break;
    	}
        if ( rv )
		{
			sizeType <<= 1;
		}
    } while ( rv );

    if (( !rv ) && ( sizeType < 16 ))
    {
        printf( "Error: Types not ok! Size=%ld\n", sizeType );
		printf( "\t* sizeof( sint8_t  ) = %d\n", sizeof( sint8_t  ));
		printf( "\t* sizeof( sint16_t ) = %d\n", sizeof( sint16_t ));
		printf( "\t* sizeof( sint32_t ) = %d\n", sizeof( sint32_t ));
		printf( "\t* sizeof( sint64_t ) = %d\n", sizeof( sint64_t ));
        printf( "==> Cannot continue - please fix it in AFATypes.h !\n" );
        exit( 1 );
    }
	else
	{
		if ( sizeof( void * ) != sizeof( addr_t ))
		{
	        printf( "Error: Type 'addr_t' is not ok! currently set to '%s', change to '%s'\n",
				( 4 == sizeof( void * ) ? "typedef uint64_t addr_t;" : "typedef uint32_t addr_t;" ),
				( 4 == sizeof( void * ) ? "typedef uint32_t addr_t;" : "typedef uint64_t addr_t;" ));
	        rv = FALSE;
		}
		else
		{
			rv = TRUE;
		}
	}

    return rv;
}
