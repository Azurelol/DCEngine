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
		}
	}

	void HazardArea::OnCollisionEndedEvent(Events::CollisionEnded * event)
	{
		if (event->OtherObject->getComponent<PlayerController>())
		{
			TouchingPlayer = false;
			Timer = DamageInterval;
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
}