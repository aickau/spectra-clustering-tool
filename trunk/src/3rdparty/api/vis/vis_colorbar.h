#ifndef _VIS_COLORBAR_H
#define _VIS_COLORBAR_H

#include "vis_types.h"
#include "vis_glhelper.h"
#include "vis_colortable.h"

/** draw color bar.
 */
class CVisColorBar: public CVisGLHelper
{
public:
	CVisColorBar();

	/** draws a colortable
     * @param colortable the colortable to draw.
	 * @param position position (upper left and lower right) in pixel coordinates of the colorbar rectangle.
	 * @param color_frame color of the background frame.
	 * @param zposition should only be something between the znear and far clipping plane.
	 */
	void		Draw(CVisColorTable &colortable, TVIS_RECT2D position, TVIS_COLOR4B &color_frame, double zposition);
};

#endif