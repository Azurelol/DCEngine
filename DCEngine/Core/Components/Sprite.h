#pragma once
#include "ComponentReference.h"
#include "../Resources/SpriteSource.h"

//Note: 1. BlendMode and SpriteSource 0%
//      2. Visible, Color, FlipX/Y need Graphics Pipeline. Local prgress done.

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
    Real4 Color = Real4(1.0f, 0.0f, 0.0f, 1.0f); //!< Default color is red.
    BlendModeType BlendMode = BlendModeType::Alpha;
    bool FlipX = false;
    bool FlipY = false;

    Sprite(Entity& owner);
    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);

    void setSpriteSource(std::string fileName);
    SpriteSource* getSpriteSource();
	  void UpdateSprite();

  private:

	bool XFlipped;
	bool YFlipped;
	glm::mat4 FlipMatrix;
	std::unique_ptr<SpriteSource> SpriteSourceObj;


	//Update
	void UpdateFlip();

  };

}
