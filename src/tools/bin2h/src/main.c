// Jesko Schwarzer, (C) 10-10-2015 Continental
// ===========================================

#include <stdio.h>
#include <stdlib.h>

#define SAL_FALSE										( 0 )
#define SAL_TRUE										( 1 )

#define JS_BIN2H_VALUES_PER_LINE			        	( 256 )
#define JS_BIN2H_ADDRESS_OPTION_ON

typedef struct
{
	size_t valuesPerLine;
	int flagAddressOutput;
    size_t maxBinaryChunkSize;
    size_t addressOffset;
} options_t;


void
DataWriteHeader(
	options_t *options,
	FILE *fileOutput,
	const char *nameOfDataSet,
	const void *dataPtr,
	const size_t dataSize )
{
	size_t valuesPerLine;
	size_t numLines;
	size_t i, j;
	unsigned char *p;
	
	fprintf( fileOutput, "// ****************************************************************************\n" );
	fprintf( fileOutput, "// **                                                                        **\n" );
	fprintf( fileOutput, "// ** bin2h generated file                                                   **\n" );
	fprintf( fileOutput, "// **                                                                        **\n" );
	fprintf( fileOutput, "// ****************************************************************************\n" );
	fprintf( fileOutput, "\n" );
	fprintf( fileOutput, "\n" );
	fprintf( fileOutput, "const unsigned long %s_size = %d; // 0x%8.8x\n", nameOfDataSet, dataSize, dataSize );
	fprintf( fileOutput, "const unsigned char %s_data[ %d ] =\n", nameOfDataSet, dataSize );
	fprintf( fileOutput, "{\n" );

	numLines = dataSize / options->valuesPerLine;
	p = ( unsigned char * )dataPtr;
	
	for ( i = 0; i < numLines; i++ )
	{
		if ( options->flagAddressOutput )
		{
			fprintf( fileOutput, "/* 0x%8.8x */ ", i * options->valuesPerLine + options->addressOffset );
		}
		else
		{
			fprintf( fileOutput, "    " );
		}

		for ( j = 0; j < options->valuesPerLine - 1; ++j )
		{
			fprintf( fileOutput, "0x%2.2x,", *p++ );
		}
		fprintf( fileOutput, "0x%2.2x,\n", *p++ );
	}
    fflush( fileOutput );

	valuesPerLine = dataSize - numLines * options->valuesPerLine;
	if ( valuesPerLine > 0 )
	{
		// write PARTIAL line
		if ( options->flagAddressOutput )
		{
			fprintf( fileOutput, "/* 0x%8.8x */ ", i * options->valuesPerLine + options->addressOffset );
		}
		else
		{
			fprintf( fileOutput, "    " );
		}

		for ( j = 0; j < valuesPerLine - 1; ++j )
		{
			fprintf( fileOutput, "0x%2.2x,", *p++ );
		}
		fprintf( fileOutput, "0x%2.2x\n", *p++ );
	}

	fprintf( fileOutput, "};\n" );
}

int
DataReadFile(
	options_t *options,
	char *inputFilename,
	char *headerFilename,
	char *arrayName )
{
	FILE *inputFileHandle;
	FILE *headerFileHandle;

	void *data;
	size_t size;

	size_t tmpVal;

	int rv = SAL_FALSE;
	int ec = 0;	// error code

	for (;;)	// only looks like endless loop; eases exits
	{
		// read input-file, create and write header-file

		// read in source file
		// ===================
		inputFileHandle = fopen( inputFilename, "r+b" );
		if ( NULL == inputFileHandle )
		{
			ec = -2;
			break;
		}

		fseek( inputFileHandle, 0L, SEEK_END );
		size = ftell( inputFileHandle );
		if ( 0 == size )
		{
			// clean up resources
			fclose( inputFileHandle );
			ec = -3;
			rv = SAL_TRUE;	// quietly terminate if size of input file is zero
			break;
		}
		fseek( inputFileHandle, 0L, SEEK_SET );

		data = malloc( size );
		if ( NULL == data )
		{
			// clean up resources
			fclose( inputFileHandle );
			ec = -4;
			break;
		}

		tmpVal = fread( data, 1, size, inputFileHandle );
		fclose( inputFileHandle );
		if ( size != tmpVal )
		{
			ec = -5;
			break;
		}
		
        options->addressOffset = 0;
        if ( size <= options->maxBinaryChunkSize )
        {
		   headerFileHandle = fopen( headerFilename, "w+b" );
		   if ( NULL == headerFileHandle )
		   {
			   // clean up resources
			   free( data );
			   ec = -6;
			   break;
		   }

		   DataWriteHeader(
			   options,
			   headerFileHandle,
			   arrayName,
			   data,
			   size );

		   fclose( headerFileHandle );
        }
        else
        {
            // loop over chunks - add counter at end of each array/file/etc.
            char newFilename[ 4096 ];
            char newArrayName[ 4096 ];
            unsigned long idx = 0;
            unsigned char *newData = ( unsigned char * )data;
            size_t dataWritten = 0;
            size_t dataToWrite = options->maxBinaryChunkSize;
            do
            {
                sprintf( newFilename, "%s%2.2d.c", headerFilename, idx );
                sprintf( newArrayName, "%s%2.2d", arrayName, idx );
                headerFileHandle = fopen( newFilename, "w+b" );
                if ( NULL == headerFileHandle )
                {
                    ec = -6;
                    break;
                }

                DataWriteHeader(
                    options,
                    headerFileHandle,
                    newArrayName,
                    newData,
                    dataToWrite );
                options->addressOffset += options->maxBinaryChunkSize;
                fclose( headerFileHandle );

                idx++;
                newData += options->maxBinaryChunkSize;
                dataWritten += dataToWrite;
                if ( size <= options->maxBinaryChunkSize + dataWritten )
                {
                    dataToWrite = size - dataWritten;
                }
            } while (( dataWritten < size ) && ( ec == 0 ));
        }
		free( data );

		rv = SAL_TRUE;
		break;
	}	// 	for (;;)	// only looks like endless loop; eases exits

	return rv ? 0 : ec;
}

void
help( char *name )
{
	printf( "usage:\n" );
	printf( "%s <file to read> <name of headerfile> <name of array> <optional chunk size>\n", name );
}

int main( int argc, char **argv )
{
	char *inputFileFilename;
	char *headerFileFilename;
	char *arrayName;

	int ec = 0;	// error code

	options_t options;
	options.valuesPerLine = JS_BIN2H_VALUES_PER_LINE;
    options.maxBinaryChunkSize = 0xffffffff;

#ifdef JS_BIN2H_ADDRESS_OPTION_ON
	options.flagAddressOutput = 1;
#else
	options.flagAddressOutput = 0;
#endif

	for (;;)	// only looks like endless loop; eases exits
	{
		// get arguments
		switch ( argc )
		{
			case 1:
			case 2:
			case 3:
			default:
			{
				help( argv[ 0 ]);
				ec = -1;
				break;
			}
         case 5:
            options.maxBinaryChunkSize = atoi( argv[ 4 ]);
			case 4:
			{
				inputFileFilename = argv[ 1 ];
				headerFileFilename = argv[ 2 ];
				arrayName = argv[ 3 ];
				break;
			}
		}

		if ( ec )	// react on potential argument problem
		{
			break;
		}
		
		ec = DataReadFile(
			&options,
			inputFileFilename,
			headerFileFilename,
			arrayName );
		break;
	}
	return ec;
}

