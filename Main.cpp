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
#  include <vector> ///FOR SHADER COMPILE TEST
#pragma comment(lib, "glew32.lib") 
#endif

#include "./src/Reader.h"
#include "./src/Object.h"

using namespace std;

Reader testCube((char*)"./Models/cube.obj");

/*
struct Vertex
{
float coords[4];
float colors[4];
};

struct Matrix4x4
{
float entries[16];
};

static const Matrix4x4 IDENTITY_MATRIX4x4 =
{
{
1.0, 0.0, 0.0, 0.0,
0.0, 1.0, 0.0, 0.0,
0.0, 0.0, 1.0, 0.0,
0.0, 0.0, 0.0, 1.0
}
};

static enum buffer { SQUARE_VERTICES };
static enum object { SQUARE };

// Globals
static Vertex squareVertices[] =
{
{ { 20.0, 20.0, 0.0, 1.0 },{ 1.0, 0.0, 0.0, 1.0 } },
{ { 80.0, 20.0, 0.0, 1.0 },{ 0.0, 0.65, 0.0, 1.0 } },
{ { 20.0, 80.0, 0.0, 1.0 },{ 0.5, 0.0, 0.0, 1.0 } },
{ { 80.0, 80.0, 0.0, 1.0 },{ 0.0, 0.25, 0.0, 1.0 } }
};

static Matrix4x4
modelViewMat = IDENTITY_MATRIX4x4,
projMat = IDENTITY_MATRIX4x4;

static unsigned int
programId,
vertexShaderId,
fragmentShaderId,
modelViewMatLoc,
projMatLoc,
buffer[1],
vao[1];


*/

/// Holds 2 buffer objects
unsigned int buffer[2];
/// Holds 2 vertex array objects
unsigned int vao[2]; // Array of VAO ids.

///Setup shaders and send data to them
unsigned int programId, vertexShaderId, fragmentShaderId, modelViewMatLoc, projMatLoc;

//Model and projection matrices
glm::mat4 modelViewMat(1.0f);
glm::mat4 projMat(1.0f);

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

// Initialization routine.
void setup(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);

	// Create shader program executable.
	char* vertexShader = readTextFile( (char *) "./vertexShader.glsl" );
	vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderId, 1, (const char**)&vertexShader, NULL);
	glCompileShader(vertexShaderId);
	std::cout << "::: VERTEX SHADER :::" << std::endl;
	shaderCompileTest(vertexShaderId);

	char* fragmentShader = readTextFile( (char *) "./fragmentShader.glsl" );
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

	testCube.LoadModel();

	//Binding stuff

	/*
	// Create VAO and VBO and associate data with vertex shader.
	glGenVertexArrays(1, vao);
	glGenBuffers(1, buffer);
	glBindVertexArray(vao[SQUARE]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[SQUARE_VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(squareVertices[0]), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(squareVertices[0]), (GLvoid*)sizeof(squareVertices[0].coords));
	glEnableVertexAttribArray(1);
	///////////////////////////////////////

	// Obtain projection matrix uniform location and set value.
	Matrix4x4 projMat =
	{
	{
	0.02, 0.0,  0.0, -1.0,
	0.0,  0.02, 0.0, -1.0,
	0.0,  0.0, -1.0,  0.0,
	0.0,  0.0,  0.0,  1.0
	}
	};
	projMatLoc = glGetUniformLocation(programId, "projMat");
	glUniformMatrix4fv(projMatLoc, 1, GL_TRUE, projMat.entries);
	///////////////////////////////////////

	// Obtain modelview matrix uniform location and set value.
	Matrix4x4 modelViewMat = IDENTITY_MATRIX4x4;
	modelViewMatLoc = glGetUniformLocation(programId, "modelViewMat");
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_TRUE, modelViewMat.entries);*/
	///////////////////////////////////////
}

// Drawing routine.
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glFlush();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

// Mouse callback routine.
void mouseControl(int key, int state, int x, int y)
{
	if (key == GLUT_RIGHT_BUTTON)
		exit(0);
}

// Main routine.
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(1920, 1080);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Modern OpenGL Petanque Game.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);

	// Register the mouse callback function.
	glutMouseFunc(mouseControl);
	glutKeyboardFunc(keyInput);


	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}


//Create vertex structure (position and colour)