#include "Camera.h"



Camera::Camera()
{
}

Camera::Camera(unsigned int programId, float cameraAngle, float width, float height, float Near, float Far, unsigned int vao, unsigned int buffer, unsigned int objectLoc, unsigned int modelMatLoc)
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
	glm::mat4 viewMat(1.0f);
	viewMatLoc = glGetUniformLocation(programId, "viewMat");
	glUniformMatrix4fv(viewMatLoc, 1, GL_TRUE, glm::value_ptr(viewMat));
	///////////////////////////////////////

	cameraPosLoc = glGetUniformLocation(programId, "cameraPos");

	this->modelMatLoc = modelMatLoc;
	this->vao = vao;
	this->buffer = buffer;
	this->objectLoc = objectLoc;

	//ball.LoadObject((char*)"./Models/ballBlender.obj");
	//ball.SetupDrawing(vao, buffer, 0, 1, 2);
}


Camera::~Camera()
{
}

void Camera::update(float angle, float camX, float camZ, int deltaTime)
{
	for (int i = 0; i < balls.size(); i++)
		balls[i]->DrawObject(vao, objectLoc, 3, modelMatLoc, glm::vec3(0, 5, 0), deltaTime);

	if (cameraMode == 0)
	{
		//Rotates the camera view up/down/left/right based on mouse input
		cameraFront.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		cameraFront.y = sin(glm::radians(pitch));
		cameraFront.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
		cameraFront = glm::normalize(cameraFront);

		float deltaSpeed = speed * deltaTime;

		//Control the camera
		if (specialKeys[GLUT_KEY_UP])
			cameraPos += deltaSpeed * cameraFront;
		else if (specialKeys[GLUT_KEY_DOWN])
			cameraPos -= deltaSpeed * cameraFront;

		if (specialKeys[GLUT_KEY_LEFT])
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * deltaSpeed;
		else if (specialKeys[GLUT_KEY_RIGHT])
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * deltaSpeed;

		cameraUp = glm::vec3(0, 1, 0);

		//Sets the viewMatrix: vec3(pos, target, up)
		view = glm::mat4(1.0f);
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	}
	else if (cameraMode == 1) //Over top view
	{
		//cameraPos = glm::vec3(0, 100, 0);
		cameraTarget = glm::vec3(5, 0, 0);

		cameraUp = glm::vec3(0, 1, 0);

		//Sets the viewMatrix: vec3(pos, target, up)
		view = glm::mat4(1.0f);
		view = glm::lookAt(glm::vec3(0, 100, 0), cameraTarget, cameraUp);
	}
	else if (cameraMode == 2) //side top view
	{
		//cameraPos = glm::vec3(0, 100, 0);
		cameraTarget = glm::vec3(5, 0, 0);

		cameraUp = glm::vec3(0, 1, 0);

		//Sets the viewMatrix: vec3(pos, target, up)
		view = glm::mat4(1.0f);
		view = glm::lookAt(glm::vec3(50, 70, 50), cameraTarget, cameraUp);
	}


	//Sends the view matrix to the vertex shader
	glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, value_ptr(view));

	//Sends the camera's position to the fragment shader
	glUniform3f(cameraPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);


}

void Camera::mouseControl(int key, int state, int x, int y)
{
	if (key == GLUT_LEFT_BUTTON)
	{
		Ball *ball = new Ball(glm::vec3(5, 5, 0), cameraFront);
		ball->LoadObject((char*)"./Models/ballBlender.obj");
		ball->SetupDrawing(vao, buffer, 0, 1, 2);
		
		balls.push_back(ball);

		generateBall = true;
		std::cout << "Spawning ball!" << std::endl;
	}
	if (key == GLUT_RIGHT_BUTTON)
		exit(0);
	glutPostRedisplay();
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
glm::mat4 Camera::getViewMat()
{
	return view;
}
