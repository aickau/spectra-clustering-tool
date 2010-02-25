#ifndef _VIS_FONT_H
#define _VIS_FONT_H

#include "vis_types.h"
#include "vis_glhelper.h"
#include "vis_typesintern.h"
#include <map>


/** always use with CVisGLFontManager. 
 * on construction this class registers a font display list to opengl
 */
class CVisGLFont: private CVisGLHelper
{
public:
	CVisGLFont(HDC hDC, const char *gl_fontname, const char *fontname, int size, bool bold = false, bool italic = false, bool underline = false);
	~CVisGLFont();

	int				GetSize();
	TVIS_FONTID		GetFontId();
	bool			IsFont(const char *gl_fontname);
	void			KillFont();

private:
	TVIS_FONTID		m_FontId;
	int				m_Size;
	HGLRC			m_hGLRC;
	CString			m_GLFontName;
};


/** Font Manager - manages all font handling
 * usage: 	
 * - use VIS_GLFONTMANAGER(); at top of every cpp file which uses VIS_INSERTGLFONT() or VIS_GETGLFONTID()
 * - use VIS_INSERTGLFONT(hDC, "Rule Font Serif8", "MS Sans Serif", 8, true, false, false); to create gl font
 * - use VIS_GETGLFONTID("Rule Font Serif8"); to get font id
 */
class CVisGLFontManager
{
public:
	~CVisGLFontManager();

	/** insert gl font.
	 * @param hDC Windows Device Context 
	 * @param gl_fontname unique font name, e.g. "My Super Rule Font". note: this is only used as key for RemoveGLFont() and GetGLFont() 
	 * and has nothing todo with windows font names.
	 * @param fontname windows font name, e.g. "Arial", "Times New Roman",..
	 * @param size font size in pt.
	 * @param bold font style
	 * @param italic font style
	 * @param underline font style
	 */
	void				InsertGLFont(HDC hDC, const char *gl_fontname, const char *fontname, int size, bool bold = false, bool italic = false, bool underline = false);

	/** get gl font.
	 * @param gl_fontname unique font name which is registered with InsertGLFont().
	 * @return if font cannot be found NULL, otherwise pointer to CVisGLFont
	 */
	CVisGLFont			*GetGLFont(const char *gl_fontname);

	/** get gl font id.
	 * @param gl_fontname unique font name which is registered with InsertGLFont().
	 * @return if font cannot be found 0, otherwise font id
	 */
	TVIS_FONTID			GetFontId(const char *gl_fontname);



private:

	typedef std::list<CVisGLFont> TFONTLIST;
	
	TFONTLIST			m_FontList;
};

#define VIS_GLFONTMANAGER()			extern CVisGLFontManager __globalfontmanager 
#define VIS_GETGLFONTID(glfont)		__globalfontmanager.GetFontId(CString(glfont))
#define VIS_INSERTGLFONT(hDC, gl_fontname, fontname, size, bold, italic, underline)		__globalfontmanager.InsertGLFont(hDC, gl_fontname, fontname, size, bold, italic, underline)				


#endif