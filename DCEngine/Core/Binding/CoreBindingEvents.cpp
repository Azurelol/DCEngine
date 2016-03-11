/******************************************************************************/
/*!
@file   CoreBindingEvents.cpp
@author Gabriel Neumann
@par    email: g.neumann\@digipen.edu
@date   2/27/2016
@brief  Binds events used in the engine so that Zilch can convert to them and
back.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "CoreBindingEvents.h"
#include "../EventsInclude.h"
#include "../Objects/Entities/GameObject.h"

namespace DCEngine {


  namespace Events {

    /********************/
    //Game Events
    /********************/
    ZilchDefineType(GameFocusIn, "GameFocusIn", DCEngineCore, builder, type)
    {
    }
    ZilchDefineType(GameFocusOut, "GameFocusOut", DCEngineCore, builder, type)
    {
    }
    ZilchDefineType(GameLoad, "GameLoad", DCEngineCore, builder, type)
    {
    }
    ZilchDefineType(GameRequestQuit, "GameRequestQuit", DCEngineCore, builder, type)
    {
    }
    ZilchDefineType(GameStarted, "GameStarted", DCEngineCore, builder, type)
    {
    }
    ZilchDefineType(GameEnded, "GameEnded", DCEngineCore, builder, type)
    {
    }
    ZilchDefineType(GameSetup, "GameSetup", DCEngineCore, builder, type)
    {
    }

    /********************/
    //  Key Events
    /********************/

    ZilchDefineType(KeyDown, "KeyDown", DCEngineCore, builder, type)
    {
      ZilchBindField(builder, type, &KeyDown::Key, "Key", Zilch::PropertyBinding::Get);
      ZilchBindField(builder, type, &KeyDown::ShiftPressed, "ShiftPressed", Zilch::PropertyBinding::Get);
    }
    ZilchDefineType(KeyUp, "KeyUp", DCEngineCore, builder, type)
    {
      ZilchBindField(builder, type, &KeyUp::Key, "Key", Zilch::PropertyBinding::Get);
      ZilchBindField(builder, type, &KeyUp::ShiftPressed, "ShiftPressed", Zilch::PropertyBinding::Get);
    }

    /********************/
    //  Mouse Events
    /********************/
    ZilchDefineType(MouseDown, "MouseDown", DCEngineCore, builder, type)
    {
      ZilchBindField(builder, type, &MouseDown::Position, "Position", Zilch::PropertyBinding::Get);
      ZilchBindField(builder, type, &MouseDown::ButtonPressed, "ButtonPressed", Zilch::PropertyBinding::Get);
    }
    ZilchDefineType(MouseUp, "MouseUp", DCEngineCore, builder, type)
    {
      ZilchBindField(builder, type, &MouseUp::Position, "Position", Zilch::PropertyBinding::Get);
      ZilchBindField(builder, type, &MouseUp::ButtonReleased, "ButtonReleased", Zilch::PropertyBinding::Get);
    }
    ZilchDefineType(MouseEnter, "MouseEnter", DCEngineCore, builder, type)
    {
    }
    ZilchDefineType(MouseExit, "MouseExit", DCEngineCore, builder, type)
    {
    }
    ZilchDefineType(MouseUpdate, "MouseUpdate", DCEngineCore, builder, type)
    {
      ZilchBindField(builder, type, &MouseUpdate::ScreenPosition, "ScreenPosition", Zilch::PropertyBinding::Get);
    }
    ZilchDefineType(MouseClickedOn, "MouseClickedOn", DCEngineCore, builder, type)
    {
    }
    ZilchDefineType(MouseScroll, "MouseScroll", DCEngineCore, builder, type)
    {
      ZilchBindField(builder, type, &MouseScroll::Delta, "Delta", Zilch::PropertyBinding::Get);
      ZilchBindField(builder, type, &MouseScroll::Direction, "Direction", Zilch::PropertyBinding::Get);
    }

    /********************/
    // Collision Events
    /********************/
    ZilchDefineType(CollisionStarted, "CollisionStarted", DCEngineCore, builder, type)
    {
      ZilchBindField(builder, type, &CollisionStarted::Object, "Object", Zilch::PropertyBinding::Get);
      ZilchBindField(builder, type, &CollisionStarted::OtherObject, "OtherObject", Zilch::PropertyBinding::Get);
      ZilchBindField(builder, type, &CollisionStarted::IsGhost, "IsGhost", Zilch::PropertyBinding::Get);
    }
    ZilchDefineType(CollisionEnded, "CollisionEnded", DCEngineCore, builder, type)
    {
      ZilchBindField(builder, type, &CollisionEnded::Object, "Object", Zilch::PropertyBinding::Get);
      ZilchBindField(builder, type, &CollisionEnded::OtherObject, "OtherObject", Zilch::PropertyBinding::Get);
      ZilchBindField(builder, type, &CollisionEnded::IsGhost, "IsGhost", Zilch::PropertyBinding::Get);
    }
    ZilchDefineType(CollisionPersisted, "CollisionPersisted", DCEngineCore, builder, type)
    {
      ZilchBindField(builder, type, &CollisionPersisted::Object, "Object", Zilch::PropertyBinding::Get);
      ZilchBindField(builder, type, &CollisionPersisted::OtherObject, "OtherObject", Zilch::PropertyBinding::Get);
      ZilchBindField(builder, type, &CollisionPersisted::IsGhost, "IsGhost", Zilch::PropertyBinding::Get);
    }

    /********************/
    //  Update Events
    /********************/
    ZilchDefineType(LogicUpdate, "LogicUpdate", DCEngineCore, builder, type)
    {
      ZilchBindField(builder, type, &LogicUpdate::Dt, "Dt", Zilch::PropertyBinding::Get);
      ZilchBindField(builder, type, &LogicUpdate::TimePassed, "TimePassed", Zilch::PropertyBinding::Get);
      ZilchBindField(builder, type, &LogicUpdate::RealTimePassed, "RealTimePassed", Zilch::PropertyBinding::Get);
    }
    ZilchDefineType(FrameUpdate, "FrameUpdate", DCEngineCore, builder, type)
    {
      ZilchBindField(builder, type, &FrameUpdate::Dt, "Dt", Zilch::PropertyBinding::Get);
      ZilchBindField(builder, type, &FrameUpdate::TimePassed, "TimePassed", Zilch::PropertyBinding::Get);
      ZilchBindField(builder, type, &FrameUpdate::RealTimePassed, "RealTimePassed", Zilch::PropertyBinding::Get);
    }
    ZilchDefineType(PhysicsUpdate, "PhysicsUpdate", DCEngineCore, builder, type)
    {
      ZilchBindField(builder, type, &PhysicsUpdate::Dt, "Dt", Zilch::PropertyBinding::Get);
      ZilchBindField(builder, type, &PhysicsUpdate::TimePassed, "TimePassed", Zilch::PropertyBinding::Get);
      ZilchBindField(builder, type, &PhysicsUpdate::RealTimePassed, "RealTimePassed", Zilch::PropertyBinding::Get);
    }



  }
}