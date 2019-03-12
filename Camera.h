#pragma once
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

#include <map>
#include <iostream>

class Camera
{
public:
	glm::vec3 cameraPos, cameraTarget, cameraDirection, up, cameraRight, cameraUp, cameraFront;
	glm::mat4 view, projMat;
	unsigned int modelViewMatLoc, projMatLoc;
	//Stores keys as pairs of key and state (true = pressed, false = released)
	std::map <int, bool> specialKeys;
	std::map <char, bool> keys;

	float speed = 0.01f;
	float sensitivity = 0.05f;
	float lastX = 960, lastY = 540;
	float yaw = 0, pitch = 0;

	Camera();
	Camera(unsigned int programId, float cameraAngle, float width, float height, float Near, float Far);
	~Camera();

	void update(float angle, float camX, float camZ, int deltaTime);

	void passiveMotionFunc(int x, int y);

	glm::mat4 getModelViewMat();
};

