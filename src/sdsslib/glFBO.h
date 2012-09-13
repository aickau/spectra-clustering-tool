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
//! \file  glFBO.h
//! \brief Plainstupid FBO implementation.

#ifndef H_GLFBO
#define H_GLFBO

#include <Windows.h>
#include <gl/gl.h>


// simple FBO class.
// usage:
//
// GLFBO fbo(640,480);
// fbo.attachRenderBuffer(GLFBO::RB_DEPTH);
// fbo.attachRenderBuffer(GLFBO::RB_COLOR0);
// if (fbo.isComplete())
// {
//     fbo.bind();
//	   doRender();
//    fbo.unbind();
// }
class GLFBO
{
public:
	GLFBO( unsigned int _widht, unsigned int _height );
	~GLFBO();

	enum RenderBufferType
	{
		RB_COLOR0=0,
		RB_COLOR1=1,
		RB_DEPTH=2,
		RB_STENCIL=3,
		RB_COUNT=4
	};

	unsigned int getID() const;

	bool isComplete() const;

	void bind();
	void unbind();

	void attachRenderBuffer( RenderBufferType _type );

private:

	void destroy();
	
	GLenum getComponentFromType( RenderBufferType _type );
	GLenum getAttachmentFromType( RenderBufferType _type );

	unsigned int m_id;
	unsigned int m_oldId;
	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_renderBufferIDs[RB_COUNT];
	bool m_bIsBound;

};

#endif 

