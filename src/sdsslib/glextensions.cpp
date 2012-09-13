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
//! additional docs : none
//!  responsibility : 1. Aick in der Au
//!                   2. 
//! \endverbatim
//!
//! \file  glextensions.cpp
//! \brief GL extension loader mechanism 

#include "sdsslib/glextensions.h"

#include <conio.h>



int glMaxTextures = 0;
int glMaxCombinersNV = 0;
int glMaxTexturesize = 0;
int glMaxLights = 0;
bool glHasVP10 = false;
bool glHasVP11 = false;


PFNGLBLENDEQUATIONEXTPROC			glBlendEquationEXT			= NULL;

PFNGLBINDPROGRAMARBPROC				glBindProgramARB			= NULL;
PFNGLPROGRAMSTRINGARBPROC			glProgramStringARB			= NULL;
PFNGLPROGRAMLOCALPARAMETER4FARBPROC	glProgramLocalParameter4fARB= NULL;
PFNGLPROGRAMENVPARAMETER4FARBPROC	glProgramEnvParameter4fARB	= NULL;
PFNGLVERTEXATTRIB3FVARBPROC			glVertexAttrib3fvARB		= NULL;
PFNGLENABLEVERTEXATTRIBARRAYARBPROC	glEnableVertexAttribArrayARB = NULL;
PFNGLDISABLEVERTEXATTRIBARRAYARBPROC glDisableVertexAttribArrayARB = NULL;



PFNGLCREATESHADEROBJECTARBPROC		glCreateShaderObjectARB		= NULL;
PFNGLISPROGRAMARBPROC				glIsProgramARB				= NULL;
PFNGLGETPROGRAMIVARBPROC			glGetProgramivARB			= NULL;
PFNGLSHADERSOURCEARBPROC			glShaderSourceARB			= NULL;
PFNGLDELETEOBJECTARBPROC			glDeleteObjectARB			= NULL;
PFNGLCOMPILESHADERARBPROC			glCompileShaderARB			= NULL;
PFNGLCREATEPROGRAMOBJECTARBPROC		glCreateProgramObjectARB	= NULL;
PFNGLATTACHOBJECTARBPROC			glAttachObjectARB			= NULL;
PFNGLDETACHOBJECTARBPROC			glDetachObjectARB			= NULL;
PFNGLLINKPROGRAMARBPROC				glLinkProgramARB			= NULL;
PFNGLUSEPROGRAMOBJECTARBPROC		glUseProgramObjectARB		= NULL;
PFNGLGETOBJECTPARAMETERIVARBPROC	glGetObjectParameterivARB	= NULL;
PFNGLGETINFOLOGARBPROC				glGetInfoLogARB				= NULL;
PFNGLUNIFORM1IARBPROC				glUniform1iARB				= NULL;
PFNGLUNIFORM3FVARBPROC				glUniform3fvARB				= NULL;
PFNGLUNIFORM4FVARBPROC				glUniform4fvARB				= NULL;
PFNGLGETUNIFORMLOCATIONARBPROC		glGetUniformLocationARB		= NULL;
PFNGLUNIFORM1FARBPROC				glUniform1fARB				= NULL;
//PFNGLUNIFORMMATRIX4FVARBPROC		glUniformMatrix4fvARB		= NULL;


PFNGLGETATTRIBLOCATIONARBPROC		glGetAttribLocationARB		= NULL;

// multi texture ext
PFNGLCLIENTACTIVETEXTUREARBPROC		glClientActiveTextureARB	= NULL;
PFNGLACTIVETEXTUREARBPROC			glActiveTextureARB			= NULL;
PFNGLMULTITEXCOORD2FARBPROC			glMultiTexCoord2fARB		= NULL;
PFNGLMULTITEXCOORD2FVARBPROC		glMultiTexCoord2fvARB		= NULL;

PFNGLGENFRAMEBUFFERSEXTPROC			glGenFramebuffersEXT		= NULL;
PFNGLDELETEFRAMEBUFFERSEXTPROC		glDeleteFramebuffersEXT		= NULL;
PFNGLBINDFRAMEBUFFEREXTPROC			glBindFramebufferEXT		= NULL;
PFNGLGENRENDERBUFFERSEXTPROC		glGenRenderbuffersEXT		= NULL;
PFNGLDELETERENDERBUFFERSEXTPROC		glDeleteRenderbuffersEXT	= NULL;
PFNGLBINDRENDERBUFFEREXTPROC		glBindRenderbufferEXT		= NULL;
PFNGLRENDERBUFFERSTORAGEEXTPROC		glRenderbufferStorageEXT	= NULL;
PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC	glFramebufferRenderbufferEXT= NULL;
PFNGLFRAMEBUFFERTEXTURE2DEXTPROC	glFramebufferTexture2DEXT	= NULL;
PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC	glCheckFramebufferStatusEXT	= NULL;
PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC glGetRenderbufferParameterivEXT = NULL;


