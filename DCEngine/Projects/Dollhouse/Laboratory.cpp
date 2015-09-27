//#include "Laboratory.h"
#include "Dollhouse.h"

namespace DCEngine {

  /* Blaine, write your tests here. */
  void DollHouse::LaboratoryTesting() {

    // Simple entity #1
    GameObjectPtr gameObj1 = ConstructGameObject("Reiner");
    gameObj1->AddComponent(ComponentPtr(new Sprite(*gameObj1)));
    gameObj1->AddComponent(ComponentPtr(new Transform(*gameObj1)));
    gameObj1->AddComponent(ComponentPtr(new RigidBody(*gameObj1)));
    gameObj1->AddComponent(ComponentPtr(new BoxCollider(*gameObj1)));
    gameObj1->AddComponent(ComponentPtr(new DebugMoveController(*gameObj1)));
    gameObj1->AddComponent(ComponentPtr(new DebugReport(*gameObj1)));
    // Transform properties
    gameObj1->getComponent<Transform>()->Translation = Real3(-2, 0.0f, 0.0f);
    gameObj1->getComponent<Transform>()->Scale = Real3(1.0f, 1.0f, 1.0f);
    // Sprite properties
    gameObj1->getComponent<Sprite>()->setSpriteSource("angryeyes.png");
    gameObj1->getComponent<Sprite>()->Color = Real4(1, 0, 0, 1); // Color: Red
    // MoveController properties
    gameObj1->getComponent<DebugMoveController>()->MoveSpeed = 0.5;
    // DebugDraw properties
    gameObj1->getComponent<DebugReport>()->DrawType = DebugDrawType::Line;
    gameObj1->getComponent<DebugReport>()->Radius = 4;
    gameObj1->getComponent<DebugReport>()->Offset = Real3(5, -2, 0);
    gameObj1->getComponent<DebugReport>()->Color = Real4(1, 0, 0, 1); // Color: Red


    // Simple entity #2
    GameObjectPtr john = ConstructGameObject("Myres");
    john->AddComponent(ComponentPtr(new Sprite(*john)));
    john->AddComponent(ComponentPtr(new Transform(*john)));
    john->AddComponent(ComponentPtr(new RigidBody(*john)));
    john->AddComponent(ComponentPtr(new BoxCollider(*john)));
    john->AddComponent(ComponentPtr(new DebugReport(*john)));
    // Change the properties of the components
    john->getComponent<Transform>()->Translation = Real3(2, 0.0f, 1.0f);
    john->getComponent<Transform>()->Scale = Real3(1, 1, 1.0f);
    // Sprite properties
    john->getComponent<Sprite>()->setSpriteSource("angryeyes.png");
    john->getComponent<Sprite>()->Color = Real4(0, 0, 1, 1); // Color: Blue
  }


}