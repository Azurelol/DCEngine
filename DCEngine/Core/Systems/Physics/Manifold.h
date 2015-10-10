#pragma once
#include "../../ComponentsInclude.h"
#include "../../Objects/Object.h"

namespace DCEngine
{
	enum Collider
	{
		Circle = 0,
		Rectangle
	};
	
	
	class Manifold
	{
	public:
		Real3 CollisionCenter;
		GameObject* Object1;
		GameObject* Object2;

		Collider obj1;
		Collider obj2;
 
    Real3 v1;
		Real3 v2;

		Real3 ContactNormal;
		float Penetration;
		float Restitution;
		float FrictionCof;

		float SeperatingVelocity;
		float ContactImpulse;
		float CalculateSeparatingVelocity();


	};









}