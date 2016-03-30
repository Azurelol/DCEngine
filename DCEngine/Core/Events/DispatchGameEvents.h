/*****************************************************************************/
/*!
@file   DispatchSystemEvents.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   10/01/2015
@note   This file includes all the headers for the components that Rebound
uses.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once

namespace DCEngine {

  // Forward declarations
  class Entity;
  class Space;
  class GameSession;

    class DispatchGameEvents {
    public:      
      static void ObjectInitializer(Entity* entity, Entity* parent, Space* space, GameSession* gameSession);
      static void AllObjectsInitialized(Entity* entity);

      // When the window gains focus
      static void GameFocusIn();
      // When the window loses focus
      static void GameFocusOut();
      // When the game is loaded
      static void GameLoad();
      // When the game wishes to quit
      static void GameRequestQuit();
      // When the game is started
      static void GameStarted();
      // When the game is ended
      static void GameEnded();
      // When ???
      static void GameSetup();

    };

}