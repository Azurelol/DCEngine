#include "Transform.h"
#include "EngineReference.h"

namespace DCEngine {
  void Transform::Initialize() {
    if (TRACE_INITIALIZE)
      trace << Owner()->Name() << "::" << _name << "::Initialize\n";
  }
  void Transform::Serialize(Json::Value & root) {
  }
  void Transform::Deserialize(Json::Value & root) {
  }
}