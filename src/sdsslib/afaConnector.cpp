//! \verbatim
//! ###########################################################################
//! # ASPECT: A spectra clustering tool for exploration of large spectral surveys - Stage I
//! #
//! # Copyright (c) 2009 Aick in der Au
//! # All rights reserved.
//! ###########################################################################
//!
//!      created by : Aick in der Au <aick.inderau@gmail.com>
//!      created on : 9/18/2016
//! additional docs : none
//!  responsibility : 1. Aick in der Au
//!                   2. 
//! \endverbatim
//!
//! \file  afaConnector.cpp
//! \brief 

#include "afaConnector.h"

#include "helpers.h"
#include "spectraVFS.h"
#include "spectra.h"

#include "../fpgaPlayground/AFA/src/AFASpectraCommon.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>


#ifdef _WIN32
#include <io.h>
#endif

#ifdef __linux
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>

#define _open open 
#define _close close 
#define _read read 
#define _write write 
#endif





//! translates sw spectra to hw spectra. Can be used for input spectra as well as network spectra.
//! spectraArraySw incoming sw spectra
//! outHWAddr allocated hardware memory where we write AFA_SPECTRA_INDEX_SIZE_IN_BYTES*numSpectra bytes
void swSpectraToHwSpectra( const Spectra &sp, unsigned int *outHWAddr )
{
	for ( int j = 0; j < AFA_SPECTRA_NUM_SAMPLES_PROCESS_HW; j++ )
	{
		// cast to float ptr to write float values
		float *tmpValFloatPtr = (( float * ) &outHWAddr[ AFA_SPECTRA_INDEX_AMPLITUDE + j ]);
		( *tmpValFloatPtr ) = sp.m_Amplitude[ j + AFA_PROCESS_PIXEL_START ];
	}

	outHWAddr[ AFA_SPECTRA_INDEX_INDEX            ] = sp.m_Index;
	outHWAddr[ AFA_DEBUG_MAGIC ] = 0x12345678;
	outHWAddr[ AFA_DEBUG_JSC ] = 0;
	outHWAddr[ AFA_SPECTRA_INDEX_SPEC_OBJ_ID_LOW  ] = ( unsigned int )( sp.m_SpecObjID & 0x0ffffffff );
	outHWAddr[ AFA_SPECTRA_INDEX_SPEC_OBJ_ID_HIGH ] = ( unsigned int )( sp.m_SpecObjID >> 32 );

	// fill unoccupied space with zeros
	for ( int i=AFA_LAST_ELM;i<AFA_SPECTRA_INDEX_SIZE_IN_UINT32;i++)
	{
		outHWAddr[ i ] = 0;
	}


}


AfaConnector::AfaConnector(const std::string path)
	:m_sstrPath(path)
	,m_deviceHandle(-1)
{
	openConnection();
}

AfaConnector::~AfaConnector()
{
	closeConnection();
}




bool AfaConnector::isAFADeviceAvailable() const
{
	if ( m_deviceHandle < 0 )
		return false;

	return true;
}

std::string AfaConnector::getErrorMsg() const
{
	return m_sstrErrorMsg;
}

bool AfaConnector::writeSpectra( SpectraVFS &spectra )
{
	const int numSpectra = spectra.getNumSpectra();

	const size_t size = (size_t)numSpectra*AFA_SPECTRA_INDEX_SIZE_IN_BYTES;
	const size_t size4 = size/4;

	if ( size > maxBufferSize )
	{
		m_sstrErrorMsg = "AfaConnector exceeded maximum buffer size of "+Helpers::numberToString<size_t>(size)+ " bytes or writing to AFA device.";
		return false;
	}

	unsigned int *pBuffer = new unsigned int[size4];

	if (pBuffer == NULL )
	{
		m_sstrErrorMsg = "AfaConnector error allocating writer buffer of "+Helpers::numberToString<size_t>(size)+ " bytes for writing to AFA device.";
		return false;
	}

	size_t o = 0;

	for ( int i=0;i<numSpectra;i++ )
	{
		Spectra *sp = spectra.beginRead(i);
		swSpectraToHwSpectra( *sp, &pBuffer[o] );
		spectra.endRead(i);

		o += AFA_SPECTRA_INDEX_SIZE_IN_UINT32;
	}

	bool retVal = writeData( pBuffer, size );

	if ( !retVal )
	{
		m_sstrErrorMsg = "AfaConnector error writing data to AFA device.";
	}

	delete[] pBuffer;

	return retVal;
}


void AfaConnector::openConnection()
{

	if ( m_deviceHandle < 0 )
	{
		m_deviceHandle = _open( m_sstrPath.c_str(), O_RDWR );

		if ( m_deviceHandle < 0 )
		{
			int errorCode = errno;

			switch ( errorCode )
			{
			case EACCES: m_sstrErrorMsg = "AfaConnector open device error: Tried to open a read-only file for writing, file's sharing mode does not allow the specified operations, or the given path is a directory.";
				break;
			case EEXIST:
				m_sstrErrorMsg = "AfaConnector open device error: _O_CREAT and _O_EXCL flags specified, but filename already exists.";	
				break;
			case EINVAL:
				m_sstrErrorMsg = "AfaConnector open device error: Invalid oflag or pmode argument.";	
				break;
			case EMFILE:
				m_sstrErrorMsg = "AfaConnector open device error: No more file descriptors are available (too many files are open).";		
				break;
			case ENOENT:
				m_sstrErrorMsg = "AfaConnector open device error: File or path not found at "+m_sstrPath+".";
				break;
			default:
				m_sstrErrorMsg = "AfaConnector open device error: Error no: "+Helpers::numberToString<int>(errorCode);
			}
		}
	}

}


void AfaConnector::closeConnection()
{
	if ( m_deviceHandle >= 0 )
		_close( m_deviceHandle );

	m_deviceHandle = -1;
}


bool AfaConnector::readData( void *buffer, unsigned int size )
{
	if ( buffer == NULL || size == 0 )
		return false;

	if ( size > maxBufferSize )
		return false;

	if ( !isAFADeviceAvailable() )
		return false;

	// ~1MB/s
	const int rc = _read( m_deviceHandle, buffer, size );		
	const bool retVal = (rc == size); 

	return retVal;
}

bool AfaConnector::writeData( void *buffer, unsigned int size )
{
	if ( buffer == NULL || size == 0 )
		return false;

	if ( size > maxBufferSize )
		return false;

	if ( !isAFADeviceAvailable() )
		return false;

	// memory mapping is a lot faster than the write option
	// currently we support this only for linux
#ifdef __linux
	void *deviceBuffer = mmap( 0, size, PROT_WRITE, MAP_SHARED, m_deviceHandle, 0 );
	if ( deviceBuffer != MAP_FAILED )
	{
		// write to device
		memcpy( deviceBuffer, buffer, size );
		return true;
	}
	else
#endif

	{
		// Fallback if memory mapping fails or we are not under linux 

		// ~16 MB/s
		const int wc = _write( m_deviceHandle, buffer, size );
		const bool retVal = (wc == size); 
		return retVal;
	}

	return false;
}
