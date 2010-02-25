#ifndef _VIS_COLORTABLE_H
#define _VIS_COLORTABLE_H

#include "vis_types.h"
#include "vis_dllexport.h"
#include "vis_gradient.h"

/** colortable class to compute gradients from CGradient
 */
class CVisColorTable
{
public:
	CVisColorTable();

	CVisColorTable &operator=(const CVisColorTable &c);

	/** get color from colorbar.
	 * @param percentage values between 0.0 - 1.0. values out of range will be clamped.
	 * @return current color.
	 */
	TVIS_COLOR4B		GetColor(double percentage);
	
	CVisGradient		GetColorTable();
	void				SetColorTable(CVisGradient &gradient);
						
private:				
	enum				{PaletteEntries = 1000};

	RGBTRIPLE			m_Pal[PaletteEntries];
	CVisGradient		m_ColTab;
};

#endif