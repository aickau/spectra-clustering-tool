// viewerDoc.cpp : implementation of the CviewerDoc class
//

#include "stdafx.h"
#include "viewer.h"

#include "viewerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CviewerDoc

IMPLEMENT_DYNCREATE(CViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CViewerDoc, CDocument)
END_MESSAGE_MAP()


// CviewerDoc construction/destruction

CViewerDoc::CViewerDoc()
{
	// TODO: add one-time construction code here

}

CViewerDoc::~CViewerDoc()
{
}

BOOL CViewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CviewerDoc serialization

void CViewerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CviewerDoc diagnostics

#ifdef _DEBUG
void CViewerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CViewerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CviewerDoc commands
