/******************************************************************************/
/*!
@file  ParticleEmitter.cpp
@author Chen Shu, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/23/2015
@brief
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/

#include "ParticleEmitter.h"
#include "EngineReference.h"

// Particle System
#include "SpriteParticleSystem.h"

namespace DCEngine {

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