#ifndef H_GLEXTENSIONS
#define H_GLEXTENSIONS

#include <gl/gl.h>
#include "../glext.h"
#include "../wglext.h"


extern PFNGLBLENDEQUATIONEXTPROC			glBlendEquationEXT			;

extern PFNGLBINDPROGRAMARBPROC				glBindProgramARB			;
extern PFNGLPROGRAMSTRINGARBPROC			glProgramStringARB			;
extern PFNGLPROGRAMLOCALPARAMETER4FARBPROC	glProgramLocalParameter4fARB;
extern PFNGLPROGRAMENVPARAMETER4FARBPROC	glProgramEnvParameter4fARB	;
extern PFNGLVERTEXATTRIB3FVARBPROC			glVertexAttrib3fvARB		;
extern PFNGLENABLEVERTEXATTRIBARRAYARBPROC	glEnableVertexAttribArrayARB ;
extern PFNGLDISABLEVERTEXATTRIBARRAYARBPROC glDisableVertexAttribArrayARB ;



extern PFNGLCREATESHADEROBJECTARBPROC		glCreateShaderObjectARB		;
extern PFNGLISPROGRAMARBPROC				glIsProgramARB				;
extern PFNGLGETPROGRAMIVARBPROC			glGetProgramivARB			;
extern PFNGLSHADERSOURCEARBPROC			glShaderSourceARB			;
extern PFNGLDELETEOBJECTARBPROC			glDeleteObjectARB			;
extern PFNGLCOMPILESHADERARBPROC			glCompileShaderARB			;
extern PFNGLCREATEPROGRAMOBJECTARBPROC		glCreateProgramObjectARB	;
extern PFNGLATTACHOBJECTARBPROC			glAttachObjectARB			;
extern PFNGLDETACHOBJECTARBPROC			glDetachObjectARB			;
extern PFNGLLINKPROGRAMARBPROC				glLinkProgramARB			;
extern PFNGLUSEPROGRAMOBJECTARBPROC		glUseProgramObjectARB		;
extern PFNGLGETOBJECTPARAMETERIVARBPROC	glGetObjectParameterivARB	;
extern PFNGLGETINFOLOGARBPROC				glGetInfoLogARB				;
extern PFNGLUNIFORM1IARBPROC				glUniform1iARB				;
extern PFNGLUNIFORM3FVARBPROC				glUniform3fvARB				;
extern PFNGLUNIFORM4FVARBPROC				glUniform4fvARB				;
extern PFNGLGETUNIFORMLOCATIONARBPROC		glGetUniformLocationARB		;
extern PFNGLUNIFORM1FARBPROC				glUniform1fARB				;
//PFNGLUNIFORMMATRIX4FVARBPROC		glUniformMatrix4fvARB		;


extern PFNGLGETATTRIBLOCATIONARBPROC		glGetAttribLocationARB		;

// multi texture ext
extern PFNGLCLIENTACTIVETEXTUREARBPROC		glClientActiveTextureARB	;
extern PFNGLACTIVETEXTUREARBPROC			glActiveTextureARB			;
extern PFNGLMULTITEXCOORD2FARBPROC			glMultiTexCoord2fARB		;
extern PFNGLMULTITEXCOORD2FVARBPROC		glMultiTexCoord2fvARB		;

// GL_ARB_vertex_buffer_object
extern PFNGLBINDBUFFERARBPROC				glBindBufferARB				;
extern PFNGLDELETEBUFFERSARBPROC			glDeleteBuffersARB			;
extern PFNGLGENBUFFERSARBPROC				glGenBuffersARB				;
extern PFNGLBUFFERDATAARBPROC				glBufferDataARB				;
extern PFNGLMAPBUFFERARBPROC				glMapBufferARB				;
extern PFNGLUNMAPBUFFERARBPROC				glUnmapBufferARB			;
// WGL_EXT_swap_control_supported
extern PFNWGLSWAPINTERVALEXTPROC			wglSwapIntervalEXT			;


// WGL_ARB_pixel_format
extern PFNWGLGETPIXELFORMATATTRIBIVARBPROC	wglGetPixelFormatAttribivARB ;
extern PFNWGLGETPIXELFORMATATTRIBFVARBPROC	wglGetPixelFormatAttribfvARB ;
extern PFNWGLCHOOSEPIXELFORMATARBPROC		wglChoosePixelFormatARB		;

// WGL_ARB_pbuffer
extern PFNWGLCREATEPBUFFERARBPROC			wglCreatePbufferARB			; 
extern PFNWGLGETPBUFFERDCARBPROC			wglGetPbufferDCARB			;
extern PFNWGLRELEASEPBUFFERDCARBPROC		wglReleasePbufferDCARB		;
extern PFNWGLDESTROYPBUFFERARBPROC			wglDestroyPbufferARB		;
extern PFNWGLQUERYPBUFFERARBPROC			wglQueryPbufferARB			; 

// WGL_ARB_render_texture
extern PFNWGLBINDTEXIMAGEARBPROC			wglBindTexImageARB			; 
extern PFNWGLRELEASETEXIMAGEARBPROC		wglReleaseTexImageARB		; 
// WGL_ARB_extensions_string
extern PFNWGLGETEXTENSIONSSTRINGARBPROC	wglGetExtensionsStringARB	;



bool LoadWGLExtensions ( bool silent );
bool LoadGLExtensions ( bool silent );

#endif
