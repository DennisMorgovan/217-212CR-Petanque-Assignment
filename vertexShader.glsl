#version 430 core


layout(location=0) in vec4 squareCoords;
layout(location=1) in vec2 squareTexCoords;
layout(location=2) in vec3 squareNormals;


uniform mat4 projMat;
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform uint object;

out vec4 colorsExport;
out vec2 texCoordsExport;
out vec3 normals;
out vec3 fragPos;

vec4 coords;

void main(void)
{
   
	coords = squareCoords;

	texCoordsExport = squareTexCoords;
	colorsExport = vec4(1,0,1,1);
	normals = squareNormals;
	fragPos = vec3(modelMat * squareCoords);

   gl_Position = projMat * viewMat * modelMat * coords;
}