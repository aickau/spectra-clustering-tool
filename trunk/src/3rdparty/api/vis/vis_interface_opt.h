#ifndef _VIS_INTERFACE_OPT_H
#define _VIS_INTERFACE_OPT_H

#include "vis_interface.h"
#include "vis_colortable.h"


class CVisGlobalOptionsView
{
public:
	CVisGlobalOptionsView();

	// don't forget to update...
	CVisGlobalOptionsView &operator=(const CVisGlobalOptionsView &g);

	TVIS_VIEWPORT_TYPE		type;						// front, top, left, or perspective

	TVIS_RECT2D				plane;						// viewport plane

	TVIS_POINT4D			position;
	TVIS_MATRIX4D			rotation;
};


/** global options, this affects the appeareance of the viewports and the datsets.
 */
class CVisGlobalOptions											
{
	public:
		VIS_DLLEXPORT  CVisGlobalOptions();
		
		/** don't forget to update this when adding attributes.
		 */
		VIS_DLLEXPORT  CVisGlobalOptions& operator=(const CVisGlobalOptions &g);

		/** background color.
		 */
		TVIS_COLOR4B		ColorBg;						
															
		/** grid color.
		 */
		TVIS_COLOR4B		ColorGrid;						

		/** selection color for element selection.
		 */
		TVIS_COLOR4B		ColorSel;						

		/** selection color for whole object selection.
		 */
		TVIS_COLOR4B		ColorObjSel;					

		/** text color.
		 */
		TVIS_COLOR4B		ColorText;		
		
		/** viewport frame color.
		 */
		TVIS_COLOR4B		ColorFrame;						

		/** viewport selected frame color.
		 */
		TVIS_COLOR4B		ColorFrameSelected;				

		/** viewport-additional elements color, eg. ksystem, grid, ..
		 */
		TVIS_COLOR4B		ColorGraphics;					

		/** global point dataset color.
		 */
		TVIS_COLOR4B 		ColorDataPoints;			

		/** global line dataset color.
		 */
		TVIS_COLOR4B 		ColorDataLines;					

		/** global triangle mesh dataset color.
		 */
		TVIS_COLOR4B 		ColorDataTriangles;			

	
		/** color code type for point-objects. 
		 * this can be a single color or can obtain color information from dataset. 
		 * this type specify which should gennerally used.
		 */
		TVIS_COLS			ColorCodePoints;				

		/** color code type for line-objects. 
		 * this can be a single color or can obtain color information from dataset. 
		 * this type specify which should gennerally used.
		 */
		TVIS_COLS			ColorCodeLines;				
		
		/** color code type for triangle-objects. 
		 * this can be a single color or can obtain color information from dataset. 
		 * this type specify which should gennerally used.
		 */
		TVIS_COLS			ColorCodeTriangles;			

		TVIS_RANGE			RangeXAxis;						// Ranges
		TVIS_RANGE			RangeYAxis;
		TVIS_RANGE			RangeZAxis;
		TVIS_RANGE			RangePointsInt;
		TVIS_RANGE			RangePointsFloat;
		TVIS_RANGE			RangeLinesInt;
		TVIS_RANGE			RangeLinesFloat;
		TVIS_RANGE			RangeTrianglesInt;
		TVIS_RANGE			RangeTrianglesFloat;
		TVIS_RANGE			RangePointSizeInt;
		TVIS_RANGE			RangePointSizeFloat;
		TVIS_RANGE			RangeLineWidthInt;
		TVIS_RANGE			RangeLineWidthFloat;

		/** color table. this is used for mapping intesity values into color values.
		 */
		CVisColorTable		ColorTable;					

		/** if set to true, a message box is displayed before selection.
		 */
		bool				ConfirmSelection;				

		/** global point style.
		 */
		TVIS_PS 			PointStyle;						

		/** point diameter in pixels.
		 */
		int 				PointSize;						

