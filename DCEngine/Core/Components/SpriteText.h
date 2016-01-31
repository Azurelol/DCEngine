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
#include "Graphical.h"
#include "Camera.h"

namespace DCEngine {
    
  namespace Systems {
    class GraphicsGL;
  }

  namespace Components {
    class Transform;

    class SpriteText : public Graphical {
      friend class GraphicsGL;
    public:
      
      DCE_DEFINE_PROPERTY(Boolean, Visible);
      DCE_DEFINE_PROPERTY(Vec4, Color);
      DCE_DEFINE_PROPERTY(String, Font);
      DCE_DEFINE_PROPERTY(Integer, FontSize);
      DCE_DEFINE_PROPERTY(String, Text);
      DCE_DEFINE_PROPERTY(Integer, PixelsPerUnit);
      DCE_DEFINE_PROPERTY(Boolean, Smoothing);
			DCE_DEFINE_PROPERTY(Integer, WordWrap);

      ZilchDeclareDerivedType(SpriteText, Graphical);
      SpriteText(Entity& owner);
      virtual ~SpriteText();
      void Register();
      void Initialize();
			void Update(double dt);
			void Draw(Camera& camera);
      Transform* TransformComponent;

			static ShaderPtr mShader;
			static GLuint mVAO, mVBO;

    private:

      Boolean Visible = true;
      Vec4 Color = Vec4(0, 0, 0, 1);
      String Font;
      Integer FontSize = 12;
      String Text = "";
      Integer PixelsPerUnit = 64;
      Boolean Smoothing = true;
			Integer WordWrap;


    };

  }
}