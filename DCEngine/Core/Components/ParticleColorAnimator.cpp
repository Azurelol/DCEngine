#include "ParticleColorAnimator.h"
#include "EngineReference.h"

namespace DCEngine {
    void ParticleColorAnimator::Serialize(Json::Value & root)
    {
    }
    void ParticleColorAnimator::Deserialize(Json::Value & root)
    {
    }

    /*!************************************************************************\
    @brief  Constructor for the ParticleColorAnimator
    \**************************************************************************/
    ParticleColorAnimator::ParticleColorAnimator(Entity & owner) : Component("ParticleColorAnimator", owner)
    {
    }

    /*!************************************************************************\
    @brief  Initializes the ParticleColorAnimator
    \**************************************************************************/
    void ParticleColorAnimator::Initialize()
    {
    }

  
}