		/** whether to use dataset buffer information or not.
		 */
		TVIS_SIZEM			PointSizeMode;					

		/** global line style.
		 */
		TVIS_LS				LineStyle;						

		/** line width in pixels.
		 */
		int 				LineWidth;						
	
		/** whether to use dataset buffer information or not.
		 */
		TVIS_SIZEM			LineWidthMode;					
		
		/** object scale in x,y,z direction. a scale factor of 1.0 means no scale at all.
		 */
		TVIS_POINT4D		Scale;				
		
		/** describes whats the standard unit.
		 * by default the length of 1.0 is equivalent to 1 m. (meter), unit_shift_factor = 0
		 * unit_shift_factor = 1 : 1.0 is equivalent to 10 m
		 * unit_shift_factor = 3 : 1.0 is equivalent to 1 km
		 * unit_shift_factor = -1 : 1.0 is equivalent to 0.1 m
		 * unit_shift_factor = -3 : 1.0 is equivalent to 1 mm
		 */
		int					unit_shift_factor;

		/** specifies which additional items are shown in the views.
		 */
		TVIS_GRITEMS_CONTAINER 	Items;						

		/** viewport frame thickness in pixels.
		 */
		int					FrameWidth;						

		/** viewport frame inset.
		 */
		int					FrameInset;						

		/** vertical (x) and horizontal (y)  ratio of the 4 viewports.
		 */
		TVIS_POINT2D		viewRatio;						
	
		/** index to selected view 0..3
		 * 0 is upper left view, other follow counter clockwise order.
		 */
		int					viewSelected;					
		
		/** if true -> selcted view is in fullscreen mode
		 */
		bool				viewFullscreen;				

		/** viewport dependant information
		 */
		CVisGlobalOptionsView	viewInfo[4];				
};


/** all dataset specific options.
 */
class CVisDataOptions	
{
	public:
		VIS_DLLEXPORT CVisDataOptions();									

		/** don't forget to update this when adding attributes.
		 */
		VIS_DLLEXPORT  CVisDataOptions& operator=(const CVisDataOptions &d);
		
		TVIS_COLOR4B 				ColorDataPoints;		// Farbe sichtbarer Daten
		TVIS_COLOR4B 				ColorDataLines;			// Farbe sichtbarer Daten
		TVIS_COLOR4B 				ColorDataTriangles;		// Farbe sichtbarer Daten
		TVIS_COLOR4B				ColorGraphics;          // Grafikelementefarbe
									
		bool						PaintName;				// Datensatzname anzeigen ?
		bool						PaintKSystem;			// Datensatzkoordinatensystem anzeigen ? (nicht benutzt im moment)
		bool						PaintMinMaxBox;			// MinMaxBox anzeigen ?
		bool						ColorCodeUsed;          // Farbkodierung benutzt ?
		bool						DataHide;		        // Datensatz versteckt ?
		bool						ShowPoints;				// display points
		bool						ShowLines;				// display lines
		bool						ShowTriangles;			// display triangles
		bool						Lighting;				// use lighting
		bool						Selected;				// is dataset selected
		bool						FlippedSurfaceNormals;	// use flipped surface normals

		bool						FromGlobalOptions_ColorDataPoints;
		bool						FromGlobalOptions_ColorDataLines;
		bool						FromGlobalOptions_ColorDataTriangles;
		bool						FromGlobalOptions_PointStyle;
		bool						FromGlobalOptions_PointSize;
		bool						FromGlobalOptions_LineStyle;
		bool						FromGlobalOptions_LineWidth;
		
		TVIS_SIZEM					PointSizeMode;			// buffer type
		TVIS_PS 					PointStyle;				// Punktstyle
		int 						PointSize;				// Punktradius
		TVIS_SIZEM					LineWidthMode;			// buffer type
		TVIS_LS						LineStyle;				// Windows PenStyles
		int 						LineWidth;				// Linienbreite
};

#endif