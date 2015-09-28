#pragma once
#include "ComponentReference.h"

namespace DCEngine {

  class Transform;
  class SpriteText : public Component {
  public:

    Boolean Visible;
    Boolean Color;
    //Font Font::
    Integer FontSize;
    String Text;
    Integer PixelsPerUnit;
    Boolean Smoothing;
     
    SpriteText(Entity& owner) : Component(std::string("SpriteText"), owner) {}
    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);

  private:
    Transform* TransformComponent;

  };

}