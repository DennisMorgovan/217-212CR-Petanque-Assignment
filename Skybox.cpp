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
	this->LoadObject((char*)"./Models/Skybox/skybox_front.obj");

	//Binding the vertex array and the buffer objects
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//Setting the data
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexNumber, finalVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(sizeof(float) * finalVerticesVertexNumber));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(sizeof(float) * finalVerticesNormalsNumber));
	glEnableVertexAttribArray(2);
}

void Skybox::DrawObject(unsigned int vao, unsigned int objectLoc)
{
	//Front
	glBindVertexArray(vao);
	glUniform1ui(objectLoc, 5);
	glDrawArrays(GL_TRIANGLES, 0, faceNumber * 4);
}

void Skybox::LoadTextures(unsigned int programId)
{
	this->programId = programId;

	// Load the images.
	image[0] = getbmp("./Textures/Skybox/hourglass_ft.bmp");
	image[1] = getbmp("./Textures/Skybox/hourglass_bk.bmp");
	image[2] = getbmp("./Textures/Skybox/hourglass_lf.bmp");
	image[3] = getbmp("./Textures/Skybox/hourglass_rt.bmp");
	image[4] = getbmp("./Textures/Skybox/hourglass_up.bmp");
	image[5] = getbmp("./Textures/Skybox/hourglass_dn.bmp");

	glGenTextures(5, texture);

	// Bind skybox images
	//Front
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	skyTexLocFt = glGetUniformLocation(programId, "skyTexFt");
	glUniform1i(skyTexLocFt, 4);

	//Back
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[1]->sizeX, image[1]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[1]->data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	skyTexLocBk = glGetUniformLocation(programId, "skyTexBk");
	glUniform1i(skyTexLocBk, 5);
}
