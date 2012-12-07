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
//! \file  spectraVFS.cpp
//! \brief virtual filesystem for spectra, uses main memory as cache

#include "sdsslib/spectraVFS.h"

#include "sdsslib/spectraVFSCached.h"
#include "sdsslib/spectraVFSMemOnly.h"
#include "sdsslib/spectraWrite.h"
#include "sdsslib/helpers.h"
#include "sdsslib/filehelpers.h"
#include "sdsslib/spectra.h"
#include "sdsslib/Timer.h"
#include "sdsslib/memory.h"
#include "sdsslib/defines.h"


#include <sstream>
#include <iostream>
#include <fstream>
#include <conio.h>
#include <assert.h>


SpectraVFS::SpectraVFS( const std::string &_sstrFilename, bool _readOnly )
:m_sstrDumpFilename(_sstrFilename)
,m_logFile(std::string(_sstrFilename+std::string(".log")).c_str())
{
#ifdef X64 
	m_pSpectraVFS = new SpectraVFSMemOnly(_sstrFilename, m_logFile, _readOnly );
#else
	m_pSpectraVFS = new SpectraVFSCached(_sstrFilename, m_logFile, _readOnly );
#endif // SPECTRAVFS_MEMONLY
}

SpectraVFS::~SpectraVFS()
{
	flush();
	delete m_pSpectraVFS;
}

const std::string &SpectraVFS::getFileName() const
{
	return m_sstrDumpFilename;
}


size_t SpectraVFS::getNumSpectra() const
{
	return m_pSpectraVFS->getNumSpectra();
}

Spectra *SpectraVFS::beginRead( size_t _nSpectraIndex )
{
	return m_pSpectraVFS->beginRead( _nSpectraIndex );
}

void SpectraVFS::endRead( size_t _nSpectraIndex )
{
	m_pSpectraVFS->endRead( _nSpectraIndex );
}


Spectra *SpectraVFS::beginWrite( size_t _nSpectraIndex )
{
	return m_pSpectraVFS->beginWrite( _nSpectraIndex );
}


void SpectraVFS::endWrite( size_t _nSpectraIndex )
{
	m_pSpectraVFS->endWrite( _nSpectraIndex );
}


void SpectraVFS::prefetch( size_t _nSpectraIndex )
{
	m_pSpectraVFS->prefetch( _nSpectraIndex );
}


void SpectraVFS::flush()
{
	m_pSpectraVFS->flush();
}

 void SpectraVFS::dumpToFile( const std::string _sstrFilename )
 {
	 const size_t numSpectra( m_pSpectraVFS->getNumSpectra() );
	 SpectraWrite w(_sstrFilename);
	 
	 for ( size_t i=0;i<numSpectra;i++ )
	 {
		 Spectra *a = beginRead( i );
		 w.write(*a);
		 endRead( i );
	 }
 }



