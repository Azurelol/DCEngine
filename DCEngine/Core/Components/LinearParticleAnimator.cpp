#include "LinearParticleAnimator.h"
#include "EngineReference.h"

namespace DCEngine {

  void LinearParticleAnimator::Serialize(Json::Value & root)
  {
  }
  void LinearParticleAnimator::Deserialize(Json::Value & root)
  {
  }

  /*!************************************************************************\
  @brief  Constructor for the LinearParticleAnimator.
  \**************************************************************************/
  LinearParticleAnimator::LinearParticleAnimator(Entity & owner) : Component("LinearParticleAnimator", owner)
  {

  }

  /*!************************************************************************\
  @brief  Initializes the LinearParticleAnimator.
  \**************************************************************************/
  void LinearParticleAnimator::Initialize()
  {

  }

}