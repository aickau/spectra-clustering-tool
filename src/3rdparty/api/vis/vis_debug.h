#ifndef _VIS_DEBUG_H
#define _VIS_DEBUG_H

#define  X_DEBUG
#ifdef X_DEBUG
#include "vis_types.h"
#include "vis_patterns.h"
#include "vis_glhelper.h"
#include "vis_font.h"
#include "GL/gl.h"
#include <vector>

VIS_GLFONTMANAGER();


/// debug identifier, to identifiy already included debug informations 
class CVisDebugIdentifier
{
public:
	CString			src_file;
	int				line;

	inline CVisDebugIdentifier& operator=(const CVisDebugIdentifier &c)
	{
		this->src_file	= c.src_file;
		this->line		= c.line;

		return *this;
	}

	inline bool operator==(const CVisDebugIdentifier &c)
	{
		if ((this->src_file == c.src_file) && (this->line == c.line)) 
			return true;
		return false;
	}
};


/// our debug object, TVIS_DBG_POINT2D, ... TVIS_DBG_BOX2D_TAG with specific drawing code
/// is inserted here
template<typename T>
class CVisDebugObject
{
public:
	CVisDebugObject():count(0),obj(2) {};
	~CVisDebugObject() 
	{
		obj.clear();
	}

	void Draw()
	{
		for (size_t i=0;i<count;i++)
		{
			obj[i].Draw();
		}
	}

	void Latch(T &elm)
	{
		bool insert = true;

		// is debug information already stored in our vector ? if yes only update
		for (size_t i=0;i<count;i++)
		{
			if (obj[i] == elm)
			{
				obj[i] = elm;
				insert = false;
			}
		}

		// ..otherwise insert 
		if (insert) 
		{
			// grow if necessary
			if (count >= obj.size())
			{
				obj.resize(obj.size()*2);
			}
			obj[count] = elm;
			count++;
		}
	}


private:
	size_t			count;
	std::vector<T>	obj;
};


/// basic framework for debug objects
typedef struct TVIS_DBG_BASE_TAG: public CVisGLHelper
{
	double					width;
	TVIS_COLOR4B			color;
	CVisDebugIdentifier		ident;

	inline TVIS_DBG_BASE_TAG &operator=(const TVIS_DBG_BASE_TAG &c)
	{
		this->width		= c.width;
		this->color		= c.color;
		this->ident		= c.ident;

		return *this;
	}

	inline bool operator==(const TVIS_DBG_BASE_TAG &c)
	{
		return ident.operator==(c.ident);
	}


} TVIS_DBG_BASE;

/// to display points in 2d
typedef struct TVIS_DBG_POINT2D_TAG: public TVIS_DBG_BASE
{
	TVIS_POINT2D	p;

	inline TVIS_DBG_POINT2D_TAG& operator=(const TVIS_DBG_POINT2D_TAG &c)
	{
		TVIS_DBG_BASE::operator=(c);
		
		this->p			= c.p;

		return *this;
	}

	inline void Draw()
	{
		vis_glPointSize(width);
		vis_glSetColor(color);
		vis_glDraw2DPoint(p, 0.0);
	}

} TVIS_DBG_POINT2D;


/// to display points in 3d
typedef struct TVIS_DBG_POINT4D_TAG: public TVIS_DBG_BASE
{
	TVIS_POINT4D	p;

	inline void Draw()
	{
		vis_glPointSize(width);
		vis_glSetColor(color);
		vis_glDraw3DPoint(p);
	}

} TVIS_DBG_POINT4D;

/// to display lines in 2d
typedef struct TVIS_DBG_LINE2D_TAG: public TVIS_DBG_BASE
{
	TVIS_POINT2D	p1, p2;

	inline void Draw()
	{
		vis_glLineParam(width, LS_LINE);
		vis_glSetColor(color);
		vis_glDraw2DLine(p1, p2, 0.0);
	}
} TVIS_DBG_LINE2D;


/// to display lines in 3d
typedef struct TVIS_DBG_LINE4D_TAG: public TVIS_DBG_BASE
{
	TVIS_POINT4D	p1, p2;

	inline void Draw()
	{
		vis_glLineParam(width, LS_LINE);
		vis_glSetColor(color);
		vis_glDraw3DLine(p1, p2);
	}

} TVIS_DBG_LINE4D;

/// to display axis aligned boxes
typedef struct TVIS_DBG_RECT2D_TAG: public TVIS_DBG_BASE
{
	bool			wire;
	TVIS_RECT2D		r;

	inline void Draw()
	{
		if (wire)
		{
			vis_glPolygonMode(GL_LINE);
			vis_glLineParam(width, LS_LINE);
		}
		else
		{
			vis_glLineParam(1.0, LS_LINE);
		}
		
		vis_glSetColor(color);
		vis_glDraw2DRectangle(r, 0.0);
	
		if (wire)
		{
			vis_glPolygonMode(GL_FILL);
		}

	}

} TVIS_DBG_RECT2D;


/// to display boxes
typedef struct TVIS_DBG_BOX2D_TAG: public TVIS_DBG_BASE
{
	bool			wire;
	TVIS_BOX2D		b;

	inline void Draw()
	{
		if (wire)
		{
			vis_glPolygonMode(GL_LINE);
			vis_glLineParam(width, LS_LINE);
		}
		else
		{
			vis_glLineParam(1.0, LS_LINE);
		}
		
		vis_glSetColor(color);
		vis_glDraw2DBox(b, 0.0);
	
		if (wire)
		{
			vis_glPolygonMode(GL_FILL);
		}
	}
} TVIS_DBG_BOX2D;


