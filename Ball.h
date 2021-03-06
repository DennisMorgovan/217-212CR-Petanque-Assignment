#pragma once
#include "Object.h"
#include "CubeCollider.h"
#include "SphereCollider.h"
#include <iostream>

#define PI 3.1415

struct ColliderVertex
{
	glm::vec4 coords;
	glm::vec3 normals;
};

class Ball : public Object
{

private:
	float speed, angle, rotation;
	float delta;
	float velocityMagnitude;
	glm::vec3 velocity;

	float frictionFactor = 0.99f;
	//dragC = air drag coefficient; surface area = surface area of front of collider
	float colliderSize, airDrag, dragC, airDensity, surfaceArea, mass;
public:
	glm::mat4 modelMat;
	glm::vec3 heading; ///<Starting facing direction of the Hovercraft.
	float colRadius;

	Ball(glm::vec3 position, glm::vec3 cameraFront, unsigned int objectLoc, unsigned int modelMatLoc, float speed, float angle, float rotation, float mass);
	~Ball();

	float magnitude(glm::vec3 a);
	void collides(Ball* otherBall, float materialBounce = 1.0f); ///<Virtual function to notify this game object that a collision has happened can be overridden (optional).
	void collidesWall(CubeCollider* other);

	void SetupDrawing(unsigned int vao, unsigned int vbo, int locationVert, int locationTex, int locationNormals);
	void DrawObject(unsigned int vao, unsigned int objectLoc, unsigned int obj, unsigned int modelMatLoc, int deltaTime, int wind, int airDragOn);
};

