#pragma once
//#ifdef Collision.h
//#define Collision.h
#include "../System.h"
#include "Manifold.h"

namespace DCEngine
{

	bool BoxtoBox(GameObject *obj1, GameObject *obj2, Manifold &result);
	bool CircletoBox(GameObject * objrect, GameObject * objcircle, Manifold &result);
	void ProjectOnTo(glm::vec3 vec, glm::vec3 Axis, glm::vec3 &result);
	bool BoxtoCircle(GameObject *obj1, GameObject *obj2, Manifold &result);
	bool CircletoCircle(GameObject *obj1, GameObject *obj2, Manifold &result);

}

//#endif