#include "SphereCollider.h"
#include <iostream>
#include "glm/gtx/string_cast.hpp"
SphereCollider::SphereCollider(glm::vec3 centre, float radius, float materialBounce)
{
	this->colliderCentre = centre;
	this->radius = radius;
	this->materialBounce = materialBounce;
}


SphereCollider::~SphereCollider()
{
}

//Collision between sphere and box
bool SphereCollider::collidesWith(CubeCollider * other)
{
	if (other == NULL) {
		return false;
	}

	glm::vec3 distance = this->colliderCentre - other->GetCenter();
	float length = distance.x * distance.x + distance.y * distance.y + distance.z * distance.z; //Length of the distance vector

	distance = glm::normalize(distance); //Normalising the distance vector, to find the vector that points from the sphere center to the box center

	if (distance.x >= distance.y && distance.x >= distance.z)
		distance /= distance.x;
	else if (distance.y >= distance.x && distance.y >= distance.z)
		distance /= distance.y;
	else
		distance /= distance.z;

	distance.x *= other->width / 2.0f;
	distance.y *= other->height / 2.0f;
	distance.z *= other->length / 2.0f;

	float finalLength = distance.x * distance.x + distance.y * distance.y + distance.z * distance.z;

	return length <= (this->radius + finalLength);
}

//Sphere-sphere collision
bool SphereCollider::collidesWithSphere(SphereCollider * other)
{
	glm::vec3 distance = this->colliderCentre - other->colliderCentre; //Distance between the two spheres
	float length = glm::length(distance);// distance.x * distance.x + distance.y * distance.y + distance.z * distance.z; //Length of the distance vector
	//float length = sqrt((other->colliderCentre.x - this->colliderCentre.x) * (other->colliderCentre.x - this->colliderCentre.x) +
	//					(other->colliderCentre.y - this->colliderCentre.y) * (other->colliderCentre.y - this->colliderCentre.y) +
	//					(other->colliderCentre.z - this->colliderCentre.z) * (other->colliderCentre.z - this->colliderCentre.z));
	float sumRadii = this->radius + other->radius; //Sum of radii of the two spheres

	//std::cout << "Ball1: " << glm::to_string(this->colliderCentre) << std::endl << "Ball2: " << glm::to_string(other->colliderCentre) << std::endl << std::endl;

	//std::cout << "Distance: " << /*glm::to_string(distance)*/ length << "\tsumRadii: " << sumRadii << std::endl;
	//if (length < 0.3)
		//return false;
	//else
	return length <= sumRadii;
}

float SphereCollider::minX()
{
	return this->colliderCentre.x - this->radius;
}

float SphereCollider::maxX()
{
	return this->colliderCentre.x + this->radius;
}

float SphereCollider::minY()
{
	return this->colliderCentre.y - this->radius;
}

float SphereCollider::maxY()
{
	return this->colliderCentre.y + this->radius;
}

float SphereCollider::minZ()
{
	return this->colliderCentre.z - this->radius;
}

float SphereCollider::maxZ()
{
	return this->colliderCentre.z + this->radius;
}
