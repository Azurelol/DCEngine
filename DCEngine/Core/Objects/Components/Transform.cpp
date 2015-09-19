#include "Transform.h"
#include "ObjectReferences.h"

namespace DCEngine {
  void Transform::Initialize() {
    if (TRACE_INITIALIZE)
      trace << Owner()->Name() << "::" << _name << "::Initialize\n";
  }
}