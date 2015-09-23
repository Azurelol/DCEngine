/******************************************************************************/
/*!
\file   Event.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/17/2015
\brief  Update event that contains current time and delta time.

*/
/******************************************************************************/
#pragma once
#include "EventReference.h"

namespace DCEngine {

  namespace Events {
    // Engine-wide update event
    class LogicUpdate : public Event {
    public:
      LogicUpdate() {}

      float Dt;
      float TimePassed;
      float RealTimePassed;


    private:


    };


  }
}