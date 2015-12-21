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
#include "LinearParticleAnimator.h"
#include "EngineReference.h"

namespace DCEngine {
  namespace Components
  {
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

}