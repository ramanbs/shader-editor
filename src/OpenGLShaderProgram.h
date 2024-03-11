#ifndef SHADER_PROGRAM_H
#include <string>
#include <map>
#include "GL/glew.h"
#include "glm/glm.hpp"

using std::string;

class OpenGLShaderProgram
{
public : 
	OpenGLShaderProgram();
	~OpenGLShaderProgram();

	enum class ShaderType { VERTEX, FRAGMENT, PROGRAM };

	GLuint getProgramHandle() { return m_handle; }

	bool loadShaders(const char* vsFilename, const char* fsFilename);
	void use();

	void setUniform(const GLchar* name, const glm::vec2& v);
	void setUniform(const GLchar* name, const glm::vec3& v);
	void setUniform(const GLchar* name, const glm::vec4& v);
	void setUniform(const GLchar* name, const glm::mat4& m);

private: 
	string fileToString(const string& filename);
	void checkCompilerErrors(GLuint shader, ShaderType type);
	GLint getUniformLocation(const GLchar* name); // expensive operation so should not be called often

	GLuint m_handle;
	std::map<string, GLint> m_uniformLocations;

};

#endif // SHADER_PROGRAM_H