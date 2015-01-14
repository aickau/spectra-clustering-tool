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
//! \file  glhelper.h
//! \brief Collection of OpenGL helper fuctions.

#ifndef H_GLHELPER
#define H_GLHELPER

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "glextensions.h"


class Matrix;


// solid GL helper class
// todo map enums to GLenums
class GLHelper
{
public:

	enum EBlendMode {
		kBlendMode_Off,
		kBlendMode_Add,
		kBlendMode_SoftAdd,
		kBlendMode_Mul,
		kBlendMode_Sub,
		kBlendMode_SoftSub,
		kBlendMode_AlphaAdd,
		kBlendMode_AlphaMul,
		kBlendMode_AlphaBlend,
		kBlendMode_AlphaNegAdd,
		kBlendMode_AlphaNegMul,
		kBlendMode_Min,
		kBlendMode_Max
	};

	enum ETexGenMode {
		kTexGenMode_DisableAuto,
		kTexGenMode_Haujobb,
		kTexGenMode_CubeMapping
	};

	enum ETexWrapMode {
		kTexWrapMode_Clamp			= 0,
		kTexWrapMode_ClampToEdge	= 1,
		kTexWrapMode_ClampToBorder	= 2,
		kTexWrapMode_Repeat			= 3,
		kTexWrapMode_Mirror			= 4
	};

	// frame buffer masks, can be combined
	enum EFBMask {
		kFBMask_Red		= 1,
		kFBMask_Green	= 2,
		kFBMask_Blue	= 4,
		kFBMask_Alpha	= 8,
		kFBMask_Color	= kFBMask_Red+kFBMask_Green+kFBMask_Blue+kFBMask_Alpha,
		kFBMask_Depth	= 16,
		kFBMask_Stencil	= 32
	};

	// array masks, can be combined
	enum EArrayMask {
		kArrayMask_Vertex	= 1,
		kArrayMask_Normal	= 2,
		kArrayMask_Binormal	= 4,
		kArrayMask_Tangent	= 8,
		kArrayMask_TC0		= 16,
		kArrayMask_TC1		= 32,
		kArrayMask_TC2		= 64,
		kArrayMask_TC3		= 128,
		kArrayMask_TC4		= 256,
		kArrayMask_TC5		= 512,
		kArrayMask_TC6		= 1024,
		kArrayMask_TC7		= 2048
	};


	enum EAlphaTest {
		kAlphaTest_Off		= 0,
		kAlphaTest_Less		= GL_LESS,
		KAlphaTest_Greater	= GL_GREATER
	};

	enum ECullMode {
		kCullMode_Off		= 0,
		kCullMode_Back		= GL_BACK,
		KCullMode_Front		= GL_FRONT,
		kCullMode_Both		= GL_FRONT_AND_BACK
	};

	enum ELightMode {
		kLightMode_Off,
		kLightMode_Flat,
		kLightMode_Smooth
	};

	static const int kMaxTexturePasses = 8;
	static const int kMaxLights = 8;

	static void			Init();
	
	// text

	// TODO: replace HDC with void* to remove windows specific interface.
	// create GL font with a given font name
	// \param windowId window handle (under windows HDC) 
	// \param fontName name of installed font (true type similar)
	// \param fontSize in pt
	// \return font id or -1 if font could not be created. 
	static int			BuildFont(HDC hdc, char *fontname, int fontsize, bool bold, bool italic);

	//! delete GL font that was created with buildFont()
	static void			KillFont(int fontid);


	//! print some text to GL.
	//! \param fontId must be greater zero, created with buildFont()
	//! \param position 2D position in world space. Use NULL to specify no position.
	//! \param fmt text with format specifiers to print, see ansi c printf() for details
	static void			Print(int fontid, float *position, const char *fmt, ...);

	// GL error handling
	static bool			PrintError(const char *_functionName=NULL); // on success (no errors) returns true
	static void			PrintStates();

	// texture 
	static void			RegTexture(	unsigned int id, 
									unsigned int sizex, 
									unsigned int sizey, 
									GLenum format, 
									GLenum type, 
									bool mipmap, 
									GLenum texmagfilter, 
									void *pt, 
									float anisotropy = 0.f, 
									GLenum target1 = GL_TEXTURE_2D, 
									GLenum target2 = GL_TEXTURE_2D );

	static unsigned int	RegTexture(	unsigned int sizex, 
									unsigned int sizey, 
									GLenum format, 
									GLenum type, 
									bool mipmap, 
									GLenum texmagfilter, 
									void *pt, 
									float anisotropy = 0.f);

