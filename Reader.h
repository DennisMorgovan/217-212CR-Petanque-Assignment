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
#include <iostream>
#include <fstream>
#include <string>

#include "./Object.h"

class Reader
{
private:
	Object obj; //Object class

	FILE *fp; //Pointer to the file
	char* filename;
	int read; //Integer used to determine the amount of items that are read per line. It is used in conjunction with fscanf, which returns the number of elements read per line
	char lineHeader[10]; //characters that start at the beginning of every .obj file line
public:
	Reader(char* filename);
	~Reader();

	void LoadModel();
};

