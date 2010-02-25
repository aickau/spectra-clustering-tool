#ifndef _VIS_DIALOGSDATAOPTIONS_H
#define _VIS_DIALOGSDATAOPTIONS_H

#include "vis_types.h"
#include "resource.h"
#include "vis_colortable.h"
#include "vis_colortable.h"
#include "ColorButton/vis_colorbutton.h"
#include "vis_dialogs.h"


/** data options dialog base class
 */
class CVisDlgDataOptionsBase : public CPropertyPage, public CVisDlgHelper
{
public:
	CVisDlgDataOptionsBase(UINT idd, CWnd *wnd);

	CWnd					*m_pModuleWnd;

protected:
	virtual BOOL			OnSetActive();
	virtual BOOL			OnApply();

};



/** data options dialog - general
 */
class CVisDlgDataOptionsGeneral : public CVisDlgDataOptionsBase
{
public:
	CVisDlgDataOptionsGeneral(CWnd *wnd, CVisDataIntern *ds);

	enum					{ IDD = IDD_DATAOPTIONS1 };

private:	

	CButton					m_Check1;
	CButton					m_Check2;
	CButton					m_Check3;

	CVisDataIntern			*m_pDataSet;

protected:					
	virtual BOOL			OnInitDialog();
	virtual void			DoDataExchange(CDataExchange* pDX);
	virtual void			OnOK();
	
	afx_msg void			OnPaint();

	DECLARE_MESSAGE_MAP()
};



/** data options dialog - points
 */
class CVisDlgDataOptionsPoints : public CVisDlgDataOptionsBase
{
public:
	CVisDlgDataOptionsPoints(CWnd *wnd, CVisDataIntern *ds, int point_size_max);

	enum					{ IDD = IDD_DATAOPTIONS2 };

private:	

	CVisColorButton			m_Color;
	CButton					m_Check;
	CButton					m_Radio[6];
	CComboBox				m_Combo;
	CSpinButtonCtrl			m_Spin;

	const int				m_PointSizeMax;
	int						m_PointSize;

	CVisDataIntern			*m_pDataSet;

protected:					
	virtual BOOL			OnInitDialog();
	virtual void			DoDataExchange(CDataExchange* pDX);
	virtual void			OnOK();

	afx_msg	void			OnUpdateControlStates();


	DECLARE_MESSAGE_MAP()
};


/** data options dialog - lines
 */
class CVisDlgDataOptionsLines : public CVisDlgDataOptionsBase
{
public:
	CVisDlgDataOptionsLines(CWnd *wnd, CVisDataIntern *ds, int line_width_max);

	enum					{ IDD = IDD_DATAOPTIONS3 };

private:	

	CVisColorButton			m_Color;
	CButton					m_Check;
	CButton					m_Radio[6];
	CComboBox				m_Combo;
	CSpinButtonCtrl			m_Spin;

	const int				m_LineWidthMax;
	int						m_LineWidth;

	CVisDataIntern			*m_pDataSet;

protected:					
	virtual BOOL			OnInitDialog();
	virtual void			DoDataExchange(CDataExchange* pDX);
	virtual void			OnOK();

	afx_msg	void			OnUpdateControlStates();


	DECLARE_MESSAGE_MAP()
};



/** data options dialog - triangles
 */
class CVisDlgDataOptionsTriangles : public CVisDlgDataOptionsBase
{
public:
	CVisDlgDataOptionsTriangles(CWnd *wnd, CVisDataIntern *ds);

	enum					{ IDD = IDD_DATAOPTIONS4 };

private:	

	CVisColorButton			m_Color;
	CButton					m_Check1;
	CButton					m_Check2;
	CButton					m_Check3;
	CButton					m_Radio[2];

	CVisDataIntern			*m_pDataSet;

protected:					
	virtual BOOL			OnInitDialog();
	virtual void			DoDataExchange(CDataExchange* pDX);
	virtual void			OnOK();

	afx_msg	void			OnUpdateControlStates();


	DECLARE_MESSAGE_MAP()
};

/** data options dialog - combines CVisDlgDataOptionsGeneral, CVisDlgDataOptionsPoints, CVisDlgDataOptionsLines, CVisDlgDataOptionsTriangles
 */
class CVisDlgDataOptions : public CPropertySheet
{
public:
	CVisDlgDataOptions(CWnd *wnd, CVisDataIntern *ds, int point_size_max, int line_width_max, UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

	virtual ~CVisDlgDataOptions();

protected:
	DECLARE_MESSAGE_MAP()
	
	virtual BOOL			OnInitDialog();

private:
	DECLARE_DYNAMIC(CVisDlgDataOptions)

	CVisDlgDataOptionsGeneral	m_PropGeneral;
	CVisDlgDataOptionsPoints	m_PropPoints;
	CVisDlgDataOptionsLines		m_PropLines;
	CVisDlgDataOptionsTriangles	m_PropTriangles;
};


#endif