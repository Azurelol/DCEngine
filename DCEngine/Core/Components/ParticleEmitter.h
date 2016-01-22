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
      // Properties
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
      DCE_DEFINE_PROPERTY(Boolean, Active);
      DCE_DEFINE_PROPERTY(Integer, EmitCount);
      DCE_DEFINE_PROPERTY(Integer, EmitRate);
      DCE_DEFINE_PROPERTY(Integer, EmitVariance);
      DCE_DEFINE_PROPERTY(Real, Size);
      DCE_DEFINE_PROPERTY(Real, SizeVariance);
      DCE_DEFINE_PROPERTY(Real, Lifetime);
      DCE_DEFINE_PROPERTY(Real, LifetimeVariance);
      DCE_DEFINE_PROPERTY(Real, Spin);
      DCE_DEFINE_PROPERTY(Real, SpinVariance);
      DCE_DEFINE_PROPERTY(Vec3, StartVelocity);
      DCE_DEFINE_PROPERTY(Vec3, RandomVelocity);


      ZilchDeclareDerivedType(ParticleEmitter, Component);
      ParticleEmitter(Entity& owner);
      void Initialize();

    private:
      SpriteParticleSystem* ParticleSystem;

    };
  }

}
