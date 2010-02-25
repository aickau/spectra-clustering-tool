#ifndef _VIS_TYPES_H
#define _VIS_TYPES_H

#include <math.h>
#include <float.h>
#include <list>
#include <vector>


#pragma warning(disable : 4244)					// conversion double 2 float
#pragma warning(disable : 4305)					// truncation const double 2 float
#pragma warning(disable : 4231)					// extern blah
#pragma warning(disable : 4786)					// long symbol names warning

// message id's
/////////////////////////////////////////////////////

/** this message is send to parent window every time an element selection occurs.
 */
#define VIS_MSGSELECTION		0x01000

/** this message is send to parent window every time the status text changes. 
 * call GetStatusbarText() to obtain satusbar information.
 */
#define VIS_MSGGETSTATUSTEXT	0x01001


// base helpers
/////////////////////////////////////////////////////

/// swap doubles
__forceinline void swapf(double &a, double &b)
{
	double c = a;
	a = b;
	b = c;
}


// base types
/////////////////////////////////////////////////////

/** for additional mesh information like colors, intensity
 */
typedef unsigned __int32 TVIS_DATAPARAM1;

/** for additional mesh information like point size, line size, intensity
 */
typedef double			 TVIS_DATAPARAM2;

/** dataset id type
 */
typedef int		 		 TVIS_OBJECTID;

/** byte bool
 */
typedef char			 TVIS_BOOL;



/** range object
*/
typedef struct TVIS_RANGE_TAG
{
	bool				autorange;
	double				source_min;		
	double				source_max;		
	double				destination_min;		
	double				destination_max;	

	inline TVIS_RANGE_TAG()
	{
		InitZeroOne();
	}

	inline void InitZeroOne()
	{
		this->autorange			= true;
		this->source_min		= 0.0;	
		this->source_max		= 1.0;		
		this->destination_min	= 0.0;
		this->destination_max	= 1.0;
	}

	inline TVIS_RANGE_TAG(double src_min, double src_max)
	{
		this->autorange			= false;
		this->source_min		= src_min;	
		this->source_max		= src_max;		
		this->destination_min	= 0.0;
		this->destination_max	= 1.0;
	}

	inline TVIS_RANGE_TAG(double src_min, double src_max, double dst_min, double dst_max)
	{
		this->autorange			= false;
		this->source_min		= src_min;	
		this->source_max		= src_max;		
		this->destination_min	= dst_min;
		this->destination_max	= dst_max;
	}

	inline TVIS_RANGE_TAG operator=(const TVIS_RANGE_TAG &rg)
	{
		this->autorange			= rg.autorange;
		this->source_min		= rg.source_min;
		this->source_max		= rg.source_max;
		this->destination_min	= rg.destination_min;
		this->destination_max	= rg.destination_max;
		return *this;
	}

	inline double GetSourceDelta()
	{
		return (source_max-source_min);
	}

	inline double GetSourceScale()
	{
		double delta = source_max-source_min;
		if (delta == 0) return 0.0;
		return (1.0/delta);
	}

	inline double GetDestinationDelta()
	{
		return (destination_max-destination_min);
	}

	inline double GetDestinationScale()
	{
		double delta = destination_max-destination_min;
		if (delta == 0) return 0.0;
		return (1.0/delta);
	}

	inline void InsertMinComponentsSource(double val)
	{
		if (val < source_min) source_min = val;
	}

	inline void InsertMaxComponentsSource(double val)
	{
		if (val > source_max) source_max = val;
	}

	inline void SetLimit()
	{
		this->source_min		= DBL_MAX;
		this->source_max		= -DBL_MAX;
	}

	inline void UpdateSource(const TVIS_RANGE_TAG &rg)
	{
		if (!autorange) return;

		this->source_min		= rg.source_min;
		this->source_max		= rg.source_max;
	}

	inline void UpdateDestination(const TVIS_RANGE_TAG &rg)
	{
		if (!autorange) return;

		this->destination_min	= rg.destination_min;
		this->destination_max	= rg.destination_max;
	}




} TVIS_RANGE;


/** line idecies
 */
typedef struct TVIS_LINES_TAG
{
	unsigned __int32	n1;
	unsigned __int32	n2;
} TVIS_LINES;


/** triangle idecies
 */
typedef struct TVIS_TRIANGLES_TAG
{
	unsigned __int32	n1;
	unsigned __int32	n2;
	unsigned __int32	n3;
} TVIS_TRIANGLES;

