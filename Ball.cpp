#include "Ball.h"


Ball::Ball(glm::vec3 position, glm::vec3 cameraFront, unsigned int objectLoc, unsigned int modelMatLoc, float speed, float angle, float rotation, float mass) : Object(position)
{
	colRadius = 1.0f;
	Ball::collider = new SphereCollider(position, colRadius, 1);
	this->collider->objectType = 0;

	velocity = glm::vec3(0.0f, 0.0f, 0.0f);

	surfaceArea = 3.1415 * colRadius * colRadius; //Side sphere area = pi * r^2
	dragC = 0.1; //Coefficient of a smooth sphere

	this->heading = cameraFront;
	this->speed = speed;
	this->angle = angle;
	this->rotation = rotation;
	this->position = position; //Resetting the position before instantiating ball 
	this->mass = mass;
	this->airDensity = 1.225f;

	this->modelMat = glm::mat4(1.0f);
	this->modelMat = glm::translate(modelMat, position);
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, value_ptr(modelMat));

	velocityMagnitude = 0.0f;
	airDrag = 1.0f;

	//std::cout << position.x << " " << position.y << " " << position.z << std::endl;
}


Ball::~Ball()
{
	delete Ball::collider;
	Ball::collider = NULL;
}


float Ball::magnitude(glm::vec3 a)
{
	return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}

void Ball::collides(Ball* otherBall, float materialBounce)
{
	glm::vec3 velocity1, velocity2;

	glm::vec3 collisionNormal = glm::normalize(this->position - otherBall->position); //Obtaining the normal of the collision plane (vector connecting two collision points)
	glm::vec3 collisionDirection(-collisionNormal.y, collisionNormal.x, 0.0f); //Direction of the normal

	//Decomposing the balls' velocities into two components: parallel and orthogonal
	glm::vec3 velocity1Parallel = glm::dot(collisionNormal, this->velocity) * collisionNormal;
	glm::vec3 velocity1Ortho = glm::dot(collisionDirection, this->velocity) * collisionDirection;
	glm::vec3 velocity2Parallel = glm::dot(collisionNormal, otherBall->velocity) * collisionNormal;
	glm::vec3 velocity2Ortho = glm::dot(collisionDirection, otherBall->velocity) * collisionDirection;

	float totalMass = this->mass + otherBall->mass;
	glm::vec3 velocity1ParallelNew = (velocity1Parallel * (this->mass - otherBall->mass) + 2 * otherBall->mass * velocity2Parallel) / totalMass;
	glm::vec3 velocity2ParallelNew = (velocity2Parallel * (otherBall->mass - this->mass) + 2 * this->mass * velocity1Parallel) / totalMass;

	velocity1Parallel = velocity1ParallelNew;
	velocity2Parallel = velocity2ParallelNew;

		this->velocity = velocity1Parallel + velocity1Ortho;
		otherBall->velocity = velocity2Parallel + velocity2Ortho;
}

void Ball::collidesWall(CubeCollider* other)
{
	this->heading *= -1;
	this->velocity *= -1;

	std::cout << "Collision wall!" << std::endl;
}

void Ball::SetupDrawing(unsigned int vao, unsigned int vbo, int locationVert, int locationTex, int locationNormals)
{
	//Binding the vertex array and the buffer objects
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//Setting the data
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexNumber, finalVertices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(locationVert, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(locationVert);
	glVertexAttribPointer(locationTex, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(sizeof(float) * finalVerticesVertexNumber));
	glEnableVertexAttribArray(locationTex);
	glVertexAttribPointer(locationNormals, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(sizeof(float) * finalVerticesNormalsNumber));
	glEnableVertexAttribArray(locationNormals);
}

void Ball::DrawObject(unsigned int vao, unsigned int objectLoc, unsigned int obj, unsigned int modelMatLoc, int deltaTime, int wind, int airDragOn)
{
	if(airDragOn == 1) //Applies air drag, reducing it by 1000 since it is too strong
		this->heading.x -= airDrag / 1000.0f;

	if (wind == 1) //Wind force, applied on the z axis of the ball
	{
		this->heading.z += 0.005f;
		if (velocityMagnitude <= 0.55f)
		{
			speed = 0.0f;
			rotation = 0.0f;
		}
	}

	if (rotation) //Applies curved path to the ball
	{
		this->heading.x += cos((PI / 180.0) * rotation)* 8.5 / 1000.0f;
		this->heading.z += sin((PI / 180.0) * rotation)* 8.5 / 1000.0f;
	}

	//Movement calculations (velocity, friction and position update)
	this->delta = deltaTime / 1000.0f;

	this->position += this->velocity * this->delta;
	this->velocity = (this->heading) * (this->speed);

	this->speed *= frictionFactor;
	this->rotation *= frictionFactor;

	velocityMagnitude = magnitude(velocity);


	//Calculating air drag
	airDrag = (airDensity * velocityMagnitude * velocityMagnitude * surfaceArea * dragC) / 2;

	//Setting the speed of the ball to 0 after a certain amount, this is to prevent the ball from rolling uncontrollably.
	if (velocityMagnitude <= 0.35f)
	{
		speed = 0.0f;
		rotation = 0.0f;
	}

	collider->SetCenter(this->position); //Updates collider centre



	//Drawing routine
	glUniform1ui(objectLoc, obj);
	glBindVertexArray(vao);
		modelMat = glm::mat4(1.0f);
		modelMat = glm::translate(modelMat, position);
			if (velocityMagnitude) //Applies ball roll; directly proportionate to the ball's speed (the slower the ball, the slower the rotation)
				modelMat = glm::rotate(modelMat, velocityMagnitude, glm::vec3(0, 0, 1));
	
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, value_ptr(modelMat));
	glDrawArrays(GL_TRIANGLES, 0, faceNumber * 3);
}