#pragma once

#include <glew.h>

class Mesh
{
public:
	Mesh();

	void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
	void RenderMesh();
	void ClearMesh();

	~Mesh();

private:
	GLuint VAO, VBO, IBO, texBO; // Agrega texBO para manejar el buffer de coordenadas de textura
	GLsizei indexCount;
};

