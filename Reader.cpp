#include "Reader.h"


Reader::Reader(char* filename)
{
	this->filename = filename;
}


Reader::~Reader()
{
}

void Reader::LoadModel()
{
	fp = fopen(this->filename, "r"); //Opens the file
	int indexVertex = 0, indexTextureVertices = 0, indexNormals = 0, indexFaces = 0;

	while (!(feof(fp))) //While we are still in the file
	{
		read = fscanf(fp, "%s", lineHeader); //Scan the first line and save the header characters into lineHeader

		if (strcmp(lineHeader, "v") == 0) //If it is a vertex line
		{
			fscanf(fp, "%f %f %f", &obj.vertices[indexVertex].x, &obj.vertices[indexVertex].y, &obj.vertices[indexVertex].z);
			//std::cout << obj.vertices[indexVertex].x << " " << obj.vertices[indexVertex].y << " " << obj.vertices[indexVertex].z << std::endl; //Debugging
			indexVertex++;
		}
		else if (strcmp(lineHeader, "vt") == 0)
		{
			fscanf(fp, "%f %f", &obj.textureFaces[indexTextureVertices].x, &obj.textureFaces[indexTextureVertices].y);
			//std::cout << obj.textureFaces[indexTextureVertices].x << " " << obj.textureFaces[indexTextureVertices].y << std::endl; //Debugging
			indexTextureVertices++;
		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			fscanf(fp, "%f %f %f", &obj.normals[indexNormals].x, &obj.normals[indexNormals].y, &obj.normals[indexNormals].z);
			//std::cout << obj.normals[indexNormals].x << " " << obj.normals[indexNormals].y << " " << obj.normals[indexNormals].z << std::endl; //Debugging
			indexNormals++;
		}
		else if (strcmp(lineHeader, "f") == 0)
		{
			fscanf(fp, "%i/%i/%i %i/%i/%i %i/%i/%i", 
				&obj.facesCoords[indexFaces].id1, &obj.textureCoords[indexFaces].id1, &obj.normalsCoords[indexFaces].id1,
				&obj.facesCoords[indexFaces].id2, &obj.textureCoords[indexFaces].id2, &obj.normalsCoords[indexFaces].id2,
				&obj.facesCoords[indexFaces].id3, &obj.textureCoords[indexFaces].id3, &obj.normalsCoords[indexFaces].id3);
			indexFaces++;
		}
	}

	obj.faceNumber = indexFaces;
	obj.vertexNumber = indexVertex;

	std::cout << indexFaces;
}
