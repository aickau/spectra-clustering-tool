
//! \verbatim
//! ###########################################################################
//! # ASPECT: A spectra clustering tool for exploration of large spectral surveys - Stage I
//! #
//! # Copyright (c) 2009 Aick in der Au
//! # All rights reserved.
//! ###########################################################################
//!
//!      created by : Aick in der Au <aick.inderau@gmail.com>
//!      created on : 7/2/2009
//! additional docs : none
//!  responsibility : 1. Aick in der Au
//!                   2. 
//! \endverbatim
//!
//! \file  gltexture.cpp
//! \brief 

#include "sdsslib/gltexture.h"

#include "sdsslib/glhelper.h"
#include "sdsslib/defines.h"

#include <assert.h>


GLint glbmodelist[] = { GL_REPLACE,GL_MODULATE,GL_ADD,GL_BLEND,GL_DECAL };
GLint glwarplist[] = { GL_CLAMP, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER_ARB,GL_REPEAT,GL_MIRRORED_REPEAT_ARB };
GLint glcubemapnames[] = {	GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB, 
GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB, 
GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB, 
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB, 
GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB, 
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB};




GLTexture::GLTexture()
:m_id(0)
,m_created(false)
,m_wrapS(3)
,m_wrapT(3)
,m_interpolate(false)
,m_format(0)
{
}



GLTexture::~GLTexture()
{
	destroy();
}

void GLTexture::destroy() 
{
	glDeleteTextures(1, &m_id);
	m_id = 0;
}

unsigned int GLTexture::getID() const
{
	return m_id;
}



void GLTexture::setTexture( int _sizeX, int _sizeY, int _format, int _type, bool _interpolate, void *texdata )
{
	m_sizeX = _sizeX;
	m_sizeY = _sizeY;
	m_target	= GL_TEXTURE_2D;
	m_type = _type;
	m_created = true;
	m_interpolate = _interpolate;
	m_format = _format;

	// id must be zero otherwise texture is already set
	assert(m_id == 0);

	m_id = GLHelper::RegTexture(m_sizeX, m_sizeY, m_format, m_type, false, (m_interpolate) ? GL_LINEAR : GL_NEAREST, texdata, 0.f );
	GLHelper::PrintError();
}

/*
void GLTexture::RenderToTextureBegin(int width, int height, bool cubemap, bool mipmap, bool interpolate, int anisotropylevel ) {
	_pbuffer.Init(width, height);
	_pbuffer.BeginTextureUpdate();
	
	// clamp size to maximum
	GLint maxsize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxsize);
	if (size>maxsize)
		size = maxsize;
	

	// we do not support mipmaps at the moment
	mipmap = false;

  	m_wrapS = 1;
	m_wrapT = 1;


	// if size or param has changed -> delete & setup new
	if ((m_id>0) && ((size!=_size) || (interpolate != m_interpolate) || (mipmap != _mipmap) || (anisotropylevel != _anisotropylevel) )) {
		Delete();
	}

	if (m_id <= 0) {
		SetTexture(size, GL_RGBA,GL_UNSIGNED_BYTE, mipmap, interpolate, texture_dump, anisotropylevel);	
	}
	m_sizeX = width;
	m_sizeY = height;
	m_interpolate = interpolate;
}

void GLTexture::RenderToTextureEnd() {
	RenderToTextureEnd(m_target);
}


void GLTexture::RenderToTextureEnd(int target2) {
	glBindTexture(m_target, m_id);
	glCopyTexSubImage2D(target2, 0, 0, 0, 0, 0 ,m_sizeX, m_sizeY);
	_pbuffer.EndTextureUpdate();
}
*/

void GLTexture::bind(int _unit)
{
	if (!m_created) 
		return;
	int wrapmax = 4;

	GLHelper::PrintError();

	CLAMP(m_wrapS,0,wrapmax);
	CLAMP(m_wrapT,0,wrapmax);

	glActiveTextureARB(GL_TEXTURE0_ARB+_unit); 
 	glBindTexture(m_target, m_id);
	glEnable(m_target);
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, glbmodelist[_textureblendmode]);

	int state = 0;
 	if (m_mappingchannel == 0)
		state = 1;
	if (m_target == GL_TEXTURE_CUBE_MAP_ARB)
		state = 2;

	GLHelper::SetEnvMapState(GLHelper::kTexGenMode_DisableAuto);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_S, glwarplist[m_wrapS]);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_T, glwarplist[m_wrapT]);
	
	GLHelper::PrintError();
}



void GLTexture::unbind(int _unit) 
{
	if (!m_created) 
		return;

	glActiveTextureARB(GL_TEXTURE0_ARB+_unit); 

	glDisable(m_target);
	GLHelper::SetEnvMapState(GLHelper::kTexGenMode_DisableAuto);

	GLHelper::PrintError();
}
