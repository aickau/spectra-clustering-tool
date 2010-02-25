#ifndef _VIS_MAIN_H
#define _VIS_MAIN_H

#include "vis_types.h"
#include "vis_dllexport.h"
#include "vis_compoundtypes.h"
#include "vis_typesintern.h"
#include "vis_cursor.h"
#include "vis_selection.h"
#include "vis_glhelper.h"
#include "vis_glview.h"
#include "vis_move.h"
#include "vis_colorbar.h"
#include "vis_app.h"


/** visualisation module.
 * CVisModule can be used as base class for window initialisation. 
 * if so, make sure to setup your project with MFC as shared DLL otherwise initialisation will fail.
 * 
 * All inserted datasets remain on user side that means they are not hold by CVisModule, so make sure they remain in memory until 
 * a) they are deleted with DeleteDataSet() or 
 * b) CVisModule is closed before datasets are deleted on user side.
 * That is especially important when datasets and CVisModule are hold by one class.
 *
 * And as always make sure user turns antialiasing off !!! Otherwise artifacts may occur (depending on used display driver).
 * I've no chance to control that by the app.
 */
class VIS_DLLEXPORT CVisModule : public CWnd, private CVisGLHelper
{
public:
	CVisModule();
	virtual ~CVisModule();

	/** create a visualisation window. this should only be called once. see CWnd::Create() for more details.
	 * @param dwStyle CWnd style. since this is an opengl view, this should contain CS_PARENTDC, WS_CLIPCHILDREN and WS_CLIPSIBLINGS,
	 * see SetPixelFormat() at msdn.
	 * @param rect window rectangle.
	 * @param pParentWnd pointer to the parent CWnd.
	 * @param nID ID of the child window.
	 * @return nonzero if successful, otherwise 0.
	 */ 
	virtual BOOL 			Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

