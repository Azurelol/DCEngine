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
// Allan was here really bored in a tech meeting, so he decided to fix the FPS issues in debug mode.
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
    ZilchDefineType(LinearParticleAnimator, "LinearParticleAnimator", DCEngineCore, builder, type) {
      // Constructor / Destructor
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(LinearParticleAnimator);
      // Properties
      DCE_BINDING_DEFINE_PROPERTY(LinearParticleAnimator, Force);
      DCE_BINDING_DEFINE_PROPERTY(LinearParticleAnimator, RandomForce);
      DCE_BINDING_DEFINE_PROPERTY(LinearParticleAnimator, Torque);
      DCE_BINDING_DEFINE_PROPERTY(LinearParticleAnimator, Growth);
      DCE_BINDING_DEFINE_PROPERTY(LinearParticleAnimator, Dampening);
    }
    #endif

    /*!************************************************************************\
    @brief  Constructor for the LinearParticleAnimator.
    \**************************************************************************/
    LinearParticleAnimator::LinearParticleAnimator(Entity & owner) : Graphical("LinearParticleAnimator", owner)
    {
			Initialize();
    }

		LinearParticleAnimator::~LinearParticleAnimator(void)
		{
		}

    /*!************************************************************************\
    @brief  Initializes the LinearParticleAnimator.
    \**************************************************************************/
    void LinearParticleAnimator::Initialize()
    {
      // Store the reference to the object's particle system component
      ParticleSystem = Owner()->getComponent<SpriteParticleSystem>();
			ParticleSystem->mLinearAnimator = this;
    }
  }

}