/** color structure, the alpha channel is not used, only for alignment purposes
 */
typedef struct TVIS_COLOR4B_TAG
{
	unsigned __int8 r;
	unsigned __int8 g;
	unsigned __int8 b;
	unsigned __int8 a;

	TVIS_COLOR4B_TAG(){}

	TVIS_COLOR4B_TAG(unsigned __int8 r, unsigned __int8 g, unsigned __int8 b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
	}
	
	inline void Set(unsigned __int8 r, unsigned __int8 g, unsigned __int8 b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
	}

	inline void Set(COLORREF col)
	{
		this->r = GetRValue(col);
		this->g = GetGValue(col);
		this->b = GetBValue(col);
	}

	inline double GetIntensity()
	{
		return ((double) this->r*(1.0/(255.0*3.0)) +
				(double) this->g*(1.0/(255.0*3.0)) + 
				(double) this->b*(1.0/(255.0*3.0))); 
	}

	inline COLORREF ToColorRef()
	{
		return RGB(r,g,b);
	}

	inline void ToFloat4(float *array)
	{
		array[0] = (float) r*(1.0/255.0);
		array[1] = (float) g*(1.0/255.0);
		array[2] = (float) b*(1.0/255.0);
		array[3] = 1.0;
	}

	inline TVIS_COLOR4B_TAG operator=(const TVIS_COLOR4B_TAG &col)
	{
		this->r = col.r;
		this->g = col.g;
		this->b = col.b;
		return *this;
	}

	inline TVIS_COLOR4B_TAG operator=(unsigned __int8 intensity)
	{
		this->r = intensity;
		this->g = intensity;
		this->b = intensity;
		return *this;
	}

	inline TVIS_COLOR4B_TAG operator+=(unsigned __int8 intensity)
	{
		int _r, _g, _b;

		_r = this->r + intensity;
		_g = this->g + intensity;
		_b = this->b + intensity;

		this->r = (_r < 0) ? 0 : (_r > 255) ? 255 : _r;
		this->g = (_g < 0) ? 0 : (_g > 255) ? 255 : _g;
		this->b = (_b < 0) ? 0 : (_b > 255) ? 255 : _b;
		return *this;
	}

	inline TVIS_COLOR4B_TAG operator-=(unsigned __int8 intensity)
	{
		int _r, _g, _b;

		_r = this->r - intensity;
		_g = this->g - intensity;
		_b = this->b - intensity;

		this->r = (_r < 0) ? 0 : (_r > 255) ? 255 : _r;
		this->g = (_g < 0) ? 0 : (_g > 255) ? 255 : _g;
		this->b = (_b < 0) ? 0 : (_b > 255) ? 255 : _b;
		return *this;
	}

} TVIS_COLOR4B;


/** 2d point structure
 */