	/** create a visualisation window. this should only be called once. see CWnd::Create() for more details.
	 * @param lpszClassName class name. 
	 * @param lpszWindowName window name. 
	 * @param dwStyle CWnd style.
	 * @param rect window rectangle.
	 * @param pParentWnd pointer to the parent CWnd.
	 * @param nID ID of the child window.
	 * @param pContext class name.
	 * @return nonzero if successful, otherwise 0.
	 */ 
	virtual BOOL			Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext);
	
	/** insert a dataset. 
	 * @param ds pointer to a CVisDataInterface object. this must be a non temporary dataset object. before you delete the dataset object, call DeleteDataSet().
	 * @param opt a CVisDataOptions object. 
	 * @return returns an unique, nonzero, positive object id. this id is used as name to identify the data object.
	 *	if dataset can not inserted (e.g. wrong pointers, indecies exceeds boundaries) etc. this returns -1. and the object is not displayed. 
	 */
	TVIS_OBJECTID			InsertDataSet(CVisDataInterface *ds, CVisDataOptions opt);

	/** remove dataset.
	 * @param id must be a valid object id from InsertDataSet()
	 */
	void					DeleteDataSet(TVIS_OBJECTID id);

	/** remove all datasets inserted with InsertDataSet().
	 */
	void					DeleteAllDataSets();

	/** set global options.
	 * @param vis_options a CVisGlobalOptions object. 
	 */
	void					SetGlobalOptions(CVisGlobalOptions vis_options);

	/** retrieve global options.
	 * @return CVisGlobalOptions object.
	 */
	CVisGlobalOptions		GetGlobalOptions();

	/** set data otions.
	 * @param id a valid object id from InsertDataSet()
	 * @param opt a CVisDataOptions object.
	 */
	void					SetDataOptions(TVIS_OBJECTID id, CVisDataOptions opt);

	/** get data otions. 
	 * @param id a valid object id from InsertDataSet()
	 * @return CVisDataOptions of object.
	 */
	CVisDataOptions			GetDataOptions(TVIS_OBJECTID id);

	/** get all selected points, lines, and triangles of an object. object selection is not taken into account.
	 * @param id a valid object id from InsertDataSet()
	 * @param points unsigned int stl vector in which all indices to selected points are written.
	 * @param lines unsigned int stl vector in which all indices to selected lines are written.
	 * @param triangles unsigned int stl vector in which all indices to selected triangles are written.
	 */
	void					GetSelectedItems(TVIS_OBJECTID id, std::vector<unsigned int> &points, std::vector<unsigned int> &lines,  std::vector<unsigned int> &triangles);

	/** get statusbar text with useful information. 
	 * call this every time you receive an VIS_MSGGETSTATUSTEXT command message.
	 * @return CString with statusbar text.
	 */
	CString					GetStatusbarText();

	///////////////////////////////////////////////////////////////////////////////////////
	// Commands
	///////////////////////////////////////////////////////////////////////////////////////

	/** set edit mode to rectangluar selection.
	 */
	void					CmdModeSelectRectangle();
	
	/** set edit mode to circular selection.
	 */
	void					CmdModeSelectCircle();

	/** set edit mode to select points.
	 */
	void					CmdModeSelectElementPoints();

	/** set edit mode to select lines.
	 */
	void					CmdModeSelectElementLines();

	/** set edit mode to select triangles.
	 */
	void					CmdModeSelectElementTriangles();

	/** select all elements/objects. by default this will take place at element level. 
	 * only when CmdSelectObjectType() is called before this takes place at object level.
	 */
	void					CmdSelectAll();

	/** invert all selected elements/objects. by default this will take place at element level. 
	 * only when CmdSelectObjectType() is called before this takes place at object level.
	 */
	void					CmdSelectInvert();

	/** select all elements of selected datasets. if no dataset is selected nothing happens.
	 */
	void					CmdSelectElementsOfSelectedObjects();

	/** set to object selection. the next and only the next selection process will take place at object level.
	*/
	void					CmdSelectObject();

	/** set to object selection / deselection. 
	 * the next and only the next deselection / selection process will take place at object level.
	 * selection or deselection depends on selected mode set by CmdModeSelectRectangle(), 
	 * CmdModeSelectCircle(), CmdModeDeselectRectangle, CmdModeDeselectCircle()
	 */
	void					CmdSelectObjectType();
	
	/** set edit mode to rectangle deselection.
	 */
	void					CmdModeDeselectRectangle();
	
	/** set edit mode to circular deselection.
	 */
	void					CmdModeDeselectCircle();

	/** set edit mode to deselect points.
	 */
	void					CmdModeDeselectElementPoints();

	/** set edit mode to deselect lines.
	 */
	void					CmdModeDeselectElementLines();
	
	/** set edit mode to deselect triangles.
	 */
	void					CmdModeDeselectElementTriangles();


	/** deselect all elements of selected datasets. if no dataset is selected nothing happens.
	 */
	void					CmdDeselectElementsOfSelectedObjects();
	
	/** set to object deselection. the next and only the next deselection process will take place at object level.
	 */
	void					CmdDeselectObject();
	
	/** deselect all elements/objects. by default this will take place at element level. 
	 * only when CmdSelectObjectType() is called before this takes place at object level.
	 */	
	void					CmdDeselectAll();

	
	/** set edit mode to move.
	 */
	void					CmdModeMove();
	
	/** set edit mode to rotate.
	 */
	void					CmdModeRotate();

	/** set edit mode to zoom per point.
	 */
	void					CmdModeZoomPoint();

	/** set edit mode to zoom per rectangle.
	 */
	void					CmdModeZoomFrame();


	/** set edit mode to 'set rotation center'. 
	 * user has to select a point (in existing datasets) which will be the center point for further rotations.
	 * after mouse right click user is automatically in rotation mode.
	 */
	void					CmdSetRotationCenter();

	/** fit all objects to current viewport.
	 */
	void					CmdFitView();
	
	/** fit all selected objects to current viewport, if no object was selected, fit all abjects..
	 */
	void					CmdFitViewSelected();

	/** toggle between fullscreen and 4-view mode.
	 */
	void					CmdToggleFullScreen();

	/** unhide all hidden objects.
	 */
	void					CmdUnhideAll();


	/** switch lighting on/off for all selected objects.
	 * this function should be integrated into a toolbar.
	 */
	void					CmdLightingOnOff();

	/** switch display triangles/solids on/off for all selected objects.
	 * this function should be integrated into a toolbar.
	 */
	void					CmdSolidOnOff();

	/** switch display lines on/off for all selected objects. 
	 * this function should be integrated into a toolbar.
	 */
	void					CmdWireFrameOnOff();

	/** switch display points on/off for all selected objects.
	 * this function should be integrated into a toolbar.
	 */
	void					CmdPointsOnOff();

	/** set color code for points, lines, triangles to single color. 
	 * this function should be integrated into a toolbar.
	 */
	void					CmdColorCodeSingle();

	/** set color code for points, lines, triangles to x-axis. 
	 * this function should be integrated into a toolbar.
	 */
	void					CmdColorCodeXAxis();

	/** set color code for points, lines, triangles to y-axis. 
	 * this function should be integrated into a toolbar.
	 */
	void					CmdColorCodeYAxis();
	
	/** set color code for points, lines, triangles to z-axis. 
	 * this function should be integrated into a toolbar.
	 */
	void					CmdColorCodeZAxis();

	/** set color code for points, lines, triangles to intensity integer buffer. 
	 * this function should be integrated into a toolbar.
	 */
	void					CmdColorCodeIntensityInt();

	/** set color code for points, lines, triangles to intensity fp buffer. 
	 * this function should be integrated into a toolbar.
	 */
	void					CmdColorCodeIntensityFloat();

	/** set color code for points, lines, triangles to color int buffer. 
	 * this function should be integrated into a toolbar.
	 */
	void					CmdColorCodeColor();

	/** set viewport type for selected viewport. 
	 * @param type can be one of the following: VP_FRONT, VP_LEFT, VP_TOP and VP_PERSPECTIVE.
	 */
	void					CmdViewport(TVIS_VIEWPORT_TYPE type);

	/** set current viewport type to front. this is the same as calling CmdViewport(VP_FRONT);
	 */
	void					CmdViewportFront();

	/** set current viewport type to top. this is the same as calling CmdViewport(VP_TOP);
	 */
	void					CmdViewportTop(); 

	/** set current viewport type to left. this is the same as calling CmdViewport(VP_LEFT);
	 */
	void					CmdViewportLeft(); 

	/** set current viewport type to perspective. this is the same as calling CmdViewport(VP_PERSPECTIVE);
	 */
	void					CmdViewportPerspective(); 

	/** opens dialog object selection.
	*/
	void					CmdDlgSelectObject();
	
	/** opens dialog for global options.
	*/
	void					CmdDlgGlobalOptions();

	/** opens dialog for dataset options.
	*/
	void					CmdDlgDataOptions();

	/** set mode to point info. if user selects a point appropriate information is displayed.
	 */
	void					CmdInfoPoints(); 

	/** set mode to line info. if user selects a point appropriate information is displayed.
	 */
	void					CmdInfoLines(); 
	
	/** set mode to triangle info. if user selects a point appropriate information is displayed.
	 */
	void					CmdInfoTriangles(); 

	/** no colorbar info. 
	 * if colorbar is enabled this function set displayinformationtype which is used by the colorbar.
	 */
	void					CmdShowColorBarInfoNone();

	/** colorbar info points. 
	 * if colorbar is enabled this function set displayinformationtype which is used by the colorbar.
	 */
	void					CmdShowColorBarInfoPoints();

	/** colorbar info lines.
 	 * if colorbar is enabled this function set displayinformationtype which is used by the colorbar.
 	 */
	void					CmdShowColorBarInfoLines();

	/** colorbar info triangles.
	 * if colorbar is enabled this function set displayinformationtype which is used by the colorbar.
	 */
	void					CmdShowColorBarInfoTriangles();

