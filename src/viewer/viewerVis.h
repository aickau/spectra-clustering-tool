
#pragma once

#include "vis/vis_main.h"
#include "vis/vis_interface.h"

class CViewerVis : public CVisModule 
{
public: 
	CViewerVis();
	DECLARE_DYNCREATE(CViewerVis)
	virtual ~CViewerVis();


	// Attributes
public:

	// Operations
public:

	// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);


	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

