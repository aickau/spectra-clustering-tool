//! \verbatim
//! ###########################################################################
//! # ASPECT: A spectra clustering tool for exploration of large spectral surveys - Stage I
//! #
//! # Copyright (c) 2009 Aick in der Au
//! # All rights reserved.
//! ###########################################################################
//!
//!      created by : Aick in der Au <aick.inderau@gmail.com>
//!      created on : 1/19/2009
//! additional docs : none
//!  responsibility : 1. Aick in der Au
//!                   2. 
//! \endverbatim
//!
//! \file  glhelper.cpp
//! \brief Collection of OpenGL helper fuctions.

#include "glhelper.h"
#include "debug.h"

#include <windows.h>
#include <stdio.h>
#include <assert.h>

#include <string>

#define DEFAULT_Z -5.f

#define GLHELPER_NUMCHARACTERS 96


int GLHelper::BuildFont(HDC hdc, char *fontname, int fontsize, bool bold, bool italic)								
{
	int	fontid = -1;

#ifdef _WIN32
	HFONT	font;

	fontid = glGenLists(GLHELPER_NUMCHARACTERS);				// Storage For 96 Characters

	int fontweight = (bold) ? FW_BOLD : FW_NORMAL;

	font = CreateFontA(	-fontsize,						// Height Of Font
						0,							// Width Of Font
						0,							// Angle Of Escapement
						0,							// Orientation Angle
						fontweight,					// Font Weight
						italic,						// Italic
						FALSE,						// Underline
						FALSE,						// Strikeout
						ANSI_CHARSET,				// Character Set Identifier
						OUT_TT_PRECIS,				// Output Precision
						CLIP_DEFAULT_PRECIS,		// Clipping Precision
						ANTIALIASED_QUALITY,		// Output Quality
						FF_DONTCARE|DEFAULT_PITCH,	// Family And Pitch
						LPCSTR(fontname));					// Font Name

	SelectObject(hdc, font);							
	bool bSuccess = (wglUseFontBitmaps(hdc, 32, GLHELPER_NUMCHARACTERS, fontid) == TRUE);			// Builds 96 Characters Starting At Character 32

	if ( !bSuccess )
	{
		// sometimes wglUseFontBitmaps() due to unknown reasons it works the second time.
		bSuccess = (wglUseFontBitmaps(hdc, 32, GLHELPER_NUMCHARACTERS, fontid) == TRUE);		
		if ( !bSuccess )
		{
			glDeleteLists(fontid, GLHELPER_NUMCHARACTERS);	
			fontid = -1;
		}
	}

	DeleteObject(font);
#endif
	return fontid;
}



void GLHelper::KillFont(int fontid)							
{
  glDeleteLists(fontid, GLHELPER_NUMCHARACTERS);							
}


void GLHelper::Print(int fontid, float *position, const char *fmt, ...)			
{	
	assert(fontid>0);
	if ( fontid<1 )
		return;


	char text[4096];

	va_list	ap;										
	va_start(ap, fmt);		
	const int len = min(_vscprintf( fmt, ap ) + 1, 4096); // +1 for terminating '\0'
	vsprintf_s( text, len, fmt, ap );		
	va_end(ap);	

	
	if (position) {
		glPushMatrix();
		glRasterPos3fv(position);
	}

	glPushAttrib(GL_LIST_BIT);							
	glListBase(fontid-32);								
	glCallLists(len, GL_UNSIGNED_BYTE, text);	
	glPopAttrib();	

	if (position) {
		glPopMatrix();
	}

}


