/******************************************************************************/
/*!
@file   GameEvents.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/30/2015
@brief  These are internal events from the engine.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include "EventReference.h"

namespace DCEngine {

  // Forward declarations
  class Entity;
  class Space;
  class GameSession;

  namespace Events {

    class ObjectInitializer : public Event {
    public:
      ObjectInitializer() : Event("ObjectInitializer") {}
      Entity* Parent;
      Space* Space;
      GameSession* GameSession;
      ZilchDeclareDerivedType(ObjectInitializer, Event);
    };

    class AllObjectsInitialized : public Event {
    public:
      AllObjectsInitialized() : Event("AllObjectsInitialized") {}
      ZilchDeclareDerivedType(AllObjectsInitialized, Event);
    };

  }
}