typedef struct TVIS_POINT2D_TAG
{
	double x,y;

	TVIS_POINT2D_TAG(){};
	TVIS_POINT2D_TAG(double x, double y)
	{
		this->x = x;
		this->y = y;
	}

	inline void Set(double x, double y)
	{
		this->x = x;
		this->y = y;
	}

	inline void SetLimitMin()
	{
		this->x = -DBL_MAX;
		this->y = -DBL_MAX;
	}

	inline void SetLimitMax()
	{
		this->x = DBL_MAX;
		this->y = DBL_MAX;
	}

	inline TVIS_POINT2D_TAG operator=(const TVIS_POINT2D_TAG &p)
	{
		this->x = p.x;
		this->y = p.y;
		return *this;
	}

	inline TVIS_POINT2D_TAG operator+=(double scalar)
	{
		this->x += scalar;
		this->y += scalar;
		return *this;
	}

	inline TVIS_POINT2D_TAG operator-=(double scalar)
	{
		this->x -= scalar;
		this->y -= scalar;
		return *this;
	}

	inline TVIS_POINT2D_TAG operator*=(double scalar)
	{
		this->x *= scalar;
		this->y *= scalar;
		return *this;
	}

	inline TVIS_POINT2D_TAG operator/=(double scalar)
	{
		this->x /= scalar;
		this->y /= scalar;
		return *this;
	}

	inline TVIS_POINT2D_TAG operator+=(const TVIS_POINT2D_TAG &p)
	{
		this->x += p.x;
		this->y += p.y;
		return *this;
	}

	inline TVIS_POINT2D_TAG operator-=(const TVIS_POINT2D_TAG &p)
	{
		this->x -= p.x;
		this->y -= p.y;
		return *this;
	}

	inline TVIS_POINT2D_TAG operator*=(const TVIS_POINT2D_TAG &p)
	{
		this->x *= p.x;
		this->y *= p.y;
		return *this;
	}

	inline TVIS_POINT2D_TAG operator/=(const TVIS_POINT2D_TAG &p)
	{
		this->x /= p.x;
		this->y /= p.y;
		return *this;
	}

	inline TVIS_POINT2D_TAG operator+(double scalar)
	{
		return TVIS_POINT2D_TAG(this->x + scalar, this->y + scalar);
	}

	inline TVIS_POINT2D_TAG operator-(double scalar)
	{
		return TVIS_POINT2D_TAG(this->x - scalar, this->y - scalar);
	}

	inline TVIS_POINT2D_TAG operator*(double scalar)
	{
		return TVIS_POINT2D_TAG(this->x * scalar, this->y * scalar);
	}

	inline TVIS_POINT2D_TAG operator/(double scalar)
	{
		return TVIS_POINT2D_TAG(this->x / scalar, this->y / scalar);
	}

	inline TVIS_POINT2D_TAG operator+(const TVIS_POINT2D_TAG &p)
	{
		return TVIS_POINT2D_TAG(this->x + p.x, this->y + p.y);
	}

	inline TVIS_POINT2D_TAG operator-(const TVIS_POINT2D_TAG &p)
	{
		return TVIS_POINT2D_TAG(this->x - p.x, this->y - p.y);
	}

	inline TVIS_POINT2D_TAG operator*(const TVIS_POINT2D_TAG &p)
	{
		return TVIS_POINT2D_TAG(this->x * p.x, this->y * p.y);
	}

	inline TVIS_POINT2D_TAG operator/(const TVIS_POINT2D_TAG &p)
	{
		return TVIS_POINT2D_TAG(this->x / p.x, this->y / p.y);
	}

	inline void Normalize()
	{
		double len = sqrt(x*x+y*y);
		if (len == 0) return;
		len = 1.0/len;
		
		x *= len;
		y *= len;
	}

	inline double Length()
	{
		return sqrt(x*x+y*y);
	}

	inline double Dot(const TVIS_POINT2D_TAG &p)
	{
		return (x*p.x+y*p.y);		
	}

	inline void InsertMinComponents(const TVIS_POINT2D_TAG &p)
	{
		if (x > p.x) x = p.x;
		if (y > p.y) y = p.y;
	}

	inline void InsertMaxComponents(const TVIS_POINT2D_TAG &p)
	{
		if (x < p.x) x = p.x;
		if (y < p.y) y = p.y;
	}


} TVIS_POINT2D;

/*
typedef struct TVIS_LINE2D_TAG
{
	TVIS_POINT2D p1;
	TVIS_POINT2D p2;

	
	TVIS_LINE2D_TAG(){};
	TVIS_LINE2D_TAG(double p1x, double p1y, double p2x, double p2y):p1(p1x,p1y), p2(p2x, p2y){}

	inline double Length()
	{
		TVIS_POINT2D t;
		t = p1-p2;
		return t.Length();
	}
} TVIS_LINE2D;
*/

/** an axis aligned rectangle
 */
typedef struct TVIS_RECT2D_TAG
{
	TVIS_POINT2D upper_left;
	TVIS_POINT2D lower_right;

	TVIS_RECT2D_TAG(){}
	TVIS_RECT2D_TAG(double p1x, double p1y, double p2x, double p2y):upper_left(p1x, p1y), lower_right(p2x, p2y){}

	inline TVIS_RECT2D_TAG operator=(const TVIS_RECT2D_TAG &p)
	{
		upper_left	= p.upper_left;
		lower_right = p.lower_right;
		return *this;
	}

	inline void Sort()
	{
		if (upper_left.x > lower_right.x)
		{
			swapf(upper_left.x, lower_right.x);
		}

		if (upper_left.y > lower_right.y)
		{
			swapf(upper_left.y, lower_right.y);
		}
	}

	inline TVIS_POINT2D GetCenter()
	{
		return((upper_left+lower_right)*0.5);
	}

	inline TVIS_POINT2D GetDimensions()
	{
		return (TVIS_POINT2D(fabs(upper_left.x-lower_right.x), fabs(upper_left.y-lower_right.y)));
	}

} TVIS_RECT2D;


/** simple 2d box - we assume that 4 points are defined along clockwise(!) direction, 
 * otherwise drawing could fail.
 */