/// to display text
typedef struct TVIS_DBG_TEXT_TAG: public TVIS_DBG_BASE
{
	CString			text;
	TVIS_POINT4D	pos;

	inline void Draw()
	{
		TVIS_FONTID id = VIS_GETGLFONTID("System2");
		vis_glSetColor(TVIS_COLOR4B(255,255,255));
		vis_glPrint(id,pos,text);
	}
} TVIS_DBG_TEXT;



/** Singleton Debug Manager
 for visualisation of debug information (points, lines, boxes, text etc.) (high experimental)
 usage: CVisDebug::Instance().Insert([paramlist]); to insert debug information
 only one point/line/box etc. is drawn. if you use this in a loop the last pass is visualized
*/
class CVisDebug: public Singleton<CVisDebug>, public CVisGLHelper
{
public:
	~CVisDebug()
	{
	}

	void Draw(double znear)
	{
		vis_glPushMatrix();
		vis_glTranslate(TVIS_POINT4D(0,0,-znear));

		dbg_p2.Draw();
		dbg_p4.Draw();
		dbg_l2.Draw();
		dbg_l4.Draw();
		dbg_r2.Draw();
		dbg_b2.Draw();
		dbg_t.Draw();

		vis_glPopMatrix();
	}


	/// point 2d
	void Insert(char *src_file, int line, TVIS_POINT2D &p, double width=1.0, TVIS_COLOR4B &col=TVIS_COLOR4B(255,0,0))
	{
		TVIS_DBG_POINT2D _dbg;
		_dbg.ident.src_file	= src_file;
		_dbg.ident.line		= line;
		_dbg.color			= col;
		_dbg.width			= width;
		_dbg.p				= p;
		dbg_p2.Latch(_dbg);
	}

	/// point 3d
	void Insert(char *src_file, int line, TVIS_POINT4D &p, double width=1.0, TVIS_COLOR4B &col=TVIS_COLOR4B(255,0,0))
	{
		TVIS_DBG_POINT4D _dbg;
		_dbg.ident.src_file	= src_file;
		_dbg.ident.line		= line;
		_dbg.color			= col;
		_dbg.width			= width;
		_dbg.p				= p;
		dbg_p4.Latch(_dbg);
	}

	/// line 2d
	void Insert(char *src_file, int line, TVIS_POINT2D &lp1, TVIS_POINT2D &lp2, double width=1.0, TVIS_COLOR4B &col=TVIS_COLOR4B(255,0,0))
	{
		TVIS_DBG_LINE2D _dbg;
		_dbg.ident.src_file	= src_file;
		_dbg.ident.line		= line;
		_dbg.color			= col;
		_dbg.width			= width;
		_dbg.p1				= lp1;
		_dbg.p2				= lp2;
		dbg_l2.Latch(_dbg);
	}

	/// line 3d
	void Insert(char *src_file, int line, TVIS_POINT4D &lp1, TVIS_POINT4D &lp2, double width=1.0, TVIS_COLOR4B &col=TVIS_COLOR4B(255,0,0))
	{
		TVIS_DBG_LINE4D _dbg;
		_dbg.ident.src_file	= src_file;
		_dbg.ident.line		= line;
		_dbg.color			= col;
		_dbg.width			= width;
		_dbg.p1				= lp1;
		_dbg.p2				= lp2;
		dbg_l4.Latch(_dbg);
	}

	/// axis aligned box
	void Insert(char *src_file, int line, TVIS_RECT2D &r2, double width=1.0, TVIS_COLOR4B &col=TVIS_COLOR4B(255,0,0))
	{
		TVIS_DBG_RECT2D _dbg;
		_dbg.ident.src_file	= src_file;
		_dbg.ident.line		= line;
		_dbg.color			= col;
		_dbg.width			= width;
		_dbg.r				= r2;
		dbg_r2.Latch(_dbg);
	}

	/// box
	void Insert(char *src_file, int line, TVIS_BOX2D &b2, double width=1.0, TVIS_COLOR4B &col=TVIS_COLOR4B(255,0,0))
	{
		TVIS_DBG_BOX2D _dbg;
		_dbg.ident.src_file	= src_file;
		_dbg.ident.line		= line;
		_dbg.color			= col;
		_dbg.width			= width;
		_dbg.b				= b2;
		dbg_b2.Latch(_dbg);
	}

	/// text
	void Insert(char *src_file, int line, TVIS_POINT4D &pos, CString &text)
	{
		TVIS_DBG_TEXT _dbg;
		_dbg.ident.src_file	= src_file;
		_dbg.ident.line		= line;
		_dbg.pos			= pos;
		_dbg.text			= text;
		dbg_t.Latch(_dbg);
	}


private:

	CVisDebugObject<TVIS_DBG_POINT2D>	dbg_p2;
	CVisDebugObject<TVIS_DBG_POINT4D>	dbg_p4;
	CVisDebugObject<TVIS_DBG_LINE2D>	dbg_l2;
	CVisDebugObject<TVIS_DBG_LINE4D>	dbg_l4;
	CVisDebugObject<TVIS_DBG_RECT2D>	dbg_r2;
	CVisDebugObject<TVIS_DBG_BOX2D>		dbg_b2;
	CVisDebugObject<TVIS_DBG_TEXT>		dbg_t;

};

/// this is inserted in the drawing class CVisGLView
#define DBG_DRAW(znear) CVisDebug::Instance().Draw(znear);

#else

/// only used in debug build
#define DBG_DRAW(znear) ;

#endif
#endif
