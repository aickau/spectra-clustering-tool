#ifndef _VIS_PROJECTION_H
#define _VIS_PROJECTION_H

#include "vis_glhelper.h"

/** all the projection stuff for viewport handling.
 */
class CVisProjection: public CVisGLHelper
{
public:
	CVisProjection();
	~CVisProjection();


	/** get viewport, all values in pixel coordinates
	 * @param px x start position (left)
	 * @param py y start position (upper)
	 * @param sx width
	 * @param sy height
	 */
	void					GetViewport(int &px, int &py, int &sx, int &sy);

	/** set viewport, all values in pixel coordinates. this sets the absolute position and size of the gl viewport
	 * @param px x start position (left)
	 * @param py y start position (upper)
	 * @param sx width
	 * @param sy height
	 * @param update can be set to anything
	 */
	void					SetViewport(int px, int py, int sx, int sy, bool update);

	/** not important (maybe)
	 */
	void					SetViewportEnd();
	
	/** get projection dimensions in viewport coordinates.
	 *	@retval width width of the viewport
	 *	@retval height height of the viewport
	 */
	void					GetProjection(double &width, double &height);

	/** get projection values for left, right, top, bottom in viewport coordinates.
	 */
	void					GetProjection(double &left, double &right, double &top, double &bottom);

	/** get projection values in viewport coordinates.
	 */
	void					GetProjection(TVIS_POINT2D &upperleft, TVIS_POINT2D &lowerright);

	/** get projection values in viewport coordinates.
	 */
	void					GetProjection(TVIS_RECT2D &rectangle);

	/** set projection, all values in viewport coordinates. 
	 * this sets the relative dimensions of the viewport.
	 * important: following conditions apply to.
	 * |left|==|right|
	 * |top|==|bottom|
	 * center must be at (0,0) 
	 * otherwise rotation fails.
	 * @param left left border
	 * @param right right border
	 * @param top top border
	 * @param bottom bottom border
	 */
	void					SetProjection(double left, double right, double top, double bottom);

	/** set projection, all values in viewport coordinates. this sets the relative dimensions of the viewport.
	 * this sets the relative dimensions of the viewport.
	 * important: following conditions apply to.
	 * |left|==|right|
	 * |top|==|bottom|
	 * center must be at (0,0) 
	 * otherwise rotation fails.
	 * @param upper_left upper left corner of the viewport
	 * @param lower_right lower right corner of the viewport
	 */
	void					SetProjection(TVIS_POINT2D &upper_left, TVIS_POINT2D &lower_right);

	/** set projection, all values in viewport coordinates. this sets the relative dimensions of the viewport.
	 * this sets the relative dimensions of the viewport.
	 * important: following conditions apply to.
	 * |left|==|right|
	 * |top|==|bottom|
	 * center must be at (0,0) 
	 * otherwise rotation fails.
	 * @param rectangle viewport rectangle
	 */
	void					SetProjection(TVIS_RECT2D &rectangle);
	
	/** get near and far clipping plane of viewing frustum.
	 * @param znear near clipping plane
	 * @param zfar far clipping plane
	 */
	void					GetZClippingPlane(double &znear, double &zfar);

	/** set near and far clipping plane of viewing frustum.
	 * @param znear near clipping plane
	 * @param zfar far clipping plane
	 */
	void					SetZClippingPlane(double znear, double zfar);

	/** aspect ratio of with to heigth
	 */
	double					GetAspectRatio();
	
	/** returns pixel size in logical units.
	*/
	double					GetPixelRatio();

	/** get temporary zoom factor. after calling SetZoomEnd() this is reset to 1.0.
	 */
	double					GetZoom();

	/** set zoom factor. 
	 * @param zoom_factor. this is clamped between 0.01 (zoom out) and 100.0 (zoom in) 
	 * @return zoomfactor delta
	 */
	double					SetZoom(double zoom_factor);
	
	/** this resets the zoom factor after zoom process (when mouse left button is released).
	 * viewport information are retained.
	 */
	void					SetZoomEnd();

	/** query if coordinate is in viewport.
	 * @param px x position in pixel coordinates.
	 * @param py y position in pixel coordinates.
	 */
	bool					IsPositionInView(int px, int py);

	/** query if coordinate is in viewport.
	 * @param p position in pixel coordinates.
	 */
	bool					IsPositionInView(CPoint &p);

	/** the pointer to height var of client window in pixel coordinates. this is used for coordinate mapping.
	 */
	void					AttachClientHeight(int *pt_client_height);
	
	/** get client height
	 * @return height in pixel coordinates
	 */
	int						GetClientHeight();

	void					UpdateProjection();


	bool					GetScissorState();

	/** set scissor state. if set to true gl commands are performed only on current viewport. 
	 * e.g. this is used for partial clear the client window.
	 * @param state enables or disables scissoring
	 */
	void					SetScissor(bool state);

	/** correct lower left and upper right points according to aspect ratio. this prevents viewports being squeezed and stretched.
	 * this function gurantees that you do not zoom in or out where rounding and viewport errors occur.
	 */
	void					CorrectProjectionPoints(TVIS_RECT2D &rect);


private:

	void					ClampMinMax(TVIS_RECT2D &rect);

	int						viewport_px;				// position
	int						viewport_py;
	int						viewport_sx;				// size
	int						viewport_sy;

	int						viewport_sx_old;			// size
	int						viewport_sy_old;

	TVIS_RECT2D				plane;						// our view coordinate system
	TVIS_RECT2D				plane2;
	TVIS_POINT2D			dimension;
	
	double					zoom;						// zoom factor
	double					zoom_min;
	double					zoom_max;

	double					aspect_ratio;

	const double			min_dimension;				// minimum dimension before drawing errors occur
	const double			max_dimension;				// maximum dimension before drawing errors occur

	double					znear;						// znear plane
	double					zfar;						// zfar plane

	bool					scissor_state;				// is set to true if scissoring is enabled

	int						*pt_client_height;			// pointer to client height, a simple y offset (when toolbars over the viewports are enabled)
	int						client_height_temp;
};


#endif