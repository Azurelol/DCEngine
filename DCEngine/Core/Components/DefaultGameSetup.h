/*****************************************************************************/
/*!
\file   DefaultGameSetup.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   4/9/2016
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include "ComponentReference.h"

namespace DCEngine {
  namespace Components {

    // Forward reference
    class SpriteText;
    class Light;

    class DefaultGameSetup : public Component {
    public:
      ZilchDeclareDerivedType(DefaultGameSetup, Component);
      DefaultGameSetup(Entity& owner);
      ~DefaultGameSetup();
      void Initialize();
      void Subscribe();
      // Properties
      DCE_DEFINE_PROPERTY(String, StartingSpace);
      DCE_DEFINE_PROPERTY(String, StartingLevel);

    private:
      String StartingSpace;
      String StartingLevel;
      // Events
      void OnGameFocusInEvent(Events::GameFocusIn* event);
      void OnGameFocusOutEvent(Events::GameFocusOut* event);
      void OnGameLoadEvent(Events::GameLoad* event);
      void OnGameRequestQuitEvent(Events::GameRequestQuit* event);
      void OnGameStartedEvent(Events::GameStarted* event);
      void OnGameEndedEvent(Events::GameEnded * event);


    };

  }
}