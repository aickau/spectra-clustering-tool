#ifndef _VIS_TYPESINTERN_H
#define _VIS_TYPESINTERN_H


// defines
////////////////////////////////////////////////////////////////////////////////

/** when dealing with point sizes or line widths this is the standard maximum value 
 * when mapping onto objects. 
 */
#define DVIS_RANGE_AUTO_SIZE_MAX	12

/** viewport minimum x dimension in pixels 
 */
#define DVIS_VIEWPORT_SIZE_MIN_X	200

/** viewport minimum y dimension in pixels 
 */
#define DVIS_VIEWPORT_SIZE_MIN_Y	100


// internal command messages
////////////////////////////////////////////////////////////////////////////////

/** update all viewports
 */
#define VIS_MSGUPDATEALL			0x01200


// types
////////////////////////////////////////////////////////////////////////////////

/** element info structure. stores, position index, and distance to some other point, e.g. mouse cursor.
 */
typedef struct TVIS_ELEMENTINFO_TAG
{
	double			distance;
	int				index;
	TVIS_POINT4D	position;
} TVIS_ELEMENTINFO;

/** element info vector
*/
typedef std::vector<TVIS_ELEMENTINFO>	TVIS_VECTORELEMENTINFO;

/** font id, when dealing with fonts
 */
typedef unsigned int					TVIS_FONTID;
										
/** integer list						
 */										
typedef std::list<int>					TVIS_LISTINT;
										
/** integer vector						
 */										
typedef std::vector<int>				TVIS_VECTORINT;
										
/** unsigned integer vector				
 */										
typedef std::vector<unsigned int>		TVIS_VECTORUINT;
										
/** point vector						
 */										
typedef std::vector<TVIS_POINT4D>		TVIS_VECTORPOINT4D;


typedef struct TVIS_DATASETPARAMRANGE_TAG
{
	TVIS_POINT4D	points_min;
	TVIS_POINT4D	points_max;

	TVIS_RANGE		point_param1;
	TVIS_RANGE		line_param1;
	TVIS_RANGE		triangle_param1;
	TVIS_RANGE		point_param2;
	TVIS_RANGE		line_param2;
	TVIS_RANGE		triangle_param2;

	inline TVIS_DATASETPARAMRANGE_TAG operator=(const TVIS_DATASETPARAMRANGE_TAG &rg)
	{
		this->points_min		= rg.points_min;
		this->points_max		= rg.points_max;

		this->point_param1		= rg.point_param1;
		this->line_param1		= rg.line_param1;
		this->triangle_param1	= rg.triangle_param1;

		this->point_param2		= rg.point_param2;
		this->line_param2		= rg.line_param2;
		this->triangle_param2	= rg.triangle_param2;

		return *this;
	}

	inline void InitZeroOne()
	{
		this->points_min.Set(-10.0, -10.0, -10.0);
		this->points_max.Set(10.0, 10.0, 10.0);

		this->point_param1.InitZeroOne();
		this->line_param1.InitZeroOne();
		this->triangle_param1.InitZeroOne();

		this->point_param2.InitZeroOne();
		this->line_param2.InitZeroOne();
		this->triangle_param2.InitZeroOne();
	}

	inline void SetLimit()
	{
		this->points_min.SetLimitMax();
		this->points_max.SetLimitMin();

		this->point_param1.SetLimit();
		this->line_param1.SetLimit();	
		this->triangle_param1.SetLimit();

		this->point_param2.SetLimit();
		this->line_param2.SetLimit();	
		this->triangle_param2.SetLimit();
	}

	inline void InsertMinMaxComponents(TVIS_DATASETPARAMRANGE_TAG &rg)
	{
		this->points_min.InsertMinComponents(rg.points_min);
		this->points_max.InsertMaxComponents(rg.points_max);

		this->point_param1.InsertMinComponentsSource(rg.point_param1.source_min);
		this->point_param1.InsertMaxComponentsSource(rg.point_param1.source_max);
		this->line_param1.InsertMinComponentsSource(rg.line_param1.source_min);
		this->line_param1.InsertMaxComponentsSource(rg.line_param1.source_max);
		this->triangle_param1.InsertMinComponentsSource(rg.triangle_param1.source_min);
		this->triangle_param1.InsertMaxComponentsSource(rg.triangle_param1.source_max);
	
		this->point_param2.InsertMinComponentsSource(rg.point_param2.source_min);
		this->point_param2.InsertMaxComponentsSource(rg.point_param2.source_max);
		this->line_param2.InsertMinComponentsSource(rg.line_param2.source_min);
		this->line_param2.InsertMaxComponentsSource(rg.line_param2.source_max);
		this->triangle_param2.InsertMinComponentsSource(rg.triangle_param2.source_min);
		this->triangle_param2.InsertMaxComponentsSource(rg.triangle_param2.source_max);
	}


} TVIS_DATASETPARAMRANGE;





// enumerators
////////////////////////////////////////////////////////////////////////////////


/** update types, specify which things get updated. only used in CVisModule.
 */
typedef enum 
{
	UPD_DIM			= 0x01,
	UPD_SIZE		= 0x02,
	UPD_VIEW		= 0x04,
	UPD_BUFCOLOR	= 0x08,
	UPD_BUFSIZE		= 0x10
} TVIS_UPDATEITEMS;


/** threestate 
 */
typedef enum 
{
	ST_UNDEFINED	= -1,
	ST_FALSE		= 0,
	ST_TRUE			= 1
} TVIS_THREESTATE;


/** color bar info
 */
typedef enum 
{
	CB_NONE,
	CB_POINTS,
	CB_LINES,
	CB_TRIANGLES
} TVIS_COLORBARINFO;


/** user states
 */
typedef enum 
{
	ST_ARROW,
	ST_POSITION,
	ST_ROTATION,
	ST_ZOOMPOINT,
	ST_ZOOMFRAME,
	ST_INFO,
	ST_SETROTATION
} TVIS_USERSTATE;

/// user state flags
typedef enum 
{
	STF_BEGIN,
	STF_PROCESSING,
	STF_END
} TVIS_USERSTATEFLAGS;


/// selection shapes
typedef enum 
{
	SEL_RECTANGLE,
	SEL_CIRCLE,
	SEL_ELEMENTPOINTS,
	SEL_ELEMENTLINES,
	SEL_ELEMENTTRIANGLES
} TVIS_SELECTION_SHAPE;

/// selection types, slection per object or slection by element basis. element = points, lines, triangles, ...
typedef enum
{
	SELTYPE_OBJECT,
	SELTYPE_ELEMENT
} TVIS_SELECTION_TYPE;

/// direction
typedef enum 
{
	DIR_HORIZONTAL,
	DIR_VERTICAL
} TVIS_DIRECTION_TYPE;

/** not used at the moment: 
 keyboard shortcuts, to bind keys to different operations. not used at the current state.
 */
typedef enum 
{
//	KB_SNAP = 'A'
} TVIS_KEYBOARD_COMMANDS;



#endif