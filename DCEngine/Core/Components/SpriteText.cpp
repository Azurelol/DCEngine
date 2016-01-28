/******************************************************************************/
/*!
@file   SpriteText.cpp
@author Chen Shu, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/23/2015
@brief
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "SpriteText.h"
#include "Transform.h"
#include "EngineReference.h"

namespace DCEngine {
  namespace Components {

    /**************************************************************************/
    /*!
    @brief Provides the definition of this class to Zilch.
    @note This can only go in the translational unit (.cpp)
    */
    /**************************************************************************/
    #if(DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDefineType(SpriteText, "SpriteText", DCEngineCore, builder, type) {
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(SpriteText);
      DCE_BINDING_DEFINE_RESOURCE_ATTRIBUTE(Font);
      DCE_BINDING_PROPERTY_DEFINE_UNSIGNED;
      DCE_BINDING_INTERNAL_COMPONENT_SET_ATTRIBUTE_RESOURCE;
      // Properties
      DCE_BINDING_DEFINE_PROPERTY(SpriteText, Visible);
      DCE_BINDING_DEFINE_PROPERTY(SpriteText, Color);
      DCE_BINDING_DEFINE_PROPERTY(SpriteText, Font);
      DCE_BINDING_PROPERTY_SET_ATTRIBUTE(propertyFont, attributeFont);
      DCE_BINDING_DEFINE_PROPERTY(SpriteText, FontSize);
      DCE_BINDING_PROPERTY_SET_UNSIGNED(propertyFontSize);
      DCE_BINDING_DEFINE_PROPERTY(SpriteText, Text);
      DCE_BINDING_DEFINE_PROPERTY(SpriteText, PixelsPerUnit);
      DCE_BINDING_DEFINE_PROPERTY(SpriteText, Smoothing);
			DCE_BINDING_DEFINE_PROPERTY(SpriteText, WordWrap);
    }
    #endif

    /**************************************************************************/
    /*!
    @brief SpriteText constructor.
    */
    /**************************************************************************/
    SpriteText::SpriteText(Entity & owner) : Graphical(std::string("SpriteText"), owner)
    {
      Register();
    }

    /**************************************************************************/
    /*!
    @brief SpriteText destructor. Deregisters the SpriteText.
    */
    /**************************************************************************/
    SpriteText::~SpriteText()
    {
      // Deregister this component from the GraphicsSpace
      SpaceRef->getComponent<GraphicsSpace>()->RemoveSpriteText(*this);
    }

    /**************************************************************************/
    /*!
    @brief Registers the SpriteText to the graphics system.
    */
    /**************************************************************************/
    void SpriteText::Register()
    {
      // Store the reference to this owner's Transform component
      TransformComponent = dynamic_cast<GameObject*>(Owner())->getComponent<Components::Transform>();
      // Subscribe this component to the graphics space
      SpaceRef->getComponent<Components::GraphicsSpace>()->AddSpriteText(*this);
    }

    void SpriteText::Initialize()
    {
    }

  }

}