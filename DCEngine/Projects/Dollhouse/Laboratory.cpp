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
    // Change the properties of the components
    gameObj1->getComponent<DebugMoveController>()->MoveSpeed = 0.25;
    gameObj1->getComponent<Transform>()->Translation = Real3(-0.5f, 0.0f, 0.0f);
    gameObj1->getComponent<Transform>()->Scale = Real3(0.1, 0.1f, 1.0f);
    gameObj1->getComponent<Sprite>()->setSpriteSource("angryeyes.png");
    gameObj1->getComponent<Sprite>()->Color = Real4(0, 0, 1, 1); // Color: Blue

    // Simple entity #2
    GameObjectPtr gameObj2 = ConstructGameObject("Myres");
    gameObj2->AddComponent(ComponentPtr(new Sprite(*gameObj2)));
    gameObj2->AddComponent(ComponentPtr(new Transform(*gameObj2)));
    gameObj2->AddComponent(ComponentPtr(new RigidBody(*gameObj2)));
    gameObj2->AddComponent(ComponentPtr(new BoxCollider(*gameObj2)));
    gameObj2->AddComponent(ComponentPtr(new DebugReport(*gameObj2)));
    // Change the properties of the components
    gameObj2->getComponent<Transform>()->Translation = Real3(0.5f, 0.0f, 1.0f);
    gameObj2->getComponent<Transform>()->Scale = Real3(0.1f, 0.1f, 1.0f);
    gameObj2->getComponent<Sprite>()->setSpriteSource("angryeyes.png");

  }


}