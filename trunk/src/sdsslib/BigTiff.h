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
//! \file  BigTiff.h
//! \brief BigTiff Saver for large tile-based and TIFFs.

#include <string>

class BigTIFF
{
public:
	BigTIFF( const std::string &filename, int numTilesX, int numTilesY, int tileSize=128, bool compression = true );
	virtual ~BigTIFF();

	bool writeTile( int index, int tileSize, unsigned char *pData  );
	void writeHeader( int tileSize, int lod );

	static void test();

	bool m_canWrite;

private:

	void *m_tiffPtr;

	int m_numTilesX;
	int m_numTilesY;
	int m_tileSize;
	bool m_compression;
};