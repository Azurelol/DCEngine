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
    
  namespace Systems {
    class GraphicsGL;
  }

  namespace Components {
    class Transform;

    class SpriteText : public Component {
      friend class GraphicsGL;
    public:

#if (DCE_USE_ZILCH_INTERNAL_BINDING)
      ZilchDeclareDerivedType(SpriteText, Component);
#endif

      DCE_DEFINE_PROPERTY(Boolean, Visible);
      DCE_DEFINE_PROPERTY(Vec4, Color);
      DCE_DEFINE_PROPERTY(String, Font);
      DCE_DEFINE_PROPERTY(Integer, FontSize);
      DCE_DEFINE_PROPERTY(String, Text);
      DCE_DEFINE_PROPERTY(Integer, PixelsPerUnit);
      DCE_DEFINE_PROPERTY(Boolean, Smoothing);

      SpriteText(Entity& owner);
      ~SpriteText();
      void Register();
      void Initialize();
      Transform* TransformComponent;

    private:

      Boolean Visible = true;
      Vec4 Color = Vec4(0, 0, 0, 1);
      String Font;
      Integer FontSize = 12;
      String Text = "";
      Integer PixelsPerUnit = 64;
      Boolean Smoothing = true;



    };

  }
}