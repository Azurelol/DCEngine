/*****************************************************************************/
/*!
@file   DispatchSystemEvents.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   12/02/2015
@note   This file includes all the headers for the components that Rebound
uses.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "DispatchSystemEvents.h"
#include "../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {

    void DispatchSystemEvents::EngineInitialized()
    {
      auto resume = new Events::EngineInitialized();
      Daisy->Dispatch<Events::EngineInitialized>(resume);
      delete resume;
    }

    void DispatchSystemEvents::EngineResume()
    {
      auto resume = new Events::EngineResume();
      Daisy->Dispatch<Events::EngineResume>(resume);
      delete resume;
    }

    void DispatchSystemEvents::EnginePause()
    {
      auto pause = new Events::EnginePause();
      Daisy->Dispatch<Events::EnginePause>(pause);
      delete pause;
    }    

    void DispatchSystemEvents::EngineExit()
    {
      auto exitEvent = new Events::EngineExit();
      Daisy->Dispatch<Events::EngineExit>(exitEvent);
      delete exitEvent;
    }

    void DispatchSystemEvents::EditorSelectObject(Object * object)
    {
      auto event = new Events::EditorSelectObject;
      //event->SelectedObject = object;
      Daisy->Dispatch<Events::EditorSelectObject>(event);
      delete event;
    }

    void DispatchSystemEvents::EditorDeselectObject()
    {
      auto event = new Events::EditorDeselectObject;
      Daisy->Dispatch<Events::EditorDeselectObject>(event);
      delete event;
    }

    void DispatchSystemEvents::SetWindowCaption(std::string caption)
    {
      auto event = new Events::SetWindowCaption;      
      event->Caption = caption;
      Daisy->Dispatch<Events::SetWindowCaption>(event);
      delete event;
    }

    void DispatchSystemEvents::WindowLostFocus()
    {
      auto event = new Events::WindowLostFocus();
      Daisy->Dispatch<Events::WindowLostFocus>(event);
      delete event;
    }

    void DispatchSystemEvents::WindowGainedFocus()
    {
      auto event = new Events::WindowGainedFocus();
      Daisy->Dispatch<Events::WindowGainedFocus>(event);
      delete event;
    }


  }
}


