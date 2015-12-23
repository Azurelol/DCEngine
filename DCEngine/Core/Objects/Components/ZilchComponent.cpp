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

  ZilchComponent::ZilchComponent(std::string name, Entity & owner) : Component(name, owner)
  {
    //if (DCE_TRACE_COMPONENT_CONSTRUCTOR)

  }
  ZilchComponent::~ZilchComponent()
  {
  }
  void ZilchComponent::Initialize()
  {
  }
}