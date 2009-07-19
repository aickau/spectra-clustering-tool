#ifndef _unz_mem_H
#define _unz_mem_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _ZLIB_H
#include "zlib/include/zlib.h"
#endif

#if defined(STRICTUNZIP) || defined(STRICTZIPUNZIP)
	/* like the STRICT of WIN32, we define a pointer that cannot be converted
	from (void*) without cast */
	typedef struct TagunzFile__ { int unused; } unzFile__; 
	typedef unzFile__ *unzFile;
#else
	typedef voidp mem_unzFile;
#endif


#define UNZ_OK                                  (0)
#define UNZ_END_OF_LIST_OF_FILE (-100)
#define UNZ_ERRNO               (Z_ERRNO)
#define UNZ_EOF                 (0)
#define UNZ_PARAMERROR                  (-102)
#define UNZ_BADZIPFILE                  (-103)
#define UNZ_INTERNALERROR               (-104)
#define UNZ_CRCERROR                    (-105)

	/* tm_unz contain date/time info */
	typedef struct mem_tm_unz_s 
	{
		uInt tm_sec;            /* seconds after the minute - [0,59] */
		uInt tm_min;            /* minutes after the hour - [0,59] */
		uInt tm_hour;           /* hours since midnight - [0,23] */
		uInt tm_mday;           /* day of the month - [1,31] */
		uInt tm_mon;            /* months since January - [0,11] */
		uInt tm_year;           /* years - [1980..2044] */
	} mem_tm_unz;

	/* unz_global_info structure contain global data about the ZIPfile
	These data comes from the end of central dir */
	typedef struct mem_unz_global_info_s
	{
		uLong number_entry;         /* total number of entries in
									the central dir on this disk */
		uLong size_comment;         /* size of the global comment of the zipfile */
	} mem_unz_global_info;


	/* unz_file_info contain information about a file in the zipfile */
	typedef struct mem_unz_file_info_s
	{
		uLong version;              /* version made by                 2 bytes */
		uLong version_needed;       /* version needed to extract       2 bytes */
		uLong flag;                 /* general purpose bit flag        2 bytes */
		uLong compression_method;   /* compression method              2 bytes */
		uLong dosDate;              /* last mod file date in Dos fmt   4 bytes */
		uLong crc;                  /* crc-32                          4 bytes */
		uLong compressed_size;      /* compressed size                 4 bytes */ 
		uLong uncompressed_size;    /* uncompressed size               4 bytes */ 
		uLong size_filename;        /* filename length                 2 bytes */
		uLong size_file_extra;      /* extra field length              2 bytes */
		uLong size_file_comment;    /* file comment length             2 bytes */

		uLong disk_num_start;       /* disk number start               2 bytes */
		uLong internal_fa;          /* internal file attributes        2 bytes */
		uLong external_fa;          /* external file attributes        4 bytes */

		mem_tm_unz tmu_date;
	} mem_unz_file_info;

	extern int ZEXPORT mem_unzStringFileNameCompare OF ((const char* fileName1,
		const char* fileName2,
		int iCaseSensitivity));
	/*
	Compare two filename (fileName1,fileName2).
	If iCaseSenisivity = 1, comparision is case sensitivity (like strcmp)
	If iCaseSenisivity = 2, comparision is not case sensitivity (like strcmpi
	or strcasecmp)
	If iCaseSenisivity = 0, case sensitivity is defaut of your operating system
	(like 1 on Unix, 2 on Windows)
	*/


	extern mem_unzFile ZEXPORT mem_unzOpen OF(( void *data, unsigned int size ));
	/*
	Open a Zip file. path contain the full pathname (by example,
	on a Windows NT computer "c:\\zlib\\zlib111.zip" or on an Unix computer
	"zlib/zlib111.zip".
	If the zipfile cannot be opened (file don't exist or in not valid), the
	return value is NULL.
	Else, the return value is a mem_unzFile Handle, usable with other function
	of this mem_unzip package.
	*/

	extern int ZEXPORT mem_unzClose OF((mem_unzFile file));
	/*
	Close a ZipFile opened with mem_unzipOpen.
	If there is files inside the .Zip opened with mem_unzOpenCurrentFile (see later),
	these files MUST be closed with mem_unzipCloseCurrentFile before call mem_unzipClose.
	return mem_unz_OK if there is no problem. */

	extern int ZEXPORT mem_unzGetGlobalInfo OF((mem_unzFile file,
		mem_unz_global_info *pglobal_info));
	/*
	Write info about the ZipFile in the *pglobal_info structure.
	No preparation of the structure is needed
	return mem_unz_OK if there is no problem. */


	extern int ZEXPORT mem_unzGetGlobalComment OF((mem_unzFile file,
		char *szComment,
		uLong uSizeBuf));
	/*
	Get the global comment string of the ZipFile, in the szComment buffer.
	uSizeBuf is the size of the szComment buffer.
	return the number of byte copied or an error code <0
	*/


	/***************************************************************************/
	/* mem_unzip package allow you browse the directory of the zipfile */

	extern int ZEXPORT mem_unzGoToFirstFile OF((mem_unzFile file));
	/*
	Set the current file of the zipfile to the first file.
	return mem_unz_OK if there is no problem
	*/

	extern int ZEXPORT mem_unzGoToNextFile OF((mem_unzFile file));
	/*
	Set the current file of the zipfile to the next file.
	return mem_unz_OK if there is no problem
	return mem_unz_END_OF_LIST_OF_FILE if the actual file was the latest.
	*/

	extern int ZEXPORT mem_unzLocateFile OF((mem_unzFile file, 
		const char *szFileName,
		int iCaseSensitivity));
	/*
	Try locate the file szFileName in the zipfile.
	For the iCaseSensitivity signification, see mem_unzStringFileNameCompare

	return value :
	mem_unz_OK if the file is found. It becomes the current file.
	mem_unz_END_OF_LIST_OF_FILE if the file is not found
	*/


	extern int ZEXPORT mem_unzGetCurrentFileInfo OF((mem_unzFile file,
		mem_unz_file_info *pfile_info,
		char *szFileName,
		uLong fileNameBufferSize,
		void *extraField,
		uLong extraFieldBufferSize,
		char *szComment,
		uLong commentBufferSize));
	/*
	Get Info about the current file
	if pfile_info!=NULL, the *pfile_info structure will contain somes info about
	the current file
	if szFileName!=NULL, the filemane string will be copied in szFileName
	(fileNameBufferSize is the size of the buffer)
	if extraField!=NULL, the extra field information will be copied in extraField
	(extraFieldBufferSize is the size of the buffer).
	This is the Central-header version of the extra field
	if szComment!=NULL, the comment string of the file will be copied in szComment
	(commentBufferSize is the size of the buffer)
	*/

	/***************************************************************************/
	/* for reading the content of the current zipfile, you can open it, read data
	from it, and close it (you can close it before reading all the file)
	*/

	extern int ZEXPORT mem_unzOpenCurrentFile OF((mem_unzFile file));
	/*
	Open for reading data the current file in the zipfile.
	If there is no error, the return value is mem_unz_OK.
	*/

	extern int ZEXPORT mem_unzCloseCurrentFile OF((mem_unzFile file));
	/*
	Close the file in zip opened with mem_unzOpenCurrentFile
	Return mem_unz_CRCERROR if all the file was read but the CRC is not good
	*/


	extern int ZEXPORT mem_unzReadCurrentFile OF((mem_unzFile file, 
		voidp buf,
		unsigned len));
	/*
	Read bytes from the current file (opened by mem_unzOpenCurrentFile)
	buf contain buffer where data must be copied
	len the size of buf.

	return the number of byte copied if somes bytes are copied
	return 0 if the end of file was reached
	return <0 with error code if there is an error
	(mem_unz_ERRNO for IO error, or zLib error for uncompress error)
	*/

	extern z_off_t ZEXPORT mem_unztell OF((mem_unzFile file));
	/*
	Give the current position in uncompressed data
	*/

	extern int ZEXPORT mem_unzeof OF((mem_unzFile file));
	/*
	return 1 if the end of file was reached, 0 elsewhere 
	*/

	extern int ZEXPORT mem_unzGetLocalExtrafield OF((mem_unzFile file,
		voidp buf,
		unsigned len));
	/*
	Read extra field from the current file (opened by unzOpenCurrentFile)
	This is the local-header version of the extra field (sometimes, there is
	more info in the local-header version than in the central-header)

	if buf==NULL, it return the size of the local extra field

	if buf!=NULL, len is the size of the buffer, the extra header is copied in
	buf.
	the return value is the number of bytes copied in buf, or (if <0) 
	the error code
	*/

#ifdef __cplusplus
}
#endif

#endif /* _unz_H */
