#ifndef _VIS_GLVIEW_H
#define _VIS_GLVIEW_H

#include "vis_compoundtypes.h"
#include "vis_typesintern.h"
#include "vis_glhelper.h"
#include "vis_projection.h"
#include "vis_font.h"
#include "vis_debug.h"

/// viewport module. all data objects are drawn here
class CVisGLView : public CVisProjection
{
public:
	CVisGLView();
	~CVisGLView();

	/** create viewport.
	 * @param wnd pointer to parent window. zero would be deadly.
	 * @param pt_global_options pointer to globabal options. zero would be deadly.
	 * @param pt_vis_datasets pointer to list with all datasets objects. 
	 * @param pt_dim_min pointer to lower left near point of bounding box in which all datasets lie.
	 * @param pt_dim_max pointer to upper right far point of bounding box in which all datasets lie.
	 * @param pt_usr_state pointer to user state. for details see TVIS_USERSTATE.
	 * @param pt_client_height pointer to parent windows client hight in pixels.
	 */
	void					Create(CWnd *wnd, CVisGlobalOptions *pt_global_options, TVIS_LIST_DATAINTERN *pt_vis_datasets, TVIS_POINT4D *pt_dim_min, TVIS_POINT4D *pt_dim_max, TVIS_USERSTATE *pt_usr_state, int *pt_client_height);	

	/** set viewport type. 
	 * @param type can be one of the following: VP_FRONT, VP_LEFT, VP_TOP and VP_PERSPECTIVE.
	 */
	void					SetViewportType(TVIS_VIEWPORT_TYPE type);

	/** get viewport type. 
	 * @return VP_FRONT, VP_LEFT, VP_TOP or VP_PERSPECTIVE.
	 */
	TVIS_VIEWPORT_TYPE		GetViewportType();

	/** get position.
	 * @return position in viewport coordinates. 
	 */
	TVIS_POINT4D			GetPosition();
	
	/** set position in viewport coordinates. 
	 */
	void					SetPosition(TVIS_POINT4D pos);

	/** get 4x4 rotation matrix. this can contain also translation values for rotation center. 
	 * so when multiplying with this matrix make sure w coordinate is set to 1.0.
	 * @return rotation matrix.
	 */
	TVIS_MATRIX4D			GetRotationMatrix();

	/** set rotation in axis-angle representation. previous rotations get overwritten.
	 * @param axis_angle point with rotation axis (x,y,z) and angle (w) in degrees.
	 */
	void					SetAbsoluteRotation(TVIS_POINT4D &axis_angle);

	/** set rotation in axis-angle representation. relative to current rotation.
	 * @param axis_angle point with rotation axis (x,y,z) and angle (w) in degrees.
	 */
	void					SetRelativeRotation(TVIS_POINT4D &axis_angle);

	/** set rotation in axis-angle representation. previous rotations get overwritten.
	 * @param axis_angle point with rotation axis (x,y,z) and angle (w) in degrees.
	 * @param reference_point rotation center point.
	 */
	void					SetAbsoluteRotation(TVIS_POINT4D &axis_angle, TVIS_POINT4D &reference_point);

	/** set rotation in axis-angle representation. relative to current rotation.
	 * @param axis_angle point with rotation axis (x,y,z) and angle (w) in degrees.
	 * @param reference_point rotation center point.
	 */
	void					SetRelativeRotation(TVIS_POINT4D &axis_angle, TVIS_POINT4D &reference_point);
	
	/** get ray position and raydirection at the current mouse position.
	 * @param mouse_position mouse- or any other position in pixel coordinates.
	 * @param ray_position output ray position in viewport coordinates.
	 * @param ray_direction output ray direction in viewport coordinates.
	 */
	void					GetRay(CPoint &mouse_position, TVIS_POINT4D &ray_position, TVIS_POINT4D &ray_direction);

	/** draw entire viewport.
	 */
	void					Draw();

	/** draw outer frame of viewport.
	 */
	void					DrawFrameOnly();

	/** draw a dataset into selectionbuffer and return index if an element was hit at mouse position.
	 * @return index to element if an object was hit otherwise -1.
	 */
	int						DrawSelection(CVisDataIntern *pDS, CPoint &mouse_pos, TVIS_SELECTION_SHAPE element_type);
	
