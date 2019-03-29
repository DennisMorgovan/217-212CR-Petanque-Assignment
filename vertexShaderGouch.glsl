#version 430 core


layout(location=0) in vec4 squareCoords;
layout(location=1) in vec2 squareTexCoords;
layout(location=2) in vec3 squareNormals;


uniform mat4 projMat;
uniform mat4 modelMat;
uniform mat4 viewMat;

out vec4 colorsExport;
out vec3 normals;
out vec3 fragPos;

void main(void)
{
	//colorsExport = vec4(1,0,1,1);
	normals = squareNormals;
	//normals = normalize(vec3(vec4(squareNormals, 0.0) * modelMat));
	fragPos = vec3(modelMat * squareCoords);

   gl_Position = projMat * viewMat * modelMat * squareCoords;
}