typedef struct TVIS_BOX2D_TAG
{
	TVIS_POINT2D p[4];

	TVIS_BOX2D_TAG(){}
	TVIS_BOX2D_TAG(TVIS_POINT2D &p0, TVIS_POINT2D &p1, TVIS_POINT2D &p2, TVIS_POINT2D &p3)
	{
		p[0] = p0;		p[1] = p1;		p[2] = p2;		p[3] = p3;
	}

	inline TVIS_BOX2D_TAG operator=(const TVIS_BOX2D_TAG &b)
	{
		p[0] = b.p[0];		p[1] = b.p[1];		p[2] = b.p[2];		p[3] = b.p[3];
		return *this;
	}

	inline TVIS_POINT2D GetCenter()
	{
		TVIS_POINT2D m = p[0];
		m += p[1];
		m += p[2];
		m += p[3];
		m *= 0.25;
		return(m);
	}

	inline void GetAxis(TVIS_POINT2D &ax, TVIS_POINT2D &ay)
	{
		ax = p[1] - p[0];
		ay = p[3] - p[0];
		ax.Normalize();
		ay.Normalize();
	}

	inline TVIS_POINT2D GetDimensions()
	{
		TVIS_POINT2D lx = p[0]-p[1];
		TVIS_POINT2D ly = p[0]-p[3];
		
		return (TVIS_POINT2D(lx.Length(), ly.Length()));
	}


} TVIS_BOX2D;


typedef struct TVIS_CIRCLE2D_TAG
{
	TVIS_POINT2D	midpoint;
	double			radius;

	TVIS_CIRCLE2D_TAG(){}
	TVIS_CIRCLE2D_TAG(TVIS_POINT2D &point, double radius)
	{
		this->midpoint	= point;
		this->radius	= radius;
	}

	inline TVIS_CIRCLE2D_TAG operator=(const TVIS_CIRCLE2D_TAG &c)
	{
		midpoint	= c.midpoint;
		radius		= c.radius;
		return *this;
	}

} TVIS_CIRCLE2D;


/** 3d point structure. w is not used in most cases and only for alignment purposes
 */