// GL_ARB_vertex_buffer_object
PFNGLBINDBUFFERARBPROC				glBindBufferARB				= NULL;
PFNGLDELETEBUFFERSARBPROC			glDeleteBuffersARB			= NULL;
PFNGLGENBUFFERSARBPROC				glGenBuffersARB				= NULL;
PFNGLBUFFERDATAARBPROC				glBufferDataARB				= NULL;
PFNGLMAPBUFFERARBPROC				glMapBufferARB				= NULL;
PFNGLUNMAPBUFFERARBPROC				glUnmapBufferARB			= NULL;

// WGL_EXT_swap_control_supported
PFNWGLSWAPINTERVALEXTPROC			wglSwapIntervalEXT			= NULL;


// WGL_ARB_pixel_format
PFNWGLGETPIXELFORMATATTRIBIVARBPROC	wglGetPixelFormatAttribivARB = NULL;
PFNWGLGETPIXELFORMATATTRIBFVARBPROC	wglGetPixelFormatAttribfvARB = NULL;
PFNWGLCHOOSEPIXELFORMATARBPROC		wglChoosePixelFormatARB		= NULL;

// WGL_ARB_pbuffer
PFNWGLCREATEPBUFFERARBPROC			wglCreatePbufferARB			= NULL; 
PFNWGLGETPBUFFERDCARBPROC			wglGetPbufferDCARB			= NULL;
PFNWGLRELEASEPBUFFERDCARBPROC		wglReleasePbufferDCARB		= NULL;
PFNWGLDESTROYPBUFFERARBPROC			wglDestroyPbufferARB		= NULL;
PFNWGLQUERYPBUFFERARBPROC			wglQueryPbufferARB			= NULL; 

// WGL_ARB_render_texture
PFNWGLBINDTEXIMAGEARBPROC			wglBindTexImageARB			= NULL; 
PFNWGLRELEASETEXIMAGEARBPROC		wglReleaseTexImageARB		= NULL; 

// WGL_ARB_extensions_string
PFNWGLGETEXTENSIONSSTRINGARBPROC	wglGetExtensionsStringARB	= NULL;



bool allExtensions = false;


static 
bool IsExtensionSupported(const char *extension) 
{
	const GLubyte *extensions = NULL;
	const GLubyte *start;
	GLubyte *where, *terminator;
	// Extension names should not have spaces.
	where = (GLubyte *) strchr(extension, ' ');
	if (where || *extension == '\0')
		return false;
	extensions = glGetString(GL_EXTENSIONS);
	if ( !extensions ) 
		return false;
	// It takes a bit of care to be fool-proof about parsing the
	//OpenGL extensions string. Don't be fooled by sub-strings,
	//etc. 
	start = extensions;
	for (;;) {
		where = (GLubyte *) strstr((const char *) start, extension);
		if (!where) break;
		terminator = where + strlen(extension);
		if (where == start || *(where - 1) == ' ')
			if (*terminator == ' ' || *terminator == '\0')
				return true;
		start = terminator;
	}
	return false;	
}

void LoadExtension (char *name, void **dest, bool silent = true ) 
{
	*dest = wglGetProcAddress(name);	
	if ( !silent ) {
		_cprintf ("  ");
		_cprintf (name);    
	}	
	if (*dest) {
		if ( !silent ) _cprintf ("... loaded.\n");
	} else {
		if ( !silent ) _cprintf ("... failed.\n");
		allExtensions = false;
	}
}

