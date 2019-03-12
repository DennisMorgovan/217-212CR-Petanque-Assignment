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
#include "Ball.h"


using namespace std;

static enum object { FIELD, SKY, CUBE }; // VAO ids.
static enum buffer { FIELD_VERTICES, SKY_VERTICES }; // VBO ids.

glm::mat4 modelViewMat(1.0f);
glm::mat4 projMat(1.0f);

Object testCube, grassfield;
Skybox skybox;
Camera* camera;
Ball ball;

/// Holds 2 buffer objects
unsigned int buffer[5];
/// Holds 2 vertex array objects
unsigned int vao[4]; // Array of VAO ids.
unsigned int sunVAO;

unsigned int texture[2]; // Array of VAO ids.

static BitMapFile *image[2]; // Local storage for bmp image data.

///Setup shaders and send data to them
unsigned int programId, vertexShaderId, fragmentShaderId, modelViewMatLoc, projMatLoc;
static unsigned int objectLoc, grassTexLoc, skyTexLoc, cubeTexLoc;

//Used to calculate in-game time and fps
int lastTime = 0, currentTime = 0, deltaTime = 1, fps;

//Used for rotating the object
float angle = 0.0f, angle1 = 0.0f;
bool isRotated = false;
float camX = 0, camZ = 0;

float vertices[] = {
	100.0f, -3.0f, 100.0f,
	100.0f, -3.0f, -100.0f,
	-100.0f, -3.0f, 100.0f,
	-100.0f, -3.0f, -100.0f
};

float sunVertices[] = {
	 -0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,

	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,

	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,

	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f
};

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

	// Create texture ids.
	glGenTextures(2, texture);

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
}


// Initialization routine.
void setup(void)
{
	glClearColor(0.2, 0.5, 0.4, 0.0);
	glEnable(GL_DEPTH_TEST);

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

	/// MISC ///

	objectLoc = glGetUniformLocation(programId, "object");


	/// Object loading ///

	testCube.LoadObject((char*)"./Models/testCube.obj");
	ball.LoadObject((char*)"./Models/ballBlender.obj");
	grassfield.LoadObject((char*)"./Models/grassfield.obj");

	/// Binding stuff ///

	//Generating the VBO's and VAO's
	glGenVertexArrays(5, vao);
	//glGenVertexArrays(1, &sunVAO);
	glGenBuffers(5, buffer);

	//Binding test cube
	testCube.SetupDrawing(vao[0], buffer[0], 0, 1);
	grassfield.SetupDrawing(vao[1], buffer[1], 0, 1); //Ask about this; why it can't be 2 and 3
	//skybox.SetupDrawing(vao[2], buffer[2]);
	//ball.SetupDrawing(vao[3], buffer[3], 0, 1);

	/// Textures ///

	loadExternalTextures();

	/// Camera ///

	camera = new Camera(programId, 40.0f, 1920, 1080, 1.0f, 100.0f);
}

// Drawing routine.
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glm::mat4 modelViewMat(1.0f);
	//glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));

	/// CAMERA ///

	camera->update(angle, camX, camZ, deltaTime);

	/// DRAWING ///

	//Testcube transforms
	modelViewMat = glm::translate(camera->getModelViewMat(), glm::vec3(0, 10, -50));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));

	//Draw testcube
	//glUniform1ui(objectLoc, CUBE);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	testCube.DrawObject(vao[0], objectLoc, CUBE);

	
	//Grassfield
	//modelViewMat = glm::translate(camera->getModelViewMat(), glm::vec3(0, 0, 0));
	//glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	grassfield.DrawObject(vao[1], objectLoc, FIELD); //Ask about this

	
	//Draw skybox;
	//skybox.DrawObject(vao[2]);

	//Draw ball
	//modelViewMat = glm::mat4(1.0f);
	//modelViewMat = glm::translate(modelViewMat, glm::vec3(20, 0, 30));
	//glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));

	//glActiveTexture(GL_TEXTURE1);
	//ball.DrawObject(vao[3], objectLoc, FIELD);

	//Angle corrections
	if (angle > 360)
		angle -= 360;
	if (angle1 > 360)
		angle1 -= 360;

	glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
}

void idle()
{
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

	glutPostRedisplay();
}

// Main routine.
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(1920, 1080);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Modern OpenGL Petanque Game");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutIdleFunc(idle);

	// Register the mouse callback function.
	glutMouseFunc([](int key, int state, int x, int y) {
		if (key == GLUT_RIGHT_BUTTON)
			exit(0);
	});

	//Lambda functions to link our code to glut's keydown and keyup. Our function deals with both regular and special keys in one.
	glutKeyboardFunc([](unsigned char key, int x, int y) {
		camera->keys[key] = true;

		if (key == 27) {
			exit(0);
		}
		else if (key == 'a')
		{
			angle += 0.1f;
			isRotated = true;
		}
		else if (key == 'q')
		{
			angle1 += 1;

			camX = sin(angle1 * 3.1415 / 180.0) * 100;
		}
		else if (key == 'e')
		{
			angle1 += 1;

			camZ = cos(angle1 * 3.1415 / 180.0) * 100;
		}
		else
		{
			isRotated = false;
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

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}


//Create vertex structure (position and colour)
//Currently, I hold stuff in vertex structure, but I need everything as an array of coordinates xyz xyz xyz etc