#pragma once
#include "ComponentReference.h"

namespace DCEngine {

  class SpriteText : public Component {
  public:
    Boolean Visible;
    Boolean Color;
    //Font Font::
    Integer FontSize;
    String Text;
    Integer PixelsPerUnit;
    Boolean Smoothing;



  private:

    SpriteText(Entity& owner);
    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);

  };

}