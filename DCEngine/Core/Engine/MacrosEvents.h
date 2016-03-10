/*****************************************************************************/
/*!
@file   MacrosEvents.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   3/3/2016
@brief  A collection of macros to alleviate rote code writing!
@note   MACROS.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once

namespace DCEngine {
  namespace Events {

    #define DCE_EVENTS_DECLARE_AND_DEFINE(EventName) \
    class EventName : public Event {                 \
      EventName() : Event("" ##EventName) {}         \
    }

    // Perhaps not that useful?
    #define DCE_EVENTS_DECLARE_LISTEN(EventClass)     \
    void On##EventClass(EventClass* event)            



    // Helpful macros for creating events :>
    //#define DCE_EVENTS_DEFINE_ENGINE_EVENT(EventName)             \
    //  class EventName : public Event {                          \    
    //}

  




  //#define DCE_EVENTS_DEFINE_DISPATCH_ENGINE_EVENT(EventClass)       \
  //    static void EventClass() {                                    \
  //      auto event = new EventClass();                              \
  //      Daisy->Dispatch<##EventClass>(event);                       \
  //      delete event;                                               \                                               
  //    }

  }
}

