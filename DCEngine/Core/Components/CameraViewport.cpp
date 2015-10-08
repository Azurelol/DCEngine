#include "CameraViewport.h"
#include "EngineReference.h"

namespace DCEngine {

  void CameraViewport::Initialize() {

    // Temporary: Look for the default camera object by searching
    // through the space's gameobjects and looking for one with the
    // camera component
    setCamera();
  }

  void CameraViewport::Serialize(Json::Value & root) {

    // Serialization will be setting the default camera for this component
  }

  void CameraViewport::Deserialize(Json::Value & root) {
  }
  void CameraViewport::setCamera() {
    cameraObj_ = SpaceRef->FindObjectByName("Camera")->getComponent<Camera>();

    // Search through the space's gameObjects list
    //for (auto gameObj : SpaceRef-> )

  }
}