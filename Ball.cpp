#include "Ball.h"


Ball::Ball(glm::vec3 position, glm::vec3 cameraFront) : Object(position)
{
	colliderSize = 5;
	Ball::collider = new CubeCollider(position, colliderSize, colliderSize, colliderSize, 1);
	this->collider->objectType = 0;

	currentSpeed.x = 0.0;
	currentSpeed.y = 0.0;
	currentSpeed.z = 0.0;

	maxSpeed.x = 1.0;
	maxSpeed.y = 1.0;
	maxSpeed.z = 1.0;

	velocity.x = 0.0;
	velocity.y = 0.0;
	velocity.z = 0.0;

	surfaceArea = colliderSize * colliderSize;
	dragC = 0.1;

	this->heading = cameraFront;
}


Ball::~Ball()
{
	//delete Ball::collider;
	//Ball::collider = NULL;
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

void Ball::DrawObject(unsigned int vao, unsigned int objectLoc, unsigned int obj, unsigned int modelMatLoc, glm::vec3 position, int deltaTime)
{
	//float turningSpeed = TURNING_SPEED * (deltaTime / 1000.0); //turning speed (degrees/sec) * deltaTime in sec = turning speed over delta time
	//airDrag = (p * currentspeedMagnitude * currentspeedMagnitude * surfaceArea * dragC) / 2;

	//position += heading;
	position.x += 1;

	/*if (collidesWithGround)
		groundFriction = 0.9;
	else
		groundFriction = 1;

	this->acceleration = (this->heading * (accelFactor - airDrag)); //Computing the acceleration

	glm::vec3 velDelta = velocity * (deltaTime / 1000.0f);

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

	//Debugging
	//std:: cout << "currentspeedMagnitude: " << currentspeedMagnitude << " maxspeedMagnitude: " << maxspeedMagnitude << " acceleration: " << acceleration.x << " " << acceleration.y << " " << acceleration.z << " " << std::endl;
	//std::cout << "Velocity: " <<glm::to_string(velocity) << std::endl;

	glUniform1ui(objectLoc, obj);
	glBindVertexArray(vao);

	glm::mat4 modelMat = glm::mat4(1.0f);
	modelMat = glm::translate(modelMat, position);
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, value_ptr(modelMat));

	glDrawArrays(GL_TRIANGLES, 0, faceNumber * 3);
}
