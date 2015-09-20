#include "Transform.h"
#include "EngineReference.h"

namespace DCEngine {
  void Transform::Initialize() {
    if (TRACE_INITIALIZE)
      trace << Owner()->Name() << "::" << _name << "::Initialize\n";
  }
}