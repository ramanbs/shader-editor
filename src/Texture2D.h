#ifndef TEXTURE2D_H
#include <GL/glew.h>
#include<string>

class Texture2D
{
public : 
	Texture2D();

	virtual ~Texture2D();

	bool loadTexture(const std::string& filename, bool generateMipMaps = true);

	void bind(GLuint texUnit = 0);

private : 

	GLuint m_texture;
};

#endif //TEXTURE2D_H

