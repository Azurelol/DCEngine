/******************************************************************************/
/*!
@file  LinearParticleAnimator.cpp
@author Chen Shu, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/23/2015
@brief
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/

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