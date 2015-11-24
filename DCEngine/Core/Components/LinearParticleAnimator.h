/******************************************************************************/
/*!
@file   LinearParticleAnimator.h
@author Chen Shu, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/6/2015
@brief  The LinearParticleAnimator component...
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ComponentReference.h"
#include "../Resources/SpriteSource.h"

namespace DCEngine {

  class LinearParticleAnimator : public Component {
  public:
    
    /* Properties */
    Vec3 Force;
    Vec3 RandomForce;
    Real Torque;
    Real Growth;
    Real Dampening;

    LinearParticleAnimator(Entity& owner);
    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);

  private:

  };

}
