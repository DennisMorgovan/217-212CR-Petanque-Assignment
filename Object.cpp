#include "Object.h"



Object::Object(char* filename)
{
	this->filename = filename;
}


Object::~Object()
{ 
	delete[] finalVertices;  
}

void Object::LoadObject()
{
	fp = fopen(this->filename, "r"); //Opens the file
	int indexVertex = 0, uvIndex = 0, indexNormals = 0, indexFaces = 0, indexVertices = 0;

	while (!(feof(fp))) //While we are still in the file
	{
		read = fscanf(fp, "%s", lineHeader); //Scan the first line and save the header characters into lineHeader

		if (strcmp(lineHeader, "v") == 0) //If it is a vertex line
		{
			fscanf(fp, "%f %f %f", &verticeCoords[indexVertex].x, &verticeCoords[indexVertex].y, &verticeCoords[indexVertex].z);
			//std::cout << obj.vertices[indexVertex].x << " " << obj.vertices[indexVertex].y << " " << obj.vertices[indexVertex].z << std::endl; //Debugging
			indexVertex++;
		}
		else if (strcmp(lineHeader, "vt") == 0) //If it is a texture vertex
		{
			fscanf(fp, "%f %f", &uv[uvIndex].x, &uv[uvIndex].y);
			//std::cout << obj.textureFaces[indexTextureVertices].x << " " << obj.textureFaces[indexTextureVertices].y << std::endl; //Debugging
			uvIndex++;
		}
		else if (strcmp(lineHeader, "vn") == 0) //If it is a normal vertex
		{
			fscanf(fp, "%f %f %f", &normals[indexNormals].x, &normals[indexNormals].y, &normals[indexNormals].z);
			//std::cout << normals[indexNormals].x << " " << normals[indexNormals].y << " " << normals[indexNormals].z << std::endl; //Debugging
			indexNormals++;
		}
		else if (strcmp(lineHeader, "f") == 0) //If it is a face
		{
			fscanf(fp, "%i/%i/%i %i/%i/%i %i/%i/%i",
				&facesCoords[indexFaces].id1, &textureCoords[indexFaces].id1, &normalsCoords[indexFaces].id1,
				&facesCoords[indexFaces].id2, &textureCoords[indexFaces].id2, &normalsCoords[indexFaces].id2,
				&facesCoords[indexFaces].id3, &textureCoords[indexFaces].id3, &normalsCoords[indexFaces].id3);

			//std::cout << facesCoords[indexFaces].id1 << " " << facesCoords[indexFaces].id2 << " " << facesCoords[indexFaces].id3 << std::endl; //Debugging

			facesCoords[indexFaces].id1--;
			facesCoords[indexFaces].id2--;
			facesCoords[indexFaces].id3--;

			//Puts the correct vertices into the vertex vector
			//It would look like this: xyz(vert1 face1) xyz(vert2 face1) xyz(vert3 face1) etc
			tempVerticesArray[indexVertices + 0] = verticeCoords[facesCoords[indexFaces].id1].x;
			tempVerticesArray[indexVertices + 1] = verticeCoords[facesCoords[indexFaces].id1].y;
			tempVerticesArray[indexVertices + 2] = verticeCoords[facesCoords[indexFaces].id1].z;

			tempVerticesArray[indexVertices + 3] = verticeCoords[facesCoords[indexFaces].id2].x;
			tempVerticesArray[indexVertices + 4] = verticeCoords[facesCoords[indexFaces].id2].y;
			tempVerticesArray[indexVertices + 5] = verticeCoords[facesCoords[indexFaces].id2].z;

			tempVerticesArray[indexVertices + 6] = verticeCoords[facesCoords[indexFaces].id3].x;
			tempVerticesArray[indexVertices + 7] = verticeCoords[facesCoords[indexFaces].id3].y;
			tempVerticesArray[indexVertices + 8] = verticeCoords[facesCoords[indexFaces].id3].z;

			indexVertices += 9;
			indexFaces++;
		}
	}
	//Adding the vertices into a vector of the correct size
	vertexNumber = indexVertices - 9;
	finalVertices = new float[vertexNumber];

	for (int i = 0; i < indexVertices - 9; i++)
	{
		finalVertices[i] = tempVerticesArray[i];

		if (i % 3 == 0 && i)
			std::cout << std::endl;
		std::cout << finalVertices[i] << " ";
	}

	faceNumber = indexFaces - 1;
	vertexCoordsNumber = indexVertex;

	std::cout << vertexNumber;
}

void Object::DrawObject()
{

}
