/******************************************************************************/
/*!
@file   SpriteParticleSystem.h
@author Chen Shu, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/6/2015
@brief  The SpriteParticleSystem component... 
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ComponentReference.h"
#include "../Resources/SpriteSource.h"

namespace DCEngine {
  
  enum class GeometryMode {
    Billboarded,
    Beam,
    Aligned,
    Outward,
    FaceVelocity,
    Flat,
  };

  namespace Components
  {
    class Transform;
    class Camera;
    class SpriteParticleSystem : public Component {
    public:

      // Properties
      Boolean Visible;
      Vec4 Tint = Vec4(1, 1, 1, 1); //!< Color to tint/multiply all particles in the system.
      String Texture; // Allows the use of textures/spritesources
      Real VelocityScale = 1;
      Real LengthScale = 1;
      Vec3 SystemSize = Vec3(4, 4, 4);
      DCE_DEFINE_PROPERTY(Boolean, Visible);
      DCE_DEFINE_PROPERTY(Vec4, Tint);
      DCE_DEFINE_PROPERTY(String, Texture);
      DCE_DEFINE_PROPERTY(Real, VelocityScale);
      DCE_DEFINE_PROPERTY(Real, LengthScale);
      DCE_DEFINE_PROPERTY(Vec3, SystemSize);


      ZilchDeclareDerivedType(SpriteParticleSystem, Component);
      SpriteParticleSystem(Entity& owner);
      void Initialize();

    private:
      Transform* TransformComponent;
      Camera* CameraComponent;

    };
  }

}
