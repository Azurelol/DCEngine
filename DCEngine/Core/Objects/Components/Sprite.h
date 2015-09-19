#pragma once
#include "..\Component.h"

//Note: 1. BlendMode and SpriteSource 0%
//      2. Visible, Color, FlipX/Y need Graphics Pipeline. Local prgress done.

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
    SpriteSource* _SpriteSource;
    bool FlipX;	
    bool FlipY;
	void UpdateSprite();

  private:
	bool XFlipped;
	bool YFlipped;
	glm::mat4 FlipMatrix;


	//Update
	void UpdateFlip();

  };

}
