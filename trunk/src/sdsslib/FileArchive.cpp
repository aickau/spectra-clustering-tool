//! \verbatim
//! ###########################################################################
//! # ASPECT: A spectra clustering tool for exploration of large spectral surveys - Stage I
//! #
//! # Copyright (c) 2009 Aick in der Au
//! # All rights reserved.
//! ###########################################################################
//!
//!      created by : Aick in der Au <aick.inderau@gmail.com>
//!      created on : 7/25/2009
//! additional docs : none
//!  responsibility : 1. Aick in der Au
//!                   2. 
//! \endverbatim
//!
//! \file  FileArchive.cpp
//! \brief 

#include "FileArchive.h"

#include "ziplib/unzipmem.h"
#include "ziplib/unzip.h"
#include "ziplib/zip.h"

DataFile::DataFile( const std::string &_sstrFilename, char *_pData, size_t _nSize, bool _bDeepCopy )
:m_sstrFileName(_sstrFilename)
,m_pData(_pData)
,m_nSize(_nSize)
,m_bDeepCopy(_bDeepCopy)
{
	if ( _bDeepCopy )
	{
		m_pData = new char[m_nSize];
		memcpy_s( m_pData, m_nSize, _pData, m_nSize );
	}
}

DataFile::DataFile( const std::string &_sstrFilename, size_t _nSize)
:m_sstrFileName(_sstrFilename)
,m_pData(NULL)
,m_nSize(_nSize)
,m_bDeepCopy(true)
{
	m_pData = new char[m_nSize];
}



DataFile::~DataFile()
{
	if ( m_bDeepCopy )
	{
		delete[] m_pData;
	}
}

const std::string &DataFile::getFilename() const
{
	return m_sstrFileName;
}

char *DataFile::getData() const
{
	return m_pData;
}

size_t DataFile::getSize() const
{
	return m_nSize;
}


FileArchive::FileArchive()
:m_hArchive(NULL)
,m_sstrFilename("")
,m_mode(AM_UNINITIALIZED)
{
}

FileArchive::~FileArchive()
{
	closeArchive();
}


bool FileArchive::createArchiveForPack( const std::string &_sstrFilename )
{
	if ( m_mode != AM_UNINITIALIZED )
	{
		return false;
	}
	m_hArchive = zipOpen( _sstrFilename.c_str(), 0 );

	if ( m_hArchive != NULL )
	{
		m_mode = AM_PACK;
		m_sstrFilename = _sstrFilename;
		return true;
	}
	return false;
}


bool FileArchive::openArchiveForUnpack( const std::string &_sstrFilename )
{
	if ( m_mode != AM_UNINITIALIZED )
	{
		return false;
	}

	m_hArchive = unzOpen(_sstrFilename.c_str());
	if ( m_hArchive != NULL )
	{
		m_mode = FileArchive::AM_UNPACK_FROM_FILE;
		return true;
	}
	return false;
}

bool FileArchive::openArchiveFromMemoryForUnpack( const char *_pData, size_t _nSize )
{
	if ( m_mode != AM_UNINITIALIZED )
	{
		return false;
	}

	m_hArchive = mem_unzOpen( (void*)(_pData), _nSize );
	if ( m_hArchive != NULL )
	{
		m_mode = FileArchive::AM_UNPACK_FROM_MEMORY;
		return true;
	}
	return false;
}




bool FileArchive::createFileInArchive( const std::string &_sstrFilename, const DataFile &_dataFile )
{
	if( m_mode == AM_PACK )
	{
		zipOpenNewFileInZip( (zipFile*)m_hArchive, _dataFile.getFilename().c_str(), NULL,	NULL, 0, NULL, 0, NULL,	Z_DEFLATED, Z_BEST_COMPRESSION);
		zipWriteInFileInZip( (zipFile*)m_hArchive, _dataFile.getData(), _dataFile.getSize() );
		return true;
	}

	return false;
}


DataFile *FileArchive::getFileInArchive( const std::string &_sstrFilename )
{
	if ( m_mode == AM_UNPACK_FROM_FILE )
	{
		if ( unzLocateFile( ( zipFile* )m_hArchive, _sstrFilename.c_str(), 0) == UNZ_OK )
		{
			unz_file_info fileInfo;
			char fileName[4096];

			if( unzGetCurrentFileInfo( ( zipFile* )m_hArchive ,&fileInfo, fileName, sizeof(fileName), NULL, 0, NULL, 0 ) == UNZ_OK ) 
			{
				if( fileInfo.uncompressed_size > 0 )
				{
					DataFile *pDataFile = new DataFile( _sstrFilename, fileInfo.uncompressed_size );
					
					unzOpenCurrentFile( ( zipFile* )m_hArchive );
					unzReadCurrentFile( ( zipFile* )m_hArchive, pDataFile->getData(), fileInfo.uncompressed_size );
					unzCloseCurrentFile( ( zipFile* )m_hArchive );
	
					return pDataFile;
				}
			}
		}
	}

	return NULL;
}
/*
	if( CurrentMode == MODE_UNPACK_FROM_MEMORY ){
		char fn_inzip[1024];
		mem_unz_file_info file_info;
		int size=0;
		if ( mem_unzLocateFile( ( zipFile* )Archive, fn, 0) == UNZ_OK ){
			if( mem_unzGetCurrentFileInfo( ( zipFile* )Archive ,&file_info, fn_inzip, sizeof(fn_inzip), NULL, 0, NULL, 0 ) == UNZ_OK ) {
				size=file_info.uncompressed_size;
				uint8_t *buffer = newuint8_t[ size ];
				mem_unzOpenCurrentFile( ( zipFile* )Archive );
				mem_unzReadCurrentFile( ( zipFile* )Archive, buffer, size );
				mem_unzCloseCurrentFile( ( zipFile* )Archive );
				DataFile *f = new DataFile( (char *)fn );
				f->AddData( buffer, size );
				Files.Add( f );
				delete [] buffer;
				prf_time += ((float)GetTickCount()-loadstarttime) ;
				return f;
			}
		}
	}
	return 0;
}
*/

