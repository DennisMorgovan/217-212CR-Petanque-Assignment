//STD includes
#include <iostream>
#include <fstream>

//OpenGL includes
#ifdef __APPLE__
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#  include <glm.hpp> ///FOR MAT4 USAGE
#  include <gtc/type_ptr.hpp> ///FOR MAT4 VALUE PTR USAGE
#  include <glm/gtc/matrix_transform.hpp>
#  include <vector> ///FOR SHADER COMPILE TEST
#pragma comment(lib, "glew32.lib") 
#endif

#include "./src/Object.h"
#include "./src/Camera.h"
#include "./src/Skybox.h"
#include "./src/getbmp.h"
#include "./src/Ball.h"
#include "./src/Lighting.h"


using namespace std;

static enum object { FIELD, SKY, CUBE }; // VAO ids.
static enum buffer { FIELD_VERTICES, SKY_VERTICES }; // VBO ids.

glm::mat4 modelMat(1.0f);
glm::mat4 viewMat(1.0f);
glm::mat4 projMat(1.0f);

Object testCube, grassfield, sun;
Object houseBase, houseSides, houseBody, houseRoof, houseDoor;
Object skyF, skyB, skyLf, skyRt, skyUp, skyDn;
Skybox skybox;
Camera* camera;

Lighting lighting(glm::vec3(0, 10, -25));

/// Holds 2 buffer objects
unsigned int buffer[15];
//VAOs and buffers from 4 to 9 are used for skybox
unsigned int vao[15]; // Array of VAO ids.

unsigned int texture[25]; // Array of VAO ids.

static BitMapFile *image[25]; // Local storage for bmp image data.

///Setup shaders and send data to them
unsigned int programId, vertexShaderId, fragmentShaderId, modelMatLoc, projMatLoc;
unsigned int objectLoc, grassTexLoc, skyTexLoc, cubeTexLoc, ballTexLoc, sunTexLoc, objColorLoc;
unsigned int skyTexLocFt, skyTexLocBk, skyTexLocLf, skyTexLocRt, skyTexLocUp, skyTexLocDn;
unsigned int skyTexLocFtNight, skyTexLocBkNight, skyTexLocLfNight, skyTexLocRtNight, skyTexLocUpNight, alphaLoc, blinnLoc;
unsigned int houseBaseTexLoc, houseSidesTexLoc, houseBodyTexLoc, houseRoofTexLoc, houseDoorTexLoc;
unsigned int testLoc;

//Used to calculate in-game time and fps
int lastTime = 0, currentTime = 0, deltaTime = 1, fps;

//Used for rotating the object
float ballAngle = 0.0f, ballRotation = 0.0f, ballSpeed = 1.0f;
float skyboxAngle = 0.0f;
float theta= 0, alpha = 0;
float camX = 0, camZ = 0;
int msaa = 1, antialias = 1, blinn = 1, wireframe = 1, debug = 1;

//Tests whether the shaders have compiled successfully or not
void shaderCompileTest(GLuint shader)
{
	GLint result = GL_FALSE;
	int logLength; glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<GLchar> vertShaderError((logLength > 1) ? logLength : 1);
	glGetShaderInfoLog(shader, logLength, NULL, &vertShaderError[0]);
	std::cout << &vertShaderError[0] << std::endl;
}

//Function to read text file.
char* readTextFile(char* aTextFile)
{
	FILE* filePointer = fopen(aTextFile, "rb");
	char* content = NULL;
	long numVal = 0;

	fseek(filePointer, 0L, SEEK_END);
	numVal = ftell(filePointer);
	fseek(filePointer, 0L, SEEK_SET);
	content = (char*)malloc((numVal + 1) * sizeof(char));
	fread(content, 1, numVal, filePointer);
	content[numVal] = '\0';
	fclose(filePointer);
	return content;
}

