#include "OpenGLShaderProgram.h"
#include <fstream>
#include <iostream>
#include <sstream>

OpenGLShaderProgram::OpenGLShaderProgram() 
	: m_handle(0)
{

}

OpenGLShaderProgram::~OpenGLShaderProgram() 
{
	glDeleteProgram(m_handle);
}

bool OpenGLShaderProgram::loadShaders(const char* vsFilename, const char* fsFilename) 
{
	string vsSrc = fileToString(vsFilename);
	string fsSrc = fileToString(fsFilename);

	const GLchar* vsSrcPtr = vsSrc.c_str();
	const GLchar* fsSrcPtr = fsSrc.c_str();

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	
	glShaderSource(vs, 1, &vsSrcPtr, NULL);
	glShaderSource(fs, 1, &fsSrcPtr, NULL);
	
	glCompileShader(vs);
	checkCompilerErrors(vs, ShaderType::VERTEX);
	
	
	glCompileShader(fs);
	checkCompilerErrors(fs, ShaderType::FRAGMENT);
	
	m_handle = glCreateProgram();

	glAttachShader(m_handle, vs);
	glAttachShader(m_handle, fs);
	
	glLinkProgram(m_handle);
	checkCompilerErrors(m_handle, ShaderType::PROGRAM);

	glDeleteShader(vs);
	glDeleteShader(fs);

	m_uniformLocations.clear();

	return true;

}

void OpenGLShaderProgram::use() 
{
	if (m_handle > 0)
		glUseProgram(m_handle);
}

string OpenGLShaderProgram::fileToString(const string& filename) 
{
	std::stringstream ss;
	std::ifstream file;
	
	file.open(filename, std::ios::in);

	if (!file.fail())
	{
		ss << file.rdbuf();
	}

	file.close();
	return ss.str();
}

void OpenGLShaderProgram::checkCompilerErrors(GLuint shader, ShaderType type) 
{
	int status = 0;

	if (type == ShaderType::PROGRAM)
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &status);

		if (status == GL_FALSE)
		{
			GLint length = 0;
			glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &length);

			string errorLog(length, ' ');
			glGetProgramInfoLog(shader, length, &length, &errorLog[0]);

			std::cerr << "OpenGL Shader Program failed to link." << errorLog << std::endl;
		}
	}
	else 
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

		if (status == GL_FALSE)
		{
			GLint length = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

			string errorLog(length, ' ');
			glGetShaderInfoLog(shader, length, &length, &errorLog[0]);

			std::cerr << "OpenGL Shader failed to compile." << errorLog << std::endl;
		}
	}
}

GLint OpenGLShaderProgram::getUniformLocation(const GLchar* name) 
{
	std::map<string, GLint>::iterator it = m_uniformLocations.find(name);

	if (it == m_uniformLocations.end())
	{
		m_uniformLocations[name] = glGetUniformLocation(m_handle, name);
	}

	return m_uniformLocations[name];
}

void OpenGLShaderProgram::setUniform(const GLchar* name, const glm::vec2& v) 
{
	GLint loc = getUniformLocation(name);
	glUniform2f(loc, v.x, v.y);
}

void OpenGLShaderProgram::setUniform(const GLchar* name, const glm::vec3& v) 
{
	GLint loc = getUniformLocation(name);
	glUniform3f(loc, v.x, v.y, v.z);

}

void OpenGLShaderProgram::setUniform(const GLchar* name, const glm::vec4& v) 
{
    GLint loc = getUniformLocation(name);
    glUniform4f(loc, v.x, v.y, v.z, v.w);
}