protected:					
	virtual BOOL			PreCreateWindow(CREATESTRUCT& cs);
	
	afx_msg BOOL			OnSetCursor(CWnd*, UINT, UINT); 
							
	afx_msg int			OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void			OnSize(UINT nType, int cx, int cy);
	afx_msg void			OnPaint();
	afx_msg BOOL			OnEraseBkgnd( CDC* pDC );
	afx_msg void			OnTimer(UINT nIDEvent);
	afx_msg void			OnDestroy();
	afx_msg void			OnMouseMove(UINT, CPoint);
	afx_msg void			OnLButtonDown(UINT, CPoint);
	afx_msg void			OnLButtonUp(UINT, CPoint);
	afx_msg void			OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void			OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags); 
	afx_msg void			OnContextMenu(CWnd *pWnd, CPoint pos);
	afx_msg void			OnUpdateAll();
	afx_msg LRESULT			OnHotKey(WPARAM wParam, LPARAM lParam);
	

private:
	void					Update(int item);
	void					UpdateGlobalDimensions();
	void					UpdateColorCode();
	void					UpdateSizeBuffers();
	void					UpdateRanges();

	TVIS_RANGE				GetRangeFromColorCodePoints();
	TVIS_RANGE				GetRangeFromColorCodeLines();
	TVIS_RANGE				GetRangeFromColorCodeTriangles();

	void					ActionGlobal(TVIS_USERSTATEFLAGS flags);
	void					ActionArrow(TVIS_USERSTATEFLAGS flags);
	void					ActionPosition(TVIS_USERSTATEFLAGS flags);
	void					ActionRotate(TVIS_USERSTATEFLAGS flags);
	void					ActionZoomPoint(TVIS_USERSTATEFLAGS flags);
	void					ActionZoomFrame(TVIS_USERSTATEFLAGS flags);
	void					ActionSelect(TVIS_USERSTATEFLAGS flags);
	void					ActionInfo(TVIS_USERSTATEFLAGS flags);
	void					ActionSetRotationCenter(TVIS_USERSTATEFLAGS flags);
	bool					ActionViewportRatioChange(TVIS_USERSTATEFLAGS flags);

	void					DoSelection(CPoint &mouse_pos, TVIS_CIRCLE2D &sel_circle, TVIS_RECT2D &sel_rect);

	/** draw all four viewports.
	 */
	void					DrawViews();

	/** draw one viewport.
	 * @param view_num viewport number. (0..3) where 0 ist the upper left, following in counter clockwise order.
	 * other values outside (0..3) are ignored.
	 */
	void					DrawViews(int view_num);
	void					DrawFramesOnly();
	void					DrawSelection(bool clear_old);

	/** clear client screen with background color.
	 */
	void					ClearGLClientScreen();

	
	/** draw color bar on right side.
	 */
	void					DrawColorBar();
	
	/** checks if colorbar is used.
	 * @return if colorbar used, width in pixels otherwise 0.
	 */
	int						IsColorBarUsed();

	/** update statusbar text and send VIS_MSGGETSTATUSTEXT to parent window.
	 * @param mode if no mouse button is pressed set this to true (mouse position is shown in statusbar) otherwise false (other information depending on current state are shown). 
	 */
	void					SetStatusBar(bool mode);

	/** set Cursorshape depending on current user states.
	 */
	void					SetCusor();
	
	/** highlights a viewport depending on current mouse position and changes viewSelected to the current 
	 * viewport number. if no viewport was selected all highlights get disabled.
	 */
	void					SetViewportSelection();

	/** load and register textures to opengl
	*/
	void					LoadTextures();

	/** send a WM_COMMAND message to the parent window.
	*/
	void					SendCommandMessageToParent(int id);

	// screen elements
	CVisGLView				*view;


	const int				viewNum;						// number of views=4
	int						viewSelected;					// current selected view
	bool					viewFullscreen;					// fullscreen view (selected)
	CRect					viewMidBorderRectX;				// vertical area between the views
	CRect					viewMidBorderRectY;				// horizontal area between the views


	// data
	CVisGlobalOptions		global_options;					
	TVIS_LIST_DATAINTERN	dsets;							// contains all objects


	TVIS_POINT4D			dimensions_min;					// global dimensions of all (!) objects
	TVIS_POINT4D			dimensions_max;
	TVIS_DATASETPARAMRANGE	dimensions_ds;					// dataset buffer ranges of all objects
	
	
	TVIS_USERSTATE			usr_state;						// ST_ARROW, ST_POSITION, ST_ROTATION, ST_ZOOMPOINT, ST_ZOOMFRAME


	bool					drag_midx;						// color code drag
	bool					drag_midy;						// color code drag
	bool					drag_cc_begin;					// color code bar begin
	bool					drag_cc_end;					// color code bar end

	bool					mouse_drag_begin;				// mouse start postion (if left mbutton pressed)
	CPoint					mouse_drag_vec;					// drag vector (if left mbutton pressed)
	CPoint					mouse_drag_pos;					// mouse position (if left mbutton pressed)
	CPoint					mouse_drag_beginpos;			// m start position (if left mbutton pressed)

	int						cb_space;						// space of view used by colorbar
	TVIS_COLORBARINFO		cb_type;						// colorbar information type

	CVisSelection			selection;						// performs element / object selection
	CVisSelectionDraw		selectiondraw;					// handles selection drawing stuff
	CVisCursor				cursor;							// to modify cursor shapes
	CVisMove				move;							// viewport move/rotate/zoom unit
	CVisColorBar			colorbar;						// to display the colortable

	CDC						*pcDC;
	HDC						hDC;							// windows device contex
	HGLRC					hGLRC;							// gl rendering handle
	CPoint					wndSize;						// window size in pixels
	CPoint					wndViewPos;						// gl-viewport position in pixels
	CPoint					wndViewSize;					// gl-viewport size in pixels

	CString					statusbartext;					// string for additional user information.

	bool					module_initialized;

	CPoint					lastpopuppos;

	#ifdef _DEBUG
	GLenum					gl_error;
	#endif

	
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNCREATE(CVisModule)
};



#endif


