#include "EnemyController.h"
#include "../../CoreComponents.h"

namespace DCEngine {

	void EnemyController::Initialize()
	{
		auto gameObj = dynamic_cast<GameObject*>(Owner());
		Connect(gameObj, Events::CollisionStarted, EnemyController::OnCollisionStartedEvent);
		Connect(gameObj, Events::CollisionEnded, EnemyController::OnCollisionEndedEvent);
		Connect(SpaceRef, Events::LogicUpdate, EnemyController::OnLogicUpdateEvent);
		TransformRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Transform>(); // ew
		RigidBodyRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<RigidBody>();
		PlayerRef = SpaceRef->FindObjectByName("Mariah");
	}

	void EnemyController::Serialize(Json::Value & root)
	{
	}

	void EnemyController::Deserialize(Json::Value & root)
	{
	}

	void EnemyController::OnCollisionStartedEvent(Events::CollisionStarted * event)
	{
		if (event->OtherObject->getComponent<BallController>())
		{
			//TransformRef->Translation.x = 1000;
			//TransformRef->Translation.y = 1000;
		}
	}

	void EnemyController::OnCollisionEndedEvent(Events::CollisionEnded * event)
	{
	}

	void EnemyController::OnLogicUpdateEvent(Events::LogicUpdate * event)
	{
		Timer += event->Dt;
		if (Timer > JumpInterval)
		{
			Timer = 0;
			auto direction = rand() % 11 - 5; //-5 to 5
			RigidBodyRef->setVelocity(RigidBodyRef->getVelocity() + Vec3(direction * MoveSpeed, JumpPower, 0));
		}
	}



	void EnemyController::PrintTranslation()
	{
		DCTrace << Owner()->Name() << "::Transform.Translation(" << TransformRef->Translation.x << ", " << TransformRef->Translation.y << ", " << TransformRef->Translation.z << ")\n";
	}

	void EnemyController::PrintVelocity()
	{
		Vec3 vel = RigidBodyRef->getVelocity();
		//DCTrace << Owner()->Name() << "::RigidBody.Velocity(" << vel.x << ", " << vel.y<< ", " << vel.z << ")\n";
	}
}