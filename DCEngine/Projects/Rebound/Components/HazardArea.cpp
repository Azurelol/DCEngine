/******************************************************************************/
/*!
@file   HazardArea.cpp
@author Connor Tilley
@par    email: connor.tilley\@digipen.edu
@date   11/23/2015
@brief  Makes an area a hazard, which will damage the player on contact.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/

#include "HazardArea.h"
#include "../../CoreComponents.h"

namespace DCEngine {

	void HazardArea::Initialize()
	{
		auto gameObj = dynamic_cast<GameObject*>(Owner());
		Connect(gameObj, Events::CollisionStarted, HazardArea::OnCollisionStartedEvent);
		Connect(gameObj, Events::CollisionEnded, HazardArea::OnCollisionEndedEvent);
		Connect(SpaceRef, Events::LogicUpdate, HazardArea::OnLogicUpdateEvent);
		TransformRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Transform>(); // ew
		RigidBodyRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<RigidBody>();
		PlayerRef = SpaceRef->FindObjectByName("Mariah");
	}

	void HazardArea::Serialize(Json::Value & root)
	{
	}

	void HazardArea::Deserialize(Json::Value & root)
	{
	}

	void HazardArea::OnCollisionStartedEvent(Events::CollisionStarted * event)
	{
		if (event->OtherObject->getComponent<PlayerController>())
		{
			TouchingPlayer = true;
			PlayerRef->getComponent<RigidBody>()->setGravity(0.5);
			auto vel = PlayerRef->getComponent<RigidBody>()->getVelocity();
			PlayerRef->getComponent<RigidBody>()->setVelocity(Vec3(vel.x / 2, vel.y / 2, 0));
			DCTrace << "HazardArea::OnCollisionStartedEvent \n";
		}
	}

	void HazardArea::OnCollisionEndedEvent(Events::CollisionEnded * event)
	{
		if (event->OtherObject->getComponent<PlayerController>())
		{
			TouchingPlayer = false;
			Timer = DamageInterval;
			PlayerRef->getComponent<RigidBody>()->setGravity(1);
			DCTrace << "HazardArea::OnCollisionEndedEvent \n";
		}
	}

	void HazardArea::OnLogicUpdateEvent(Events::LogicUpdate * event)
	{
		if(TouchingPlayer)
		{
			Timer += event->Dt;
			if (Timer > DamageInterval)
			{
				Timer = 0;
				PlayerRef->getComponent<PlayerController>()->TakeDamage(Damage);
			}
		}
	}



	void HazardArea::PrintTranslation()
	{
		DCTrace << Owner()->Name() << "::Transform.Translation(" << TransformRef->Translation.x << ", " << TransformRef->Translation.y << ", " << TransformRef->Translation.z << ")\n";
	}

	void HazardArea::PrintVelocity()
	{
		Vec3 vel = RigidBodyRef->getVelocity();
		//DCTrace << Owner()->Name() << "::RigidBody.Velocity(" << vel.x << ", " << vel.y<< ", " << vel.z << ")\n";
	}

	/**************************************************************************/
	/*!
	@brief Provides the definition of this class to Zilch.
	@note This can only go in the translational unit (.cpp)
	*/
	/**************************************************************************/
	#if(DCE_USE_ZILCH_INTERNAL_BINDING)
		ZilchDefineType(HazardArea, "HazardArea", DCEngineCore, builder, type) {
			// Constructor / Destructor
			ZilchBindConstructor(builder, type, HazardArea, "owner", Entity&);
			ZilchBindDestructor(builder, type, HazardArea);
			// Properties
			ZilchBindProperty(builder, type, &Transform::getTranslation, &Transform::setTranslation, "Translation");
			ZilchBindProperty(builder, type, &Transform::getRotation, &Transform::setRotation, "Rotation");
			ZilchBindProperty(builder, type, &Transform::getScale, &Transform::setScale, "Scale");
		}
	#endif
}