bool GLHelper::PrintError(const char *_functionName) 
{
	std::string sstrErrorText("");
	std::string sstrFunctionName("");
	if ( _functionName )
	{
		sstrFunctionName = _functionName;
	}
	// gl errors
	GLenum glerr = glGetError();
	switch(glerr) {
		case GL_INVALID_ENUM		: sstrErrorText=std::string("GL Error: invalid enum");break;
		case GL_INVALID_VALUE		: sstrErrorText=std::string("GL Error: invalid value");break;
		case GL_INVALID_OPERATION	: sstrErrorText=std::string("GL Error: invalid operation");break;
		case GL_STACK_OVERFLOW		: sstrErrorText=std::string("GL Error: stack overflow");break;
		case GL_STACK_UNDERFLOW		: sstrErrorText=std::string("GL Error: stack underflow");break;
		case GL_OUT_OF_MEMORY		: sstrErrorText=std::string("GL Error: out of memory");break;
	}

	if ( !sstrErrorText.empty() )
	{
		std::string sstrErrMsg(sstrFunctionName+" "+sstrErrorText);
		Debug::PrintConsole(sstrErrMsg.c_str());
	}


	//ASSERT(glerr == GL_NO_ERROR);

	// win32/wgl errors

	DWORD err = GetLastError();
	switch(err)
	{
	case ERROR_INVALID_PIXEL_FORMAT:		Debug::PrintConsole("Win32 Error:  ERROR_INVALID_PIXEL_FORMAT\n");	break;
	case ERROR_NO_SYSTEM_RESOURCES:			Debug::PrintConsole("Win32 Error:  ERROR_NO_SYSTEM_RESOURCES\n");		break;
	case ERROR_INVALID_DATA:				Debug::PrintConsole("Win32 Error:  ERROR_INVALID_DATA\n");		break;
	case ERROR_INVALID_WINDOW_HANDLE:		Debug::PrintConsole("Win32 Error:  ERROR_INVALID_WINDOW_HANDLE\n");		break;
	case ERROR_RESOURCE_TYPE_NOT_FOUND:		Debug::PrintConsole("Win32 Error:  ERROR_RESOURCE_TYPE_NOT_FOUND\n");		break;
	case ERROR_SUCCESS:		break;
	default:	Debug::PrintConsole("Win32 Error: %i \n",err);		break;
	}
//	ASSERT(err==ERROR_SUCCESS);

	SetLastError(0);

	return ( (err==ERROR_SUCCESS) && (err==ERROR_SUCCESS) );
}
/*

void GLHelper::PrintBool(char *name, GLenum state) {
	GLboolean b;
	glGetBooleanv(state,&b);
	Debug::PrintConsole("%s %i\n",name, b);
}
*/

void GLHelper::PrintStates() 
{
	ASSERT(0);
/*
	Debug::PrintConsole("== GL States ============================================\n\n");
	PrintBool("GL_DEPTH_TEST", GL_DEPTH_TEST);
	PrintBool("GL_STENCIL_TEST", GL_STENCIL_TEST);
	PrintBool("GL_BLEND", GL_BLEND);
	PrintBool("GL_LIGHTING", GL_LIGHTING);
	PrintBool("GL_LIGHT_MODEL_TWO_SIDE", GL_LIGHT_MODEL_TWO_SIDE);
	PrintBool("GL_COLOR_MATERIAL", GL_COLOR_MATERIAL);
	PrintBool("GL_CULL_FACE", GL_CULL_FACE);

	Debug::PrintConsole("GL_CULL_FACE_MODE ");
	GLint a;
	glGetIntegerv(GL_CULL_FACE_MODE,&a);
	switch(a){
		case GL_FRONT : Debug::PrintConsole("GL_FRONT\n");break;
		case GL_BACK : Debug::PrintConsole("GL_BACK\n");break;
		case GL_FRONT_AND_BACK : Debug::PrintConsole("GL_FRONT_AND_BACK\n");break;
	}

	for (int i=0;i<8;i++) {
		glActiveTextureARB(GL_TEXTURE0_ARB+i); 
		Debug::PrintConsole("TEX %i:\n", i);
		SS_PrintBool("GL_TEXTURE_2D", GL_TEXTURE_2D);	
		SS_PrintBool("GL_TEXTURE_RECTANGLE_EXT", GL_TEXTURE_RECTANGLE_EXT);	
		SS_PrintBool("GL_TEXTURE_CUBE_MAP_ARB", GL_TEXTURE_CUBE_MAP_ARB);	
	}
	glActiveTextureARB(GL_TEXTURE0_ARB); 
	Debug::PrintConsole("== GL States End ========================================\n\n");
*/
}



