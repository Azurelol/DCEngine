#pragma once
#include "ComponentReference.h"

namespace DCEngine {
    
  class Transform;
  class SpriteText : public Component {
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