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
//! \file  glshaderprogram.cpp
//! \brief 

#include "sdsslib/glshaderprogram.h"
#include "sdsslib/helpers.h"
#include "sdsslib/glextensions.h"

#include <assert.h>

#define GLSHADERPROGRAM_LOGSIZE 16384

GLShaderProgram::GLShaderProgram() 
:m_sstrVertexShader("")
,m_sstrPixelShader("")
,m_shaderHandle(0)
,m_vId(0)
,m_pId(0)
{
}


void Destroy();
void SetShader();
void Reset();
bool SetVar(const std::string &_sstrName, int _value);
bool SetVar(const std::string &_sstrName, float _value);
bool SetVec(const std::string &_sstrName, float *_pValue);


GLShaderProgram::~GLShaderProgram() 
{
	destroy();
}


bool GLShaderProgram::create( const std::string &_sstrVertexShader,  const std::string &_sstrPixelShader ) 
{
	m_sstrVertexShader = _sstrVertexShader;
	m_sstrPixelShader = _sstrPixelShader;
	/* 
	to obtain logs:
	char log[10000];
	int logsize;
	int st;
	glGetObjectParameterivARB(vid, GL_OBJECT_COMPILE_STATUS_ARB, &st);
	glGetInfoLogARB(vid, 10000, &logsize, log);
	*/

	m_shaderHandle = glCreateProgramObjectARB();

	if (!m_sstrVertexShader.empty()) 
	{
		m_vId = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
		const char *pVs = m_sstrVertexShader.c_str();
		glShaderSourceARB( m_vId, 1, &pVs, 0);
		glCompileShaderARB( m_vId );
		int st;
		glGetObjectParameterivARB(m_vId, GL_OBJECT_COMPILE_STATUS_ARB, &st);
		if (st == 0) 
		{
			char log[GLSHADERPROGRAM_LOGSIZE];
			int logsize;
			glGetInfoLogARB(m_vId, GLSHADERPROGRAM_LOGSIZE, &logsize, log);
			Helpers::print("Vertex Shader Compile Error:\n%s\n"+std::string(log) );
			Helpers::print("Vertex Code:\n\n%s\n\n" + m_sstrVertexShader );
			return false;
		}
	}

	if (!m_sstrPixelShader.empty()) 
	{
		m_pId = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
		const char *pPs = m_sstrPixelShader.c_str();
		glShaderSourceARB(m_pId, 1, &pPs, 0);
		glCompileShaderARB(m_pId);
		int st;
		glGetObjectParameterivARB(m_pId, GL_OBJECT_COMPILE_STATUS_ARB, &st);
		if (st == 0) 
		{
			char log[GLSHADERPROGRAM_LOGSIZE];
			int logsize;
			glGetInfoLogARB(m_pId, GLSHADERPROGRAM_LOGSIZE, &logsize, log);
			Helpers::print("Fragment Shader Compile Error:\n%s\n" + std::string(log) );
			Helpers::print("Fragment Code:\n\n%s\n\n" + m_sstrPixelShader);
			return false;
		}
	}

	// attach
	glAttachObjectARB(m_shaderHandle, m_vId);
	glAttachObjectARB(m_shaderHandle, m_pId);
	glLinkProgramARB(m_shaderHandle);

	return true;
}



void GLShaderProgram::destroy() 
{
	if (m_shaderHandle) 
		glDeleteObjectARB(m_shaderHandle);
	m_shaderHandle=0;
	m_vId = m_pId = 0;
}

void GLShaderProgram::bind() 
{
	assert(m_shaderHandle != 0);
	glUseProgramObjectARB(m_shaderHandle);
}

void GLShaderProgram::unbind()
{
	glUseProgramObjectARB(0);
}

bool GLShaderProgram::setSampler(const std::string &_sstrName, int _unit)
{
	assert(m_shaderHandle != 0);
	int loc = glGetUniformLocationARB(m_shaderHandle, _sstrName.c_str() );
	if (loc >= 0) {
		glUniform1iARB(loc, _unit);
		return true;
	}
return false;
}


bool GLShaderProgram::setSampler(const std::string &_sstrName, float _value)
{
	assert(m_shaderHandle != 0);
	int loc = glGetUniformLocationARB(m_shaderHandle, _sstrName.c_str() );
	if (loc >= 0) {
		glUniform1fARB(loc, _value);
		return true;
	}
	return false;
}

bool GLShaderProgram::setVec( const std::string &_sstrName, float *_pValue )
{
	assert(m_shaderHandle != 0);
	int loc = glGetUniformLocationARB(m_shaderHandle, _sstrName.c_str() );
	if (loc >= 0) {
		glUniform4fvARB(loc, 1, _pValue);
		return true;
	}
	return false;
}

int GLShaderProgram::getAttributeLocation( const std::string &_sstrName ) 
{
	assert(m_shaderHandle != 0);
	return glGetAttribLocationARB(m_shaderHandle, _sstrName.c_str() );
}





