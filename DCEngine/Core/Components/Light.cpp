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
#include "../Objects/Entities/Space.h"
#include "GraphicsSpace.h"

/**************************************************************************/
/*!
@brief Binds the ProjectionMode enum class.
*/
/**************************************************************************/
ZilchDefineExternalType(DCEngine::LightType, "LightType", DCEngine::DCEngineCore, builder, type) {
  ZilchBindEnum(builder, type, SpecialType::Enumeration);

  ZilchBindEnumValue(builder, type, DCEngine::LightType::Point, "Point");
  ZilchBindEnumValue(builder, type, DCEngine::LightType::Spot, "Spot");
  ZilchBindEnumValue(builder, type, DCEngine::LightType::Ambient, "Ambient");
}

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
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(Light);
      // Properties
      DCE_BINDING_PROPERTY_DEFINE_UNSIGNED;
      DCE_BINDING_DEFINE_PROPERTY(Light, Visible);
      DCE_BINDING_DEFINE_PROPERTY(Light, VisibilityCulling);
      DCE_BINDING_DEFINE_PROPERTY(Light, VisibilityEvents);
      DCE_BINDING_DEFINE_PROPERTY(Light, CastShadows);
			DCE_BINDING_DEFINE_PROPERTY(Light, Type);

      DCE_BINDING_DEFINE_PROPERTY(Light, Color);
      DCE_BINDING_DEFINE_PROPERTY(Light, Intensity);
      DCE_BINDING_DEFINE_PROPERTY(Light, Range);
      DCE_BINDING_DEFINE_PROPERTY(Light, Falloff);
      DCE_BINDING_DEFINE_PROPERTY(Light, DirectionVector);
			DCE_BINDING_DEFINE_PROPERTY(Light, InnerAngle);
			DCE_BINDING_DEFINE_PROPERTY(Light, OuterAngle);
    }
    #endif

    /**************************************************************************/
    /*!
    @brief  Light constructor.
    */
    /**************************************************************************/
    Light::Light(Entity & owner) : Component(std::string("Light"), owner), 
      Visible(true), CastShadows(true), Type(LightType::Point), 
      Color(1.0f, 1.0f, 1.0f, 1.0f), Range(20.0f), Intensity(1.0f), Falloff(0.1f),
      DirectionVector(Vec3(1.0f, 0.0f, 0.0f)), InnerAngle(0.6f), OuterAngle(0.8f)
    {
			Initialize();
    }

    /**************************************************************************/
    /*!
    @brief  Light destructor.
    */
    /**************************************************************************/
    Light::~Light()
    {
			SpaceRef->getComponent<GraphicsSpace>()->RemoveLightComponent(this);
    }

    /**************************************************************************/
    /*!
    @brief  Initializes the Light component.
    */
    /**************************************************************************/
    void Light::Initialize()
    {
			SpaceRef->getComponent<GraphicsSpace>()->RegisterLightComponent(this);
    }

    /**************************************************************************/
    /*!
    @brief Returns the light's current type as an integer rather than an enum.
    @return The light type, as an integer.
    */
    /**************************************************************************/
    int Light::getTypeAsInt()
    {
      switch (Type) {
      case LightType::Point:
        return 0;
        break;

      case LightType::Spot:
        return 1;
        break;

      case LightType::Ambient:
        return 2;
        break;
      }

      return -1;
    }

  }
}