//! \verbatim
//! ###########################################################################
//! # ASPECT: A spectra clustering tool for exploration of large spectral surveys - Stage I
//! #
//! # Copyright (c) 2009 Aick in der Au
//! # All rights reserved.
//! ###########################################################################
//!
//!      created by : Aick in der Au <aick.inderau@gmail.com>
//!      created on : 7/4/2009
//! code taken from : Shiva of Kolor (but not sure)
//! additional docs : none
//!  responsibility : 1. Aick in der Au
//!                   2. 
//! \endverbatim
//!
//! \file  glextensions.h
//! \brief 


#ifndef H_GLEXTENSIONS
#define H_GLEXTENSIONS

#include <Windows.h>
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
extern PFNGLGETPROGRAMIVARBPROC				glGetProgramivARB			;
extern PFNGLSHADERSOURCEARBPROC				glShaderSourceARB			;
extern PFNGLDELETEOBJECTARBPROC				glDeleteObjectARB			;
extern PFNGLCOMPILESHADERARBPROC			glCompileShaderARB			;
extern PFNGLCREATEPROGRAMOBJECTARBPROC		glCreateProgramObjectARB	;
extern PFNGLATTACHOBJECTARBPROC				glAttachObjectARB			;
extern PFNGLDETACHOBJECTARBPROC				glDetachObjectARB			;
extern PFNGLLINKPROGRAMARBPROC				glLinkProgramARB			;
extern PFNGLUSEPROGRAMOBJECTARBPROC			glUseProgramObjectARB		;
extern PFNGLGETOBJECTPARAMETERIVARBPROC		glGetObjectParameterivARB	;
extern PFNGLGETINFOLOGARBPROC				glGetInfoLogARB				;
extern PFNGLUNIFORM1IARBPROC				glUniform1iARB				;
extern PFNGLUNIFORM3FVARBPROC				glUniform3fvARB				;
extern PFNGLUNIFORM4FVARBPROC				glUniform4fvARB				;
extern PFNGLGETUNIFORMLOCATIONARBPROC		glGetUniformLocationARB		;
extern PFNGLUNIFORM1FARBPROC				glUniform1fARB				;
//PFNGLUNIFORMMATRIX4FVARBPROC			glUniformMatrix4fvARB		;


extern PFNGLGETATTRIBLOCATIONARBPROC		glGetAttribLocationARB		;

// multi texture ext
extern PFNGLCLIENTACTIVETEXTUREARBPROC		glClientActiveTextureARB	;
extern PFNGLACTIVETEXTUREARBPROC			glActiveTextureARB			;
extern PFNGLMULTITEXCOORD2FARBPROC			glMultiTexCoord2fARB		;
extern PFNGLMULTITEXCOORD2FVARBPROC			glMultiTexCoord2fvARB		;

// FBO Ext
extern PFNGLGENFRAMEBUFFERSEXTPROC			glGenFramebuffersEXT;
extern PFNGLDELETEFRAMEBUFFERSEXTPROC		glDeleteFramebuffersEXT;
extern PFNGLBINDFRAMEBUFFEREXTPROC			glBindFramebufferEXT;
extern PFNGLGENRENDERBUFFERSEXTPROC			glGenRenderbuffersEXT;
extern PFNGLDELETERENDERBUFFERSEXTPROC		glDeleteRenderbuffersEXT;
extern PFNGLBINDRENDERBUFFEREXTPROC			glBindRenderbufferEXT;
extern PFNGLRENDERBUFFERSTORAGEEXTPROC		glRenderbufferStorageEXT;
extern PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC	glFramebufferRenderbufferEXT;
extern PFNGLFRAMEBUFFERTEXTURE2DEXTPROC		glFramebufferTexture2DEXT;
extern PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC	glCheckFramebufferStatusEXT;
extern PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC glGetRenderbufferParameterivEXT;

// GL_ARB_vertex_buffer_object
extern PFNGLBINDBUFFERARBPROC				glBindBufferARB;
extern PFNGLDELETEBUFFERSARBPROC			glDeleteBuffersARB;
extern PFNGLGENBUFFERSARBPROC				glGenBuffersARB;
extern PFNGLBUFFERDATAARBPROC				glBufferDataARB;
extern PFNGLMAPBUFFERARBPROC				glMapBufferARB;
extern PFNGLUNMAPBUFFERARBPROC				glUnmapBufferARB;

// WGL_EXT_swap_control_supported
extern PFNWGLSWAPINTERVALEXTPROC			wglSwapIntervalEXT;

// WGL_ARB_pixel_format
extern PFNWGLGETPIXELFORMATATTRIBIVARBPROC	wglGetPixelFormatAttribivARB;
extern PFNWGLGETPIXELFORMATATTRIBFVARBPROC	wglGetPixelFormatAttribfvARB;
extern PFNWGLCHOOSEPIXELFORMATARBPROC		wglChoosePixelFormatARB;

// WGL_ARB_render_texture
extern PFNWGLBINDTEXIMAGEARBPROC			wglBindTexImageARB; 
extern PFNWGLRELEASETEXIMAGEARBPROC			wglReleaseTexImageARB; 

// WGL_ARB_extensions_string
extern PFNWGLGETEXTENSIONSSTRINGARBPROC		wglGetExtensionsStringARB;

namespace GLExtensions
{
	bool loadGLExtensions( bool _silent = true );
}

#endif // H_GLEXTENSIONS
