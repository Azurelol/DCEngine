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

<<<<<<< .merge_file_a13700
  // Forward declarations
  class Transform;
=======
  class Transform;
  class SpriteSource;
>>>>>>> .merge_file_a14088

  class Sprite : public Component {
  public:

<<<<<<< .merge_file_a13700
    Transform* TransformComponent;
    String SpriteSource = "Square";
=======
>>>>>>> .merge_file_a14088
    bool Visible = true;
    Real4 Color = Real4(1.0f, 0.0f, 0.0f, 1.0f); //!< Default color is red.
    BlendModeType BlendMode = BlendModeType::Alpha;
    bool FlipX = false;
    bool FlipY = false;

<<<<<<< .merge_file_a13700

=======
>>>>>>> .merge_file_a14088
    Sprite(Entity& owner);
    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);
<<<<<<< .merge_file_a13700
=======
    void setSpriteSource(std::string fileName);
    SpriteSource* getSpriteSource();
>>>>>>> .merge_file_a14088
	  void UpdateSprite();

  private:

<<<<<<< .merge_file_a13700
=======
    Transform* TransformComponent;
>>>>>>> .merge_file_a14088

    bool XFlipped;
    bool YFlipped;
    glm::mat4 FlipMatrix;
<<<<<<< .merge_file_a13700
=======
    std::unique_ptr<SpriteSource> SpriteSourceObj;
>>>>>>> .merge_file_a14088


	//Update
	void UpdateFlip();

  };

}