typedef struct TVIS_POINT4D_TAG
{
	double x,y,z,w;

	TVIS_POINT4D_TAG(){};
	TVIS_POINT4D_TAG(double x, double y, double z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = 1.0;
	}

	TVIS_POINT4D_TAG(double x, double y, double z, double w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	inline TVIS_POINT4D_TAG operator=(const TVIS_POINT4D_TAG &p)
	{
		this->x = p.x;
		this->y = p.y;
		this->z = p.z;
		this->w = p.w;
		return *this;
	}

	inline TVIS_POINT4D_TAG operator+=(double scalar)
	{
		this->x += scalar;
		this->y += scalar;
		this->z += scalar;
		return *this;
	}

	inline TVIS_POINT4D_TAG operator-=(double scalar)
	{
		this->x -= scalar;
		this->y -= scalar;
		this->z -= scalar;
		return *this;
	}

	inline TVIS_POINT4D_TAG operator*=(double scalar)
	{
		this->x *= scalar;
		this->y *= scalar;
		this->z *= scalar;
		return *this;
	}

	inline TVIS_POINT4D_TAG operator/=(double scalar)
	{
		this->x /= scalar;
		this->y /= scalar;
		this->z /= scalar;
		return *this;
	}

	inline TVIS_POINT4D_TAG operator+=(const TVIS_POINT4D_TAG &p)
	{
		this->x += p.x;
		this->y += p.y;
		this->z += p.z;
		return *this;
	}

	inline TVIS_POINT4D_TAG operator-=(const TVIS_POINT4D_TAG &p)
	{
		this->x -= p.x;
		this->y -= p.y;
		this->z -= p.z;
		return *this;
	}

	inline TVIS_POINT4D_TAG operator*=(const TVIS_POINT4D_TAG &p)
	{
		this->x *= p.x;
		this->y *= p.y;
		this->z *= p.z;
		return *this;
	}

	inline TVIS_POINT4D_TAG operator/=(const TVIS_POINT4D_TAG &p)
	{
		this->x /= p.x;
		this->y /= p.y;
		this->z /= p.z;
		return *this;
	}

	inline TVIS_POINT4D_TAG operator+(double scalar)
	{
		return TVIS_POINT4D_TAG(this->x + scalar, this->y + scalar, this->z + scalar);
	}

	inline TVIS_POINT4D_TAG operator-(double scalar)
	{
		return TVIS_POINT4D_TAG(this->x - scalar, this->y - scalar, this->z - scalar);
	}

	inline TVIS_POINT4D_TAG operator*(double scalar)
	{
		return TVIS_POINT4D_TAG(this->x * scalar, this->y * scalar, this->z * scalar);
	}

	inline TVIS_POINT4D_TAG operator/(double scalar)
	{
		return TVIS_POINT4D_TAG(this->x / scalar, this->y / scalar, this->z / scalar);
	}

	inline TVIS_POINT4D_TAG operator+(const TVIS_POINT4D_TAG &p)
	{
		return TVIS_POINT4D_TAG(this->x + p.x, this->y + p.y, this->z + p.z);
	}

	inline TVIS_POINT4D_TAG operator-(const TVIS_POINT4D_TAG &p)
	{
		return TVIS_POINT4D_TAG(this->x - p.x, this->y - p.y, this->z - p.z);
	}
	
	inline TVIS_POINT4D_TAG operator*(const TVIS_POINT4D_TAG &p)
	{
		return TVIS_POINT4D_TAG(this->x * p.x, this->y * p.y, this->z * p.z);
	}

	inline TVIS_POINT4D_TAG operator/(const TVIS_POINT4D_TAG &p)
	{
		return TVIS_POINT4D_TAG(this->x / p.x, this->y / p.y, this->z / p.z);
	}

	inline void Set(double x, double y, double z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = 1.0;
	}

	inline void Set(double x, double y, double z, double w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	inline void SetLimitMin()
	{
		this->x = -DBL_MAX;
		this->y = -DBL_MAX;
		this->z = -DBL_MAX;
	}

	inline void SetLimitMax()
	{
		this->x = DBL_MAX;
		this->y = DBL_MAX;
		this->z = DBL_MAX;
	}

	inline void Normalize()
	{
		double len = sqrt(x*x+y*y+z*z);
		if (len == 0) return;
		len = 1.0/len;
		
		x *= len;
		y *= len;
		z *= len;
	}

	inline double Length()
	{
		return sqrt(x*x+y*y+z*z);
	}

	inline double Dot(const TVIS_POINT4D_TAG &p)
	{
		return (x*p.x+y*p.y+z*p.z);		
	}

	inline void Cross(const TVIS_POINT4D_TAG &vec1, const TVIS_POINT4D_TAG &vec2)
	{
		double x,y,z;
		x = vec1.y*vec2.z - vec1.z*vec2.y;
		y =	vec1.z*vec2.x - vec1.x*vec2.z;
		z =	vec1.x*vec2.y - vec1.y*vec2.x;
		this->x = x;
		this->y = y;
		this->z = z;
	}

	inline void InsertMinComponents(const TVIS_POINT4D_TAG &p)
	{
		if (x > p.x) x = p.x;
		if (y > p.y) y = p.y;
		if (z > p.z) z = p.z;
	}

	inline void InsertMaxComponents(const TVIS_POINT4D_TAG &p)
	{
		if (x < p.x) x = p.x;
		if (y < p.y) y = p.y;
		if (z < p.z) z = p.z;
	}

} TVIS_POINT4D;

/** 4x4 matrix, like opengl column major !
 */
typedef struct TVIS_MATRIX4D_TAG
{
	double m[16];
	
	inline TVIS_MATRIX4D_TAG operator=(const TVIS_MATRIX4D_TAG &p)
	{
		m[0] = p.m[0];		m[1] = p.m[1];		m[2] = p.m[2];		m[3] = p.m[3];
		m[4] = p.m[4];		m[5] = p.m[5];		m[6] = p.m[6];		m[7] = p.m[7];
		m[8] = p.m[8];		m[9] = p.m[9];		m[10] = p.m[10];	m[11] = p.m[11];
		m[12] = p.m[12];	m[13] = p.m[13];	m[14] = p.m[14];	m[15] = p.m[15];

		return *this;
	}

	// column major..
	inline TVIS_POINT4D operator*(const TVIS_POINT4D &p)
	{
		return TVIS_POINT4D_TAG(m[0]*p.x + m[4]*p.y + m[8]*p.z + m[12]*p.w, 
								m[1]*p.x + m[5]*p.y + m[9]*p.z + m[13]*p.w,
								m[2]*p.x + m[6]*p.y + m[10]*p.z + m[14]*p.w,
								m[3]*p.x + m[7]*p.y + m[11]*p.z + m[15]*p.w);
	}

	// this is useful to invert an rotation.
	inline void Flip()
	{
		swapf(m[1], m[4]);				
		swapf(m[2], m[8]);				
		swapf(m[3], m[12]);				
		swapf(m[6], m[9]);				
		swapf(m[7], m[13]);				
		swapf(m[11], m[14]);				
	}

} TVIS_MATRIX4D;












// enumerators
/////////////////////////////////////////////////////////////////////////////////////////////////////



/** viewport type
 */
typedef enum 
{
	VP_FRONT,
	VP_LEFT,
	VP_TOP,
	VP_PERSPECTIVE
} TVIS_VIEWPORT_TYPE;


/** point style
 */
typedef enum 
{
	PS_POINT,			// real points
	PS_CROSS			// crosses
} TVIS_PS;	



/** line style
 */
typedef enum 
{
	LS_DOT,				// dot dot dot..
	LS_LINE,			// normal line
	LS_LINEDOT,			// line dot line dot..
	LS_LINELINE			// line short line line short line..
} TVIS_LS;	


/** dataset point  / line mode
 */
typedef enum 
{
	SM_SINGLE,			// single size
	SM_INT,				// size int buffer (TVIS_DATAPARAM1)
	SM_FLOAT			// size from float buffer (TVIS_DATAPARAM2)
} TVIS_SIZEM;



/** dataset colorize style
 */
typedef enum 
{
	CS_SINGLE,			// single color
	CS_PLANE_X,			// colorize to x position 
	CS_PLANE_Y,			// colorize to y position 
	CS_PLANE_Z,			// colorize to z position 
	CS_INTENSITYINT,	// intensity values (TVIS_DATAPARAM1) from dataset
	CS_INTENSITYFLOAT,	// intensity values (TVIS_DATAPARAM2) from dataset
	CS_COLOR			// color values (TVIS_DATAPARAM1) from dataset
} TVIS_COLS;	



/** cursorshapes
 */
typedef enum
{
	CUR_ARROW,			// standard cursor
	CUR_ARROWNS,		// north south
	CUR_ARROWEW,		// east west
	CUR_ARROWNSEW,		// all directions
	CUR_ARROWROTATE,	// arrow describing a rotation
	CUR_HGLASS,			// hourglass
	CUR_HAND,			
	CUR_MAGINFIER,		
	CUR_RECTMAGNIFIER,
	CUR_SELRECT,		// rectangular selection arrow
	CUR_SELCIRCLE,		// circular selection arrow
	CUR_DESELRECT,		// rectangular deselection arrow
	CUR_DESELCIRCLE,	// circular deselection arrow
	CUR_OBJSELRECT,		// rectangular selection arrow for object selection
	CUR_OBJSELCIRCLE,	// circular selection arrow for object selection
	CUR_OBJDESELRECT,	// rectangular deselection arrow for object selection
	CUR_OBJDESELCIRCLE,	// circular deselection arrow for object selection
	CUR_ELEMENT,		// element selection cross
	CUR_ELEMENTSEL,		// element selection cross with (+)
	CUR_ELEMENTDESEL	// element deselection cross with (-)
} TVIS_CURSORSHAPE;


/** additional graphic items, 
 */
typedef enum 
{
	SHOW_WINCENTRE	= 0x01,		// window center - not used at the moment
	SHOW_TRBALL		= 0x02, 	// trackball for rotation
	SHOW_COLORBAR	= 0x04,		// colorbar
	SHOW_WKSYSTEM	= 0x08,		// world coordinate system
	SHOW_MMKSYSTEM	= 0x10,		// model coordinate system
	SHOW_GRID		= 0x20,		// grid
	SHOW_GRIDUNITS	= 0x40,		// units on grid
	SHOW_CURSPOS	= 0x80,		// cursor position - not used at the moment
	
} TVIS_GRITEMS;

/** item container, used to deal with TVIS_GRITEMS
 */
typedef struct TVIS_GRITEMS_CONTAINER_TAG
{
private:
	TVIS_GRITEMS items;

public:

	TVIS_GRITEMS_CONTAINER_TAG()
	{
		items = (TVIS_GRITEMS) 0;
	}

	inline bool IsSet(int i)
	{
		if (items & i) return true;
		return false;
	}

	inline void Set(int i)
	{
		items = (TVIS_GRITEMS) (items | i); 
	}

	inline void Remove(int i)
	{
		items = (TVIS_GRITEMS) (items & (0x0ffffff - i)); 
	}
	
} TVIS_GRITEMS_CONTAINER;



#endif