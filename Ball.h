#pragma once
#include "Object.h"
#include "CubeCollider.h"
#include <iostream>

class Ball : public Object
{

private:
	float speed, angle, rotation;
	float delta;

	float accelInput = 0, accelFactor = 0.3;
	glm::vec3 acceleration;
	glm::vec3 velocity;

	float frictionFactor = 0.5f;
	//dragC = air drag coefficient; surface area = surface area of front of collider
	float colliderSize, airDrag, dragC, surfaceArea;
public:
	glm::mat4 modelMat;

	//float angle; //Angle of hovercraft movement
	glm::vec3 heading; ///<Starting facing direction of the Hovercraft.
	glm::vec3 newHeading;
	float rotationAngle, pitchAngle = 0.0; ///<Rotation and pitch angles for our Hovercraft
	bool collidesWithGround;
	float groundFriction = 1;
	float velocityMagnitude, currentspeedMagnitude, maxspeedMagnitude;//Vector magnitudes

	//float speed = 0.1f;

	Ball(glm::vec3 position, glm::vec3 cameraFront, unsigned int objectLoc, unsigned int modelMatLoc, float speed, float angle, float rotation);
	~Ball();

	void update(int deltaTime, unsigned int programId, unsigned int modelMatLoc);
	float magnitude(glm::vec3 a);
	void collides(Collider* other, float materialBounce = 1.0f); ///<Virtual function to notify this game object that a collision has happened can be overridden (optional).
	void collidesRacetrack(Collider* other);

	void DrawObject(unsigned int vao, unsigned int objectLoc, unsigned int obj, unsigned int modelMatLoc, int deltaTime);
};