GLenum GLHelper::GetInternalFormat(GLenum _format, GLenum _type )
{
	int internalFormat=0;

	if ( _type == GL_FLOAT )
	{
		switch (_format)
		{
		case GL_RGB				: internalFormat = GL_RGB_FLOAT32_ATI;				break;
		case GL_RGBA			: internalFormat = GL_RGBA_FLOAT32_ATI;				break;
		case GL_ALPHA			: internalFormat = GL_ALPHA_FLOAT32_ATI;			break;
		case GL_LUMINANCE		: internalFormat = GL_LUMINANCE_FLOAT32_ATI;		break;
		case GL_LUMINANCE_ALPHA	: internalFormat = GL_LUMINANCE_ALPHA_FLOAT32_ATI;	break;
		}
	}
	else
	{
		switch (_format)
		{
		case GL_RGB				: internalFormat = GL_RGB8;				break;
		case GL_RGBA			: internalFormat = GL_RGBA8;				break;
		case GL_ALPHA			: internalFormat = GL_ALPHA8;				break;
		case GL_LUMINANCE		: internalFormat = GL_LUMINANCE8;			break;
		case GL_LUMINANCE_ALPHA	: internalFormat = GL_LUMINANCE8_ALPHA8;	break;
		}
	}
	
	return internalFormat;
}


int GLHelper::GetNumComponents(GLenum format)
{	
	int num=0;
	switch (format)
	{
		case GL_RGB				: num = 3;	break;
		case GL_RGBA			: num = 4;	break;
		case GL_ALPHA			: num = 1;	break;
		case GL_LUMINANCE		: num = 1;	break;
		case GL_LUMINANCE_ALPHA	: num = 2;	break;
	}
	
	return num;
}


void GLHelper::Init() {

	// set some static gl flags

	glShadeModel(GL_SMOOTH);							
	glDepthFunc(GL_LEQUAL);			
	glEnable(GL_DEPTH_TEST);							

	glPolygonMode( GL_FRONT, GL_FILL );
	glPolygonMode( GL_BACK, GL_FILL );
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	
	SetPropertyState(GL_NORMALIZE, true);
	
	glClearStencil(0);	
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	glLightModeli (GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	

	PrintError();
}



void GLHelper::RegTexture(unsigned int id, unsigned int sizex, unsigned int sizey, GLenum format, GLenum type, bool mipmap, GLenum texmagfilter, void *pt, float anisotropy, GLenum target1, GLenum target2)
{	
	glBindTexture(target1, id);
	glTexParameteri(target1, GL_TEXTURE_MAG_FILTER, texmagfilter);
	glTexParameterf(target1, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);

	if(mipmap)
	{
		GLenum minfilter = (texmagfilter == GL_LINEAR) ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST;
		glTexParameteri(target1, GL_TEXTURE_MIN_FILTER, minfilter);
		if (pt) 
			gluBuild2DMipmaps(target2, GetNumComponents(format), sizex, sizey, format, type, pt);
	}
	else
	{
		glTexParameteri(target1, GL_TEXTURE_MIN_FILTER, texmagfilter);
		if (pt)
			glTexImage2D(target2, 0, GetInternalFormat(format, type), sizex, sizey, 0, format, type, pt);
	}
}



unsigned int GLHelper::RegTexture(unsigned int sizex, unsigned int sizey, GLenum format, GLenum type, bool mipmap, GLenum texmagfilter, void *pt, float anisotropy)
{
	unsigned int texid;
	glGenTextures(1, &texid);
	RegTexture(texid, sizex, sizey, format, type, mipmap, texmagfilter, pt, anisotropy);
	return texid;
}



void GLHelper::SetTexturePass(unsigned int id, int pass, GLenum textureblendmode, ETexWrapMode warpx, ETexWrapMode warpy, bool env, GLenum target, float *bordercolor)
{
	int wx = static_cast<int>(warpx);
	int wy = static_cast<int>(warpy);
	static int glwraplist[] = {GL_CLAMP, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER_ARB,GL_REPEAT,GL_MIRRORED_REPEAT_ARB};

	glActiveTextureARB(GL_TEXTURE0_ARB+pass); 
	glBindTexture(target, id);
	glEnable(target);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, textureblendmode);

	// GL_CLAMP_TO_BORDER_ARB and/or GL_MIRRORED_REPEAT_ARB can cost much if not supported.
	glTexParameteri(target, GL_TEXTURE_WRAP_S, glwraplist[wx]);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, glwraplist[wy]);

	if ( bordercolor != NULL ) {
		glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, bordercolor);
	}

	// this generates an gl error in pbuffer mode
	SetEnvMapState( env ? kTexGenMode_Haujobb : kTexGenMode_DisableAuto );
}



