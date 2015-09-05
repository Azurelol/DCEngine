#include "Component.h"

#include "Entity.h" // EntityPtr

namespace DCEngine {

  Component::Component(EnumeratedComponent type, BitfieldComponent mask) 
                        : Object("Component"), _type(type), _mask(mask) {
    
	  
  }

  Entity* Component::Owner() {
    return (Entity*)_owner;
  }

}


