
#pragma once

class CViewerVis;

class CViewerView : public CView
{
protected: // create from serialization only
	CViewerView();
	DECLARE_DYNCREATE(CViewerView)

// Attributes
public:
	CViewerDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
protected:

// Implementation
public:
	virtual ~CViewerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CViewerVis *m_pVis;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in viewerView.cpp
inline CViewerDoc* CViewerView::GetDocument() const
   { return reinterpret_cast<CViewerDoc*>(m_pDocument); }
#endif

