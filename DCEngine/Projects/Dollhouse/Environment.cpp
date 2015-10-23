#include "Dollhouse.h"

namespace DCEngine {

  void DollHouse::EnvironmentSetup() {

    GameObjectPtr floor = ConstructGameObject("Floor");
    floor->AddComponent(ComponentPtr(new Transform(*floor)));
    floor->AddComponent(ComponentPtr(new Sprite(*floor)));
    floor->AddComponent(ComponentPtr(new RigidBody(*floor)));
    floor->getComponent<Sprite>()->Color = Real4(0, 1, 0, 1); // Color: Green
    floor->AddComponent(ComponentPtr(new BoxCollider(*floor)));
    floor->getComponent<BoxCollider>()->IsDrawingCollider = true;
    floor->getComponent<BoxCollider>()->Size = Real3(1, 1, 1);
    floor->getComponent<Transform>()->Translation = Real3(0, -1, 0);
    //floor->getComponent<Transform>()->Scale = Real3(20, 1, 1);

    GameObjectPtr backgroundLayer1 = ConstructGameObject("BackgroundLayer1");
    backgroundLayer1->AddComponent(ComponentPtr(new Transform(*backgroundLayer1)));
    backgroundLayer1->AddComponent(ComponentPtr(new Sprite(*backgroundLayer1)));
    backgroundLayer1->getComponent<Sprite>()->Color = Real4(0.6, 0.6, 0.3, 1); // Color: Green
    backgroundLayer1->getComponent<Transform>()->Translation = Real3(0, 0, -5);
    backgroundLayer1->getComponent<Transform>()->Scale = Real3(100, 50, 1);

  }

}