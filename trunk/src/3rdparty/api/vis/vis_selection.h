#ifndef _VIS_SELECTION_H
#define _VIS_SELECTION_H

#include "vis_types.h"
#include "vis_typesintern.h"
#include "vis_helper.h"
#include "vis_glhelper.h"
#include "vis_glview.h"



class CVisSelection
{
public:
	CVisSelection(TVIS_LIST_DATAINTERN *pdatasets);

	void					SetShape(TVIS_SELECTION_SHAPE shape);
	TVIS_SELECTION_SHAPE	GetShape();
	
	void					SetType(TVIS_SELECTION_TYPE type);
	TVIS_SELECTION_TYPE		GetType();			

	void					SetState(bool state);
	bool					GetState();

	/** not used at the moment. set epsilon range for element picking in pixels. range is clamped to 1.0 - 10000.0.
	 */
	void					SetPickingRange(double range);

	/** highlight element begin.
	 * @param view pointer to the current viewport
	 * @param element type to highlight. can be one of the following: 
	 * SEL_ELEMENTPOINTS, SEL_ELEMENTLINES, SEL_ELEMENTTRIANGLES.
	 */
	void					HighLightElementBegin(CVisGLView *view, TVIS_SELECTION_SHAPE element_type);

	/** highlight element.
	 * @return element index of highlighted element, -1 if no element was hit.
	 */
	int						HighLightElement(CPoint &mouse_pos);
	int						HighLightElement(CPoint &mouse_pos, CVisDataIntern **pdataset);
	
	/** highlight element end.
	 */
	void					HighLightElementEnd();

	CString					GetElementInformation(CVisGLView *view, TVIS_SELECTION_SHAPE element_type, CPoint &mouse_pos);

	/** do selection. selection types are specified via SetShape(), SetType() and SetState().
  	 * @param view pointer to the current viewport
	 * @param mouse_pos mouse picking position in client (pixel) coordinates.
	 * @param sel_circle selection circle in client (pixel) coordinates. this is only used for circular selection.
	 * @param sel_rect selection rectangle in client (pixel) coordinates. this is only used for rectangular selection.
	 */
	void					DoSelection(CVisGLView *view, CPoint &mouse_pos, TVIS_CIRCLE2D &sel_circle, TVIS_RECT2D &sel_rect);

	/** returns nearest object which bounding box hits mouse coordinates. 
	 * if more than one object can be returned, 
	 * @param view pointer to the current viewport.
	 * @param mouse_pos mouse picking position in client (pixel) coordinates.
	 * @param position determinates which object should be returned. position 0 returns nearest object, 1 second nearest, 2 third nearest. 
	 * @return pointer to a valid dataset or NULL if no object was hit.
	 */
	CVisDataIntern			*PickDataset(CVisGLView *view, CPoint &mouse_pos, int position=0);

	/** returns nearest object and index to nearest element.
	 * @param view pointer to the current viewport.
	 * @param element type to pick. can be one of the following: 
	 * SEL_ELEMENTPOINTS, SEL_ELEMENTLINES, SEL_ELEMENTTRIANGLES.
	 * @param mouse_pos mouse picking position in client (pixel) coordinates.
	 * @param pdataset out value, returned dataset. if no dataset could be found 0.-1.
	 */
	void					PickElement(CVisGLView *view, TVIS_SELECTION_SHAPE element_type, CPoint &mouse_pos, CVisDataIntern **pdataset, int *pindex);

private:

	void					DoSelectionRayElement(CVisGLView *view, CPoint &mouse_pos);

	void					DoSelectionRayObject(CVisGLView *view, CPoint &mouse_pos);

	/** performs a rectangular selection on object or element basis depending on selection type.
	 * if selection type == SELTYPE_ELEMENT, all element types (triangles, lines, points) are selected in selection shape.
	 * @param view pointer to the current viewport
	 * @param sel_rect selection rectangle in screen coordinates.
	 */
	void					DoSelectionRectangle(CVisGLView *view, TVIS_RECT2D &sel_rect);

	/** performs a circular selection on object or element basis depending on selection type.
	 * if selection type == SELTYPE_ELEMENT, all element types (triangles, lines, points) are selected in selection shape.
	 * @param view pointer to the current viewport
	 * @param sel_rect selection rectangle in screen coordinates.
	 */
	void					DoSelectionCircle(CVisGLView *view, TVIS_CIRCLE2D &circle);


	TVIS_SELECTION_SHAPE	m_Shape;					// SEL_RECTANGLE or SEL_CIRCLE 
	TVIS_SELECTION_TYPE		m_Type;						// SELTYPE_OBJECT or SELTYPE_ELEMENT 
	bool					m_State;					// true = select, false = deselect

	double					m_EpsilonRange;

	CVisGLView				*m_pHLView;
	TVIS_SELECTION_SHAPE	m_HLType;
	bool					m_HLActive;
	CVisDataIntern			*m_pHLDS;

	TVIS_LIST_DATAINTERN	*m_pDataSets;

};




/** selection & deselection drawing class. 
 * this is mainly responsible for drawing an animated selection shape on the screen.
 */
class CVisSelectionDraw: public CVisGLHelper
{
public:
	CVisSelectionDraw();

	/** set selection shape.
	 * @param sel_shape set to SEL_RECTANGLE for rectangular selection or SEL_CIRCLE for a circular selection shape
	 */
	void					SetSelectionShape(TVIS_SELECTION_SHAPE sel_shape);

	/// returns SEL_RECTANGLE or SEL_CIRCLE
	TVIS_SELECTION_SHAPE	GetSelectionShape();

	/** returns selection range, it's up to you to intepret range as circle or rectangle independent from the selection type.
	 * @param view pointer to the current viewport
	 * @retval sel_rect selection rectangle in viewport coordinates
	 * @retval sel_circle selection circle in viewport coordinates
	 */
	void					GetSelectionRange(CVisGLView *view, TVIS_RECT2D &sel_rect, TVIS_CIRCLE2D &sel_circle);
	
	/** start selection.
	 * @param start_pos selection start position in pixel coordinates.
	 */
	void					Begin(CWnd *wnd, CPoint &start_pos);

	/** end selection.
	 */	
	void					End(CWnd *wnd);

	/** draw selection rectangle / circle. make sure to set an appropriate viewport befor calling Draw().
	 * @param current_pos current mouse position in pixel coordinates
	 * @param clear_old if set to true old selection rectangle/circle will be cleared. 
	 * if no selection was drawn before this will cause drawing artifacts.
	 * @param zmid z coordinate, should be something between znear and zfar clipping plane.
	 */
	void					Draw(CPoint &current_pos, bool clear_old, double zmid);

private:

	TVIS_SELECTION_SHAPE	m_Shape;							// SEL_RECTANGLE or SEL_CIRCLE 
	TVIS_POINT2D			m_StartPos;				

	bool					m_Draw;							// draw selction ?
	bool					m_FirstDraw;
	unsigned __int16		m_StipplePattern;				// selection stipple pattern

	TVIS_RECT2D				m_SelRect;						// selection rectangle 
	TVIS_RECT2D				m_SelRectOld;

	double					m_SelRadius;					// selection circle 
	double					m_SelRadiusOld;

	const int				m_SelUpdateTime;				// selection rectangle/circle update time in millisconds						

	/// animation factor for line stipple patterns, 1=fastest
	const int				m_AnimationSpeed;				
	int						m_AnimationCounter;
};

#endif

