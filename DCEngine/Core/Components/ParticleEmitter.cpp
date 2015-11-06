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