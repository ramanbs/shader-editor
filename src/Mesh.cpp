#include "Mesh.h"
#include <iostream>
#include <sstream>
#include <fstream>


std::vector<std::string> split(std::string s, std::string t)
{
	std::vector<std::string> res;
	while (1)
	{
		int pos = s.find(t);

		if (pos == -1)
		{
			res.push_back(s);
			break;
		}
		res.push_back(s.substr(0, pos));
		s = s.substr(pos + 1, s.size() - pos - 1);
	}

	return res;
}

Mesh::Mesh()
	:m_loaded(false)
{
}


Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
}

bool Mesh::loadOBJ(const std::string& filename)
{
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> tempVertices;
	std::vector<glm::vec2> tempUVs;
	std::vector<glm::vec3> tempNormals;

	if (filename.find(".obj") != std::string::npos)
	{
		std::ifstream fin(filename, std::ios::in);

		if (!fin)
		{
			std::cerr << "Cannot open " << filename << std::endl;
			return false;
		}

		std::cout << "Loading OBJ file " << filename << "..." << std::endl;

		std::string lineBuffer;

		while (std::getline(fin, lineBuffer))
		{
			std::stringstream ss(lineBuffer);
			std::string cmd;

			ss >> cmd;

			if (cmd == "v")
			{

				glm::vec3 vertex;
				//std::get() does not ignore whitespace whereas extraction operator does
				int dim = 0;
				while (dim < 3 && ss >> vertex[dim])
					dim++;

				tempVertices.push_back(vertex);
			}
			else if (cmd == "vt")
			{
				glm::vec2 uv;

				int dim = 0;
				while (dim < 2 && ss >> uv[dim])
					dim++;

				tempUVs.push_back(uv);
			}
			else if (cmd == "vn")
			{
				glm::vec3 normal;

				int dim = 0;
				while (dim < 3 && ss >> normal[dim])
					dim++;

				normal = glm::normalize(normal);

				tempNormals.push_back(normal);
			}
			else if (cmd == "f")
			{
				std::string faceData;
				int vertexIndex, uvIndex, normalIndex; // v/vt/vn 

				while (ss >> faceData)
				{
					std::vector<std::string> data = split(faceData, "/");

					// vertex index
					if (data[0].size() > 0)
					{
						sscanf_s(data[0].c_str(), "%d", &vertexIndex);
						vertexIndices.push_back(vertexIndex);
					}

					// if the face vertex has a texture coordinate index 
					if (data.size() >= 1)
					{
						if (data[1].size() > 0)
						{
							sscanf_s(data[1].c_str(), "%d", &uvIndex);
							uvIndices.push_back(uvIndex);
						}
					}

					// if the face vertex has a normal index 
					if (data.size() >= 2)
					{
						if (data[2].size() > 0)
						{
							sscanf_s(data[2].c_str(), "%d", &normalIndex);
							normalIndices.push_back(normalIndex);
						}
					}
				}

			}
		}
		//Close the file
		fin.close();

		// For each vertex of the triangle
		for (unsigned int i = 0; i < vertexIndices.size(); i++)
		{
			Vertex meshVertex;

			if (tempVertices.size() > 0)
			{
				glm::vec3 vertex = tempVertices[vertexIndices[i] - 1];
				meshVertex.position = vertex;
			}

			if (tempNormals.size() > 0)
			{
				glm::vec3 normal = tempNormals[normalIndices[i] - 1];
				meshVertex.normal = normal;
			}

			if (tempUVs.size() > 0)
			{
				glm::vec2 uv = tempUVs[uvIndices[i] - 1];
				meshVertex.texCoords = uv;
			}

			m_vertices.push_back(meshVertex);
		}

		//Create and initialize buffers
		initBuffers();

		return (m_loaded = true);
	}

	return false;
}

void Mesh::draw()
{
	if (!m_loaded) return;

	glBindVertexArray(m_vao);

	glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
	glBindVertexArray(0);
}

void Mesh::initBuffers()
{
	glGenBuffers(1, &m_vbo); 
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	//GL_STATIC_DRAW - create once, setup once use it a lot, GL_DYNAMIC_DRAW - create once, change a lot and use it a lot, GL_STREAM_DRAW - create once, setup once and use once
	//mVertices.size() - gives the count of elements in that data structure
	//TODO &mVertices[0] - why first element
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao); // make it the current vao

	//position attrib pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL); 
	glEnableVertexAttribArray(0);

	// Normals Attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)((sizeof(GLfloat) * 3)));
	glEnableVertexAttribArray(1);

	//texture attrib
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)((sizeof(GLfloat) * 6)));
	glEnableVertexAttribArray(2);

	// Unbind vertex array
	glBindVertexArray(0);

}
