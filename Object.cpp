#include "Object.h"



Object::Object()
{
}

Object::Object(glm::vec3 position)
{
	this->position = position;
}


Object::~Object()
{ 
	delete[] finalVertices;  
}

void Object::LoadObject(char* filename)
{
	fp = fopen(filename, "r"); //Opens the file
	int indexVertex = 0, uvIndex = 0, indexNormals = 0, indexFaces = 0, indexVertices = 0, indexUVs = 0, indexNormal = 0;

	while (!(feof(fp))) //While we are still in the file
	{
		read = fscanf(fp, "%s", lineHeader); //Scan the first line and save the header characters into lineHeader

		if (strcmp(lineHeader, "v") == 0) //If it is a vertex line
		{
			fscanf(fp, "%f %f %f", &verticeCoords[indexVertex].x, &verticeCoords[indexVertex].y, &verticeCoords[indexVertex].z);
			indexVertex++;
		}
		else if (strcmp(lineHeader, "vt") == 0) //If it is a texture vertex
		{
			fscanf(fp, "%f %f", &uv[uvIndex].x, &uv[uvIndex].y);
			uvIndex++;
		}
		else if (strcmp(lineHeader, "vn") == 0) //If it is a normal vertex
		{
			fscanf(fp, "%f %f %f", &normals[indexNormals].x, &normals[indexNormals].y, &normals[indexNormals].z);
			indexNormals++;
		}
		else if (strcmp(lineHeader, "f") == 0) //If it is a face
		{
			fscanf(fp, "%i/%i/%i %i/%i/%i %i/%i/%i",
				&facesCoords[indexFaces].id1, &textureCoords[indexFaces].id1, &normalsCoords[indexFaces].id1,
				&facesCoords[indexFaces].id2, &textureCoords[indexFaces].id2, &normalsCoords[indexFaces].id2,
				&facesCoords[indexFaces].id3, &textureCoords[indexFaces].id3, &normalsCoords[indexFaces].id3);

			facesCoords[indexFaces].id1--;
			facesCoords[indexFaces].id2--;
			facesCoords[indexFaces].id3--;
			textureCoords[indexFaces].id1--;
			textureCoords[indexFaces].id2--;
			textureCoords[indexFaces].id3--;
			normalsCoords[indexFaces].id1--;
			normalsCoords[indexFaces].id2--;
			normalsCoords[indexFaces].id3--;

			//Puts the correct vertices into the vertex vector
			//It would look like this: xyz(vert1 face1) xyz(vert2 face1) xyz(vert3 face1) etc
			tempVerticesArray[indexVertices + 0] = verticeCoords[facesCoords[indexFaces].id1].x;
			tempVerticesArray[indexVertices + 1] = verticeCoords[facesCoords[indexFaces].id1].y;
			tempVerticesArray[indexVertices + 2] = verticeCoords[facesCoords[indexFaces].id1].z;
			tempUVArray[indexUVs + 0] = uv[textureCoords[indexFaces].id1].x;
			tempUVArray[indexUVs + 1] = uv[textureCoords[indexFaces].id1].y;
			tempNormalsArray[indexNormal + 0] = normals[normalsCoords[indexFaces].id1].x;
			tempNormalsArray[indexNormal + 1] = normals[normalsCoords[indexFaces].id1].y;
			tempNormalsArray[indexNormal + 2] = normals[normalsCoords[indexFaces].id1].z;

			tempVerticesArray[indexVertices + 3] = verticeCoords[facesCoords[indexFaces].id2].x;
			tempVerticesArray[indexVertices + 4] = verticeCoords[facesCoords[indexFaces].id2].y;
			tempVerticesArray[indexVertices + 5] = verticeCoords[facesCoords[indexFaces].id2].z;
			tempUVArray[indexUVs + 2] = uv[textureCoords[indexFaces].id2].x;
			tempUVArray[indexUVs + 3] = uv[textureCoords[indexFaces].id2].y;
			tempNormalsArray[indexNormal + 3] = normals[normalsCoords[indexFaces].id2].x;
			tempNormalsArray[indexNormal + 4] = normals[normalsCoords[indexFaces].id2].y;
			tempNormalsArray[indexNormal + 5] = normals[normalsCoords[indexFaces].id2].z;

			tempVerticesArray[indexVertices + 6] = verticeCoords[facesCoords[indexFaces].id3].x;
			tempVerticesArray[indexVertices + 7] = verticeCoords[facesCoords[indexFaces].id3].y;
			tempVerticesArray[indexVertices + 8] = verticeCoords[facesCoords[indexFaces].id3].z;
			tempUVArray[indexUVs + 4] = uv[textureCoords[indexFaces].id3].x;
			tempUVArray[indexUVs + 5] = uv[textureCoords[indexFaces].id3].y;
			tempNormalsArray[indexNormal + 6] = normals[normalsCoords[indexFaces].id3].x;
			tempNormalsArray[indexNormal + 7] = normals[normalsCoords[indexFaces].id3].y;
			tempNormalsArray[indexNormal + 8] = normals[normalsCoords[indexFaces].id3].z;

			indexNormal += 9;
			indexVertices += 9;
			indexUVs += 6;
			indexFaces++;
		}
	}
	//Adding the vertices into a vector of the correct size
	vertexNumber = 2 * (indexVertices - 9) + (indexUVs - 6);
	finalVertices = new float[vertexNumber];

	//Adding the vertex coords to the array
	for (int i = 0; i < indexVertices - 9; i++)
		finalVertices[i] = tempVerticesArray[i];

	//Adding the texture coords to the array
	int j = 0;
	for (int i = indexVertices - 9; i < indexVertices - 9 + indexUVs - 6; i ++)
	{
		finalVertices[i] = tempUVArray[j];
		j++;
	}

	//Adding normals coords to the array
	j = 0;
	for (int i = indexVertices - 9 + indexUVs - 6; i < vertexNumber; i++)
	{
		finalVertices[i] = tempNormalsArray[j];
		j++;
	}

	faceNumber = indexFaces - 1;
	vertexCoordsNumber = indexVertex;
	uvNumber = uvIndex;

	//Used for subdata
	finalVerticesVertexNumber = indexVertices - 9;
	finalVerticesNormalsNumber = indexVertices - 9 + indexUVs - 6;
}

