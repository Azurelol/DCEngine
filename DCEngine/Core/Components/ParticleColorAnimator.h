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
#include "ComponentReference.h"

namespace DCEngine {

    class ParticleColorAnimator : public Component {
    public:

      /* Properties */
      Vec4 Color0;
      Vec4 Color1;
      Vec4 Color2;
      Vec4 Color3;
      Vec4 Color4;

      ParticleColorAnimator(Entity& owner);
      void Initialize();
      virtual void Serialize(Json::Value& root);
      virtual void Deserialize(Json::Value& root);

    private:

    };


  
}