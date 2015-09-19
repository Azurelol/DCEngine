#pragma once
#include "../Component.h"
#include "../Resources/SpriteSource.h"

namespace DCEngine {

  enum class BlendMode {
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
    Sprite(Entity& owner) : Component(std::string("Sprite"), 
                            EnumeratedComponent::Sprite, 
                            BitfieldComponent::Sprite, owner) {}
    void Initialize();

    // Properties
    bool Visible;
    Real4 Color;
    BlendMode _BlendMode;
    SpriteSource* getSpriteSource();
    bool FlipX;
    bool FlipY;

  private:
    std::unique_ptr<SpriteSource> SpriteSourceObj;

  };

}
