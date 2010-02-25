#ifndef _VIS_RULER_H
#define _VIS_RULER_H

#include "vis_types.h"
#include "vis_glhelper.h"

/** base class for measurement objects. this is only used internally.
 */
class CVisMeasureBase: public CVisGLHelper
{
public:
	/** @param fntid font id for text output. this is only used in CVisRuler and CVisGrid.
	 */
	CVisMeasureBase(TVIS_FONTID fntid);

	/** get measurement unit e.g. m, cm, mm etc.
	 */
	void				GetUnitString(CString &str);

	/** get unit factor. 1.0 means meters, 100 means centimeters, 0.001 means kilometers.
	 */
	double				GetUnitFactor();

	/** get smallest step of ruler/grid. a normal school ruler would have the smallest step of 1mm.
	*/
	double				GetDiffFactor();

	/** get nearest base value smaller than value. 
	 * No need to call SetParamaters() before. if delta <= 0 function returns -1.0
	 */
	double				GetBaseMin(double value, double delta);

	/** set important parameters needed to calculate unit factor and unit string. 
	 * make sure this is called befor query GetUnitString(), GetUnitFactor() etc.
	 * @param delta distance
	 * @param shift base unit
	 * description:
	 *
	 *		base unit:	1.0 = m if shift = 0
	 *					1.0 = mm if shift = -3
	 *					1.0 = km if shift = 3
	 *	
	 *		e.g. if you want to get a normal school ruler of 30cm you would do:
	 *
	 *			SetParameters(0.3, 0);
	 *
	 *		you then get:
	 *			m_StrUnit		= "cm"
	 *			m_UnitFactor	= 100
	 *		which results in:
	 *			0.3*100 = 30 cm 
	 */
	void				SetParameters(double delta, int shift);

protected:
	int					m_iTemp1;
	double				m_UnitFactor;
	int					m_UnitShift;

	TVIS_FONTID			m_FontID;
	CString				m_StrUnit;

	bool				m_ParametersSet;
};

/** ruler class to draw a ruler
 */
class CVisRuler:public CVisMeasureBase
{
public:

	/** constructor.
	 * @param fntid a valid gl font id for text labels.
	 */
	CVisRuler(TVIS_FONTID fntid);
	
	/** draws a 2d ruler. the ruler can be horizontal or vertical make sure an appropriate viewport is set before.
	 * @param draw_rect drawing rectangle in viewport coordinates
	 * @param min_val minimum value of the ruler
	 * @param max_val maximum value of the ruler
	 * @param ruler_color color for ruler.
	 * @param text_color color for text labels.
	 * @param znear should only be something between the znear and zfar clipping plane.
	 */
	void				DrawRuler2D(TVIS_RECT2D &draw_rect, double min_val, double max_val, int shift, TVIS_COLOR4B &ruler_color, TVIS_COLOR4B &text_color, double znear);
	
	/** draw 3 labeled coordinate axis. x-axis is drawn from pmin to pmax(x), 
	 * y-axis is drawn from pmin to pmax(y),  z-axis is drawn from pmin to pmax(z).
	 * make sure an appropriate viewport is set before.
	 * @param pmin minimum position of axis aligned box.
 	 * @param pmin maximum position of axis aligned box.
	 * @param shift base unit
	 * description:
	 *
	 *		base unit:	1.0 = m if shift = 0
	 *					1.0 = mm if shift = -3
	 *					1.0 = km if shift = 3
	 *	
	 *		e.g. if you want to get a normal school ruler of 30cm you would do:
	 *
	 *			SetParameters(0.3, 0);
	 *
	 *		you then get:
	 *			m_StrUnit		= "cm"
	 *			m_UnitFactor	= 100
	 *		which results in:
	 *			0.3*100 = 30 cm 
	 *
	 * @param ruler_color color for ruler.
	 * @param text_color color for text labels.
	 * @param pixel_ratio pixel size in logical units
	 */
	void				DrawRulerCross3D(TVIS_POINT4D &pmin, TVIS_POINT4D &pmax, int shift, TVIS_COLOR4B &ruler_color, TVIS_COLOR4B &text_color, double pixel_ratio);

private:			
	
	void				SetParameters(double min_val, double max_val, int shift, TVIS_COLOR4B &ruler_color, TVIS_COLOR4B &text_color);

	void				DrawRulerHorizontal(double resolution, double ruler_width, bool draw_text);
	void				DrawRulerVertical(double resolution, double ruler_width, bool draw_text);
	void				DrawRuler3D(TVIS_POINT4D &pmin, TVIS_POINT4D &pmax, double min, double max, TVIS_POINT4D &ax, TVIS_POINT4D &ay, const char *label);
	void				DrawRuler3D(TVIS_POINT4D &pmin, TVIS_POINT4D &pmax, double min, double max, TVIS_POINT4D &ax, TVIS_POINT4D &ay, double resolution, double markersize, bool draw_text);
		
	double				m_PixelRatio;			
	double				m_MaxValue;
	double				m_MinValue;
	double				m_Delta;
	double				m_Factor;
	double				m_Base;
	double				m_Dec;
						
	TVIS_RECT2D			m_DrawRect;
						
	TVIS_COLOR4B		m_ColorRuler;
	TVIS_COLOR4B		m_ColorText;
						
	TVIS_DIRECTION_TYPE	m_Direction;

	double				m_ZNear;
};


/** grid class to draw a quadratic grid
 */
class CVisGrid:public CVisMeasureBase
{
public:
	CVisGrid(TVIS_FONTID fntid);
	
	/** set ruler dependant parameters.
	 * @param draw_rect drawing rectangle in viewport coordinates
	 * @param min_val minimum extension of the grid 
	 * @param max_val maximum extension of the grid 
	 * @param shift base unit
	 * description:
	 *
	 *		base unit:	1.0 = m if shift = 0
	 *					1.0 = mm if shift = -3
	 *					1.0 = km if shift = 3
	 *	
	 *		e.g. if you want to get a normal school ruler of 30cm you would do:
	 *
	 *			SetParameters(0.3, 0);
	 *
	 *		you then get:
	 *			m_StrUnit		= "cm"
	 *			m_UnitFactor	= 100
	 *		which results in:
	 *			0.3*100 = 30 cm 
	 *
	 * @param ruler_color color of ruler.
	 */
	void				SetParameters(TVIS_RECT2D &draw_rect, TVIS_POINT2D &min_val, TVIS_POINT2D &max_val, int shift, TVIS_COLOR4B &ruler_color, double pixel_ratio);

	/** draws the grid. make sure SetParameters() is called before and an appropriate viewport is set.
	 */
	void				DrawGrid(bool display_units);
						
private:	
	
	/** draws the grid.
	 * @param draw_text this parameter is ignored.
	 */
	void				DrawGrid(double resolution, bool draw_text);

	TVIS_POINT2D		m_MaxValue;
	TVIS_POINT2D		m_MinValue;
	TVIS_POINT2D		m_Base;
	double				m_Delta;
	double				m_Factor;
	double				m_Dec;

	double				m_PixelRatio;
						
				
	TVIS_RECT2D			m_DrawRect;
	
	TVIS_COLOR4B		m_ColorRuler;
	TVIS_COLOR4B		m_ColorRuler2;
	TVIS_COLOR4B		m_ColorRuler3;

	TVIS_COLOR4B		m_ColorText;
};


#endif