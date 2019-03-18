#include "CubeCollider.h"
#include <iostream>

CubeCollider::CubeCollider(glm::vec3 centre, float cubeWidth, float cubeHeight, float cubeLength, float materialBounce)
{
	this->colliderCentre = centre;
	this->width = cubeWidth;
	this->height = cubeHeight;
	this->length = cubeLength;
	this->materialBounce = materialBounce;
}

CubeCollider::~CubeCollider()
{
}

bool CubeCollider::collidesWith(Collider * other)
{
	if (other == NULL) {
		return false;
	}

	bool overlapX = other->minX() <= this->maxX() && other->maxX() >= this->minX();
	bool overlapY = other->minY() <= this->maxY() && other->maxY() >= this->minY();
	bool overlapZ = other->minZ() <= this->maxZ() && other->maxZ() >= this->minZ();
	return overlapX && overlapY && overlapZ;
}

float CubeCollider::minX()
{
	return this->colliderCentre.x - this->width / 2.0f;
}

float CubeCollider::maxX()
{
	return this->colliderCentre.x + this->width / 2.0f;
}

float CubeCollider::minY()
{
	return this->colliderCentre.y - this->height / 2.0f;
}

float CubeCollider::maxY()
{
	return this->colliderCentre.y + this->height / 2.0f;
}

float CubeCollider::minZ()
{
	return this->colliderCentre.z - this->length / 2.0f;
}

float CubeCollider::maxZ()
{
	return this->colliderCentre.z + this->length / 2.0f;
}

