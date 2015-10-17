//#include "Laboratory.h"
#include "Dollhouse.h"

namespace DCEngine {

  /* Blaine, write your tests here. */
  void DollHouse::LaboratoryTesting() {

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
    reiner->getComponent<Transform>()->Translation = Real3(-2, 0.0f, 0.0f);
    reiner->getComponent<Transform>()->Scale = Real3(1.0f, 1.0f, 1.0f);
    // Sprite properties
    //reiner->getComponent<Sprite>()->setSpriteSource("angryeyes.png");
    reiner->getComponent<Sprite>()->Color = Real4(1, 0.5, 0, 1); // Color: Orange
    // MoveController properties
    reiner->getComponent<DebugMoveController>()->MoveSpeed = 0.5f;
    // DebugDraw properties
    reiner->getComponent<DebugReport>()->DrawType = DebugDrawType::Line;
    reiner->getComponent<DebugReport>()->Radius = 8.0f;
    reiner->getComponent<DebugReport>()->Offset = Real3(5.0f, 0.0f, 0.0f);
    reiner->getComponent<DebugReport>()->Color = Real4(1.0f, 0.0f, 1.0f, 1.0f); // Purple line


    // Simple entity #2
    GameObjectPtr john = ConstructGameObject("Myres");
    john->AddComponent(ComponentPtr(new Sprite(*john)));
    john->AddComponent(ComponentPtr(new Transform(*john)));
    john->AddComponent(ComponentPtr(new RigidBody(*john)));
    john->AddComponent(ComponentPtr(new BoxCollider(*john)));
    john->AddComponent(ComponentPtr(new DebugCollider(*john)));
    john->AddComponent(ComponentPtr(new DebugReport(*john)));
    // Change the properties of the components
    john->getComponent<Transform>()->Translation = Real3(2.0f, 0.0f, 1.0f);
    john->getComponent<Transform>()->Scale = Real3(1.0f, 1.0f, 1.0f);
    // Sprite properties
    john->getComponent<Sprite>()->Color = Real4(0.0f, 0.0f, 1.0f, 1.0f); // Color: Blue
    // BoxCollider properties
    john->getComponent<BoxCollider>()->Size = Real3(4, 4, 4);
    john->getComponent<BoxCollider>()->IsDrawingCollider = true;


	// Simple entity #3
	GameObjectPtr ball = ConstructGameObject("Basketball");
	ball->AddComponent(ComponentPtr(new Sprite(*ball)));
	ball->AddComponent(ComponentPtr(new Transform(*ball)));
	ball->AddComponent(ComponentPtr(new RigidBody(*ball)));
	ball->AddComponent(ComponentPtr(new BoxCollider(*ball)));
	ball->AddComponent(ComponentPtr(new DebugCollider(*ball)));
	ball->AddComponent(ComponentPtr(new DebugReport(*ball)));
	ball->AddComponent(ComponentPtr(new BallController(*ball)));
	// Change the properties of the components
	ball->getComponent<Transform>()->Translation = Real3(0.0f, 10.0f, 1.0f);
	ball->getComponent<Transform>()->Scale = Real3(1.0f, 1.0f, 1.0f);
	// Sprite properties
	ball->getComponent<Sprite>()->Color = Real4(0.0f, 0.7f, 0.3f, 1.0f); 
																		 // BoxCollider properties
	ball->getComponent<BoxCollider>()->Size = Real3(2, 2, 2);
	ball->getComponent<BoxCollider>()->IsDrawingCollider = true;

	///////////////////////////////////////////////////////
	GameObjectPtr floor = ConstructGameObject("floor");
	floor->AddComponent(ComponentPtr(new Sprite(*floor)));
	floor->AddComponent(ComponentPtr(new Transform(*floor)));
	floor->AddComponent(ComponentPtr(new RigidBody(*floor)));
	floor->AddComponent(ComponentPtr(new BoxCollider(*floor)));
	// Change the properties of the components
	floor->getComponent<Transform>()->Translation = Real3(0.0f, -10.0f, 1.0f);
	floor->getComponent<Transform>()->Scale = Real3(100.0f, 1.0f, 1.0f);
	// Sprite properties
	floor->getComponent<Sprite>()->Color = Real4(0.0f, 0.7f, 0.3f, 1.0f);
	// BoxCollider properties
	floor->getComponent<BoxCollider>()->Size = Real3(100, 1, 2);
	floor->getComponent<BoxCollider>()->IsDrawingCollider = true;
	//rb prop
	floor->getComponent<RigidBody>()->setMass(0);
	floor->getComponent<RigidBody>()->DynamicState = DynamicStateType::Static;
	floor->getComponent<RigidBody>()->setRestitution(0);


	GameObjectPtr mariah = ConstructGameObject("mariah");
	mariah->AddComponent(ComponentPtr(new Sprite(*mariah)));
	mariah->AddComponent(ComponentPtr(new Transform(*mariah)));
	mariah->AddComponent(ComponentPtr(new RigidBody(*mariah)));
	mariah->AddComponent(ComponentPtr(new BoxCollider(*mariah)));
	mariah->AddComponent(ComponentPtr(new GravityEffect(*mariah)));
	// Change the properties of the components
	mariah->getComponent<Transform>()->Translation = Real3(6.0f, 10.0f, 1.0f);
	mariah->getComponent<Transform>()->Scale = Real3(2.0f, 2.0f, 1.0f);
	// Sprite properties
	mariah->getComponent<Sprite>()->Color = Real4(0.0f, 0.7f, 0.3f, 1.0f);
	// BoxCollider properties
	mariah->getComponent<BoxCollider>()->Size = Real3(2, 2, 2);
	mariah->getComponent<BoxCollider>()->IsDrawingCollider = true;
  }


}