void FileArchive::closeArchive()
{
	if ( m_hArchive == NULL )
	{
		return;
	}

	if ( m_mode == AM_PACK )
	{
		zipClose( m_hArchive, NULL );
	}
	else if ( m_mode == AM_UNPACK_FROM_MEMORY )
	{
		mem_unzClose( m_hArchive );
	}
	else if ( m_mode == AM_UNPACK_FROM_FILE )
	{
		unzClose( m_hArchive );
	}
}

/*

DataFile* Packer::CreateFileInArchive(  char *fn ){
	if( CurrentMode == MODE_PACK_ ){
		DataFile *f = new DataFile( fn );
		Files.Add( f );
		return f;
	}
	return 0;
}

DataFile *Packer::GetFileInArchive( const char *fn ){
	int loadstarttime = GetTickCount();
	prf_cnt++;
	char fn_inzip[256];
	if( CurrentMode == MODE_UNPACK_ ){
		unz_file_info file_info;
		int size=0;
		uint8_t *buffer = 0;
		if ( unzLocateFile( ( zipFile* )Archive, fn, 0) == UNZ_OK ){
			if( unzGetCurrentFileInfo( ( zipFile* )Archive ,&file_info, fn_inzip, sizeof(fn_inzip), NULL, 0, NULL, 0 ) == UNZ_OK ) {
				size=file_info.uncompressed_size;
				if( size > 0 ){
					buffer = new uint8_t[ size ];
					DataFile *f = new DataFile( (char *)fn );
					unzOpenCurrentFile( ( zipFile* )Archive );
					unzReadCurrentFile( ( zipFile* )Archive, buffer, size );
					unzCloseCurrentFile( ( zipFile* )Archive );
					f->AddData( buffer, size );
					Files.Add( f );
					delete [] buffer;


					return f;
				}
			}
		}
	}
	if( CurrentMode == MODE_UNPACK_FROM_MEMORY ){
		char fn_inzip[1024];
		mem_unz_file_info file_info;
		int size=0;
		if ( mem_unzLocateFile( ( zipFile* )Archive, fn, 0) == UNZ_OK ){
			if( mem_unzGetCurrentFileInfo( ( zipFile* )Archive ,&file_info, fn_inzip, sizeof(fn_inzip), NULL, 0, NULL, 0 ) == UNZ_OK ) {
				size=file_info.uncompressed_size;
				uint8_t *buffer = new uint8_t[ size ];
				mem_unzOpenCurrentFile( ( zipFile* )Archive );
				mem_unzReadCurrentFile( ( zipFile* )Archive, buffer, size );
				mem_unzCloseCurrentFile( ( zipFile* )Archive );
				DataFile *f = new DataFile( (char *)fn );
				f->AddData( buffer, size );
				Files.Add( f );
				delete [] buffer;
				prf_time += ((float)GetTickCount()-loadstarttime) ;
				return f;
			}
		}
	}
	return 0;
}


bool Packer::CloseArchive(){
	if( CurrentMode == MODE_PACK_ ){
		if( Compression == CompressionType::Zip ){
			for( unsigned int i = 0; i <  Files.GetCount(); i++  ) {
				DataFile *cf = Files.Get( i );
				zipOpenNewFileInZip( ( zipFile *)Archive ,
					cf->GetName(),
					NULL,
					NULL,
					0,
					NULL,
					0,
					NULL,
					Z_DEFLATED,
					Z_BEST_COMPRESSION);
				zipWriteInFileInZip( ( zipFile *)Archive, cf->GetData(), cf->GetSize() );

			}

			zipClose( Archive, "" );
			Archive = 0;
			Files.ClearAndDelete();
		}
	}

	if( CurrentMode == MODE_UNPACK_ ){
		if( Compression == CompressionType::Zip ){
			unzClose( Archive );
			Archive = 0;
			Files.ClearAndDelete();
		}
	}

	printf("\n Packer unpacked %d files, took oa %f secs\n", prf_cnt, ((float)prf_time)/1000.f );
	CurrentMode = 0;
	return true;
}*/