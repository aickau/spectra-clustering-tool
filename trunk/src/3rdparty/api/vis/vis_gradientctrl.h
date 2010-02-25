#ifndef _VIS_GRADIENTCTRL_H
#define _VIS_GRADIENTCTRL_H

#include "vis_gradient.h"

class CVisGradientCtrl : public CWnd
{
public:
	CVisGradientCtrl();
	virtual ~CVisGradientCtrl();

	BOOL			Create(const RECT& rect, CWnd* pParentWnd, UINT nID);
	CVisGradient	&GetGradient() {return m_Gradient;}


	void			DeleteSelected();
	int				MoveSelected(float newpos);



protected:
	void			Draw(CDC &dc);
	BOOL			RegisterWindowClass();
	
	CVisPeg			*GetSelectedPeg();
	CRect			GetGradientPixelRectangle();
	BOOL			GetPegPixelRectangle(int index, CRect &outrect);
	BOOL			GetPegPixelPosition(int index, CPoint &outpos);
	float			GetPegPositionFromPixel(int ypixel);


	afx_msg UINT	OnGetDlgCode();
	afx_msg void	OnPaint();
	afx_msg void	OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void	OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void	OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void	OnLButtonDblClk(UINT nFlags, CPoint point);



	CVisGradient	m_Gradient;
	int				m_SelectedPeg;
	CPoint			m_MouseDown;

	DECLARE_MESSAGE_MAP()
};

#endif