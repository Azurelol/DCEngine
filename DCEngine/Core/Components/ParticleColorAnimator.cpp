/******************************************************************************/
/*!
@file   ParticleColorAnimator.cpp
@author Chen Shu, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/6/2015
@brief  The ParticleColorAnimator component...
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "ParticleColorAnimator.h"
#include "EngineReference.h"

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
    ZilchDefineType(ParticleColorAnimator, "ParticleColorAnimator", DCEngineCore, builder, type) {
      // Constructor / Destructor
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(ParticleColorAnimator);
      // Properties
      DCE_BINDING_PROPERTY_DEFINE_RANGE(Color, 0, 1);
      DCE_BINDING_DEFINE_PROPERTY(ParticleColorAnimator, Color0);
      DCE_BINDING_PROPERTY_SET_ATTRIBUTE(propertyColor0, attributeRangeColor);
      DCE_BINDING_DEFINE_PROPERTY(ParticleColorAnimator, Color1);
      DCE_BINDING_PROPERTY_SET_ATTRIBUTE(propertyColor1, attributeRangeColor);
      DCE_BINDING_DEFINE_PROPERTY(ParticleColorAnimator, Color2);
      DCE_BINDING_PROPERTY_SET_ATTRIBUTE(propertyColor2, attributeRangeColor);
      DCE_BINDING_DEFINE_PROPERTY(ParticleColorAnimator, Color3);
      DCE_BINDING_PROPERTY_SET_ATTRIBUTE(propertyColor3, attributeRangeColor);
      DCE_BINDING_DEFINE_PROPERTY(ParticleColorAnimator, Color4);
      DCE_BINDING_PROPERTY_SET_ATTRIBUTE(propertyColor4, attributeRangeColor);
    }
    #endif

    /*!************************************************************************\
    @brief  Constructor for the ParticleColorAnimator
    \**************************************************************************/
    ParticleColorAnimator::ParticleColorAnimator(Entity & owner) : Graphical("ParticleColorAnimator", owner),
			Color0(Vec4(0,0,0,.5)), Color1(Vec4(0, 0, 0, .5)), Color2(Vec4(0, 0, 0, .5)),
			Color3(Vec4(0, 0, 0, .5)), Color4(Vec4(0, 0, 0, .5))
    {
			Initialize();
    }

		ParticleColorAnimator::~ParticleColorAnimator(void)
		{
		}

    /*!************************************************************************\
    @brief  Initializes the ParticleColorAnimator
    \**************************************************************************/
    void ParticleColorAnimator::Initialize()
    {
      // Store the reference to the object's particle system component
      ParticleSystem = Owner()->getComponent<SpriteParticleSystem>();
			if(ParticleSystem)
				ParticleSystem->mColorAnimator = this;
    }


  }
}