#include "Sprite.h"
#include "ObjectReferences.h"

namespace DCEngine {
  void Sprite::Initialize() {
    if (TRACE_INITIALIZE)
      trace << Owner()->Name() << "::" << _name << "::Initialize\n";

    // Register this component, and thus its GameObject to the GraphicsSpace
    auto registerEvent = new Events::SpriteRegistration();
    registerEvent->SpriteObj = (GameObject*)Owner(); // SHOULD THIS BE CASTED?
    space_->Dispatch<Events::SpriteRegistration>(registerEvent);
    
  }
}