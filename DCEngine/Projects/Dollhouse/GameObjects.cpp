//#include "Laboratory.h"
#include "Dollhouse.h"
#include <random> // random monkeys

float frand(float a = 0, float b = 1) {
  return a + (b - a)*float(rand()) / float(RAND_MAX);
}


namespace DCEngine {

  void DollHouse::GenerateMonkeyLOL(Vec3 translation, Vec3 scale, Vec4 color) {
    GameObjectPtr monkey = ConstructGameObject("Monkey");
    monkey->AddComponent<Transform>();
    monkey->AddComponent<Sprite>();
    monkey->AddComponent<RigidBody>();
    monkey->AddComponent<BoxCollider>();
    monkey->AddComponent<DebugCollider>();
    monkey->AddComponent<EnemyController>();
    // Change the properties of the components
    monkey->getComponent<Transform>()->Translation = translation;
    monkey->getComponent<Transform>()->Scale = scale;
    // Sprite properties
    //monkey->getComponent<Sprite>()->SpriteSource = "MonkeyStand";
    monkey->getComponent<Sprite>()->SpriteSource = "MonkeyWalk1";
    monkey->getComponent<Sprite>()->AnimationActive = true;
    monkey->getComponent<Sprite>()->HaveAnimation = true;
    monkey->getComponent<Sprite>()->AnimationSpeed = 10;
    monkey->getComponent<Sprite>()->setColor(Vec4(color));
    // BoxCollider properties
    monkey->getComponent<BoxCollider>()->Size = Vec3(2, 2, 2);
    monkey->getComponent<BoxCollider>()->IsDrawingCollider = false;
    // Rigidbody properties
    monkey->getComponent<RigidBody>()->setGravity(true);
    //std::uniform_real_distribution<float> dist(0, 5);
    //std::mt
    monkey->getComponent<EnemyController>()->JumpInterval = frand(1, 6);
    monkey->getComponent<EnemyController>()->MoveSpeed = frand(2, 10);
  }

	void DollHouse::GenerateTerrain(Vec3 translation, Vec3 scale, Vec4 color)
	{
		//char* name = "Ground";
		//char* buffer = "100000000000";
		//itoa(TerrainPiecesCreated, buffer, 10);
		//strcat(name, buffer);
		//GameObjectPtr ground = ConstructGameObject(name);


		GameObjectPtr ground = ConstructGameObject("Ground");
		ground->AddComponent<Transform>();
		ground->AddComponent<Sprite>();
		ground->AddComponent<RigidBody>();
		ground->AddComponent<BoxCollider>();
		ground->AddComponent<DebugCollider>();
		// Change the properties of the components
		ground->getComponent<Transform>()->Translation = translation;
		ground->getComponent<Transform>()->Scale = scale;
		// Sprite properties
		ground->getComponent<Sprite>()->setColor(color);
		// BoxCollider properties
		ground->getComponent<BoxCollider>()->Size = Vec3(2, 2, 2);
		ground->getComponent<BoxCollider>()->IsDrawingCollider = true;
		// Rigidbody properties
		ground->getComponent<RigidBody>()->setDynamicState(DynamicStateType::Static);
	}

	void DollHouse::GenerateHazardArea(Vec3 translation, Vec3 scale, Vec4 color)
	{
		//char* name = "Hazard";
		//char* buffer = "100000000000";
		//itoa(TerrainPiecesCreated, buffer, 10);
		//strcat(name, buffer);
		//GameObjectPtr ground = ConstructGameObject(name);

		
		GameObjectPtr obj = ConstructGameObject("HazardArea");
		obj->AddComponent<Transform>();
		obj->AddComponent<Sprite>();
		obj->AddComponent<RigidBody>();
		obj->AddComponent<BoxCollider>();
		obj->AddComponent<DebugCollider>();
		obj->AddComponent<HazardArea>();
		// Change the properties of the components
		obj->getComponent<Transform>()->Translation = translation;
		obj->getComponent<Transform>()->Scale = scale;
		// Sprite properties
		obj->getComponent<Sprite>()->setColor(color);
		// BoxCollider properties
		obj->getComponent<BoxCollider>()->Size = Vec3(2, 2, 2);
		obj->getComponent<BoxCollider>()->IsDrawingCollider = true;
		obj->getComponent<BoxCollider>()->setGhost(true);
		// Rigidbody properties
		obj->getComponent<RigidBody>()->setDynamicState(DynamicStateType::Static);

		
	}