void GLHelper::ResetTexturePasses()
{
	for (int i=kMaxTexturePasses-1;i>=0;i--) {
		glActiveTextureARB(GL_TEXTURE0_ARB+i); 
		glDisable(GL_TEXTURE_2D);
		SetEnvMapState( kTexGenMode_DisableAuto );
	}
}


void GLHelper::SetEyeSpace(bool enable) 
{
	GLint mm;
	float m[16];
	glGetIntegerv(GL_MATRIX_MODE,&mm);
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	if (enable) {
		glMultMatrixf(m);
	}
	glMatrixMode(mm);
}


void GLHelper::SetEnvMapState(ETexGenMode state)
{
	switch (state)
	{
	case kTexGenMode_Haujobb:
		SetPropertyState(GL_TEXTURE_GEN_S, true);
		SetPropertyState(GL_TEXTURE_GEN_T, true);
		//SetPropertyState(GL_TEXTURE_GEN_R, true);
		glTexGeni(GL_T,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		glTexGeni(GL_S,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP); 
		//glTexGeni(GL_R,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		break;
	case kTexGenMode_CubeMapping:
		 SetPropertyState(GL_TEXTURE_GEN_S, true);
		 SetPropertyState(GL_TEXTURE_GEN_T, true);
		 SetPropertyState(GL_TEXTURE_GEN_R, true);
         glTexGeni (GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_ARB);
         glTexGeni (GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_ARB);
         //glTexGeni (GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_ARB);
		break;
	default:
		SetPropertyState(GL_TEXTURE_GEN_S, false);
		SetPropertyState(GL_TEXTURE_GEN_T, false);
		//SetPropertyState(GL_TEXTURE_GEN_R, false);
	}
}



void GLHelper::MaskBuffers( unsigned mask ) 
{
	glDepthMask( ((mask & kFBMask_Depth) > 0) );
	glColorMask( ((mask & kFBMask_Red) > 0), ((mask & kFBMask_Green) > 0), ((mask & kFBMask_Blue) > 0), ((mask & kFBMask_Alpha) > 0) );
	unsigned int sm = (mask & kFBMask_Stencil) > 0 ? 0x0ffffffff : 0;
	glStencilMask(sm);
}



void GLHelper::EnableArray(EArrayMask arraymask, int indexbinormal, int indextangent) {
	if (( arraymask & kArrayMask_Vertex) > 0 )
		glEnableClientState(GL_VERTEX_ARRAY);
	
	if (( arraymask & kArrayMask_Normal) > 0)
		glEnableClientState(GL_NORMAL_ARRAY);
	
	if ((arraymask & kArrayMask_Binormal) > 0)
		glEnableVertexAttribArrayARB(indexbinormal);
	
	if ((arraymask & kArrayMask_Tangent) > 0)
		glEnableVertexAttribArrayARB(indextangent);

	int msk = static_cast<int>(kArrayMask_TC0);
	for (int i=0;i<8;i++) {
		if (( arraymask & msk) > 0) {
			glClientActiveTextureARB(GL_TEXTURE0_ARB+i);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		}
		msk *= 2;
	}
}


void GLHelper::DisableArray(EArrayMask arraymask, int indexbinormal, int indextangent) {
	if (( arraymask & kArrayMask_Vertex) > 0 )
		glDisableClientState(GL_VERTEX_ARRAY);

	if (( arraymask & kArrayMask_Normal) > 0)
		glDisableClientState(GL_NORMAL_ARRAY);

	if ((arraymask & kArrayMask_Binormal) > 0)
		glDisableVertexAttribArrayARB(indexbinormal);

	if ((arraymask & kArrayMask_Tangent) > 0)
		glDisableVertexAttribArrayARB(indextangent);

	int msk = static_cast<int>(kArrayMask_TC0);
	for (int i=0;i<8;i++) {
		if (( arraymask & msk) > 0) {
			glClientActiveTextureARB(GL_TEXTURE0_ARB+i);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
		msk *= 2;
	}
}


//GLint glcubemapnames[] = {GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB,	GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB,	GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB,
//						GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB,	GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB,	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB};


void GLHelper::SetBlendMode(EBlendMode mode)
{
	if (mode == kBlendMode_Off) {
		SetPropertyState(GL_BLEND, false);
		return;
	}

	GLenum sfactor		= GL_ONE;
	GLenum destfactor	= GL_ONE;

	SetPropertyState(GL_BLEND, true);
	glBlendEquationEXT(GL_FUNC_ADD_EXT);																	// add

	switch(mode) {
		case kBlendMode_Mul:			sfactor = GL_DST_COLOR; destfactor = GL_ZERO; break;				// multiply
		case kBlendMode_Sub:			glBlendEquationEXT(GL_FUNC_REVERSE_SUBTRACT_EXT); break;			// sub
		case kBlendMode_AlphaAdd:		sfactor = GL_SRC_ALPHA; destfactor = GL_ONE; break;					// alpha add
		case kBlendMode_AlphaMul:		sfactor = GL_ZERO;		destfactor = GL_SRC_ALPHA; break;			// alpha mul
		case kBlendMode_AlphaBlend:		sfactor = GL_SRC_ALPHA; destfactor = GL_ONE_MINUS_SRC_ALPHA; break;	// alpha blend
		case kBlendMode_AlphaNegAdd:	sfactor = GL_ONE_MINUS_SRC_ALPHA;		destfactor = GL_ONE; break;	// alpha neg add
		case kBlendMode_AlphaNegMul:	sfactor = GL_ZERO;		destfactor = GL_ONE_MINUS_SRC_ALPHA; break;	// alpha neg mul
		case kBlendMode_Min:			glBlendEquationEXT(GL_MIN_EXT);break;								// min
		case kBlendMode_Max:			glBlendEquationEXT(GL_MAX_EXT);break;								// max
		case kBlendMode_SoftAdd:		sfactor = GL_ONE; destfactor = GL_ONE_MINUS_SRC_COLOR; break;		// soft add
		case kBlendMode_SoftSub:		sfactor = GL_ONE; destfactor = GL_ONE_MINUS_SRC_COLOR; glBlendEquationEXT(GL_FUNC_REVERSE_SUBTRACT_EXT);break;		// soft sub

	}

	glBlendFunc(sfactor, destfactor);
}


void GLHelper::SetAlphaTest( EAlphaTest alphatest, float treshold )
{
	if ( alphatest != kAlphaTest_Off ) {
		glEnable( GL_ALPHA_TEST );
		glAlphaFunc( static_cast<GLenum>( alphatest ), treshold );
	} else {
		glDisable( GL_ALPHA_TEST );
	}
}

void GLHelper::SetCullMode( ECullMode cullmode )
{
	if ( cullmode != kCullMode_Off ) {
		glEnable( GL_CULL_FACE );
		glCullFace( static_cast<GLenum>(cullmode) );
	} else {
		glDisable( GL_CULL_FACE );
	}
}


void GLHelper::SetLineStipple( unsigned int factor, unsigned int pattern )
{
	if ( pattern==0 || factor==0 )  {
		glDisable(GL_LINE_STIPPLE);
	} else {
		glEnable(GL_LINE_STIPPLE);
		glLineStipple( factor, pattern );
	}
}



void GLHelper::SetPropertyState(GLenum property, bool state)
{
	if (state)  {
		glEnable(property);
	} else {
		glDisable(property);
	}
}




void GLHelper::DrawQuad(float *points)
{
	glBegin(GL_QUADS);
	//	glNormal3f(0,0,1);
		glVertex3fv(&points[0]);
		glVertex3fv(&points[3]);
		glVertex3fv(&points[6]);
		glVertex3fv(&points[9]);
	glEnd();
}



void GLHelper::DrawQuadn(float *points, float *normals)
{
	glBegin(GL_QUADS);
		glNormal3fv(normals);
		glVertex3fv(&points[0]);
		glVertex3fv(&points[3]);
		glVertex3fv(&points[6]);
		glVertex3fv(&points[9]);
	glEnd();
}



void GLHelper::DrawQuad(float *points, float *texcoords)
{
	glBegin(GL_QUADS);
		glNormal3f(0,0,1);

		glTexCoord2fv(&texcoords[0]);
		glVertex3fv(&points[0]);

		glTexCoord2fv(&texcoords[2]);
		glVertex3fv(&points[3]);

		glTexCoord2fv(&texcoords[4]);
		glVertex3fv(&points[6]);

		glTexCoord2fv(&texcoords[6]);
		glVertex3fv(&points[9]);
	glEnd();
}



void GLHelper::DrawQuad(float *points, float *texcoords1, float *texcoords2)
{
	glBegin(GL_QUADS);
		glMultiTexCoord2fvARB(GL_TEXTURE0_ARB, &texcoords1[0]);
		glMultiTexCoord2fvARB(GL_TEXTURE1_ARB, &texcoords2[0]);
		glVertex3fv(&points[0]);

		glMultiTexCoord2fvARB(GL_TEXTURE0_ARB, &texcoords1[2]);
		glMultiTexCoord2fvARB(GL_TEXTURE1_ARB, &texcoords2[2]);
		glVertex3fv(&points[3]);

		glMultiTexCoord2fvARB(GL_TEXTURE0_ARB, &texcoords1[4]);
		glMultiTexCoord2fvARB(GL_TEXTURE1_ARB, &texcoords2[4]);
		glVertex3fv(&points[6]);

		glMultiTexCoord2fvARB(GL_TEXTURE0_ARB, &texcoords1[6]);
		glMultiTexCoord2fvARB(GL_TEXTURE1_ARB, &texcoords2[6]);
		glVertex3fv(&points[9]);
	glEnd();
}



void GLHelper::DrawQuad(float uwidth, float vheight, bool upsidedown)
{
	float aspect = uwidth/vheight;

	float v0=0.f;
	float v1=vheight;
	if (upsidedown) {
		v0 = vheight;
		v1 = 0.f;
	}

	float t[] = {
		0.f, v1,
		uwidth, v1,
		uwidth, v0,
		0.f, v0
	};

	float p[] = {
		-aspect,-1.0f,0.f,
		aspect,-1.0f,0.f,
		aspect,1.0f,0.f,
		-aspect,1.0f,0.f
	};

	DrawQuad(p, t);
}



void GLHelper::DrawQuad(float _width, float _height, float _z)
{
	float p[] = {
		-_width,-_height,_z,
		_width,-_height,_z,
		_width,_height,_z,
		-_width,_height,_z
	};

	DrawQuad(p);
}


void GLHelper::DrawQuad(bool upsidedown)
{
	DrawQuad(1.f,1.f,upsidedown);	 
}





void GLHelper::DrawBox()
{
	glPushMatrix();

	glTranslatef(0,0,1);
	DrawQuad();
	glTranslatef(0,0,-1);
	glRotatef(90,1,0,0);
	glTranslatef(0,0,1);
	DrawQuad();
	glTranslatef(0,0,-1);
	glRotatef(90,1,0,0);
	glTranslatef(0,0,1);
	DrawQuad();
	glTranslatef(0,0,-1);
	glRotatef(90,1,0,0);
	glTranslatef(0,0,1);
	DrawQuad();
	glTranslatef(0,0,-1);
	glRotatef(90,0,1,0);
	glTranslatef(0,0,1);
	DrawQuad();
	glTranslatef(0,0,-1);
	glRotatef(180,0,1,0);
	glTranslatef(0,0,1);
	DrawQuad();

	glPopMatrix();
}




void GLHelper::DrawSphere()
{
	GLUquadric *q = gluNewQuadric();
 	gluSphere(q, 0.5,24, 24);
	gluDeleteQuadric(q);
}




void GLHelper::DrawCylinder()
{
	GLUquadric *q = gluNewQuadric();
	glPushMatrix();
	glTranslatef(0,0,-0.5);
	gluCylinder(q, 1,1, 1, 12, 1);
	glPopMatrix();
	gluDeleteQuadric(q);
}



void GLHelper::DrawLine(float *l1, float *l2)
{
	glBegin ( GL_LINES );
		glVertex3fv (l1);      
		glVertex3fv (l2);      
	glEnd ();
}


void GLHelper::DrawLine(float _l1x, float _l1y, float _l2x, float _l2y)
{
	glBegin ( GL_LINES );
		glVertex3f( _l1x,_l1y, DEFAULT_Z );
		glVertex3f( _l2x,_l2y, DEFAULT_Z );
	glEnd ();
}


void GLHelper::DrawPoint( float _lx, float _ly, float _size )
{
	glPointSize( _size );
	glBegin ( GL_POINTS );
		glVertex3f( _lx,_ly, DEFAULT_Z );
	glEnd();
	glPointSize( 1.f );

}




void GLHelper::DrawDiagram( float *_values, size_t _numValues, size_t _strideInBytes, size_t _offsetInBytes, 
				 float _xoffset, float _yoffset, float _xscale, float _yscale, bool yNegate  )
{
	assert( _values != NULL );
	if ( _strideInBytes == 0 )
	{
		_strideInBytes = 4;
	}
	char *p = reinterpret_cast<char*>(_values)+_offsetInBytes;
	glBegin(GL_LINE_STRIP);
	for ( size_t i=0;i<_numValues;i++)
	{
		float *v = reinterpret_cast<float*>( p );
		float yVal = (yNegate) ? -*v : *v;
		glVertex3f(static_cast<float>(i)*_xscale+_xoffset, yVal*_yscale+_yoffset, DEFAULT_Z );
		p += _strideInBytes;
	}
	glEnd();
}


void GLHelper::DrawDiagramColored( float *_values, float *_colorRGB, size_t _numValues, size_t _strideInBytes, size_t _offsetInBytes, 
								   float _xoffset, float _yoffset, float _xscale, float _yscale )
{
	assert( _values != NULL );
	assert( _colorRGB != NULL );
	if ( _strideInBytes == 0 )
	{
		_strideInBytes = 4;
	}
	char *p = reinterpret_cast<char*>(_values)+_offsetInBytes;
	char *p2 = reinterpret_cast<char*>(_colorRGB)+_offsetInBytes;
	glBegin(GL_LINE_STRIP);
	for ( size_t i=0;i<_numValues;i++)
	{
		float *v = reinterpret_cast<float*>( p );
		float *c = reinterpret_cast<float*>( p2 );
		glColor3fv( c );  
		glVertex3f(static_cast<float>(i)*_xscale+_xoffset, -*v*_yscale+_yoffset, DEFAULT_Z );
		p += _strideInBytes;
		p2 += _strideInBytes*3;
	}
	glEnd();
	glColor3f( 1.f,1.f,1.f );  
}






void GLHelper::Fog(float dense, float *color)
{
	if (dense > 0.0f) {
		SetPropertyState(GL_FOG, true);
		glFogi (GL_FOG_MODE, GL_EXP2); 
		glFogfv (GL_FOG_COLOR, color);
		glFogf (GL_FOG_DENSITY, dense);

		// for linear mode only -> disabled
//		glFogf (GL_FOG_START, start);
//		glFogf (GL_FOG_END, end);
	}
	else {
		SetPropertyState(GL_FOG, false);
	}
}



void GLHelper::DisableGLLights() 
{
	for (int i=0;i<kMaxLights;i++)
		SetPropertyState(GL_LIGHT0+i, false);
}


void GLHelper::GetModelViewMatrix(float *mout)
{
	glGetFloatv(GL_MODELVIEW_MATRIX, mout);
}



void GLHelper::GetProjectionMatrix(float *mout)
{
	ASSERT(mout);
	glGetFloatv(GL_PROJECTION_MATRIX, mout);
}



void GLHelper::GetModelViewProjectionMatrix(float *mout) 
{
	ASSERT(mout);
	float proj[16];
	float modl[16];
	int i,j,k;
	glGetFloatv( GL_PROJECTION_MATRIX, proj );
	glGetFloatv( GL_MODELVIEW_MATRIX, modl );

	k=0;
	for(i=0;i<16;i+=4) {
		for(j=0;j<4;j++) {
			mout[k] = modl[i]*proj[j] + modl[i+1]*proj[j+4]+modl[i+2]*proj[j+8] + modl[i+3] * proj[j+12];
			k++;
		}
	}
}



void GLHelper::SetModelViewMatrix(float *m)
{
	ASSERT(m);
	glLoadMatrixf(m);
}



void GLHelper::SetProjectionMatrix(float *m)
{
	ASSERT(m);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(m);
	glMatrixMode(GL_MODELVIEW);
}



void GLHelper::SetOrtho(bool correct_aspect, float zNear, float zFar)
{
	int width, height;
	float aspectw = 1.0f;
	float aspecth = 1.0f;

	if (correct_aspect) {
		GetViewportSize(width, height);
		if (width > height) 
			aspectw = (float)width/(float)height;
		else 
			aspecth = (float)height/(float)width;
	}

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-aspectw, aspectw, -aspecth, aspecth, zNear, zFar);
	glMatrixMode(GL_MODELVIEW);
}

void GLHelper::SetOrtho(float left, float right, float bottom, float top, float zNear, float zFar )
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho( left, right, bottom, top, zNear, zFar );
	glMatrixMode(GL_MODELVIEW);
}



void GLHelper::ResetProjection() 
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}


void GLHelper::CalcViewportParams(float &left, float &right, float &bottom, float &top,float &znear, float &zfar)
{
	// perspective projection assumed
	float a,b,c,d,e,f;
	float pr[16];

	glGetFloatv(GL_PROJECTION_MATRIX, pr);

	// see glfrustum further details
	a = pr[8];	b = pr[0]; 	c = pr[10];	d = pr[14];	e = pr[9]; f = pr[5];
	right = (a+1.f)*d/(b*(c-1.f));
	left = d*(a-1.f)/(b*(c-1.f));
	top = (e+1.f)*d/(f*(c-1.f));
	bottom = d*(e-1.f)/(f*(c-1.f));
	znear = d/(c-1.f);
	zfar = d/(c+1.f);
}



void GLHelper::GetViewportSize(int &width, int &height) {
	int vp[4];
	glGetIntegerv(GL_VIEWPORT, vp);
	width = vp[2];
	height = vp[3];
}






