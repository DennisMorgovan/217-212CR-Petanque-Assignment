#pragma once
#ifndef OPENGL_INCLUDES
#define OPENGL_INCLUDES
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#  include <glm.hpp> ///FOR MAT4 USAGE
#  include <gtc/type_ptr.hpp> ///FOR MAT4 VALUE PTR USAGE
#  include <glm/gtc/matrix_transform.hpp>
#pragma comment(lib, "glew32.lib") 
#endif 

#include "./Collider.h"

#include<vector>
#include<iostream>

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

struct UV
{
	float x;
	float y;
};

class Object
{
private:
	FILE *fp; //Pointer to the file
	int read; //Integer used to determine the amount of items that are read per line. It is used in conjunction with fscanf, which returns the number of elements read per line
	char lineHeader[10]; //characters that start at the beginning of every .obj file line
public:
	Vertex verticeCoords[MAXVERT], normals[MAXVERT]; //Arrays of vertices, texture vertices and normal vertices
	Face facesCoords[MAXVERT], normalsCoords[MAXVERT], textureCoords[MAXVERT]; //Coordinates for faces, normals and textures
	UV uv[MAXVERT];
	int faceNumber, vertexCoordsNumber, vertexNumber, uvNumber, finalVerticesVertexNumber, finalVerticesNormalsNumber; //Total number of faces and vertices

	float tempVerticesArray[MAXVERT], tempUVArray[MAXVERT], tempNormalsArray[MAXVERT];
	float* finalVertices;

	Collider* collider = NULL;  ///<Pointer to a collider for the game object. Initially NULL to indicate that we do not want to calculate collisions.
	glm::vec3 position;

	Object();
	Object(glm::vec3 position);
	~Object();

	void LoadObject(char* filename);

	void SetupDrawing(unsigned int vao, unsigned int vbo, int locationVert, int locationTex, int locationNormals);
	void DrawObject(unsigned int vao,  unsigned int objectLoc, unsigned int obj, unsigned int modelMatLoc, glm::vec3 position);
	void DrawObject(unsigned int vao, unsigned int objectLoc, unsigned int obj);
};

