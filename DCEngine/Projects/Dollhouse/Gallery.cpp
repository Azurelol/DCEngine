//#include "Gallery.h"
#include "Dollhouse.h"

namespace DCEngine {

  /* Chen, write your tests here. */
  void DollHouse::GalleryTesting()
  {
    // Default camera object for the space
    GameObjectPtr cameraObj = ConstructGameObject("Camera");
    cameraObj->AddComponent(ComponentPtr(new Transform(*cameraObj)));
    cameraObj->AddComponent(ComponentPtr(new Camera(*cameraObj)));
    cameraObj->AddComponent(ComponentPtr(new Camera(*cameraObj)));
    // cameraObj->AddComponent(ComponentPtr(new DebugMoveController(*cameraObj)));
    cameraObj->AddComponent(ComponentPtr(new DebugReport(*cameraObj)));
    cameraObj->getComponent<Transform>()->Translation = Real3(1.0f, 1.0f, 1.0f);
    

  }
}