//! \verbatim
//! ###########################################################################
//! # ASPECT: A spectra clustering tool for exploration of large spectral surveys - Stage I
//! #
//! # Copyright (c) 2009 Aick in der Au
//! # All rights reserved.
//! ###########################################################################
//!
//!      created by : Aick in der Au <aick.inderau@gmail.com>
//!      created on : 11/15/2009
//! additional docs : none
//!  responsibility : 1. Aick in der Au
//!                   2. 
//! \endverbatim
//!
//! \file  BigTiff.cpp


#include "BigTiff.h"
#include "defines.h"
#include "sdssSoftwareVersion.h"
#include "fileHelpers.h"

#include "tiff.h"
#include "tiffio.h"

#include <assert.h>


void warningHandler(const char* mod, const char* fmt, va_list ap)
{
	char buff[65536];
	_vsnprintf(buff, 1024, fmt, ap);
	printf("TIFF Warning: %s\n", buff);
}

void errorHandler(const char* mod, const char* fmt, va_list ap)
{
	char buff[65536];
	_vsnprintf(buff, 1024, fmt, ap);
	printf("TIFF Error: %s\n", buff);
}



void BigTIFF::test()
{
	TIFFSetWarningHandler(warningHandler);
	TIFFSetErrorHandler(errorHandler);

	 TIFF* tif = TIFFOpen("map.tif", "r");
    uint16 compress;
    uint16 config;
    uint16 samplesPerPixel;
    uint32 imageWidth, imageLength;
    uint32 tileWidth, tileLength;

    int count = 0;


    TIFFGetField(tif, TIFFTAG_COMPRESSION, &compress);
    TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &config);
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &imageWidth);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &imageLength);
    TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tileWidth);
    TIFFGetField(tif, TIFFTAG_TILELENGTH, &tileLength);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesPerPixel);

	unsigned char Buff[128*128*3];
	int retVal = TIFFReadTile(tif,Buff,0,0,0,0);

	FileHelpers::writeFile("hui.raw", Buff,128*128*3);

	TIFFClose(tif);
}


BigTIFF::BigTIFF( const std::string &filename, int numTilesX, int numTilesY, int tileSize, bool compression )
	:m_canWrite(false)
	,m_numTilesX(numTilesX)
	,m_numTilesY(numTilesY)
	,m_tileSize(tileSize)
	,m_compression(compression)
	,m_tiffPtr(NULL)
{
#if _DEBUG
	TIFFSetWarningHandler(warningHandler);
	TIFFSetErrorHandler(errorHandler);
#else
	TIFFSetWarningHandler (NULL);
	TIFFSetErrorHandler   (NULL);
#endif

	if ( tileSize < 2 )
		return;


	TIFF *file = TIFFOpen(filename.c_str(), "w8");
	if (file == NULL) 
	{
		return;
	}

	m_tiffPtr = (void*)file;

	writeHeader( m_tileSize, 0 );

	m_canWrite = true;
}


void BigTIFF::writeHeader( int tileSize, int lod )
{
	TIFF *file = (TIFF *)m_tiffPtr;
	if ( file == NULL || tileSize < 2 )
		return;

	const int w = m_numTilesX*tileSize;
	const int h = m_numTilesY*tileSize;

	if ( lod > 0 )
		TIFFWriteDirectory(file);

	// for image pyramids
	if ( lod > 0 )
		TIFFSetField(file, TIFFTAG_SUBFILETYPE, FILETYPE_REDUCEDIMAGE );

	const int compress = (m_compression) ? COMPRESSION_LZW : COMPRESSION_NONE;

	TIFFSetField(file, TIFFTAG_IMAGEWIDTH, w);
	TIFFSetField(file, TIFFTAG_IMAGELENGTH, h);
	TIFFSetField(file, TIFFTAG_BITSPERSAMPLE, 8);
	TIFFSetField(file, TIFFTAG_COMPRESSION, compress );

	TIFFSetField(file, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
	TIFFSetField(file, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB);
	TIFFSetField(file, TIFFTAG_DOCUMENTNAME, "BigTIFF Image");
	TIFFSetField(file, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT );
	TIFFSetField(file, TIFFTAG_SAMPLESPERPIXEL, 3);

	TIFFSetField(file, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

	//	TIFFSetField(file, TIFFTAG_ROWSPERSTRIP, 1);
	//	TIFFSetField(file, TIFFTAG_ARTIST, "ASPECT");


	TIFFSetField(file, TIFFTAG_SOFTWARE, sstrSDSSVersionString.c_str());

	TIFFSetField(file, TIFFTAG_TILEWIDTH, tileSize);
	TIFFSetField(file, TIFFTAG_TILELENGTH, tileSize);
}


BigTIFF::~BigTIFF()
{
	TIFF *file = (TIFF *)m_tiffPtr;

	if ( file != NULL )
	{
		TIFFClose(file);
	}
}


bool BigTIFF::writeTile( int index, int tileSize, unsigned char *pData )
{
	if ( pData == NULL || !m_canWrite )
		return false;

	TIFF *file = (TIFF *)m_tiffPtr;

	tdata_t *tiffData = (tdata_t *)pData;
	tsize_t tiffSize = tileSize*tileSize*3;

//	TIFFWriteTile( file, index, tiffData, tiffSize );
//	TIFFWriteEncodedTile( file, index, tiffData, tiffSize );

	const int w = m_numTilesX*tileSize;
	const int h = m_numTilesY*m_tileSize;

	int xp = (index*tileSize) % w;
	int yp = (index*tileSize) / w;


	const int tiffTileSize = TIFFTileSize(file);
//	const int numTiles = TIFFNumberOfTiles(file);
//	const int res = TIFFCheckTile(file, xp, yp, 0, 0);
//	ttile_t tileID = TIFFComputeTile(file, xp, yp, 0, 0);
	if ( tiffSize != tiffTileSize )
	{
		assert(0);
		return false;
	}

	if( TIFFWriteTile( file, tiffData, xp, yp, 0, 0 ) == -1 )
	{
		assert(0);
		return false;
	}

	return true;
}

