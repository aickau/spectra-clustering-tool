#ifndef _VIS_MOVE_H
#define _VIS_MOVE_H

#include "vis_compoundtypes.h"
#include "vis_typesintern.h"
#include "vis_glview.h"
#include "vis_selection.h"

/** resposible for move, rotate trackball & zoom
*/
class CVisMove
{
public:
	CVisMove();

	/** move viewport along the viewport axis. call this every time a new movement starts.
	 */
	void					MoveBegin();

	/** move viewport along the viewport axis.
	 * @param view viewport to move.
	 * @param mouse_drag_vector mouse vector (in pixel coordinates) in which the mouse moves.
	 *		  starting position is the first time the left mouse button is pressed.
	 */
	void					Move(CVisGLView &view, CPoint &mouse_drag_vector);

	/** rotate viewport around viewport center. call this every time a new rotation starts.
	 * @param view viewport to rotate.
	 * @param mouse_pos current mouse position (in pixel coordinates).
	 */
	void					RotateBegin(CVisGLView &view, CPoint &mouse_pos, TVIS_LIST_DATAINTERN &ds);

	/** rotate viewport around viewport center. 
	 * @param view viewport to rotate.
	 * @param mouse_pos current mouse position.
	 */
	void					Rotate(CVisGLView &view, CPoint &mouse_pos);

	/** center viewport around a certain point. this is useful for easier rotation.
	 * @param view viewport to center.
	 * @param orgin some point in 3d-space.
	 */
	void					SetTBCustomOrgin(CVisGLView &view, TVIS_POINT4D &orgin);


	/** zoom viewport. call this every time a new zoom starts.
	 * @param view viewport to zoom.
	 * @param mouse_pos current mouse position (in pixel coordinates), the zoom mid point.
	 */
	void					ZoomPointBegin(CVisGLView &view, CPoint &mouse_pos);

	/** zoom viewport. call this every time the zoom ends.
	 * @param view viewport to zoom.
	 */
	void					ZoomPointEnd(CVisGLView &view);

	/** zoom viewport.
	 * @param view viewport to zoom.
	 * @param mouse_drag_vector mouse vector (in pixel coordinates) in which the mouse moves.
	 *		  starting position is the first time the left mouse button is pressed.
	 */
	void					ZoomPoint(CVisGLView &view, CPoint &mouse_drag_vector);

	/** zoom viewport. the user drags a selection rectangle. call this every time a new zoom starts.
	 * @param wnd pointer to CWnd from CVisModule.
	 * @param selection selection member from CVisModule.
	 * @param mouse_pos current mouse position (in pixel coordinates), the first frame coordinate.
	 */
	void					ZoomFrameBegin(CWnd *wnd, CVisSelectionDraw &selection, CPoint &mouse_pos);
	

	/** zoom viewport. the user drags a selection rectangle. call this every time the zoom ends.
	 * @param wnd pointer to CWnd from CVisModule.
	 * @param selection selection member from CVisModule.
	 * @param view viewport to zoom.
	 * @return returns true if selection was successful.
	 *         the zoom is succesful if the rectangle width and height are greater than zero.
	 */
	bool					ZoomFrameEnd(CWnd *wnd, CVisSelectionDraw &selection, CVisGLView &view);

	/** zoom viewport. the user drags a selection rectangle. call this every time a new zoom starts.
	 * @param view viewport to zoom.
	 * @param mouse_pos current mouse position (in pixel coordinates).
	 */
	void					ZoomFrame(CVisGLView &view, CPoint &mouse_pos);

private:
	double					CalcTBRadius(CVisGLView &view);
	TVIS_POINT4D			CalcTBPosition(CVisGLView &view, CPoint &mouse_pos);

	// position
	TVIS_POINT4D			m_PositionTemp;

	// rotation
	TVIS_POINT4D			m_TBPosition;							// virtual trackball
	TVIS_POINT4D			m_TBPositionOld;
	TVIS_POINT4D			m_TBPositionOrigin;						// trackball orgin

	// zoom
	const double			m_ZoomSensitivity;
	double					m_ZoomTemp;
	TVIS_POINT2D			m_ZoomScreenReferencePoint;
	TVIS_RECT2D				m_ZoomFrame;
	CPoint					m_ZoomFrameBeginPos;
};

#endif