void Object::SetupDrawing(unsigned int vao, unsigned int vbo, int locationVert, int locationTex, int locationNormals)
{
	//Binding the vertex array and the buffer objects
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//Setting the data
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexNumber, finalVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(locationVert, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(locationVert);
	glVertexAttribPointer(locationTex, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) (sizeof(float) * finalVerticesVertexNumber));
	glEnableVertexAttribArray(locationTex);
	glVertexAttribPointer(locationNormals, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(sizeof(float) * finalVerticesNormalsNumber));
	glEnableVertexAttribArray(locationNormals);
}

void Object::DrawObject(unsigned int vao, unsigned int objectLoc, unsigned int obj, unsigned int modelMatLoc, glm::vec3 position)
{
	glUniform1ui(objectLoc, obj);
	glBindVertexArray(vao);

	glm::mat4 modelMat = glm::mat4(1.0f);
	modelMat = glm::translate(modelMat, position);
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, value_ptr(modelMat));
	
	glDrawArrays(GL_TRIANGLES, 0, faceNumber * 3);
}

void Object::DrawObject(unsigned int vao, unsigned int objectLoc, unsigned int obj)
{
	glUniform1ui(objectLoc, obj);
	glBindVertexArray(vao);

	glDrawArrays(GL_TRIANGLES, 0, faceNumber * 3);
}

void Object::DrawObjectInstance(unsigned int vao, unsigned int objectLoc, unsigned int obj, int instanceNumber)
{
	glUniform1ui(objectLoc, obj);
	glBindVertexArray(vao);

	glDrawArraysInstanced(GL_TRIANGLES, 0, faceNumber * 3, instanceNumber);
}

