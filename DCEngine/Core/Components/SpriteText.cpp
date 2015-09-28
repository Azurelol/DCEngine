#include "SpriteText.h"
#include "Transform.h"
#include "EngineReference.h"

namespace DCEngine {
  

  void SpriteText::Initialize()
  {
    // Store the reference to this owner's Transform component
    TransformComponent = dynamic_cast<GameObject*>(Owner())->getComponent<Transform>();
    // Subscribe this component to the graphics space
    space_->getComponent<GraphicsSpace>()->AddSpriteText(*this);
  }

  void SpriteText::Serialize(Json::Value & root)
  {

  }

  void SpriteText::Deserialize(Json::Value & root)
  {
  }

}