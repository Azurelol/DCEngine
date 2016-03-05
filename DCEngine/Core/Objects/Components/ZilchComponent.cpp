/*****************************************************************************/
/*!
\file   ZilchComponent.cpp
\author Christian Sagel, Connor Tilley
\par    email: c.sagel\@digipen.edu
\date   9/22/2015
\brief  Zilch components are used by gameplay programmers to write game logic into.
These components, alongside events, drive the logic of a game project.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "ZilchComponent.h"

namespace DCEngine {

  /**************************************************************************/
  /*!
  @brief Checks if this is a Zilch Component.
  @param component A pointer to the component.
  @return True if a Zilch component, false otherwise.
  */
  /**************************************************************************/
  bool ZilchComponent::IsZilchComponent(ComponentPtr component)
  {
    if (Zilch::TypeBinding::IsA(component->ZilchGetStaticType(), ZilchComponent::ZilchGetStaticType()))
      return false;
    return true;
  }

  /**************************************************************************/
  /*!
  @brief Default constructor for Zilch components.
  @param name The name of the component.
  @param owner A reference to the owner of this component.
  */
  /**************************************************************************/
  ZilchComponent::ZilchComponent(std::string name, Entity & owner) : Component(name, owner)
  {
    //if (DCE_TRACE_COMPONENT_CONSTRUCTOR)
  }

  /**************************************************************************/
  /*!
  @brief ZilchComponent destructor.
  */
  /**************************************************************************/
  ZilchComponent::~ZilchComponent()
  {
  }

  /**************************************************************************/
  /*!
  @brief Initializes the Zilch component.
  */
  /**************************************************************************/
  void ZilchComponent::Initialize()
  {
  }

  /**************************************************************************/
  /*!
  @brief ZilchComponent default constructor
  */
  /**************************************************************************/
  ZilchComponent::ZilchComponent()
  {
  }
}