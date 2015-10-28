#include "Dollhouse.h"

namespace DCEngine {

  void DollHouse::EnvironmentSetup() {

    GameObjectPtr floor = ConstructGameObject("Basketfloor");
    floor->AddComponent(ComponentPtr(new Sprite(*floor)));
    floor->AddComponent(ComponentPtr(new Transform(*floor)));
    floor->AddComponent(ComponentPtr(new RigidBody(*floor)));
    floor->AddComponent(ComponentPtr(new BoxCollider(*floor)));
    //floor->AddComponent(ComponentPtr(new DebugCollider(*floor)));
    floor->AddComponent(ComponentPtr(new DebugReport(*floor)));
    // Change the properties of the components
    floor->getComponent<Transform>()->Translation = Real3(0.0f, -5.0f, 0.0f);
    floor->getComponent<Transform>()->Scale = Real3(100.0f, 1.0f, 1.0f);
    // Sprite properties
    floor->getComponent<Sprite>()->Color = Real4(1.0f, 0.4f, 0.1f, 1.0f);
    // BoxCollider properties
    floor->getComponent<BoxCollider>()->Size = Real3(100, 2, 2);
    floor->getComponent<BoxCollider>()->IsDrawingCollider = true;
    // Rigidbody properties
    floor->getComponent<RigidBody>()->DynamicState = DynamicStateType::Static;


	GameObjectPtr floor2 = ConstructGameObject("Basketfloor2");
	floor2->AddComponent(ComponentPtr(new Sprite(*floor2)));
	floor2->AddComponent(ComponentPtr(new Transform(*floor2)));
	floor2->AddComponent(ComponentPtr(new RigidBody(*floor2)));
	floor2->AddComponent(ComponentPtr(new BoxCollider(*floor2)));
	//floor2->AddComponent(ComponentPtr(new DebugCollider(*floor2)));
	floor2->AddComponent(ComponentPtr(new DebugReport(*floor2)));
	// Change the properties of the components
	floor2->getComponent<Transform>()->Translation = Real3(0.0f, 20.0f, 0.0f);
	floor2->getComponent<Transform>()->Scale = Real3(100.0f, 1.0f, 1.0f);
	// Sprite properties
	floor2->getComponent<Sprite>()->Color = Real4(1.0f, 0.4f, 0.1f, 1.0f);
	// BoxCollider properties
	floor2->getComponent<BoxCollider>()->Size = Real3(100, 2, 2);
	floor2->getComponent<BoxCollider>()->IsDrawingCollider = true;
	// Rigidbody properties
	floor2->getComponent<RigidBody>()->DynamicState = DynamicStateType::Static;


    
    GameObjectPtr backgroundLayer1 = ConstructGameObject("BackgroundLayer1");
    backgroundLayer1->AddComponent(ComponentPtr(new Transform(*backgroundLayer1)));
    backgroundLayer1->AddComponent(ComponentPtr(new Sprite(*backgroundLayer1)));
    backgroundLayer1->getComponent<Sprite>()->Color = Real4(0.6, 0.6, 0.3, 1); // Color: Green
    backgroundLayer1->getComponent<Transform>()->Translation = Real3(0, 0, -5);
    backgroundLayer1->getComponent<Transform>()->Scale = Real3(100, 50, 1);

  }

}