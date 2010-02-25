// viewerDoc.h : interface of the CviewerDoc class
//


#pragma once


class CViewerDoc : public CDocument
{
protected: // create from serialization only
	CViewerDoc();
	DECLARE_DYNCREATE(CViewerDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CViewerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


