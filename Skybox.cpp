#include "./Skybox.h"



Skybox::Skybox()
{
	vertexNumber = 24;

	//Vertices = new float[vertexNumber];
	//Vertices = 
	faceNumber = 8;
}


Skybox::~Skybox()
{
	//delete[] Vertices;
}

void Skybox::SetupDrawing(unsigned int vao, unsigned int vbo)
{
	//Binding the vertex array and the buffer objects
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//Setting the data
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexNumber, skyboxVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void Skybox::DrawObject(unsigned int vao)
{
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, faceNumber * 4);
}
