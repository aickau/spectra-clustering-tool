#ifndef _VIS_DIALOGS_H
#define _VIS_DIALOGS_H

#include "vis_types.h"
#include "resource.h"
#include "vis_colortable.h"
#include "vis_colortable.h"
#include "ColorButton/vis_colorbutton.h"



/** Dialog Helper class
 */
class CVisDlgHelper
{
public:

	/** enables or disables a dialog control.
	 * @param pDlg pointer to the dialog control.
	 * @param state if true control will be enabled otherwise it will be disabled.
	 * @param ID control id.
	 */
	void SetItemsEnableState(CDialog *pDlg, bool state, int nID);

	/** enables or disables a set of dialog controls.
	 * @param pDlg pointer to the dialog control.
	 * @param state if true control will be enabled otherwise it will be disabled.
	 * @param pnID pointer to a list of control ids.
	 * @param numID number of control ids.
	 */
	void SetItemsEnableState(CDialog *pDlg, bool state, int *pnID, int numID);
};



/** an object selection dialog. 
 - if select_only_one = true, call GetSelectedItem() after DoModal() returned IDOK. if DoModal() doesn't return IDOK GetSelectedItem() returns -1
 - if select_only_one = false, the dialog sets all CVisDataOptions::Selected flags automatically
*/
class CVisDlgSelectObject : public CDialog
{
public:
	CVisDlgSelectObject(TVIS_LIST_DATAINTERN *dataset, bool select_only_one);

	enum					{ IDD1 = IDD_SELECTOBJECT1 };	// single selection
	enum					{ IDD2 = IDD_SELECTOBJECT2 };	// multiple selection

	int						GetSelectedItem();
							
private:					
	CListBox				m_lb;
							
	TVIS_LIST_DATAINTERN	*m_pDataset;

	bool					m_SelectOnlyOne;
	int						m_SelectedItem;

							
protected:					
	virtual BOOL			OnInitDialog();
	virtual void			DoDataExchange(CDataExchange* pDX);
	virtual void			OnOK();

	DECLARE_MESSAGE_MAP()
};


/** range adjustment dialog.
 * @param range the range to adjust.
 * @param source_only if true source values are only written.
 */
class CVisDlgMapping : public CDialog, public CVisDlgHelper
{
public:
	CVisDlgMapping(TVIS_RANGE &range, TVIS_RANGE &range_auto, bool source_only);
	CVisDlgMapping(TVIS_RANGE &range, bool source_only);

	enum					{ IDD1 = IDD_MAPPINGUNIT1 };
	enum					{ IDD2 = IDD_MAPPINGUNIT2 };

private:
	bool					m_SourceOnly;
	TVIS_RANGE				*m_pRange;
	TVIS_RANGE				*m_pRangeAuto;

	CButton					m_Check;
	CEdit					m_Edit[4];

protected:					
	virtual BOOL			OnInitDialog();
	virtual void			DoDataExchange(CDataExchange* pDX);
	virtual void			OnOK();

	afx_msg	void			OnUpdateControlStates();

	DECLARE_MESSAGE_MAP()
};


class CVisDlgInfo : public CDialog
{
public:

	CVisDlgInfo(CString &infotext);

	enum					{IDD = IDD_INFO};
	
protected:					
	virtual BOOL			OnInitDialog();

	DECLARE_MESSAGE_MAP()
private:
	CString					m_InfoText;
};



#endif

