#include "Ball.h"


Ball::Ball(glm::vec3 position, glm::vec3 cameraFront, unsigned int objectLoc, unsigned int modelMatLoc, float speed, float angle, float rotation) : Object(position)
{
	colliderSize = 5;
	Ball::collider = new CubeCollider(position, colliderSize, colliderSize, colliderSize, 1);
	this->collider->objectType = 0;

	velocity = glm::vec3(0.0f, 0.0f, 0.0f);

	surfaceArea = colliderSize * colliderSize;
	dragC = 0.1;

	this->heading = cameraFront;
	this->speed = speed;
	this->angle = angle;
	this->rotation = rotation;

	this->modelMat = glm::mat4(1.0f);
	this->modelMat = glm::translate(modelMat, position + glm::vec3(5, -3, 0));
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, value_ptr(modelMat));
}


Ball::~Ball()
{
	delete Ball::collider;
	Ball::collider = NULL;
}

void Ball::update(int deltaTime, unsigned int programId, unsigned int modelMatLoc)
{

}

float Ball::magnitude(glm::vec3 a)
{
	return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}

void Ball::collides(Collider * other, float materialBounce)
{
}

void Ball::collidesRacetrack(Collider * other)
{
}

void Ball::DrawObject(unsigned int vao, unsigned int objectLoc, unsigned int obj, unsigned int modelMatLoc, int deltaTime)
{
	//airDrag = (p * currentspeedMagnitude * currentspeedMagnitude * surfaceArea * dragC) / 2;

	//this->position += this->heading;
	//this->position *= speed * deltaTime / 1000.0f;

	this->delta = deltaTime / 1000.0f;
	this->velocity = this->heading * this->speed;

	this->position += (this->velocity * this->delta);
	std::cout << position.x << " " << position.y << " " << position.z << std::endl;

	this->velocity *= frictionFactor;

	/*if (collidesWithGround)
		groundFriction = 0.9;
	else
		groundFriction = 1;

	this->acceleration = (this->heading * (accelFactor - airDrag)); //Computing the acceleration

	velocityMagnitude = magnitude(velDelta);

	if (velocityMagnitude != 0) //If magnitude != 0, make current speed equal to normalized velocity vector
		currentSpeed = velDelta;
	else
		currentSpeed = glm::vec3(0, 0, 0);

	currentspeedMagnitude = magnitude(currentSpeed);
	maxspeedMagnitude = magnitude(maxSpeed);

	if (currentspeedMagnitude < maxspeedMagnitude)
		velocity += acceleration * groundFriction;

	this->position += velDelta;*/

	//velocity *= frictionFactor;
	//Velocity and current speed stay constant after this amount, so I am brute forcing by making them equal to zero
	//if (currentspeedMagnitude <= 0.153f)
	//{
	//	velocity = glm::vec3(0, 0, 0);
	//	currentspeedMagnitude = 0;
	//}

	//this->heading = glm::rotate(this->heading, glm::radians(rotationAngle), glm::vec3(0.0, 1.0, 0.0));

	collider->SetCenter(this->position);

	glUniform1ui(objectLoc, obj);
	glBindVertexArray(vao);

	modelMat = glm::translate(modelMat, position);
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, value_ptr(modelMat));

	glDrawArrays(GL_TRIANGLES, 0, faceNumber * 3);
}