bool loadWGLExtensions ( bool _silent ) 
{
	allExtensions = true;
	LoadExtension ("wglGetExtensionsStringARB", (void**)&wglGetExtensionsStringARB, _silent );
	LoadExtension ("wglGetPixelFormatAttribivARB", (void**)&wglGetPixelFormatAttribivARB, _silent );
	LoadExtension ("wglGetPixelFormatAttribfvARB", (void**)&wglGetPixelFormatAttribfvARB, _silent );
	LoadExtension ("wglChoosePixelFormatARB", (void**)&wglChoosePixelFormatARB, _silent);

	// swap control wgl ext
	LoadExtension ("wglSwapIntervalEXT",		(void**)&wglSwapIntervalEXT);

	// WGL_ARB_pixel_format
	LoadExtension ("wglGetPixelFormatAttribivARB",		(void**)&wglGetPixelFormatAttribivARB, _silent );
	LoadExtension ("wglGetPixelFormatAttribfvARB",		(void**)&wglGetPixelFormatAttribfvARB, _silent );
	LoadExtension ("wglChoosePixelFormatARB",			(void**)&wglChoosePixelFormatARB, _silent );

	// WGL_ARB_pbuffer
	LoadExtension ("wglCreatePbufferARB", (void**)&wglCreatePbufferARB, _silent );
	LoadExtension ("wglGetPbufferDCARB", (void**)&wglGetPbufferDCARB, _silent );
	LoadExtension ("wglReleasePbufferDCARB", (void**)&wglReleasePbufferDCARB, _silent );
	LoadExtension ("wglDestroyPbufferARB", (void**)&wglDestroyPbufferARB, _silent );
	LoadExtension ("wglQueryPbufferARB", (void**)&wglQueryPbufferARB, _silent );

	// WGL_ARB_render_texture
	LoadExtension ("wglBindTexImageARB",		(void**)&wglBindTexImageARB, _silent );
	LoadExtension ("wglReleaseTexImageARB",		(void**)&wglReleaseTexImageARB, _silent );

	// WGL_ARB_extensions_string
	LoadExtension ("wglGetExtensionsStringARB",	(void**)&wglGetExtensionsStringARB, _silent );

	return allExtensions;
}

