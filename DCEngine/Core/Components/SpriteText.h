#pragma once
#include "ComponentReference.h"

namespace DCEngine {
    
  class Transform;
<<<<<<< .merge_file_a07644
  class GraphicsGL;

  class SpriteText : public Component {
    friend class GraphicsGL;
=======
  class SpriteText : public Component {
>>>>>>> .merge_file_a12264
  public:

    Transform* TransformComponent;
    Boolean Visible = true;
    Real4 Color = Real4(0, 0, 0, 1);
    String Font = "Verdana";
    Integer FontSize = 12;
    String Text = "";
    Integer PixelsPerUnit = 64;
    Boolean Smoothing = true;
     
    SpriteText(Entity& owner) : Component(std::string("SpriteText"), owner) {}
    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);

  private:

  };

}