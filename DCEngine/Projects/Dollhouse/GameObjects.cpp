//#include "Laboratory.h"
#include "Dollhouse.h"

namespace DCEngine {

	/* Blaine, write your tests here. */
	void DollHouse::GameObjects() {




		// Player
		GameObjectPtr mariah = ConstructGameObject("Mariah");
		mariah->AddComponent(ComponentPtr(new Sprite(*mariah)));
		mariah->AddComponent(ComponentPtr(new Transform(*mariah)));
		mariah->AddComponent(ComponentPtr(new RigidBody(*mariah)));
		mariah->AddComponent(ComponentPtr(new BoxCollider(*mariah)));
		mariah->AddComponent(ComponentPtr(new DebugCollider(*mariah)));
		mariah->AddComponent(ComponentPtr(new DebugReport(*mariah)));
		mariah->AddComponent(ComponentPtr(new PlayerController(*mariah)));
		// Change the properties of the components
		mariah->getComponent<Transform>()->Translation = Real3(4.0f, 10.0f, 0.0f);
		mariah->getComponent<Transform>()->Scale = Real3(1.0f, 1.0f, 1.0f);
		// Sprite properties
		mariah->getComponent<Sprite>()->Color = Real4(1.0f, 0.7f, 0.3f, 1.0f);
		// BoxCollider properties
		mariah->getComponent<BoxCollider>()->Size = Real3(2, 2, 2);
		mariah->getComponent<BoxCollider>()->IsDrawingCollider = true;
		// Rigidbody properties
		mariah->getComponent<RigidBody>()->setGravity(true);

		// Enemy
		GameObjectPtr enemy = ConstructGameObject("enemy");
		enemy->AddComponent(ComponentPtr(new Sprite(*enemy)));
		enemy->AddComponent(ComponentPtr(new Transform(*enemy)));
		enemy->AddComponent(ComponentPtr(new RigidBody(*enemy)));
		enemy->AddComponent(ComponentPtr(new BoxCollider(*enemy)));
		enemy->AddComponent(ComponentPtr(new DebugCollider(*enemy)));
		enemy->AddComponent(ComponentPtr(new DebugReport(*enemy)));
		enemy->AddComponent(ComponentPtr(new EnemyController(*enemy)));
		// Change the properties of the components
		enemy->getComponent<Transform>()->Translation = Real3(14.0f, 0.0f, 0.0f);
		enemy->getComponent<Transform>()->Scale = Real3(1.0f, 1.0f, 1.0f);
		// Sprite properties
		enemy->getComponent<Sprite>()->Color = Real4(1.0f, 0.0f, 1.0f, 1.0f);
		// BoxCollider properties
		enemy->getComponent<BoxCollider>()->Size = Real3(2, 2, 2);
		enemy->getComponent<BoxCollider>()->IsDrawingCollider = true;
		// Rigidbody properties
		enemy->getComponent<RigidBody>()->setGravity(true);


		// Ball
		GameObjectPtr ball = ConstructGameObject("Basketball");
		ball->AddComponent(ComponentPtr(new Sprite(*ball)));
		ball->AddComponent(ComponentPtr(new Transform(*ball)));
		ball->AddComponent(ComponentPtr(new RigidBody(*ball)));
		ball->AddComponent(ComponentPtr(new BoxCollider(*ball)));
		ball->AddComponent(ComponentPtr(new DebugCollider(*ball)));
		ball->AddComponent(ComponentPtr(new DebugReport(*ball)));
		ball->AddComponent(ComponentPtr(new BallController(*ball)));
		// Change the properties of the components
		ball->getComponent<Transform>()->Translation = Real3(0.0f, 10.0f, 0.0f);
		ball->getComponent<Transform>()->Scale = Real3(0.7f, 0.7f, 1.0f);
		// Sprite properties
		ball->getComponent<Sprite>()->Color = Real4(0.0f, 0.7f, 0.3f, 1.0f);
		// BoxCollider properties
		ball->getComponent<BoxCollider>()->Size = Real3(2, 2, 2);
		ball->getComponent<BoxCollider>()->IsDrawingCollider = true;
		// Rigidbody properties
		ball->getComponent<RigidBody>()->setGravity(true);



	}


}