bool GLExtensions::loadGLExtensions ( bool _silent ) 
{
	allExtensions = true;

	loadWGLExtensions( _silent );

	// blend equ ext
	LoadExtension ("glBlendEquationEXT",		(void**)&glBlendEquationEXT);

	// vertex program arb
	LoadExtension ("glBindProgramARB",			(void**)&glBindProgramARB);
	LoadExtension ("glProgramStringARB",		(void**)&glProgramStringARB);
	LoadExtension ("glProgramLocalParameter4fARB",(void**)&glProgramLocalParameter4fARB);
	LoadExtension ("glProgramEnvParameter4fARB",(void**)&glProgramEnvParameter4fARB);
	LoadExtension ("glVertexAttrib3fvARB",		(void**)&glVertexAttrib3fvARB);
	LoadExtension ("glEnableVertexAttribArrayARB",(void**)&glEnableVertexAttribArrayARB);
	LoadExtension ("glDisableVertexAttribArrayARB",	(void**)&glDisableVertexAttribArrayARB);

	// shader objects ext
	LoadExtension ("glCreateShaderObjectARB",	(void**)&glCreateShaderObjectARB);
	LoadExtension ("glIsProgramARB",			(void**)&glIsProgramARB);
	LoadExtension ("glGetProgramivARB",			(void**)&glGetProgramivARB);
	LoadExtension ("glShaderSourceARB",			(void**)&glShaderSourceARB);
	LoadExtension ("glDeleteObjectARB",			(void**)&glDeleteObjectARB);
	LoadExtension ("glCompileShaderARB",		(void**)&glCompileShaderARB);
	LoadExtension ("glCreateProgramObjectARB",	(void**)&glCreateProgramObjectARB);
	LoadExtension ("glAttachObjectARB",			(void**)&glAttachObjectARB);
	LoadExtension ("glDetachObjectARB",			(void**)&glDetachObjectARB);
	LoadExtension ("glLinkProgramARB",			(void**)&glLinkProgramARB);
	LoadExtension ("glUseProgramObjectARB",		(void**)&glUseProgramObjectARB);
	LoadExtension ("glGetObjectParameterivARB",	(void**)&glGetObjectParameterivARB);
	LoadExtension ("glGetInfoLogARB",			(void**)&glGetInfoLogARB);
	LoadExtension ("glUniform1iARB",			(void**)&glUniform1iARB);
	LoadExtension ("glUniform1fARB",			(void**)&glUniform1fARB);
	LoadExtension ("glUniform3fvARB",			(void**)&glUniform3fvARB);
	LoadExtension ("glUniform4fvARB",			(void**)&glUniform4fvARB);
	LoadExtension ("glGetUniformLocationARB",	(void**)&glGetUniformLocationARB);
	//LoadExtension ("glUniformMatrix4fvARB",		(void**)&glUniformMatrix4fvARB);

	// vertex shader ext
	LoadExtension ("glGetAttribLocationARB",	(void**)&glGetAttribLocationARB);

	// GL_ARB_multitexture
	LoadExtension ("glActiveTextureARB",		(void**)&glActiveTextureARB);
	LoadExtension ("glClientActiveTextureARB",	(void**)&glClientActiveTextureARB);
	LoadExtension ("glMultiTexCoord2fARB",		(void**)&glMultiTexCoord2fARB);
	LoadExtension ("glMultiTexCoord2fvARB",		(void**)&glMultiTexCoord2fvARB);

	// GL_ARB_vertex_buffer_object
	LoadExtension ("glBindBufferARB",			(void**)&glBindBufferARB);
	LoadExtension ("glDeleteBuffersARB",		(void**)&glDeleteBuffersARB);
	LoadExtension ("glGenBuffersARB",			(void**)&glGenBuffersARB);
	LoadExtension ("glBufferDataARB",			(void**)&glBufferDataARB);
	LoadExtension ("glMapBufferARB",			(void**)&glMapBufferARB);
	LoadExtension ("glUnmapBufferARB",			(void**)&glUnmapBufferARB);

	// GL_EXT_framebuffer_object
	LoadExtension ("glGenFramebuffersEXT",		(void**)&glGenFramebuffersEXT);
	LoadExtension ("glDeleteFramebuffersEXT",	(void**)&glDeleteFramebuffersEXT);
	LoadExtension ("glBindFramebufferEXT",		(void**)&glBindFramebufferEXT);
	LoadExtension ("glGenRenderbuffersEXT",		(void**)&glGenRenderbuffersEXT);
	LoadExtension ("glDeleteRenderbuffersEXT",	(void**)&glDeleteRenderbuffersEXT);
	LoadExtension ("glBindRenderbufferEXT",		(void**)&glBindRenderbufferEXT);
	LoadExtension ("glRenderbufferStorageEXT",	(void**)&glRenderbufferStorageEXT);
	LoadExtension ("glFramebufferRenderbufferEXT",(void**)&glFramebufferRenderbufferEXT);
	LoadExtension ("glFramebufferTexture2DEXT",	(void**)&glFramebufferTexture2DEXT);
	LoadExtension ("glCheckFramebufferStatusEXT",(void**)&glCheckFramebufferStatusEXT);
	LoadExtension ("glGetRenderbufferParameterivEXT",(void**)&glGetRenderbufferParameterivEXT);
					
									
					

	allExtensions &= IsExtensionSupported("GL_EXT_texture_filter_anisotropic");
	allExtensions &= IsExtensionSupported("GL_ARB_multitexture");
	allExtensions &= IsExtensionSupported("GL_ARB_texture_border_clamp");
	allExtensions &= IsExtensionSupported("GL_ARB_texture_mirrored_repeat");
	allExtensions &= IsExtensionSupported("GL_EXT_blend_subtract");
	allExtensions &= IsExtensionSupported("GL_ARB_texture_cube_map");
	allExtensions &= (	IsExtensionSupported("GL_EXT_texture_rectangle") || 
		IsExtensionSupported("GL_NV_texture_rectangle") );
	//GLEXTallExtensions &= IsExtensionSupported("GL_HP_occlusion_test");
	allExtensions &= IsExtensionSupported("GL_ARB_vertex_buffer_object");
	allExtensions &= IsExtensionSupported("GL_EXT_stencil_wrap");

	// rm can run without shaders too.
	allExtensions &= IsExtensionSupported("GL_ARB_shading_language_100");
	allExtensions &= IsExtensionSupported("GL_ARB_vertex_shader");
	allExtensions &= IsExtensionSupported("GL_ARB_fragment_shader");
	allExtensions &= IsExtensionSupported("GL_ARB_vertex_program");


	if ( IsExtensionSupported( "GL_NV_register_combiners" ) ) 
		glGetIntegerv( GL_MAX_GENERAL_COMBINERS_NV, &glMaxCombinersNV );
	else
		glMaxCombinersNV = 0;
	if ( IsExtensionSupported( "GL_ARB_multitexture" ) || IsExtensionSupported( "ARB_multitexture" ) ) 
		glGetIntegerv( GL_MAX_TEXTURE_UNITS_ARB, &glMaxTextures );
	else
		glMaxTextures = 1;

	if ( IsExtensionSupported( "GL_NV_vertex_program" ) ) {
		glHasVP10 = true;
		if ( !_silent ) _cprintf ("  VP 1.0 found...");
		if ( IsExtensionSupported( "GL_NV_vertex_program1_1" ) ) {
			glHasVP11 = true;
			if ( !_silent ) _cprintf ("VP 1.1 found. hooray!\n");
		} else {
			if ( !_silent ) _cprintf ("but no VP 1.1.\n");
		}
	}

	glGetIntegerv( GL_MAX_LIGHTS, &glMaxLights );
	glGetIntegerv( GL_MAX_TEXTURE_SIZE, &glMaxTexturesize );

	return allExtensions;
}