	/** fit viewport so that all or selected objects can be seen by the user.
	 * @param selected_only if set to true only selected objects get fit to viewport, otherwise all objects.
	 */
	void					FitView(bool selected_only);
private:
	void					FitView(TVIS_POINT4D &dim_min, TVIS_POINT4D &dim_max);
public:

	/** mark window as selected window. selected means the window frame color is different
	 * to gain user feedback. no other functionality.
	 * @param selected set to true if window should be selected otherwise false. 
	 */
	void					Select(bool selected);

	/* return selection state.
	 */
	bool					IsSelected();

	/** show or hide window. this function is used to disable/enable windows.
	 * @param set to true if window should be visible otherwise false. 
	 */
	void					Show(bool visible);

	/* return window visibility state.
	 * @return true if window is visible, otherwise false.
	 */
	bool					IsVisible();

	/* get information about grid.
	 * @param unit out value: unit string e.g. "cm","mm"
	 * @param grid out value: string with all grid information, e.g. "Grid: 5 m", which says that the smaller grid cells are 5x5 m  
	 * @param unitfactor out value: unitfactor.  
	 */
	void					GetGridInformation(CString &unit, CString &grid, double &unitfactor);
	
	/** convert client coordinates into viewport coordinates. 
	 * client coordinates are the coordinates of the client window, where one (or more) gl viewports are installed.
	 * viewport coordinates are the pixel (integer coordinates) of the gl viewport. (0,0) is at the lower left corner, see glViewport() for details.
	 * @param cl_point input client coordinates
	 * @param vp_point output viewport coordinates
	 */
	void					ClientToViewport(CPoint &cl_point, CPoint &vp_point);

	/** convert client coordinates into 2D screen coordinates. 
	 * client coordinates are the coordinates of the client window, where one (or more) gl viewports are installed.
	 * 2D screen coordinates are floating point coordinates of the gl window, see glOrtho() for more details.
	 * @param cl_point input client coordinates
	 * @param screen_point output screen coordinates
	 */
	void					ClientToScreen(CPoint &cl_point, TVIS_POINT2D &screen_point);
//	void					ScreenToClient(TVIS_POINT2D &screen_point, CPoint &cl_point);

	/** convert client coordinates into 3D viewport coordinates. 
	 * client coordinates are the coordinates of the client window, where one (or more) gl viewports are installed.
	 * 3D viewport coordinates are floating point coordinates of the 3d viewport space, see glOrtho() for more details.
	 * @param cl_point input client coordinates
	 * @param screen_point output viewport coordinates
	 */
	void					ClientTo3DSpace(CPoint &cl_point, TVIS_POINT4D &space_point, bool transform_before_rotate = false);
	

private:

	// additional items to draw
	bool					DrawPoints(TVIS_LIST_DATAINTERN::iterator it);
	bool					DrawLines(TVIS_LIST_DATAINTERN::iterator it);
	bool					DrawTriangles(TVIS_LIST_DATAINTERN::iterator it);
	void					DrawFrame();
	void					DrawGrid();
	void					DrawTrackball();
	void					DrawCoordinateSystem();
	void					DrawCoordinateSystemModel(TVIS_LIST_DATAINTERN::iterator it);
	void					DrawObjectSelection(TVIS_LIST_DATAINTERN::iterator it);
	void					DrawObjectName(TVIS_LIST_DATAINTERN::iterator it);
	void					DrawBoundingBox(TVIS_LIST_DATAINTERN::iterator it);

	void					UpdateViewportType(TVIS_POINT4D &axis_angle);

	bool					selected;
	bool					visible;

	TVIS_POINT4D			position;					// dataset positions

	TVIS_MATRIX4D			rotation_matrix_abs;		// absolute rotation
	TVIS_MATRIX4D			rotation_matrix_rel;		// relative roation to absolute rotation

	CVisGlobalOptions		*pt_global_options;
	TVIS_LIST_DATAINTERN	*pt_vis_datasets;

	TVIS_POINT4D			*pt_dim_min;
	TVIS_POINT4D			*pt_dim_max;

	TVIS_USERSTATE			*pt_usr_state;

	CWnd					*wnd;

	CString					viewport_name;
	TVIS_VIEWPORT_TYPE		viewport_type;

// temp:
	TVIS_POINT4D			reference_point;
};



#endif