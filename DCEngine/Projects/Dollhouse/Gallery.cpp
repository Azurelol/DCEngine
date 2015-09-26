//#include "Gallery.h"
#include "Dollhouse.h"

namespace DCEngine {

  /* Chen, write your tests here. */
  void DollHouse::GalleryTesting()
  {
    // Construct and draw something else here?

    GameObjectPtr chenObj = ConstructGameObject("Chen");
    chenObj->AddComponent(ComponentPtr(new Transform(*chenObj)));
    chenObj->AddComponent(ComponentPtr(new Sprite(*chenObj)));
    chenObj->AddComponent(ComponentPtr(new DebugReport(*chenObj)));
    // Transform properties
    chenObj->getComponent<Transform>()->Translation = Real3(5, 5, 0.0f);
    chenObj->getComponent<Transform>()->Scale = Real3(2.0f, 2.0f, 2.0f);
    // Sprite properties
    chenObj->getComponent<Sprite>()->setSpriteSource("angryeyes.png");
    chenObj->getComponent<Sprite>()->Color = Real4(0, 1, 1, 1); // Color: Blue

    GameObjectPtr connorObj = ConstructGameObject("Connor");
    connorObj->AddComponent(ComponentPtr(new Transform(*connorObj)));
    connorObj->AddComponent(ComponentPtr(new Sprite(*connorObj)));
    connorObj->AddComponent(ComponentPtr(new DebugReport(*connorObj)));
    // Transform properties
    connorObj->getComponent<Transform>()->Translation = Real3(-5, 5, 0.0f);
    connorObj->getComponent<Transform>()->Scale = Real3(2.0f, 2.0f, 2.0f);
    // Sprite properties
    connorObj->getComponent<Sprite>()->setSpriteSource("angryeyes.png");
    connorObj->getComponent<Sprite>()->Color = Real4(0, 1, 0, 1); // Color: Blue

  }
}