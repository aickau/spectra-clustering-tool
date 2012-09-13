//! \verbatim
//! ###########################################################################
//! # ASPECT: A spectra clustering tool for exploration of large spectral surveys - Stage I
//! #
//! # Copyright (c) 2009 Aick in der Au
//! # All rights reserved.
//! ###########################################################################
//!
//!      created by : Aick in der Au <aick.inderau@gmail.com>
//!      created on : 7/19/2009
//! additional docs : none
//!  responsibility : 1. Aick in der Au
//!                   2. 
//! \endverbatim
//!
//! \file  FileArchive.h
//! \brief 

#ifndef __FILEARCHIVE_H__
#define __FILEARCHIVE_H__

#include <string>

class DataFile
{
public:
	/** @name CONSTRUCTORS / DESTRUCTORS
	*/
	//@{

	// bDeepCopy if true, data is copied, and DataFile is the holder of the data.
	DataFile( const std::string &_sstrFilename, char *_pData, size_t _nSize, bool _bDeepCopy = false );
	
	// creates an empty data file. 
	DataFile( const std::string &_sstrFilename, size_t _nSize); 

	~DataFile();

	//@}

	/** @name ACCESSORS
	*/
	//@{

	const std::string &getFilename() const;
	char *getData() const;
	size_t getSize() const;

	//@}

	/** @name MODIFIERS
	*/
	//@{
	//@}

private:
	const bool m_bDeepCopy;
	const std::string m_sstrFileName;
	char *m_pData;
	size_t m_nSize;
};


class FileArchive
{
public:
	/** @name CONSTRUCTORS / DESTRUCTORS
	*/
	//@{

	FileArchive();
	virtual ~FileArchive();

	//@}

	/** @name ACCESSORS
	*/
	//@{
	
	//@}

	/** @name MODIFIERS
	*/
	//@{

	// create archive 
	bool createArchiveForPack( const std::string &_sstrFilename );
	bool openArchiveForUnpack( const std::string &_sstrFilename );
	bool openArchiveFromMemoryForUnpack( const char *_pData, size_t _nSize );

	// add file to archive, only works in pack mode
	bool createFileInArchive( const std::string &_sstrFilename, const DataFile &_dataFile );

	// works only in unpack mode
	DataFile *getFileInArchive( const std::string &_sstrFilename );

	//@}

private:

	enum ArchiveMode
	{
		AM_UNINITIALIZED=0,
		AM_PACK,
		AM_UNPACK_FROM_FILE,
		AM_UNPACK_FROM_MEMORY
	};

	void closeArchive();

	void *m_hArchive;
	std::string m_sstrFilename;
	ArchiveMode m_mode;
};

#endif // __FILEARCHIVE_H__