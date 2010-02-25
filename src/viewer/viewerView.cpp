// viewerView.cpp : implementation of the CViewerView class
//

#include "stdafx.h"
#include "viewer.h"

#include "viewerDoc.h"
#include "viewerView.h"
#include "viewerVis.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



IMPLEMENT_DYNCREATE(CViewerView, CView)

BEGIN_MESSAGE_MAP(CViewerView, CView)
END_MESSAGE_MAP()


CViewerView::CViewerView()
:m_pVis(NULL)
{
	m_pVis = new CViewerVis();
}

CViewerView::~CViewerView()
{
	delete m_pVis;
}

BOOL CViewerView::PreCreateWindow(CREATESTRUCT& cs)
{
	BOOL bRetVal = CView::PreCreateWindow(cs);

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER ;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS|CS_OWNDC, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);


	bRetVal &= m_pVis->PreCreateWindow(cs);
	return bRetVal;
}

BOOL CViewerView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext )
{
	CView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext );
	return m_pVis->Create(lpszClassName, lpszWindowName, dwStyle|WS_VISIBLE| WS_CLIPCHILDREN | WS_CLIPSIBLINGS, rect, this, nID, pContext );// | WS_CLIPCHILDREN | WS_CLIPSIBLINGS
}



void CViewerView::OnDraw(CDC* /*pDC*/)
{
	CViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

}



// CViewerView diagnostics

#ifdef _DEBUG
void CViewerView::AssertValid() const
{
	CView::AssertValid();
}

void CViewerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CViewerDoc* CViewerView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CViewerDoc)));
	return (CViewerDoc*)m_pDocument;
}
#endif //_DEBUG


// CViewerView message handlers
