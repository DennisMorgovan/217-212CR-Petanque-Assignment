#version 430 core


layout(location=0) in vec4 squareCoords;
layout(location=1) in vec2 squareTexCoords;
layout(location=2) in vec3 squareNormals;
//layout(location=3) in vec4 textCoords; // vec4 made up of two vec2s: vec2 pos, vec2 tex

uniform mat4 projMat;
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform uint object;
uniform int offset;
vec3 offsets[20];

out vec4 colorsExport;
out vec2 texCoordsExport;
out vec3 normals;
out vec3 fragPos;
out float visibility;

vec4 coords, posRelativeToCamera;
float distance;

const float density = 0.05;
const float gradient = 1.5;

void generateOffsets()
{
	offsets[0] = vec3(10, -2, 40); offsets[1] = vec3(5, -2, 45); offsets[2] = vec3(0, -2, 30); offsets[3] = vec3(-25, -2, 15); offsets[4] = vec3(5, -2, 25); offsets[5] = vec3(15, -2, 55); offsets[6] = vec3(-15, -2, 12);
	offsets[7] = vec3(-12, -2, 20); offsets[8] = vec3(-7, -2, 25); offsets[9] = vec3(-25, -2, -35); offsets[10] = vec3(-15, -2, -25); offsets[11] = vec3(-25, -2, -10); offsets[12] = vec3(-18, -2, -15); offsets[13] = vec3(-3, -2, -22);
	offsets[14] = vec3(5, -2, -18); offsets[15] = vec3(10, -2, -18); offsets[16] = vec3(-8, -2, -15); offsets[16] = vec3(-8, -2, -15); offsets[17] = vec3(10, -2, 20); offsets[18] = vec3(5, -2, 15); offsets[19] = vec3(-12, -2, 20);
}

void main(void)
{
	generateOffsets();
    vec3 offset = offsets[gl_InstanceID];
	coords = squareCoords;
	if(object == 18 || object == 19)
		coords = vec4(squareCoords.x + offset.x, squareCoords.y + offset.y, squareCoords.z + offset.z, squareCoords.w);

	texCoordsExport = squareTexCoords;
	colorsExport = vec4(1,0,1,1);
	normals = squareNormals;
	fragPos = vec3(modelMat * squareCoords);

	//Fog calculations
	posRelativeToCamera = viewMat * coords;
	distance = length(posRelativeToCamera.xyz);

	visibility = exp(-pow((distance * density), gradient));
	visibility = clamp(visibility, 0, 1);

    gl_Position = projMat * viewMat * modelMat * coords;
}