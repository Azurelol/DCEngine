#pragma once
#include "../../ComponentsInclude.h"
#include "../../Objects/Object.h"

namespace DCEngine
{
	class Manifold
	{
	public:
		Real3 CollisionCenter;
		float Penetration;
		GameObject* Object1;
		GameObject* Object2;

	  
       
        Real3 v1;
		Real3 v2;


	};









}