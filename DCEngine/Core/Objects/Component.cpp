#include "Component.h"

namespace DCEngine {

  Component::Component(EnumeratedComponent type, BitfieldComponent mask) 
                        : Object("Component"), _type(type), _mask(mask) {}

}