void loadExternalTextures()
{
	// Load the images.
	image[0] = getbmp("./Textures/metal.bmp");
	image[1] = getbmp("./Textures/grass.bmp");
	image[2] = getbmp("./Textures/fiberglass.bmp");
	image[3] = getbmp("./Textures/sun.bmp");
	//Skybox day
	image[4] = getbmp("./Textures/Skybox/hourglass_ft.bmp");
	image[5] = getbmp("./Textures/Skybox/hourglass_bk.bmp");
	image[6] = getbmp("./Textures/Skybox/hourglass_lf.bmp");
	image[7] = getbmp("./Textures/Skybox/hourglass_rt.bmp");
	image[8] = getbmp("./Textures/Skybox/hourglass_up.bmp");
	//image[9] = getbmp("./Textures/Skybox/hourglass_dn.bmp");
	//Skybox night
	image[9] = getbmp("./Textures/SkyboxNight/purplenebula_ft.bmp");

	//House
	image[10] = getbmp("./Textures/House/houseConcrete.bmp");
	image[11] = getbmp("./Textures/House/houseRoof.bmp");
	image[12] = getbmp("./Textures/House/houseStone.bmp");
	image[13] = getbmp("./Textures/House/houseWood.bmp");
	image[14] = getbmp("./Textures/House/houseDoor.bmp");
	image[15] = getbmp("./Textures/SkyboxNight/purplenebula_bk.bmp");
	image[16] = getbmp("./Textures/SkyboxNight/purplenebula_lf.bmp");
	image[17] = getbmp("./Textures/SkyboxNight/purplenebula_rt.bmp");
	image[18] = getbmp("./Textures/SkyboxNight/purplenebula_up.bmp");

	// Create texture ids.
	glGenTextures(25, texture);

	// Bind metal cube image.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	cubeTexLoc = glGetUniformLocation(programId, "cubeTex");
	glUniform1i(cubeTexLoc, 0);

	// Bind grass image.
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[1]->sizeX, image[1]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[1]->data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	grassTexLoc = glGetUniformLocation(programId, "grassTex");
	glUniform1i(grassTexLoc, 1);

	// Bind ball texture1 image.
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[2]->sizeX, image[2]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[2]->data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	ballTexLoc = glGetUniformLocation(programId, "ballTex");
	glUniform1i(ballTexLoc, 2);

	// Bind sun texture image.
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[3]->sizeX, image[3]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[3]->data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	sunTexLoc = glGetUniformLocation(programId, "sunTex");
	glUniform1i(sunTexLoc, 3);

	// Bind skybox images
	//Front
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[4]->sizeX, image[4]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[4]->data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	skyTexLocFt = glGetUniformLocation(programId, "skyTexFt");
	glUniform1i(skyTexLocFt, 4);

	//Back
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[5]->sizeX, image[5]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[5]->data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	skyTexLocBk = glGetUniformLocation(programId, "skyTexBk");
	glUniform1i(skyTexLocBk, 5);

	//Left
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, texture[6]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[6]->sizeX, image[6]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[6]->data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	skyTexLocLf = glGetUniformLocation(programId, "skyTexLf");
	glUniform1i(skyTexLocLf, 6);

	//Right
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, texture[7]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[7]->sizeX, image[7]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[7]->data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	skyTexLocRt = glGetUniformLocation(programId, "skyTexRt");
	glUniform1i(skyTexLocRt, 7);

	//Up
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, texture[8]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[8]->sizeX, image[8]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[8]->data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	skyTexLocUp = glGetUniformLocation(programId, "skyTexUp");
	glUniform1i(skyTexLocUp, 8);

	// Bind skybox night images
	//Front
	glActiveTexture(GL_TEXTURE9);
	glBindTexture(GL_TEXTURE_2D, texture[9]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[9]->sizeX, image[9]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[9]->data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	skyTexLocFtNight = glGetUniformLocation(programId, "skyTexNightFt");
	glUniform1i(skyTexLocFtNight, 9);

	//House
	//Body
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, texture[10]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[10]->sizeX, image[10]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[10]->data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	houseBodyTexLoc = glGetUniformLocation(programId, "houseBodyTex");
	glUniform1i(houseBodyTexLoc, 10);

	//Roof
	glActiveTexture(GL_TEXTURE11);
	glBindTexture(GL_TEXTURE_2D, texture[11]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[11]->sizeX, image[11]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[11]->data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	houseRoofTexLoc = glGetUniformLocation(programId, "houseRoofTex");
	glUniform1i(houseRoofTexLoc, 11);

	//Base
	glActiveTexture(GL_TEXTURE12);
	glBindTexture(GL_TEXTURE_2D, texture[12]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[12]->sizeX, image[12]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[12]->data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	houseBaseTexLoc = glGetUniformLocation(programId, "houseBaseTex");
	glUniform1i(houseBaseTexLoc, 12);

	//side
	glActiveTexture(GL_TEXTURE13);
	glBindTexture(GL_TEXTURE_2D, texture[13]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[13]->sizeX, image[13]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[13]->data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	houseSidesTexLoc = glGetUniformLocation(programId, "houseSidesTex");
	glUniform1i(houseSidesTexLoc, 13);

	//sides & door
	glActiveTexture(GL_TEXTURE14);
	glBindTexture(GL_TEXTURE_2D, texture[14]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[14]->sizeX, image[14]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[14]->data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	houseDoorTexLoc = glGetUniformLocation(programId, "houseDoorTex");
	glUniform1i(houseDoorTexLoc, 14);

	//Sky night
	//Back
	glActiveTexture(GL_TEXTURE15);
	glBindTexture(GL_TEXTURE_2D, texture[15]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[15]->sizeX, image[15]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[15]->data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	skyTexLocBkNight = glGetUniformLocation(programId, "skyTexNightBk");
	glUniform1i(skyTexLocBkNight, 15);

	//Left
	glActiveTexture(GL_TEXTURE16);
	glBindTexture(GL_TEXTURE_2D, texture[16]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[16]->sizeX, image[16]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[16]->data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	skyTexLocLfNight = glGetUniformLocation(programId, "skyTexNightLf");
	glUniform1i(skyTexLocLfNight, 16);

	//Right
	glActiveTexture(GL_TEXTURE17);
	glBindTexture(GL_TEXTURE_2D, texture[17]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[17]->sizeX, image[17]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[17]->data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	skyTexLocRtNight = glGetUniformLocation(programId, "skyTexNightRt");
	glUniform1i(skyTexLocRtNight, 17);

	//Up
	glActiveTexture(GL_TEXTURE18);
	glBindTexture(GL_TEXTURE_2D, texture[18]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[18]->sizeX, image[18]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[18]->data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	skyTexLocUpNight = glGetUniformLocation(programId, "skyTexNightUp");
	glUniform1i(skyTexLocUpNight, 18);

}

// Initialization routine.
void setup(void)
{
	glClearColor(0.2, 0.5, 0.4, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// Create shader program executable.
	char* vertexShader = readTextFile((char *) "vertexShader.glsl");
	vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderId, 1, (const char**)&vertexShader, NULL);
	glCompileShader(vertexShaderId);
	std::cout << "::: VERTEX SHADER :::" << std::endl;
	shaderCompileTest(vertexShaderId);

	char* fragmentShader = readTextFile((char *) "fragmentShader.glsl");
	fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderId, 1, (const char**)&fragmentShader, NULL);
	glCompileShader(fragmentShaderId);
	std::cout << "::: FRAGMENT SHADER :::" << std::endl;
	shaderCompileTest(fragmentShaderId);

	programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);
	glUseProgram(programId);

	/// DEPTH MAP ///

	/// MISC ///

	modelMatLoc = glGetUniformLocation(programId, "modelMat");
	objectLoc = glGetUniformLocation(programId, "object");
	objColorLoc = glGetUniformLocation(programId, "objColor");
	alphaLoc = glGetUniformLocation(programId, "alpha");
	blinnLoc = glGetUniformLocation(programId, "blinn");
	testLoc = glGetUniformLocation(programId, "test");
	glUniform4f(objColorLoc, 1.0, 1.0, 1.0, 1.0);

	/// Object loading ///

	testCube.LoadObject((char*)"./Models/testCube.obj");
	//ball.LoadObject((char*)"./Models/ballBlender.obj");
	sun.LoadObject((char*)"./Models/testCube.obj");
	grassfield.LoadObject((char*)"./Models/grassfield.obj");
	skyF.LoadObject((char*)"./Models/Skybox/skybox_front.obj");
	skyB.LoadObject((char*)"./Models/Skybox/skybox_back.obj");
	skyLf.LoadObject((char*)"./Models/Skybox/skybox_left.obj");
	skyRt.LoadObject((char*)"./Models/Skybox/skybox_right.obj");
	skyUp.LoadObject((char*)"./Models/Skybox/skybox_up.obj");
	houseBase.LoadObject((char*)"./Models/House/houseBase.obj");
	houseBody.LoadObject((char*)"./Models/House/houseBody.obj");
	houseDoor.LoadObject((char*)"./Models/House/houseDoor.obj");
	houseRoof.LoadObject((char*)"./Models/House/houseRoof.obj");
	houseSides.LoadObject((char*)"./Models/House/houseSides.obj");

	/// Binding stuff ///

	//Generating the VBO's and VAO's
	glGenVertexArrays(15, vao);
	//glGenVertexArrays(1, &sunVAO);
	glGenBuffers(15, buffer);

	//Binding test cube
	testCube.SetupDrawing(vao[0], buffer[0], 0, 1, 2);
	sun.SetupDrawing(vao[0], buffer[0], 0, 1, 2);
	grassfield.SetupDrawing(vao[1], buffer[1], 0, 1, 2);

	//ball.SetupDrawing(vao[3], buffer[3], 0, 1, 2);
	//skybox.SetupDrawing(vao[4], buffer[4]);
	skyF.SetupDrawing(vao[4], buffer[4], 0, 1, 2);
	skyB.SetupDrawing(vao[5], buffer[5], 0, 1, 2);
	skyLf.SetupDrawing(vao[6], buffer[6], 0, 1, 2);
	skyRt.SetupDrawing(vao[7], buffer[7], 0, 1, 2);
	skyUp.SetupDrawing(vao[8], buffer[8], 0, 1, 2);

	//VAOs and VBO's from 4 to 9 are used for skybox

	//House
	houseBase.SetupDrawing(vao[9], buffer[9], 0, 1, 2);
	houseBody.SetupDrawing(vao[10], buffer[10], 0, 1, 2);
	houseDoor.SetupDrawing(vao[11], buffer[11], 0, 1, 2);
	houseRoof.SetupDrawing(vao[12], buffer[12], 0, 1, 2);
	houseSides.SetupDrawing(vao[13], buffer[13], 0, 1, 2);
	//Setup for the camera heading line

	/// Camera ///

	camera = new Camera(programId, 40.0f, 1920, 1080, 1.0f, 100.0f, vao[3], buffer[3], objectLoc, modelMatLoc);

	/// Textures ///

	loadExternalTextures();

	/// Lighting ///

	lighting.SetupLighting(programId);
}

// Drawing routine.
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/// CAMERA ///

	camera->update(deltaTime, ballAngle, ballRotation);

	/// DRAWING ///

	//Sun
	modelMat = glm::mat4(1.0f);
	modelMat = glm::translate(modelMat, lighting.lightPos);
	modelMat = glm::scale(modelMat, glm::vec3(0.1, 0.1, 0.1));
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, value_ptr(modelMat));


	lighting.DrawLighting(vao[0], objectLoc, 4, theta);
	glUniform3f(objColorLoc, 1.0, 1.0, 1.0);
	testCube.DrawObject(vao[0], objectLoc, 4);

	//testcube
	glUniform3f(objColorLoc, 0.5, 0.7, 1.0);
	testCube.DrawObject(vao[0], objectLoc, 2, modelMatLoc, glm::vec3(0, 10, 0));
	
	//Grassfield
	glUniform3f(objColorLoc, 0.0, 0.5, 0.0);
	grassfield.DrawObject(vao[1], objectLoc, 0, modelMatLoc, glm::vec3(0, -5, 0));

	//Draw skybox;

	modelMat = glm::mat4(1.0f);
	//modelMat = glm::translate(modelMat, lighting.lightPos);
	//modelMat = glm::scale(modelMat, glm::vec3(0.1, 0.1, 0.1));
	modelMat = glm::rotate(modelMat, glm::radians(skyboxAngle), glm::vec3(0, 1, 0));
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, value_ptr(modelMat));
	//Front
	skyF.DrawObject(vao[4], objectLoc, 5);
	//Back
	skyB.DrawObject(vao[5], objectLoc, 6);
	//Left
	skyLf.DrawObject(vao[6], objectLoc, 7);
	//Right
	skyRt.DrawObject(vao[7], objectLoc, 8);
	//Up
	skyUp.DrawObject(vao[8], objectLoc, 9);

	//Draw ball
	//glUniform4f(objColorLoc, 0.2, 0.5, 0.5, 1.0);
	//ball.DrawObject(vao[3], objectLoc, 3, modelMatLoc, glm::vec3(20, 0, 30), deltaTime);

	//House
	modelMat = glm::mat4(1.0f);
	modelMat = glm::rotate(modelMat, glm::radians(180.0f), glm::vec3(0, 1, 0));
	modelMat = glm::translate(modelMat, glm::vec3(-45, -5, 30));
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, value_ptr(modelMat));

	houseBase.DrawObject(vao[9], objectLoc, 10);
	houseBody.DrawObject(vao[10], objectLoc, 11);
	houseDoor.DrawObject(vao[11], objectLoc, 12);
	houseRoof.DrawObject(vao[12], objectLoc, 13);
	houseSides.DrawObject(vao[13], objectLoc, 14);

	//Angle corrections
	if (theta > 360)
		theta -= 360;
	if (skyboxAngle > 360)
		skyboxAngle -= 360;

	glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
}

void idle()
{
	Sleep(1000 / 60);
	theta += 0.08;

	if (theta <= 90.0)
		alpha = theta / 90.0;
	else
		alpha = (180.0 - theta) / 90.0;

	glUniform1f(alphaLoc, alpha);

	//Calculates delta time (in ms)
	lastTime = currentTime;
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	deltaTime = currentTime - lastTime;

	//If the last frame was rendered less than 1 ms ago, the detalaTime will be 0 ms. This causes problems in calculations, so sleep for 1ms to adjust.
	if (deltaTime == 0) {
		Sleep(1);
		currentTime = glutGet(GLUT_ELAPSED_TIME);
		deltaTime = currentTime - lastTime;
	}
	fps = 1000 / deltaTime;

	skyboxAngle += 0.01f;

	for (std::vector<Object>::size_type i = 0; i != camera->balls.size(); i++) {
		for (std::vector<Object>::size_type j = i + 1; j != camera->balls.size(); j++) {
			if (camera->balls[i]->collider != NULL) {
				SphereCollider *col1 = (SphereCollider*)camera->balls[i]->collider;
				SphereCollider *col2 = (SphereCollider*)camera->balls[j]->collider;

				if (col1->collidesWithSphere(col2))//If it collides
				{
					camera->balls[i]->collides(camera->balls[j], 1);
					
					//Collision resolution
					/*if (cubeColliderVector[i]->objectType == 1 && cubeColliderVector[j]->objectType == 2) //If collision between hovercraft & obstacle
					{
						hovercraft.collides(cubeColliderVector[j], cubeColliderVector[j]->materialBounce);
						std::cout << "Collides with obstacle!" << std::endl;
					}*/
				}

			}
		}
	}

	glutPostRedisplay();
}

// Main routine.
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(1920, 1080);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Modern OpenGL Petanque Game");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutIdleFunc(idle);

	//Lambda functions to link our code to glut's keydown and keyup. Our function deals with both regular and special keys in one.
	glutKeyboardFunc([](unsigned char key, int x, int y) {
		camera->keys[key] = true;

		if (key == 27) {
			exit(0);
		}
		else if (key == '1')
		{
			camera->cameraMode = 0;
		}
		else if (key == '2')
		{
			camera->cameraMode = 1;
		}
		else if (key == '3')
		{
			camera->cameraMode = 2;
		}
		else if (key == 'x')
		{
			if (msaa == 1)
			{
				glEnable(GL_MULTISAMPLE_ARB);
				cout << "MSAA on" << endl;
			}
			else
			{
				glDisable(GL_MULTISAMPLE_ARB);
				cout << "MSAA off" << endl;
			}
			msaa *= -1;
		}
		else if (key == 'z')
		{
			if (antialias == 1)
			{
				glEnable(GL_BLEND); // Enable blending.
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Specify blending parameters.

				glEnable(GL_LINE_SMOOTH); // Enable line antialiasing.
				glHint(GL_LINE_SMOOTH_HINT, GL_NICEST); // Ask for best line antialiasing.
				glEnable(GL_POINT_SMOOTH);  // Enable point antialiasing.
				glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); // Ask for best point antialiasing.
				cout << "Antialias on" << endl;
			}
			else
			{
				glDisable(GL_BLEND);
				glDisable(GL_LINE_SMOOTH);
				glDisable(GL_POINT_SMOOTH);
				cout << "Antialias off" << endl;
			}

			antialias *= -1;
		}
		else if (key == 'c')
		{
			if (blinn == 1)
			{
				glUniform1i(blinnLoc, 1);
				cout << "Blinn-Phong lighting model" << endl;
			}
			else
			{
				glUniform1i(blinnLoc, -1);
				cout << "Phong lighting model" << endl;
			}
			blinn *= -1;
		}
		else if (key == 'v')
		{
			if(wireframe == 1)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			wireframe *= -1;
		}
		else if (key == 'w' && ballAngle < 90.0f)
		{
			ballAngle += 5.0f;
			std::cout << ballAngle << std::endl;
		}
		else if (key == 's' && ballAngle > 0.0f)
		{
			ballAngle -= 5.0f;
			std::cout << ballAngle << std::endl;
		}
		else if (key == 'd' && ballRotation < 180.0f)
		{
			ballRotation += 5.0f;
			std::cout << "Ball rotation: " << ballRotation << std::endl;
		}
		else if (key == 'a' && ballRotation > 0.0f)
		{
			ballRotation -= 5.0f;
			std::cout << "Ball rotation: " << ballRotation << std::endl;
		}
		else if (key == 'q' && ballSpeed < 20.0f)
		{
			ballSpeed += 1.0f;
			std::cout << ballSpeed<< std::endl;
		}
		else if (key == 'e' && ballSpeed > 1.0f)
		{
			ballSpeed -= 1.0f;
			std::cout << ballSpeed << std::endl;
		}
		else if (key == 'p') //Enable debug mode
		{
			if (debug == 1)
			{
				camera->debugMode = true;
			}
			else
			{
				camera->debugMode = false;
			}
			debug *= -1;
		}

	});

	glutSpecialFunc([](int key, int x, int y) {
		camera->specialKeys[key] = true;
		glutPostRedisplay();
	});

	glutKeyboardUpFunc([](unsigned char key, int x, int y) {
		camera->keys[key] = false;
	});

	glutSpecialUpFunc([](int key, int x, int y) {
		camera->specialKeys[key] = false;
	});

	glutPassiveMotionFunc([](int x, int y) {
		camera->passiveMotionFunc(x, y);
	});

	glutMouseFunc([](int key, int state, int x, int y) {
		camera->mouseControl(key, state, x, y, objectLoc, modelMatLoc, ballSpeed, ballAngle, ballRotation);
	});

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}