/*********************************************************************************

 file:			vis_io.h
 author:		aick in der au
 description:	IO helper routines

**********************************************************************************/


#ifndef _VIS_IO_H
#define _VIS_IO_H


/// very simple helper class to display windows standard open/save(as) dialog box and to retrieve filename
class CVisOpenSaveFilename
{
public:
	/// somthing like: Extension = "dat", ExtensionDescription = "Data File"
	CVisOpenSaveFilename(LPCTSTR Extension, LPCTSTR ExtensionDescription);
	~CVisOpenSaveFilename();

	BOOL			GetOpenFilename(CString &FileName);
	BOOL			GetSaveFilename(CString &FileName);

private:
	CString			m_StrExtension;						
	CString			m_StrExtensionDescription;		
	CString			m_StrFilter;
};




#endif