	// texblendmode = GL_REPLACE,GL_MODULATE,GL_ADD,GL_BLEND,GL_DECAL
	static void			SetTexturePass(unsigned int id, int pass, GLenum textureblendmode, ETexWrapMode warpx, ETexWrapMode warpy, bool env, GLenum target, float *bordercolor=NULL);
	static void			ResetTexturePasses();

	static void			SetEnvMapState(ETexGenMode state);
	static void			SetEyeSpace(bool enable);

	static void			DisableGLLights();

	static void			Fog(float dense, float *color);

	static void			SetBlendMode( EBlendMode mode );	
	static void			SetAlphaTest( EAlphaTest alphatest, float treshold );
	static void			SetCullMode( ECullMode cullmode );

	// if one of the parameters is 0, stippling is disabled.
	// factor [1..256]
	// pattern 16 bit integer describing the pattern e.g. 0x0aaaa for "....", 0xcccc for "- - ", 0xc8c8 for "- . "
	static void			SetLineStipple( unsigned int factor=1, unsigned int pattern=0x0aaaa );
	static void			SetPropertyState(GLenum property, bool state);

	static GLenum		GetInternalFormat(GLenum _format, GLenum _type );
	static int			GetNumComponents(GLenum format);

	// indexbinormal:index to binormal vertex shader attribute, indextangent:index to tangent vertex shader attribute 
	// --> must be set if mask contains kArrayMask_Binormal / kArrayMask_Tangent
	static void			EnableArray(EArrayMask arraymask, int indexbinormal=0, int indextangent=0);

	// indexbinormal:index to binormal vertex shader attribute, indextangent:index to tangent vertex shader attribute 
	// --> must be set if mask contains kArrayMask_Binormal / kArrayMask_Tangent
	static void			DisableArray(EArrayMask arraymask, int indexbinormal=0, int indextangent=0);

	// enables/disables buffer writes: S
	static void			MaskBuffers( unsigned mask );

	// drawing
	static void			DrawQuad(bool upsidedown=false);
	static void			DrawQuad(float uwidth, float vheight, bool upsidedown=false);
	static void			DrawQuad(float _width, float _height, float _z);
	static void			DrawQuad(float *points);
	static void			DrawQuadn(float *points, float *normals);
	static void			DrawQuad(float *points, float *texcoords);
	static void			DrawQuad(float *points, float *texcoords1, float *texcoords2);

	static void			DrawBox();
	static void			DrawSphere();
	static void			DrawCylinder();

	static void			DrawLine(float *l1, float *l2);
	static void			DrawLine(float _l1x, float _l1y, float _l2x, float _l2y);
	static void			DrawPoint(float _lx, float _ly, float _size = 1.f );

	// yNegate : negate y to match with bottom-left vs top-left viewports. For top-left, use negate.
	static void			DrawDiagram( float *_values, size_t _numValues, size_t _strideInBytes=4, size_t _offsetInBytes=0, 
                                     float _xoffset=0.0f, float _yoffset=0.0f, float _xscale=1.f, float _yscale=1.f, bool yNegate=false );


	static void			DrawDiagramColored( float *_values, float *_colorRGB, size_t _numValues, size_t _strideInBytes=4, size_t _offsetInBytes=0, 
										   float _xoffset=0.0f, float _yoffset=0.0f, float _xscale=1.f, float _yscale=1.f );


	// projection 
	static void			GetModelViewMatrix(float *mout);
	static void			GetProjectionMatrix(float *mout);
	static void			GetModelViewProjectionMatrix(float *mout);
	static void			SetModelViewMatrix(float *m);
	static void			SetProjectionMatrix(float *m);

	// set orthographic projection in range -1..1 
	// correct_aspect if true aspect is corrected, e.g. 
	// for full HD (1920x1024) we would get an orthographic window for drawing in range
	// -1.875..1875 for x and -1..1 in y
	static void			SetOrtho(bool correct_aspect, float zNear=0.1f, float zFar=10.0f);
	static void			SetOrtho(float left, float right, float top, float bottom, float zNear=0.1f, float zFar=10.0f );
	static void			ResetProjection();
	static void			CalcViewportParams(float &left, float &right, float &bottom, float &top,float &znear, float &zfar);
	static void			GetViewportSize(int &width, int &height);

};


#define CHECK_GL(funcname) assert(GLHelper::PrintError(funcname));

#endif