	void DollHouse::GeneratePlayer(Vec3 translation, Vec3 scale, Vec4 color)
	{
		GameObjectPtr mariah = ConstructGameObject("Mariah");
		mariah->AddComponent<Transform>();
		mariah->AddComponent<Sprite>();
		mariah->AddComponent<RigidBody>();
		mariah->AddComponent<BoxCollider>();
		mariah->AddComponent<DebugCollider>();
		mariah->AddComponent<DebugReport>();
		mariah->AddComponent<PlayerController>();
		mariah->getComponent<Sprite>()->SpriteSource = "MonkeyWalk1";
		mariah->getComponent<Sprite>()->AnimationActive = true;
		mariah->getComponent<Sprite>()->HaveAnimation = true;
		mariah->getComponent<Sprite>()->AnimationSpeed = 10;
		// Change the properties of the components
		mariah->getComponent<Transform>()->Translation = Vec3(4.0f, 10.0f, 0.0f);
		mariah->getComponent<Transform>()->Scale = Vec3(2.0f, 2.0f, 1.0f);
		// Sprite properties
		mariah->getComponent<Sprite>()->Color = color;
		// BoxCollider properties
		mariah->getComponent<BoxCollider>()->Size = Vec3(2, 2, 2);
		mariah->getComponent<BoxCollider>()->IsDrawingCollider = true;
		// Rigidbody properties
		mariah->getComponent<RigidBody>()->setGravity(true);
	}


	void DollHouse::GenerateBall(Vec3 translation, Vec3 scale, Vec4 color)
	{
		GameObjectPtr ball = ConstructGameObject("Basketball");
		ball->AddComponent<Transform>();
		ball->AddComponent<Sprite>();
		ball->AddComponent<RigidBody>();
		ball->AddComponent<BoxCollider>();
		ball->AddComponent<DebugCollider>();
		ball->AddComponent<DebugReport>();
		ball->AddComponent<BallController>();
		// Change the properties of the components
		ball->getComponent<Transform>()->Translation = translation;
		ball->getComponent<Transform>()->Scale = scale;
		// Sprite properties
		ball->getComponent<Sprite>()->SpriteSource = "Circle";
		ball->getComponent<Sprite>()->Color = color;

		// BoxCollider properties
		ball->getComponent<BoxCollider>()->Size = Vec3(2, 2, 2);
		ball->getComponent<BoxCollider>()->IsDrawingCollider = true;
		// Rigidbody properties
		ball->getComponent<RigidBody>()->setGravity(true);
		ball->getComponent<RigidBody>()->setRestitution(0.6f);
	}

	void DollHouse::GenerateCamera(Vec3 translation, Vec3 scale, Vec4 color)
	{
	}

