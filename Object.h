#pragma once
#ifndef OPENGL_INCLUDES
#define OPENGL_INCLUDES
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#  include <glm/glm.hpp> 
#pragma comment(lib, "glew32.lib") 
#endif 

#include<vector>

#define MAXVERT 50000

struct Vertex
{
	float x;
	float y;
	float z;
};

struct Face
{
	int id1;
	int id2;
	int id3;
};

struct TextureFace
{
	float x;
	float y;
};

class Object
{
public:
	Vertex vertices[MAXVERT], normals[MAXVERT]; //Arrays of vertices, texture vertices and normal vertices
	Face facesCoords[MAXVERT], normalsCoords[MAXVERT], textureCoords[MAXVERT]; //Coordinates for faces, normals and textures
	TextureFace textureFaces[MAXVERT];
	int faceNumber, vertexNumber; //Total number of faces and vertices

	Object();
	~Object();

	void DrawObject();
};