size_t SpectraVFS::write( const std::string &_sstrDir, const std::string &_sstrFileName, unsigned int _spectraFilter, std::ofstream *_logStream, std::map<std::string,float> *pFITSFilenameSet )
{
	std::vector<std::string> fileList;

	size_t numFiles = FileHelpers::getFileList( _sstrDir, fileList );
	size_t numSpectraWritten = 0;

	Helpers::print( "Source directory contains "+ Helpers::numberToString<size_t>(numFiles)+" files.\n", _logStream );
	
	if ( pFITSFilenameSet != NULL )
	{
		Helpers::print( "Selection list contains "+ Helpers::numberToString<size_t>(pFITSFilenameSet->size())+" files.\n", _logStream );
	}

	if ( pFITSFilenameSet && pFITSFilenameSet->empty() )
	{
		pFITSFilenameSet = NULL;
	}


	Spectra compositeSpec;
	compositeSpec.clear();

	{
	SpectraWrite w(_sstrFileName);

	Spectra spec;

	size_t c = 0;
	for ( size_t i=0;i<numFiles;i++ )
	{
		float multiplier = 1.f;
		if ( pFITSFilenameSet )
		{
			std::string sstrFilename(FileHelpers::getFileName(fileList.at(i)));
			std::map<std::string,float>::iterator it = pFITSFilenameSet->find(sstrFilename);
			if ( it == pFITSFilenameSet->end() )
			{
//				Helpers::print( "skipping "+fileList.at(i)+"\n", _logStream );
				continue;
			}
			// make sure item is not written twice.
			multiplier = it->second;
			pFITSFilenameSet->erase( it );
		}
		spec.clear();

		const std::string sstrFilename( fileList.at(i) );
		const std::string sstrExtension = FileHelpers::getFileExtension( sstrFilename );


		bool bResult = false; // be pessimistic:)
		
		if ( sstrExtension == ".fit" )
		{
			bResult = spec.loadFromFITS_SDSS( sstrFilename );
		}
		else if ( sstrExtension == ".csv" )
		{
			bResult = spec.loadFromCSV( sstrFilename );
		}

		if ( bResult )
		{
			if ( (spec.m_Type & _spectraFilter) > 0 )
			{
				if ( spec.hasBadPixels() )
				{
//					Helpers::print( "Over 5%% bad pixels detected in spectrum "+sstrFilename+"\n", _logStream );
				}

//				if ( multiplier != 1.f )
//				{
//					Helpers::print( "multiplying spectrum "+sstrFilename+" with "+ Helpers::numberToString<float>(multiplier)+"\n", _logStream );
//					spec.multiply(multiplier);
//				}

				compositeSpec.add(spec);
				bResult = w.write(spec);
				c++;
			}
		}
		else 
		{
			Helpers::print( "failed to load "+fileList.at(i)+"\n", _logStream );
			if ( !spec.checkConsistency() )
			{
				Helpers::print( "Reason: spectrum contains NANs, infinite numbers or just insane high numbers.\n", _logStream );
			}
		}
	}

	numSpectraWritten = c;
	
	compositeSpec.multiply( 1.0f/static_cast<float>(c) );
	Helpers::print( "writing composite spectra.\n", _logStream );
	compositeSpec.saveToCSV( std::string("compositeSpectrum.csv") );

	}
	
	// for composite substraction
//	SpectraVFS vfs( _sstrFileName, false );
//	const size_t numSpectra = vfs.getNumSpectra();
//	for ( size_t i=0;i<numSpectra;i++ )
//	{
//		Spectra *a = vfs.beginWrite(i);
//		a->subtract( compositeSpec );
//		vfs.endWrite(i);
//	}

	return numSpectraWritten;
}

void SpectraVFS::write( size_t _gridSize, float _minPeak, float _maxPeak, const std::string &_sstrFileName )
{
	size_t gridSizeSqr = _gridSize*_gridSize;
	float strengthScale = static_cast<float>(gridSizeSqr)*2.f;

	SpectraWrite w(_sstrFileName);

	Spectra spec;
	spec.clear();
	for ( size_t i=0;i<gridSizeSqr;i++)
	{
		size_t x = i%_gridSize;
		size_t y = i/_gridSize;
		float strength = (static_cast<float>(x*x+y*y)*0.25f)/strengthScale;
		spec.randomize( _minPeak*strength, _maxPeak*strength );

		w.write(spec);
	}
}


void SpectraVFS::write( size_t _count, float _noize, const std::string &_sstrFileName )
{
	if ( _count <= 0 ) 
	{
		return;
	}

	SpectraWrite w(_sstrFileName);

	SSE_ALIGN Spectra spec;
	spec.clear();
	const float freqMin = 0.00001f;
	const float freqMax = 0.04f;
	const float freqStepSize = (freqMax-freqMin)/static_cast<float>(_count);
	float freq = freqMin;

	for ( size_t i=0;i<_count;i++)
	{
	//	spec.set((i%4)+1,0.f);
		spec.setSine( freq, 0.f, 1.f, _noize );
		freq += freqStepSize; 
		w.write(spec);
	}
}


