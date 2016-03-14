/******************************************************************************/
/*!
@file   ZilchInterfaceUtilities.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   2/27/2016
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include "../../Binding/CoreBindingEventStrings.h"

namespace DCEngine {
    
  class Component;
  class Event;
  
  namespace Systems {

    class ZilchInterface;
    class StringArray {};
    struct ComponentData {
      Zilch::BoundType* Type;
      ZilchInterface* Interface;
    };

    // Components
    Component* GetComponent(Zilch::Call& call, Zilch::ExceptionReport& report);
    void GetNativeComponent(Zilch::Call& call, Zilch::ExceptionReport& report);
    void GetZilchComponent(Zilch::Call& call, Zilch::ExceptionReport& report);
    // Events
    Zilch::String GetEventName(Zilch::Call& call, Zilch::ExceptionReport& report);
    void GetZilchEvent(Zilch::Call& call, Zilch::ExceptionReport& report);

  }
}

