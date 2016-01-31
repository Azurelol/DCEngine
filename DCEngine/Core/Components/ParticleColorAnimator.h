/******************************************************************************/
/*!
@file   ParticleColorAnimator.h
@author Chen Shu, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/6/2015
@brief  The ParticleColorAnimator component...
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Graphical.h"

namespace DCEngine {
  namespace Components
  {
    class SpriteParticleSystem;
    class ParticleColorAnimator : public Graphical {
    public:
      // Properties
      Vec4 Color0;
      Vec4 Color1;
      Vec4 Color2;
      Vec4 Color3;
      Vec4 Color4;
      DCE_DEFINE_PROPERTY(Vec4, Color0);
      DCE_DEFINE_PROPERTY(Vec4, Color1);
      DCE_DEFINE_PROPERTY(Vec4, Color2);
      DCE_DEFINE_PROPERTY(Vec4, Color3);
      DCE_DEFINE_PROPERTY(Vec4, Color4);

      ZilchDeclareDerivedType(ParticleColorAnimator, Graphical);
      ParticleColorAnimator(Entity& owner);
			virtual ~ParticleColorAnimator(void);
      void Initialize();
    private:
      SpriteParticleSystem* ParticleSystem;

    };

  }

  
}