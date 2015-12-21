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

      /* Properties */
      Boolean Visible;
      Vec4 Tint = Vec4(1, 1, 1, 1); //!< Color to tint/multiply all particles in the system.
      String SpriteSource;
      Real VelocityScale = 1;
      Real LengthScale = 1;
      Vec3 SystemSize = Vec3(4, 4, 4);

      SpriteParticleSystem(Entity& owner);
      void Initialize();
      virtual void Serialize(Json::Value& root);
      virtual void Deserialize(Json::Value& root);

    private:
      Transform* TransformComponent;
      Camera* CameraComponent;

    };
  }

}
