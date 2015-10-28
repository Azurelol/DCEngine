#include "EnemyController.h"
#include "../../CoreComponents.h"

namespace DCEngine {

	void EnemyController::Initialize()
	{
		auto gameObj = dynamic_cast<GameObject*>(Owner());
		//Connect(Daisy->getMouse(), Events::MouseDown, EnemyController::OnMouseDownEvent);
		//Connect(Daisy->getMouse(), Events::MouseUp, EnemyController::OnMouseUpEvent);
		//Connect(Daisy->getKeyboard(), Events::KeyDown, EnemyController::OnKeyDownEvent);
		Connect(gameObj, Events::CollisionStarted, EnemyController::OnCollisionStartedEvent);
		Connect(gameObj, Events::CollisionEnded, EnemyController::OnCollisionEndedEvent);
		Connect(SpaceRef, Events::LogicUpdate, EnemyController::OnLogicUpdateEvent);
		TransformRef = dynamic_cast<GameObject*>(owner_)->getComponent<Transform>(); // ew
		RigidBodyRef = dynamic_cast<GameObject*>(owner_)->getComponent<RigidBody>();
		
	}

	void EnemyController::Serialize(Json::Value & root)
	{
	}

	void EnemyController::Deserialize(Json::Value & root)
	{
	}

	void EnemyController::OnMouseDownEvent(Events::MouseDown * event)
	{

	}
	void EnemyController::OnMouseUpEvent(Events::MouseUp * event)
	{

	}

	void EnemyController::OnKeyDownEvent(Events::KeyDown* event)
	{

	}

	void EnemyController::OnCollisionStartedEvent(Events::CollisionStarted * event)
	{

	}

	void EnemyController::OnCollisionEndedEvent(Events::CollisionEnded * event)
	{
	}

	void EnemyController::OnLogicUpdateEvent(Events::LogicUpdate * event)
	{
		RigidBodyRef->setVelocity(RigidBodyRef->getVelocity() + Real3(-MoveSpeed, 0, 0));
	}



	void EnemyController::PrintTranslation()
	{
		trace << Owner()->Name() << "::Transform.Translation(" << TransformRef->Translation.x << ", " << TransformRef->Translation.y << ", " << TransformRef->Translation.z << ")\n";
	}

	void EnemyController::PrintVelocity()
	{
		Real3 vel = RigidBodyRef->getVelocity();
		//trace << Owner()->Name() << "::RigidBody.Velocity(" << vel.x << ", " << vel.y<< ", " << vel.z << ")\n";
	}
}