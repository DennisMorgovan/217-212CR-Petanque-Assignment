#pragma once
#include "Collider.h"

class SphereCollider : public Collider
{
public:
	float radius = 0.0f;

	SphereCollider(glm::vec3 centre, float radius, float materialBounce = 0);
	~SphereCollider();

	bool collidesWith(Collider* other); ///<pure virtual function. very collider must implement collidesWith, which calculates whethwer this collider collides with other.

	/* Every collider must implement the following pure virtual function or otherwise cause a compile error.*/
	float minX(); ///<Lowest x value for the collider. Must override in inherited classes.
	float maxX(); ///<Highest x value for the collider. Must override in inherited classes.
	float minY(); ///<Lowest y value for the collider. Must override in inherited classes.
	float maxY(); ///<Highest y value for the collider. Must override in inherited classes.
	float minZ(); ///<Lowest z value for the collider. Must override in inherited classes.
	float maxZ(); ///<Highest z value for the collider. Must override in inherited classes.
};

