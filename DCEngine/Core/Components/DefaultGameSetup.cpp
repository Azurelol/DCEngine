/*****************************************************************************/
/*!
\file   DefaultGameSetup.cpp
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   4/9/2016
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "DefaultGameSetup.h"

#include "EngineReference.h"

namespace DCEngine {
  namespace Components {

    /**************************************************************************/
    /*!
    \brief DefaultGameSetup type definition.
    */
    /**************************************************************************/
    ZilchDefineType(DefaultGameSetup, "DefaultGameSetup", DCEngineCore, builder, type) {
      DCE_BINDING_DEFINE_RESOURCE_ATTRIBUTE(Level);
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(DefaultGameSetup);
      DCE_BINDING_DEFINE_PROPERTY(DefaultGameSetup, StartingSpace);
      DCE_BINDING_DEFINE_PROPERTY(DefaultGameSetup, StartingLevel);
      DCE_BINDING_PROPERTY_SET_RESOURCE_ATTRIBUTE(propertyStartingLevel, attributeLevel);
    }

    DefaultGameSetup::DefaultGameSetup(Entity & owner) : Component(std::string("DefaultGameSetup"), owner)
    {
    }

    DefaultGameSetup::~DefaultGameSetup()
    {
    }

    void DefaultGameSetup::Initialize()
    {
      Subscribe();
    }

    void DefaultGameSetup::Subscribe()
    {
      Daisy->Connect<Events::GameRequestQuit>(Owner(), &DefaultGameSetup::OnGameRequestQuitEvent, this);
      Daisy->Connect<Events::GameFocusIn>(Owner(), &DefaultGameSetup::OnGameFocusInEvent, this);      
      Daisy->Connect<Events::GameFocusOut>(Owner(), &DefaultGameSetup::OnGameFocusOutEvent, this);
      Daisy->Connect<Events::GameStarted>(Owner(), &DefaultGameSetup::OnGameStartedEvent, this);
      Daisy->Connect<Events::GameEnded>(Owner(), &DefaultGameSetup::OnGameEndedEvent, this);
      Daisy->Connect<Events::GameLoad>(Owner(), &DefaultGameSetup::OnGameLoadEvent, this);
    }

    void DefaultGameSetup::OnGameFocusInEvent(Events::GameFocusIn * event)
    {
    }

    void DefaultGameSetup::OnGameFocusOutEvent(Events::GameFocusOut * event)
    {
    }

    void DefaultGameSetup::OnGameLoadEvent(Events::GameLoad * event)
    {
    }

    void DefaultGameSetup::OnGameRequestQuitEvent(Events::GameRequestQuit * event)
    {
      Systems::DispatchSystemEvents::EngineExit();
    }

    void DefaultGameSetup::OnGameStartedEvent(Events::GameStarted * event)
    {
    }

    void DefaultGameSetup::OnGameEndedEvent(Events::GameEnded * event)
    {
    }
  }
}