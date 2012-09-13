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
//! \file  glshaderprogram.h
//! \brief 

#ifndef H_SHADERPROGRAM
#define H_SHADERPROGRAM

#include "glhelper.h"

#include <string>

// holds a shader program. create, set variables, setshader, etc.
class GLShaderProgram 
{
public:
	GLShaderProgram();
	~GLShaderProgram();

	bool create( const std::string &_sstrVertexShader,  const std::string &_sstrPixelShader);
	void destroy();
	void bind();
	void unbind();
	bool setSampler(const std::string &_sstrName, int _unit);
	bool setSampler(const std::string &_sstrName, float _value);
	bool setVec(const std::string &_sstrName, float *_pValue);
	int getAttributeLocation( const std::string &_sstrName );

	GLhandleARB m_shaderHandle;
	unsigned int m_vId;
	unsigned int m_pId;

	std::string m_sstrVertexShader;
	std::string m_sstrPixelShader;
};



#endif // H_SHADERPROGRAM