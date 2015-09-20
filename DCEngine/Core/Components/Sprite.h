#pragma once
#include "ComponentReference.h"
#include "../Resources/SpriteSource.h"

namespace DCEngine {

  enum class BlendModeType {
    Alpha,
    Additive,
    Multiply,
    MultiplyAdd,
    ColorAdditive,
    None,
  };

  class SpriteSource;
  class Sprite : public Component {
  public:

    bool Visible = true;
    Real3 Color = Real3(1.0f, 1.0f, 1.0f);
    BlendModeType BlendMode = BlendModeType::Alpha;
    bool FlipX = false;
    bool FlipY = false;


    Sprite(Entity& owner) : Component(std::string("Sprite"), 
                            EnumeratedComponent::Sprite, 
                            BitfieldComponent::Sprite, owner) {}
    void Initialize();    
    void setSpriteSource(std::string fileName);
    SpriteSource* getSpriteSource();

  private:
    std::unique_ptr<SpriteSource> SpriteSourceObj;

  };

}
