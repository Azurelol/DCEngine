/******************************************************************************/
/*!
@file   Light.cpp
@author William Mao, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   1/20/2015
@brief  Light component for dynamic scene lightning.

@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Light.h"

namespace DCEngine {
  namespace Components {

    /**************************************************************************/
    /*!
    @brief Provides the definition of this class to Zilch.
    @note This can only go in the translational unit (.cpp)
    */
    /**************************************************************************/
    #if(DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDefineType(Light, "Light", DCEngineCore, builder, type) {
      // Constructor / Destructor
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(Light);
      // Properties
      DCE_BINDING_DEFINE_PROPERTY(Light, Visible);
      DCE_BINDING_DEFINE_PROPERTY(Light, VisibilityCulling);
      DCE_BINDING_DEFINE_PROPERTY(Light, VisibilityEvents);
      DCE_BINDING_DEFINE_PROPERTY(Light, CastShadows);

      //DCE_BINDING_DEFINE_PROPERTY(Light, LightType);

      DCE_BINDING_DEFINE_PROPERTY(Light, Color);
      DCE_BINDING_DEFINE_PROPERTY(Light, Intensity);
      DCE_BINDING_DEFINE_PROPERTY(Light, Range);
      DCE_BINDING_DEFINE_PROPERTY(Light, Falloff);
      DCE_BINDING_DEFINE_PROPERTY(Light, Angle);
      DCE_BINDING_DEFINE_PROPERTY(Light, Size);
    }
    #endif

    /**************************************************************************/
    /*!
    @brief  Light constructor.
    */
    /**************************************************************************/
    Light::Light(Entity & owner) : Component(std::string("Light"), owner)
    {
    }

    /**************************************************************************/
    /*!
    @brief  Light destructor.
    */
    /**************************************************************************/
    Light::~Light()
    {
    }

    /**************************************************************************/
    /*!
    @brief  Initializes the Light component.
    */
    /**************************************************************************/
    void Light::Initialize()
    {
    }

  }
}