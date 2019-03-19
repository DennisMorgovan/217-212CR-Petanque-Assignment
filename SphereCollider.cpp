#include "SphereCollider.h"

SphereCollider::SphereCollider(glm::vec3 centre, float radius, float materialBounce)
{
	this->colliderCentre = centre;
	this->radius = radius;
	this->materialBounce = materialBounce;
}


SphereCollider::~SphereCollider()
{
}

bool SphereCollider::collidesWith(Collider * other)
{
	return false;
}

float SphereCollider::minX()
{
	return 0.0f;
}

float SphereCollider::maxX()
{
	return 0.0f;
}

float SphereCollider::minY()
{
	return 0.0f;
}

float SphereCollider::maxY()
{
	return 0.0f;
}

float SphereCollider::minZ()
{
	return 0.0f;
}

float SphereCollider::maxZ()
{
	return 0.0f;
}
