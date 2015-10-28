//#include "Laboratory.h"
#include "Dollhouse.h"

namespace DCEngine {

	/* Blaine, write your tests here. */
	void DollHouse::GameObjects() {

		// Simple entity #1
		GameObjectPtr reiner = ConstructGameObject("Reiner");
		reiner->AddComponent(ComponentPtr(new Sprite(*reiner)));
		reiner->AddComponent(ComponentPtr(new Transform(*reiner)));
		reiner->AddComponent(ComponentPtr(new RigidBody(*reiner)));
		reiner->AddComponent(ComponentPtr(new BoxCollider(*reiner)));
		reiner->AddComponent(ComponentPtr(new DebugCollider(*reiner)));
		reiner->AddComponent(ComponentPtr(new DebugMoveController(*reiner)));
		reiner->AddComponent(ComponentPtr(new DebugReport(*reiner)));
		// BoxCollider properties
		reiner->getComponent<BoxCollider>()->Size = Real3(2, 2, 2);
		reiner->getComponent<BoxCollider>()->IsDrawingCollider = true;
		// Transform properties
		reiner->getComponent<Transform>()->Translation = Real3(-2, 1.0f, 0.f);
		reiner->getComponent<Transform>()->Scale = Real3(2.0f, 2.0f, 1.0f);
    reiner->getComponent<RigidBody>()->setGravity(true);
		// Sprite properties
		//reiner->getComponent<Sprite>()->setSpriteSource("angryeyes.png");
																	 // MoveController properties
		reiner->getComponent<Sprite>()->SpriteSource = "KnightAni1mation";
		reiner->getComponent<Sprite>()->AnimationActive = true;
		reiner->getComponent<Sprite>()->HaveAnimation = true;
		reiner->getComponent<Sprite>()->AnimationSpeed = 10;
		reiner->getComponent<DebugMoveController>()->MoveSpeed = 10;
		// DebugDraw properties
		reiner->getComponent<DebugReport>()->DrawType = DebugDrawType::Line;
		reiner->getComponent<DebugReport>()->Radius = 8.0f;
		reiner->getComponent<DebugReport>()->Offset = Real3(5.0f, 0.0f, 0.0f);
		//reiner->getComponent<DebugReport>()->Color = Real4(1.0f, 0.0f, 1.0f, 1.0f); // Purple line


   // Simple entity #2
		GameObjectPtr john = ConstructGameObject("Myres");
		john->AddComponent(ComponentPtr(new Sprite(*john)));
		john->AddComponent(ComponentPtr(new Transform(*john)));
		john->AddComponent(ComponentPtr(new RigidBody(*john)));
    john->getComponent<RigidBody>()->setGravity(true);
		john->AddComponent(ComponentPtr(new BoxCollider(*john)));
		john->AddComponent(ComponentPtr(new DebugCollider(*john)));
		//john->AddComponent(ComponentPtr(new DebugReport(*john)));
		// Change the properties of the components
		john->getComponent<Transform>()->Translation = Real3(5.0f, 1.0f, 0.0f);
		john->getComponent<Transform>()->Scale = Real3(4.0f, 4.0f, 1.0f);
		// Sprite properties
    //john->getComponent<Sprite>()->Color = Real4(1, 1, 1, 1); // Color: Blue
    john->getComponent<Sprite>()->SpriteSource = "MonkeyStand";
		//john->getComponent<Sprite>()->Color = Real4(0.0f, 0.0f, 1.0f, 1.0f); // Color: Blue
																			 // BoxCollider properties
		john->getComponent<BoxCollider>()->Size = Real3(4, 4, 4);
		john->getComponent<BoxCollider>()->IsDrawingCollider = true;


    /* Moonward Logo??? */
    GameObjectPtr boo = ConstructGameObject("Boo");
    boo->AddComponent(ComponentPtr(new Transform(*boo)));
    boo->AddComponent(ComponentPtr(new Sprite(*boo)));
    boo->AddComponent(ComponentPtr(new DebugReport(*boo)));
    // Transform properties
    boo->getComponent<Transform>()->Translation = Real3(40, 5, 0.0f);
    boo->getComponent<Transform>()->Scale = Real3(2.0f, 2.0f, 1.0f);
    boo->getComponent<Transform>()->Rotation = Real3(0.0f, 0.0f, 2.0f);
    // Sprite properties
    //chenObj->getComponent<Sprite>()->Color = Real4(1, 1, 0, 0.5); // Color: Blue
    boo->getComponent<Sprite>()->SpriteSource = "MonkeyStand";

		// Player
		//GameObjectPtr mariah = ConstructGameObject("Mariah");
		//mariah->AddComponent(ComponentPtr(new Sprite(*mariah)));
		//mariah->AddComponent(ComponentPtr(new Transform(*mariah)));
		//mariah->AddComponent(ComponentPtr(new RigidBody(*mariah)));
		//mariah->AddComponent(ComponentPtr(new BoxCollider(*mariah)));
		//mariah->AddComponent(ComponentPtr(new DebugCollider(*mariah)));
		//mariah->AddComponent(ComponentPtr(new DebugReport(*mariah)));
		//mariah->AddComponent(ComponentPtr(new PlayerController(*mariah)));
		//// Change the properties of the components
		//mariah->getComponent<Transform>()->Translation = Real3(4.0f, 10.0f, 0.0f);
		//mariah->getComponent<Transform>()->Scale = Real3(1.0f, 1.0f, 1.0f);
		//// Sprite properties
		//mariah->getComponent<Sprite>()->Color = Real4(1.0f, 0.7f, 0.3f, 1.0f);
		//// BoxCollider properties
		//mariah->getComponent<BoxCollider>()->Size = Real3(2, 2, 2);
		//mariah->getComponent<BoxCollider>()->IsDrawingCollider = true;
		//// Rigidbody properties
		//mariah->getComponent<RigidBody>()->setGravity(true);


		//// Ball
		//GameObjectPtr ball = ConstructGameObject("Basketball");
		//ball->AddComponent(ComponentPtr(new Sprite(*ball)));
		//ball->AddComponent(ComponentPtr(new Transform(*ball)));
		//ball->AddComponent(ComponentPtr(new RigidBody(*ball)));
		//ball->AddComponent(ComponentPtr(new BoxCollider(*ball)));
		//ball->AddComponent(ComponentPtr(new DebugCollider(*ball)));
		//ball->AddComponent(ComponentPtr(new DebugReport(*ball)));
		//ball->AddComponent(ComponentPtr(new BallController(*ball)));
		//// Change the properties of the components
		//ball->getComponent<Transform>()->Translation = Real3(0.0f, 10.0f, 0.0f);
		//ball->getComponent<Transform>()->Scale = Real3(1.0f, 1.0f, 1.0f);
		//// Sprite properties
		//ball->getComponent<Sprite>()->Color = Real4(0.0f, 0.7f, 0.3f, 1.0f);
		//// BoxCollider properties
		//ball->getComponent<BoxCollider>()->Size = Real3(2, 2, 2);
		//ball->getComponent<BoxCollider>()->IsDrawingCollider = true;
		//// Rigidbody properties
		//ball->getComponent<RigidBody>()->setGravity(true);



	}


}
