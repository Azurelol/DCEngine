/******************************************************************************/
/*!
@file  ParticleColorAnimator.cpp
@author Chen Shu, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/23/2015
@brief
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/

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