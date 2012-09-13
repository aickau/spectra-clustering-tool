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
//! \file  glFBO.cpp
//! \brief 

#include "sdsslib/glFBO.h"

#include "sdsslib/glextensions.h"

#include <assert.h>

GLFBO::GLFBO( unsigned int _widht, unsigned int _height )
:m_id(0)
,m_oldId(0)
,m_width(_widht)
,m_height(_height)
,m_bIsBound(false)
{
	glGenFramebuffersEXT(1, &m_id);
	for (size_t i=0;i<RB_COUNT;i++)
	{
		m_renderBufferIDs[i] = 0;
	}
}

GLFBO::~GLFBO()
{
	destroy();
}

void GLFBO::destroy()
{
	if ( m_bIsBound )
	{
		unbind();
	}

	glDeleteFramebuffersEXT(1, &m_id);

	for (size_t i=0;i<RB_COUNT;i++)
	{
		if ( m_renderBufferIDs[i] != 0 )
		{
			glDeleteRenderbuffersEXT(1, &m_renderBufferIDs[i] );
		}
	}

}


unsigned int GLFBO::getID() const
{
	return m_id;
}

bool GLFBO::isComplete() const
{
	GLenum fboStatus = glCheckFramebufferStatusEXT( GL_FRAMEBUFFER_EXT );

	return ( fboStatus == GL_FRAMEBUFFER_COMPLETE_EXT);
}


void GLFBO::bind()
{
	if ( isComplete() && !m_bIsBound )
	{
		GLint p;
		glGetIntegerv( GL_FRAMEBUFFER_BINDING_EXT, &p );
		m_oldId = p;
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_id);

		glPushAttrib(GL_VIEWPORT_BIT);
		glViewport(0,0,m_width, m_height);

		m_bIsBound = true;
	}
}


void GLFBO::unbind()
{
	if ( m_bIsBound )
	{
		glPopAttrib();
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_oldId);
	}
	else
	{
		assert(0);
	}
}

GLenum GLFBO::getComponentFromType( RenderBufferType _type )
{
	if ( _type == RB_DEPTH )
		return GL_DEPTH_COMPONENT;
	if ( _type == RB_STENCIL )
		return GL_STENCIL_INDEX8_EXT;
	if ( _type == RB_COLOR0 )
		return GL_RGBA;
	if ( _type == RB_COLOR0 )
		return GL_RGBA;

	return GL_RGBA;
}

GLenum GLFBO::getAttachmentFromType( RenderBufferType _type )
{
	if ( _type == RB_COLOR0 )
		return GL_COLOR_ATTACHMENT0_EXT;
	if ( _type == RB_COLOR1 )
		return GL_COLOR_ATTACHMENT1_EXT;
	if ( _type == RB_DEPTH )
		return GL_DEPTH_ATTACHMENT_EXT;
	if ( _type == RB_STENCIL )
		return GL_STENCIL_ATTACHMENT_EXT;

	return GL_COLOR_ATTACHMENT0_EXT;
}



void GLFBO::attachRenderBuffer( RenderBufferType _type )
{
	if ( m_renderBufferIDs[_type] == 0 )
	{
		GLint currentRenderbuffer;
		glGetIntegerv( GL_RENDERBUFFER_BINDING_EXT, &currentRenderbuffer );

		glGenRenderbuffersEXT(1, &m_renderBufferIDs[_type] );
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_renderBufferIDs[_type] );

		// get render buffer size
		GLint width, height;
		glGetRenderbufferParameterivEXT( GL_RENDERBUFFER_EXT, GL_RENDERBUFFER_WIDTH_EXT, &width );
		glGetRenderbufferParameterivEXT( GL_RENDERBUFFER_EXT, GL_RENDERBUFFER_HEIGHT_EXT, &height );

		if ( width != m_width || height != m_height )
		{
			glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, getComponentFromType(_type), m_width, m_height);
		}

		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, currentRenderbuffer );
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, getAttachmentFromType(_type), GL_RENDERBUFFER_EXT, m_renderBufferIDs[_type]);
	}
	else
	{
		assert(0);
	}

}
