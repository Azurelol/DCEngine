/******************************************************************************/
/*!
@file   Sprite.h
@author Chen Shu, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   9/19/2015
@brief  The Sprite component allows the graphical representation of this object
        in the world space through the drawing of sprites.
*/
/******************************************************************************/
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

  class Transform;
  class Sprite : public Component {
  public:

    Transform* TransformComponent;
    String SpriteSource = "Square";
    bool Visible = true;
    Real4 Color = Real4(1.0f, 0.0f, 0.0f, 1.0f); //!< Default color is red.
    BlendModeType BlendMode = BlendModeType::Alpha;
    bool FlipX = false;
    bool FlipY = false;

    Sprite(Entity& owner);
    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);
	  void UpdateSprite();

  private:
    
    bool XFlipped;
    bool YFlipped;
    glm::mat4 FlipMatrix;

	//Update
	void UpdateFlip();

  };

}
