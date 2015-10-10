#pragma once
//#ifdef Collision.h
//#define Collision.h
#include "../System.h"
#include "Manifold.h"

namespace DCEngine 
{

	bool BoxtoBox(GameObject *obj1, GameObject *obj2);
	bool CircletoBox(GameObject *obj1, GameObject *obj2);
	bool BoxtoCircle(GameObject *obj1, GameObject *obj2);
	bool CircletoCircle(GameObject *obj1, GameObject *obj2);

}

//#endif