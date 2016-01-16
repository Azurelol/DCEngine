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
    void ParticleEmitter::Serialize(Json::Value & root)
    {
    }
    void ParticleEmitter::Deserialize(Json::Value & root)
    {
    }

    /*!************************************************************************\
    @brief  Constructor for the ParticleEmitter.
    \**************************************************************************/
    ParticleEmitter::ParticleEmitter(Entity & owner) : Component("BoxParticleEmitter", owner)
    {
			SpaceRef->getComponent<Components::GraphicsSpace>()->AddParticleEmitter(*this);
    }

    /*!************************************************************************\
    @brief  Initializes the ParticleEmitter.
    \**************************************************************************/
    void ParticleEmitter::Initialize()
    {
      // Store the reference to the object's particle system component
      ParticleSystem = Owner()->getComponent<SpriteParticleSystem>();
    }
  }

}