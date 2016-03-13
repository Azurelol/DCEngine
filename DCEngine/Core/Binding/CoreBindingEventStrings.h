/******************************************************************************/
/*!
@file   CoreBindingEvents.cpp
@author Christian Sagel
@par    email: g.neumann\@digipen.edu
@date   2/27/2016
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once

namespace DCEngine {

  #define DCE_EVENTS_DECLARE_PROPERTY(EventName) \
  static std::string EventName() {               \
    return "" #EventName;                        \
  }

  #define DCE_EVENTS_DEFINE_PROPERTY(EventName) \
  ZilchBindProperty(builder, type, &EventStrings::EventName, ZilchNoSetter, "" #EventName, ZilchNoNames)->IsStatic = true;

  /**************************************************************************/
  /*!
  @class EventStrings Provides a quick interface for proving core engine events
  to Zilch, as strings.
  */
  /**************************************************************************/
  class EventStrings : public Zilch::IZilchObject {
  public:
    ZilchDeclareBaseType(EventStrings, Zilch::TypeCopyMode::ReferenceType);
    // Game Events
    DCE_EVENTS_DECLARE_PROPERTY(GameFocusIn);
    DCE_EVENTS_DECLARE_PROPERTY(GameFocusOut);
    DCE_EVENTS_DECLARE_PROPERTY(GameLoad);
    DCE_EVENTS_DECLARE_PROPERTY(GameRequestQuit);
    DCE_EVENTS_DECLARE_PROPERTY(GameStarted);
    DCE_EVENTS_DECLARE_PROPERTY(GameEnded);
    DCE_EVENTS_DECLARE_PROPERTY(GameSetup);
    // Input Events
    DCE_EVENTS_DECLARE_PROPERTY(KeyDown);
    DCE_EVENTS_DECLARE_PROPERTY(KeyUp);
    DCE_EVENTS_DECLARE_PROPERTY(MouseDown);
    DCE_EVENTS_DECLARE_PROPERTY(MouseUp);
    DCE_EVENTS_DECLARE_PROPERTY(MouseExit);
    DCE_EVENTS_DECLARE_PROPERTY(MouseUpdate);
    DCE_EVENTS_DECLARE_PROPERTY(MouseClickedOn);
    DCE_EVENTS_DECLARE_PROPERTY(MouseScroll);
    // Physics Events
    DCE_EVENTS_DECLARE_PROPERTY(CollisionStarted);
    DCE_EVENTS_DECLARE_PROPERTY(CollisionEnded);
    DCE_EVENTS_DECLARE_PROPERTY(CollisionPersisted);
    // Update Events
    DCE_EVENTS_DECLARE_PROPERTY(LogicUpdate);
    DCE_EVENTS_DECLARE_PROPERTY(FrameUpdate);
    DCE_EVENTS_DECLARE_PROPERTY(PhysicsUpdate);
  };

}