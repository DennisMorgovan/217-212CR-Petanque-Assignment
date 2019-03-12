#include "Camera.h"



Camera::Camera()
{
}

Camera::Camera(unsigned int programId, float cameraAngle, float width, float height, float Near, float Far)
{
	cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
	cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	cameraDirection = glm::normalize(cameraPos - cameraTarget);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	cameraFront = glm::vec3(0, 0, -1);

	cameraUp = glm::cross(cameraDirection, cameraRight);

	//projMat = glm::frustum(-15.0, 15.0, -15.0, 15.0, 5.0, 500.0);
	projMat = glm::perspective(glm::radians(cameraAngle), width/height, Near, Far);

	projMatLoc = glGetUniformLocation(programId, "projMat");
	glUniformMatrix4fv(projMatLoc, 1, GL_TRUE, glm::value_ptr(projMat));
	///////////////////////////////////////

	// Obtain modelview matrix uniform location and set value.
	glm::mat4 modelViewMat(1.0f);
	modelViewMatLoc = glGetUniformLocation(programId, "modelViewMat");
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_TRUE, glm::value_ptr(modelViewMat));
	///////////////////////////////////////

}


Camera::~Camera()
{
}

void Camera::update(float angle, float camX, float camZ, int deltaTime)
{
	//Rotates the camera view up/down/left/right based on mouse input
	cameraFront.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	cameraFront.y = sin(glm::radians(pitch));
	cameraFront.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));	
	cameraFront = glm::normalize(cameraFront);

	float deltaSpeed = speed * deltaTime;

	if (specialKeys[GLUT_KEY_UP]) { //Adding deltaTime makes the current speed magnitude static?
		cameraPos += deltaSpeed * cameraFront;
		//std::cout << "Moving forward! " << std::endl;
	}
	else if (specialKeys[GLUT_KEY_DOWN]) {
		cameraPos -= deltaSpeed * cameraFront;
	}

	if (specialKeys[GLUT_KEY_LEFT]) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * deltaSpeed;
	}

	if (specialKeys[GLUT_KEY_RIGHT]) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * deltaSpeed;
	}

	view = glm::mat4(1.0f);
	//cameraPos = glm::vec3(camX, 0.0f, camZ);
	//view = glm::rotate(view, angle, glm::vec3(0.0, 1.0, 0.0));
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(view));
}

void Camera::passiveMotionFunc(int x, int y)
{
	//Generates mouse input and modifies the pitch/yaw

	float xoffset = x - lastX;
	float yoffset = lastY - y; // reversed since y-coordinates range from bottom to top

	lastX = x;
	lastY = y;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	//if (x != 960 || y != 540) glutWarpPointer(960, 540);
}

//Getter for the model view matrix
glm::mat4 Camera::getModelViewMat()
{
	return view;
}
