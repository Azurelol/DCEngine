/******************************************************************************/
/*!
@file   ZilchInterfaceUtilities.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   2/27/2016
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "ZilchInterfaceUtilities.h"

#include "../../Engine/Engine.h"
#include "../../Debug/Debug.h"

namespace DCEngine {
  namespace Systems {

    /*============
    HELPERS
    =============*/
    /**************************************************************************/
    /*!
    @brief Returns a component pointer from the specified data.
    @param call A call object.
    @param report A report object.
    */
    /**************************************************************************/
    Component* GetComponent(Zilch::Call& call, Zilch::ExceptionReport& report) {
      // Grabs the object containing the boundtype
      auto& userData = call.GetFunction()->ComplexUserData.ReadObject<ComponentData>(0);
      // Grabs a pointer to the 'Entity' class
      Entity* owner = reinterpret_cast<Entity*>(call.GetHandle(Zilch::Call::This).Dereference());
      if (owner == 0)
      {
        DCException("Component not found on Object");
      }
      // Return a pointer to the component
      return owner->getComponent(userData.Type);
    }

    /**************************************************************************/
    /*!
    @brief Returns a C++ component pointer from the specified data.
    @param call A call object.
    @param report A report object.
    */
    /**************************************************************************/
    void GetNativeComponent(Zilch::Call& call, Zilch::ExceptionReport& report) {
      // Grabs the object containing the boundtype
      auto& userData = call.GetFunction()->ComplexUserData.ReadObject<ComponentData>(0);
      // Find the component pointer
      Component* foundComponent = GetComponent(call, report);
      Zilch::Handle handle;
      handle.Manager = call.GetState()->GetHandleManager<Zilch::PointerManager>();
      handle.Type = userData.Type;
      handle.Manager->ObjectToHandle((const byte*)foundComponent, handle);

      call.SetHandle(Zilch::Call::Return, handle);
    }

    /**************************************************************************/
    /*!
    @brief Returns a Zilch component pointer from the specified data.
    @param call A call object.
    @param report A report object.
    */
    /**************************************************************************/
    void GetZilchComponent(Zilch::Call& call, Zilch::ExceptionReport& report) {
      ZilchComponent* foundComponent = dynamic_cast<ZilchComponent*>(GetComponent(call, report));
      if (foundComponent) {
        call.SetHandle(Zilch::Call::Return, foundComponent->Handle());
      }
      else {
        call.SetHandle(Zilch::Call::Return, Zilch::Handle());
      }
    }

    /*========
      EVENTS
    =========*/
    Zilch::String GetEventName(Zilch::Call & call, Zilch::ExceptionReport & report)
    {
      // Grabs the object containing the boundtype
      auto& userData = call.GetFunction()->ComplexUserData.ReadObject<ComponentData>(0);
      // Grab the name of the event
      auto name = userData.Type->Name;
      // Return the name of the event
      return name;
    }

    void GetZilchEvent(Zilch::Call & call, Zilch::ExceptionReport & report)
    {
      // Grab the name of the event
      Zilch::String zilchEventName = GetEventName(call, report);
      // Return the name of the event
      call.SetHandle(Zilch::Call::Return, zilchEventName);
    }

  }
}