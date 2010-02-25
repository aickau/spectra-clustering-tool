#ifndef _VIS_GRADIENTDLG_H
#define _VIS_GRADIENTDLG_H

#include "vis_types.h"
#include "resource.h"
#include "vis_gradientctrl.h"
#include "ColorButton/vis_colorbutton.h"
#include "vis_colortable.h"

/// gradient control dialog class. we use CGradient & CGradientCtrl
class CVisDlgGradientCtrl : public CDialog
{
public:
	CVisDlgGradientCtrl(CVisColorTable *coltab);
	~CVisDlgGradientCtrl();

	enum				{ IDD = IDD_GRADIENTCTRL};


private:
	CVisGradientCtrl	m_Gradient;

	CVisColorButton		m_NewPegColor;
	CVisColorButton		m_SelPegColor;

	CEdit				m_wndQuantizationEntries;
	CComboBox			m_wndMethodCombo;
	CSpinButtonCtrl		m_wndQuantizationSpin;

	float				m_NewPegPosition;
	float				m_SelPegPosition;
	BOOL				m_EnableQuantization;
	int					m_QuantizationEntries;
	bool				m_InitFinished;

	CVisColorTable		*m_pColorTable;


protected:
	virtual BOOL		OnInitDialog();
	virtual void		DoDataExchange(CDataExchange* pDX);

	afx_msg void		OnOK();
	afx_msg void		OnAddPeg();
	afx_msg void		OnDelPeg();
	afx_msg void		OnImport();
	afx_msg void		OnExport();
	afx_msg long		OnSelEndOK(UINT /*lParam*/, LONG /*wParam*/);
	afx_msg void		OnChangeSelPegPosition();
	afx_msg void		OnSelchangeMethodCombo();
	afx_msg void		OnEnableQuantization();
	afx_msg void		OnChangeQuantizationEntries();
	afx_msg void		OnNotifyChangeSelPeg(NMHDR * pNotifyStruct, LRESULT *result);
	afx_msg void		OnNotifyPegMove(NMHDR * pNotifyStruct, LRESULT *result);
	afx_msg void		OnNotifyPegMoved(NMHDR * pNotifyStruct, LRESULT *result);
	afx_msg void		OnNotifyPegRemoved(NMHDR * pNotifyStruct, LRESULT *result);

	DECLARE_MESSAGE_MAP()
};

#endif