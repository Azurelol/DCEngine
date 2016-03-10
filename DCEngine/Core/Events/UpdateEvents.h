/******************************************************************************/
/*!
\file   UpdateEvents.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/17/2015
\brief  Update events sent by the engine.
*/
/******************************************************************************/
#pragma once
#include "EventReference.h"

namespace DCEngine {

  namespace Events {

    class LogicUpdate : public Event {
    public:
      ZilchDeclareDerivedType(LogicUpdate, Event);
      LogicUpdate() : Event("LogicUpdate") {}
      float Dt;
      float TimePassed;
      float RealTimePassed;
    };

    class FrameUpdate : public Event {
    public:
      ZilchDeclareDerivedType(FrameUpdate, Event);
      FrameUpdate() : Event("FrameUpdate") {}
      float Dt;
      float TimePassed;
      float RealTimePassed;
    };

    class PhysicsUpdate : public Event {
    public:
      ZilchDeclareDerivedType(PhysicsUpdate, Event);
      PhysicsUpdate() : Event("PhysicsUpdate") {}
      float Dt;
      float TimePassed;
      float RealTimePassed;

    };

  }
}