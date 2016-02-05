/******************************************************************************/
/*!
@file   ParticleEmitter.cpp
@author Chen Shu, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/6/2015
@brief  The ParticleEmitter component...
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "ParticleEmitter.h"
#include "EngineReference.h"

// Particle System
#include "SpriteParticleSystem.h"

namespace DCEngine {

  namespace Components
  {

    /**************************************************************************/
    /*!
    @brief Provides the definition of this class to Zilch.
    @note This can only go in the translational unit (.cpp)
    */
    /**************************************************************************/
    #if(DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDefineType(ParticleEmitter, "ParticleEmitter", DCEngineCore, builder, type) {
      // Constructor / Destructor
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(ParticleEmitter);
      DCE_BINDING_PROPERTY_DEFINE_UNSIGNED;
      // Properties
      DCE_BINDING_DEFINE_PROPERTY(ParticleEmitter, Active);
			DCE_BINDING_DEFINE_PROPERTY(ParticleEmitter, ResetCount);
      DCE_BINDING_DEFINE_PROPERTY(ParticleEmitter, EmitCount);
      DCE_BINDING_PROPERTY_SET_UNSIGNED(propertyEmitCount);
      DCE_BINDING_DEFINE_PROPERTY(ParticleEmitter, EmitRate);
      DCE_BINDING_PROPERTY_SET_UNSIGNED(propertyEmitRate);
      DCE_BINDING_DEFINE_PROPERTY(ParticleEmitter, EmitVariance);
      DCE_BINDING_PROPERTY_SET_UNSIGNED(propertyEmitVariance);
      DCE_BINDING_DEFINE_PROPERTY(ParticleEmitter, Size);
      DCE_BINDING_DEFINE_PROPERTY(ParticleEmitter, SizeVariance);
      DCE_BINDING_DEFINE_PROPERTY(ParticleEmitter, Lifetime);
      DCE_BINDING_DEFINE_PROPERTY(ParticleEmitter, LifetimeVariance);
      DCE_BINDING_DEFINE_PROPERTY(ParticleEmitter, Spin);
      DCE_BINDING_DEFINE_PROPERTY(ParticleEmitter, SpinVariance);
      DCE_BINDING_DEFINE_PROPERTY(ParticleEmitter, StartVelocity);
      DCE_BINDING_DEFINE_PROPERTY(ParticleEmitter, RandomVelocity);
    }
    #endif

    /*!************************************************************************\
    @brief  Constructor for the ParticleEmitter.
    \**************************************************************************/
    ParticleEmitter::ParticleEmitter(Entity & owner) : Graphical("ParticleEmitter", owner),
			Active(true), EmitRate(10), EmitVariance(0), Size(1), SizeVariance(1), Lifetime(1),
			LifetimeVariance(1), Spin(100), SpinVariance(100), StartVelocity(Vec3(0,0,0)), RandomVelocity(Vec3(5,5,0))
    {
			Initialize();
    }

		ParticleEmitter::~ParticleEmitter(void)
		{
		}

    /*!************************************************************************\
    @brief  Initializes the ParticleEmitter.
    \**************************************************************************/
    void ParticleEmitter::Initialize()
    {
      // Store the reference to the object's particle system component
      ParticleSystem = Owner()->getComponent<SpriteParticleSystem>();
			ParticleSystem->mParticleEmitter = this;
    }
  }

}