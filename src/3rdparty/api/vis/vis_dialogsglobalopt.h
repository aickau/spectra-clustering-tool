#ifndef _VIS_DIALOGSGLOBALOPTIONS_H
#define _VIS_DIALOGSGLOBALOPTIONS_H

#include "vis_types.h"
#include "resource.h"
#include "vis_colortable.h"
#include "vis_colortable.h"
#include "ColorButton/vis_colorbutton.h"
#include "vis_dialogs.h"



/** data options dialog base class
 */
class CVisDlgGlobalOptionsBase : public CPropertyPage, public CVisDlgHelper
{
public:
	CVisDlgGlobalOptionsBase(UINT idd, CWnd *wnd);

	CWnd					*m_pModuleWnd;

protected:
	virtual BOOL			OnSetActive();
	virtual BOOL			OnApply();

};


/** global options dialog - general.
 */
class CVisDlgGlobalOptionsGeneral : public CVisDlgGlobalOptionsBase
{
public:
	CVisDlgGlobalOptionsGeneral(CWnd *wnd, CVisGlobalOptions *opt);

	enum					{ IDD = IDD_GLOBALOPTIONS1 };
private:
	CVisColorButton			m_Color[8];
	CButton					m_Check[7];
	CSpinButtonCtrl			m_Spin;

	CVisGlobalOptions		*m_pOpt;

	int						m_UnitShift;
	bool					m_IsInitialized;

protected:					
	virtual BOOL			OnInitDialog();
	virtual void			DoDataExchange(CDataExchange* pDX);
	virtual void			OnOK();

	afx_msg void			OnUpdateControlStates();
	afx_msg void			UpdateUnitInfo();
	
	afx_msg void			OnPaint();

	DECLARE_MESSAGE_MAP()
};


/** global options dialog - general dataset.
 */
class CVisDlgGlobalOptionsGeneralData : public CVisDlgGlobalOptionsBase
{
public:
	CVisDlgGlobalOptionsGeneralData(CWnd *wnd, CVisGlobalOptions *opt);

	enum					{ IDD = IDD_GLOBALOPTIONS2 };

private:
	CVisGlobalOptions		*m_pOpt;

	CButton					m_Btn;

	CVisColorTable			m_ColorTable;
	TVIS_POINT4D			m_Scale;

protected:					
	virtual BOOL			OnInitDialog();
	virtual void			DoDataExchange(CDataExchange* pDX);
	virtual void			OnOK();
	
	afx_msg void			OnColorTable();

	DECLARE_MESSAGE_MAP()
};


/** global options dialog - points.
 */
class CVisDlgGlobalOptionsPoints : public CVisDlgGlobalOptionsBase
{
public:
	CVisDlgGlobalOptionsPoints(CWnd *wnd, CVisGlobalOptions *opt, TVIS_DATASETPARAMRANGE *range, int point_size_max);

	enum					{ IDD = IDD_GLOBALOPTIONS3 };

private:
	const int				m_PointSizeMax;
	int						m_PointSize;
	TVIS_RANGE				m_Range[7];

	TVIS_DATASETPARAMRANGE	*m_pDatasetRange;
	
	CVisGlobalOptions		*m_pOpt;
		

	CVisColorButton			m_Color;
	CButton					m_Radio[10];
	CComboBox				m_Combo;
	CSpinButtonCtrl			m_Spin;

protected:					
	virtual BOOL			OnInitDialog();
	virtual void			DoDataExchange(CDataExchange* pDX);
	virtual void			OnOK();

	afx_msg	void			OnUpdateControlStates();
	afx_msg	void			OnMap1();
	afx_msg	void			OnMap2();
	afx_msg	void			OnMap3();
	afx_msg	void			OnMap4();
	afx_msg	void			OnMap5();
	afx_msg	void			OnMap6();
	afx_msg	void			OnMap7();

	DECLARE_MESSAGE_MAP()
};


/** global options dialog - lines.
 */
class CVisDlgGlobalOptionsLines : public CVisDlgGlobalOptionsBase
{
public:
	CVisDlgGlobalOptionsLines(CWnd *wnd, CVisGlobalOptions *opt, TVIS_DATASETPARAMRANGE *range, int line_width_max);

	enum					{ IDD = IDD_GLOBALOPTIONS4 };

private:
	const int				m_LineWidthMax;
	int						m_LineWidth;
	TVIS_RANGE				m_Range[7];

	TVIS_DATASETPARAMRANGE	*m_pDatasetRange;

	CVisGlobalOptions		*m_pOpt;

	CVisColorButton			m_Color;
	CButton					m_Radio[10];
	CComboBox				m_Combo;
	CSpinButtonCtrl			m_Spin;

protected:					
	virtual BOOL			OnInitDialog();
	virtual void			DoDataExchange(CDataExchange* pDX);
	virtual void			OnOK();

	afx_msg	void			OnUpdateControlStates();
	afx_msg	void			OnMap1();
	afx_msg	void			OnMap2();
	afx_msg	void			OnMap3();
	afx_msg	void			OnMap4();
	afx_msg	void			OnMap5();
	afx_msg	void			OnMap6();
	afx_msg	void			OnMap7();


	DECLARE_MESSAGE_MAP()
};


/** global options dialog - triangles.
 */
class CVisDlgGlobalOptionsTriangles : public CVisDlgGlobalOptionsBase
{
public:
	CVisDlgGlobalOptionsTriangles(CWnd *wnd, CVisGlobalOptions *opt, TVIS_DATASETPARAMRANGE *range);

	enum					{ IDD = IDD_GLOBALOPTIONS5 };
	
private:
	CVisGlobalOptions		*m_pOpt;

	TVIS_DATASETPARAMRANGE	*m_pDatasetRange;

	TVIS_RANGE				m_Range[5];

	CVisColorButton			m_Color;
	CButton					m_Radio[7];

protected:					
	virtual BOOL			OnInitDialog();
	virtual void			DoDataExchange(CDataExchange* pDX);
	virtual void			OnOK();
	
	afx_msg	void			OnUpdateControlStates();
	afx_msg	void			OnMap1();
	afx_msg	void			OnMap2();
	afx_msg	void			OnMap3();
	afx_msg	void			OnMap4();
	afx_msg	void			OnMap5();


	DECLARE_MESSAGE_MAP()
};




/** global options dialog - combines CVisDlgGlobalOptionsGeneral, 
 * CVisDlgGlobalOptionsPoints, CVisDlgGlobalOptionsLines, CVisDlgGlobalOptionsTriangles.
 */
class CVisDlgGlobalOptions : public CPropertySheet
{
public:
	CVisDlgGlobalOptions(CWnd *wnd, CVisGlobalOptions *opt, TVIS_DATASETPARAMRANGE *range,	int point_size_max, int line_width_max, UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

	virtual ~CVisDlgGlobalOptions();

protected:
	DECLARE_MESSAGE_MAP()
	
	virtual BOOL					OnInitDialog();

private:
	DECLARE_DYNAMIC(CVisDlgGlobalOptions)

	CVisDlgGlobalOptionsGeneral		m_PropGeneral;
	CVisDlgGlobalOptionsGeneralData	m_PropGeneralData;
	CVisDlgGlobalOptionsPoints		m_PropPoints;
	CVisDlgGlobalOptionsLines		m_PropLines;
	CVisDlgGlobalOptionsTriangles	m_PropTriangles;
};




#endif