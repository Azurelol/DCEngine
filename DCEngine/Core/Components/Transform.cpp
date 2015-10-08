#include "Transform.h"
#include "EngineReference.h"

namespace DCEngine {

  void Transform::Initialize() {
    if (TRACE_COMPONENT_INITIALIZE)
      trace << Owner()->Name() << "::" << ObjName << "::Initialize\n";
  }

  void Transform::Serialize(Json::Value & root) {
  }

  void Transform::Deserialize(Json::Value & root) {
  }
}