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
//! \file  gltexture.h
//! \brief 

#ifndef H_TEXTURE
#define H_TEXTURE


class GLTexture
{
public:
	GLTexture();
	~GLTexture();


	unsigned int getID() const;


	// texture construction
	/////////////////////////////////
	
	// format:	GL_RGB, GL_RGBA, GL_ALPHA, GL_LUMINANCE, GL_LUMINANCE_ALPHA
	void setTexture( int _sizeX, int _sizeY, int format, int type, bool _interpolate, void *texdata );

	//void RenderToTextureBegin(int width, int height, bool cubemap=false, bool mipmap=false, bool interpolate=true, int anisotropylevel = 0);
	//void RenderToTextureEnd();
	//void RenderToTextureEnd(int target2);

	// manipulation / activation
	//////////////////////////////////

	void bind(int _unit);
	void unbind(int _unit);

	// drawing
	//////////////////////////////////

	// 0=envmap otherwise uvw mapping channel
	int			m_mappingchannel;	
	// enable texture
	bool		m_created;			

	// 0 = clamp, 1 = clamp to edge, 2 = clamp border, 3 = repeat, 4 = mirror repeat
	int			m_wrapS;			
	// 0 = clamp, 1 = clamp to edge, 2 = clamp border, 3 = repeat, 4 = mirror repeat
	int			m_wrapT;			
	
	int			m_sizeX;
	int			m_sizeY;

	// GL_TEXTURE2D, GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_RECTANGLE_EXT
	int			m_target;		
	
	// GL_UNSIGNED_BYTE, GL_FLOAT
	int			m_type;

	int			m_format;

	bool		m_interpolate;

private:

	void destroy();

	unsigned int m_id;
};



#endif