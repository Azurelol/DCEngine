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
    mariah->getComponent<Sprite>()->SpriteSource = "MonkeyWalk1";
    mariah->getComponent<Sprite>()->AnimationActive = true;
    mariah->getComponent<Sprite>()->HaveAnimation = true;
    mariah->getComponent<Sprite>()->AnimationSpeed = 10;
    // Change the properties of the components
    mariah->getComponent<Transform>()->Translation = Vec3(4.0f, 10.0f, 0.0f);
    mariah->getComponent<Transform>()->Scale = Vec3(2.0f, 2.0f, 1.0f);
    // Sprite properties
    //mariah->getComponent<Sprite>()->Color = Vec4(1.0f, 0.7f, 0.3f, 1.0f);
    // BoxCollider properties
    mariah->getComponent<BoxCollider>()->Size = Vec3(2, 2, 2);
    mariah->getComponent<BoxCollider>()->IsDrawingCollider = true;
    // Rigidbody properties
    mariah->getComponent<RigidBody>()->setGravity(true);

    {
      GameObjectPtr enemy = ConstructGameObject("groundenemy");
      enemy->AddComponent(ComponentPtr(new Sprite(*enemy)));
      enemy->AddComponent(ComponentPtr(new Transform(*enemy)));
      enemy->AddComponent(ComponentPtr(new RigidBody(*enemy)));
      enemy->AddComponent(ComponentPtr(new BoxCollider(*enemy)));
      enemy->AddComponent(ComponentPtr(new DebugCollider(*enemy)));
      enemy->AddComponent(ComponentPtr(new DebugReport(*enemy)));
      enemy->AddComponent(ComponentPtr(new EnemyController(*enemy)));
      // Change the properties of the components
      enemy->getComponent<Transform>()->Translation = Vec3(25.0f, 1.0f, 0.0f);
      enemy->getComponent<Transform>()->Scale = Vec3(1.0f, 1.0f, 1.0f);
      // Sprite properties
      //enemy->getComponent<Sprite>()->Color = Vec4(1.0f, 0.0f, 1.0f, 1.0f);
      enemy->getComponent<Sprite>()->SpriteSource = "MonkeyStand";
      // BoxCollider properties
      enemy->getComponent<BoxCollider>()->Size = Vec3(2, 2, 2);
      enemy->getComponent<BoxCollider>()->IsDrawingCollider = true;
      // Rigidbody properties
      enemy->getComponent<RigidBody>()->setGravity(true);
    }
    {
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
      ball->getComponent<Transform>()->Translation = Vec3(0.0f, 10.0f, 0.0f);
      ball->getComponent<Transform>()->Scale = Vec3(0.7f, 0.7f, 1.0f);
      // Sprite properties
      ball->getComponent<Sprite>()->SpriteSource = "Circle";
      ball->getComponent<Sprite>()->Color = Vec4(0.0f, 0.7f, 0.3f, 1.0f);

      // BoxCollider properties
      ball->getComponent<BoxCollider>()->Size = Vec3(2, 2, 2);
      ball->getComponent<BoxCollider>()->IsDrawingCollider = true;
      // Rigidbody properties
      ball->getComponent<RigidBody>()->setGravity(true);
      ball->getComponent<RigidBody>()->setRestitution(0.6f);
      
      // Attach the ball to mariah??
      ball->AttachTo(mariah);
      DCTrace << "Ball is attached to: " << ball->Parent()->Name() << "\n";

    }

    
    DCTrace << "Mariah has the following children: \n";
    for (auto child : mariah->Children()) {
      DCTrace << "- " << child->Name() << "\n";
    }

		//// Simple entity #1
		//GameObjectPtr reiner = ConstructGameObject("Reiner");
		//reiner->AddComponent(ComponentPtr(new Sprite(*reiner)));
		//reiner->AddComponent(ComponentPtr(new Transform(*reiner)));
		//reiner->AddComponent(ComponentPtr(new RigidBody(*reiner)));
		//reiner->AddComponent(ComponentPtr(new BoxCollider(*reiner)));
		//reiner->AddComponent(ComponentPtr(new DebugCollider(*reiner)));
		//reiner->AddComponent(ComponentPtr(new DebugMoveController(*reiner)));
		//reiner->AddComponent(ComponentPtr(new DebugReport(*reiner)));
		//// BoxCollider properties
		//reiner->getComponent<BoxCollider>()->Size = Vec3(2, 2, 2);
		//reiner->getComponent<BoxCollider>()->IsDrawingCollider = true;
		//// Transform properties
		//reiner->getComponent<Transform>()->Translation = Vec3(-2, 1.0f, 0.f);
		//reiner->getComponent<Transform>()->Scale = Vec3(2.0f, 2.0f, 1.0f);
  //  reiner->getComponent<RigidBody>()->setGravity(true);
		//// Sprite properties
		////reiner->getComponent<Sprite>()->setSpriteSource("angryeyes.png");
		//															 // MoveController properties
		//reiner->getComponent<Sprite>()->SpriteSource = "KnightAni1mation";
		//reiner->getComponent<Sprite>()->AnimationActive = true;
		//reiner->getComponent<Sprite>()->HaveAnimation = true;
		//reiner->getComponent<Sprite>()->AnimationSpeed = 10;
		//reiner->getComponent<DebugMoveController>()->MoveSpeed = 10;
		//// DebugDraw properties
		//reiner->getComponent<DebugReport>()->DrawType = DebugDrawType::Line;
		//reiner->getComponent<DebugReport>()->Radius = 8.0f;
		//reiner->getComponent<DebugReport>()->Offset = Vec3(5.0f, 0.0f, 0.0f);
		////reiner->getComponent<DebugReport>()->Color = Vec4(1.0f, 0.0f, 1.0f, 1.0f); // Purple line


  // // Simple entity #2
		//GameObjectPtr john = ConstructGameObject("Myres");
		//john->AddComponent(ComponentPtr(new Sprite(*john)));
		//john->AddComponent(ComponentPtr(new Transform(*john)));
		//john->AddComponent(ComponentPtr(new RigidBody(*john)));
  //  john->getComponent<RigidBody>()->setGravity(true);
		//john->AddComponent(ComponentPtr(new BoxCollider(*john)));
		//john->AddComponent(ComponentPtr(new DebugCollider(*john)));
		////john->AddComponent(ComponentPtr(new DebugReport(*john)));
		//// Change the properties of the components
		//john->getComponent<Transform>()->Translation = Vec3(5.0f, 1.0f, 0.0f);
		//john->getComponent<Transform>()->Scale = Vec3(4.0f, 4.0f, 1.0f);
		//// Sprite properties
  //  //john->getComponent<Sprite>()->Color = Vec4(1, 1, 1, 1); // Color: Blue
  //  john->getComponent<Sprite>()->SpriteSource = "MonkeyStand";
  //  john->getComponent<Sprite>()->FlipX = true;
		////john->getComponent<Sprite>()->Color = Vec4(0.0f, 0.0f, 1.0f, 1.0f); // Color: Blue
		//																	 // BoxCollider properties
		//john->getComponent<BoxCollider>()->Size = Vec3(4, 4, 4);
		//john->getComponent<BoxCollider>()->IsDrawingCollider = true;



	}


}


