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
#include "Event.h"

namespace DCEngine {

  namespace Events {
    // Engine-wide update event
    class UpdateEvent : public Event {
    public:
      UpdateEvent() { 
        EventID = 12; 
        //trace << "UpdateEvent::UpdateEvent - Constructor, ID: " << EventID << "\n";
      } // Test

      float Dt;
      float TimePassed;
      float RealTimePassed;


    private:


    };


  }
}