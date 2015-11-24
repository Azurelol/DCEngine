/******************************************************************************/
/*!
@file   SpriteText.h
@author Chen Shu, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/23/2015
@brief
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "ComponentReference.h"

namespace DCEngine {
    
  class Transform;
  class GraphicsGL;

  class SpriteText : public Component {
    friend class GraphicsGL;
  public:

    Transform* TransformComponent;
    Boolean Visible = true;
    Vec4 Color = Vec4(0, 0, 0, 1);
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