	/* Blaine, write your tests here. */
	void DollHouse::GameObjects() {
    srand(unsigned(time(0)));

    // Player
    GameObjectPtr mariah = ConstructGameObject("Mariah");
    mariah->AddComponent<Transform>();
    mariah->AddComponent<Sprite>();
    mariah->AddComponent<RigidBody>();
    mariah->AddComponent<BoxCollider>();
    mariah->AddComponent<DebugCollider>();
    mariah->AddComponent<DebugReport>();
    mariah->AddComponent<PlayerController>();
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
    
    // Attach the the camera to the player character.
    //cameraObj->AttachTo(mariah);
    /*
	
	GenerateMonkeyLOL(Vec3(5, 1, 0), Vec3(1, 1, 1), Vec4(1, 1, 1.0, 1));
    GenerateMonkeyLOL(Vec3(15, 1, 0), Vec3(2, 2, 1), Vec4(1, 1, 1.0, 1));
    GenerateMonkeyLOL(Vec3(25, 1, 0), Vec3(3, 3, 1), Vec4(1, 1, 1.0, 1));
    GenerateMonkeyLOL(Vec3(20, 1, 0), Vec3(2.5, 2.5, 1), Vec4(1, 1, 1.0, 1));
    GenerateMonkeyLOL(Vec3(-5, 1, 0), Vec3(2, 2, 1), Vec4(1, 1, 1.0, 1));
    GenerateMonkeyLOL(Vec3(-15, 1, 0), Vec3(2, 2, 1), Vec4(1, 1, 1.0, 1));
    GenerateMonkeyLOL(Vec3(-25, 1, 0), Vec3(1, 1, 1), Vec4(1, 1, 1.0, 1));
    GenerateMonkeyLOL(Vec3(-20, 1, 0), Vec3(4, 4, 1), Vec4(1, 1, 1.0, 1));

	*/
	GenerateTerrain(Vec3(0, 0, 0), Vec3(1, 1, 1), Vec4(0, 1, 0, 1));
	GenerateHazardArea(Vec3(5, 0, 0), Vec3(2, 2, 1), Vec4(0, 1, 0, 1));

    //{
    //  GameObjectPtr enemy = ConstructGameObject("Chen");
    //  enemy->AddComponent<Transform>();
    //  enemy->AddComponent<Sprite>();
    //  enemy->AddComponent<RigidBody>();
    //  enemy->AddComponent<BoxCollider>();
    //  enemy->AddComponent<DebugCollider>();
    //  enemy->AddComponent<DebugReport>();
    //  enemy->AddComponent<EnemyController>();
    //  // Change the properties of the components
    //  enemy->getComponent<Transform>()->Translation = Vec3(5.0f, 1.0f, 0.0f);
    //  enemy->getComponent<Transform>()->Scale = Vec3(1.0f, 1.0f, 1.0f);
    //  // Sprite properties
    //  //enemy->getComponent<Sprite>()->Color = Vec4(1.0f, 0.0f, 1.0f, 1.0f);
    //  enemy->getComponent<Sprite>()->SpriteSource = "MonkeyStand";
    //  enemy->getComponent<Sprite>()->setColor(Vec4(0.5, 0.25, 0, 1));
    //  // BoxCollider properties
    //  enemy->getComponent<BoxCollider>()->Size = Vec3(2, 2, 2);
    //  enemy->getComponent<BoxCollider>()->IsDrawingCollider = false;
    //  // Rigidbody properties
    //  enemy->getComponent<RigidBody>()->setGravity(true);
    //}
    //{
    //  GameObjectPtr enemy = ConstructGameObject("Connor");
    //  enemy->AddComponent<Transform>();
    //  enemy->AddComponent<Sprite>();
    //  enemy->AddComponent<RigidBody>();
    //  enemy->AddComponent<BoxCollider>();
    //  enemy->AddComponent<DebugCollider>();
    //  enemy->AddComponent<DebugReport>();
    //  enemy->AddComponent<EnemyController>();
    //  // Change the properties of the components
    //  enemy->getComponent<Transform>()->Translation = Vec3(10.0f, 1.0f, 0.0f);
    //  enemy->getComponent<Transform>()->Scale = Vec3(2.0f, 2.0f, 1.0f);
    //  // Sprite properties
    //  //enemy->getComponent<Sprite>()->Color = Vec4(1.0f, 0.0f, 1.0f, 1.0f);
    //  enemy->getComponent<Sprite>()->SpriteSource = "MonkeyStand";
    //  // BoxCollider properties
    //  enemy->getComponent<BoxCollider>()->Size = Vec3(2, 2, 2);
    //  enemy->getComponent<BoxCollider>()->IsDrawingCollider = false;
    //  // Rigidbody properties
    //  enemy->getComponent<RigidBody>()->setGravity(true);
    //  enemy->getComponent<EnemyController>()->MoveSpeed = 6.0f;
    //  enemy->getComponent<EnemyController>()->JumpInterval = 1.0f;
    //}
    //{
    //  GameObjectPtr enemy = ConstructGameObject("Reiner");
    //  enemy->AddComponent<Transform>();
    //  enemy->AddComponent<Sprite>();
    //  enemy->AddComponent<RigidBody>();
    //  enemy->AddComponent<BoxCollider>();
    //  enemy->AddComponent<DebugCollider>();
    //  enemy->AddComponent<DebugReport>();
    //  enemy->AddComponent<EnemyController>();
    //  // Change the properties of the components
    //  enemy->getComponent<Transform>()->Translation = Vec3(20.0f, 1.0f, 0.0f);
    //  enemy->getComponent<Transform>()->Scale = Vec3(3.0f, 3.0f, 1.0f);
    //  // Sprite properties
    //  //enemy->getComponent<Sprite>()->Color = Vec4(1.0f, 0.0f, 1.0f, 1.0f);
    //  enemy->getComponent<Sprite>()->SpriteSource = "MonkeyStand";
    //  enemy->getComponent<Sprite>()->setColor(Vec4(1.0, 0.5, 1.0, 1.0));
    //  // BoxCollider properties
    //  enemy->getComponent<BoxCollider>()->Size = Vec3(2, 2, 2);
    //  enemy->getComponent<BoxCollider>()->IsDrawingCollider = false;
    //  // Rigidbody properties
    //  enemy->getComponent<RigidBody>()->setGravity(true);
    //  enemy->getComponent<EnemyController>()->MoveSpeed = 6.0f;
    //  enemy->getComponent<EnemyController>()->JumpInterval = 1.0f;
    //}


    {
      // Ball
      GameObjectPtr ball = ConstructGameObject("Basketball");
      ball->AddComponent<Transform>();
      ball->AddComponent<Sprite>();
      ball->AddComponent<RigidBody>();
      ball->AddComponent<BoxCollider>();
      ball->AddComponent<DebugCollider>();
      ball->AddComponent<DebugReport>();
      ball->AddComponent<BallController>();
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
      //ball->AttachTo(mariah);
      //DCTrace << "Ball is attached to: " << ball->Parent()->Name() << "\n";

    }

	GameObjectPtr chargeBar = ConstructGameObject("ChargeBar");
	  chargeBar->AddComponent<Transform>();
	  chargeBar->AddComponent<Sprite>();
	  chargeBar->AddComponent<DebugCollider>();
	  chargeBar->AddComponent<DebugReport>();
  chargeBar->AddComponent<ChargeBar>();
	//Parenting to the player
	chargeBar->AttachTo(mariah);
	// Change the properties of the components
	chargeBar->getComponent<Transform>()->Translation = Vec3(0.0f, 3.5f, 0.0f);
	chargeBar->getComponent<Transform>()->Scale = Vec3(0, 0.3f, 1.0f);
	// Sprite properties

	// BoxCollider properties

	// Rigidbody properties




    
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


