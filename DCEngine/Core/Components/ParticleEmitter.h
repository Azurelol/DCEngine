/******************************************************************************/
/*!
@file   ParticleEmitter.h
@author Chen Shu, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/6/2015
@brief  The ParticleEmitter component...
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include "ComponentReference.h"

namespace DCEngine {
  namespace Components
  {
    class SpriteParticleSystem;
    class ParticleEmitter : public Component {
    public:

      /* Properties */
      Boolean Active;
      Integer EmitCount; // Number of particles to emit per reset
      Integer EmitRate; // Rate that particles spawn per second
      Integer EmitVariance; // How much emit can vary per sample    
      Real Size; // Size of each particle spawned
      Real SizeVariance; // How much the emit can vary per sample
      Real Lifetime; // How much of a particle's starting lifetime is
      Real LifetimeVariance; // How much lifetime can vary per particle
      Real Spin; // Speed in rads per second of the particle
      Real SpinVariance; // How much speed per rad can the particle vary
      Vec3 StartVelocity; // Velocity of each particle at start
      Vec3 RandomVelocity; // Random velocity per particle

      SpriteParticleSystem* ParticleSystem;

      ParticleEmitter(Entity& owner);
      void Initialize();
      virtual void Serialize(Json::Value& root);
      virtual void Deserialize(Json::Value& root);

    private:

    };
  }

}
