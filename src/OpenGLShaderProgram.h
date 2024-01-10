#ifndef SHADER_PROGRAM_H
#include <string>
#include "GL/glew.h"
using std::string;

class OpenGLShaderProgram
{
public : 
	OpenGLShaderProgram();
	~OpenGLShaderProgram();

	enum ShaderType { VERTEX, FRAGMENT, PROGRAM };

	bool loadShaders(const char* vsFilename, const char* fsFilename);
	void use();

private: 
	string fileToString(const string& filename);
	void checkCompilerErrors(GLuint shader, ShaderType type);

	GLuint m_handle;
};

#endif // SHADER_PROGRAM_H