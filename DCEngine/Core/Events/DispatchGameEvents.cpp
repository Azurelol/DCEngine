/*****************************************************************************/
/*!
@file   DispatchGameEvents.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   12/02/2015
@note   This file includes all the headers for the components that Rebound
uses.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "DispatchGameEvents.h"
#include "../Engine/Engine.h"

namespace DCEngine {
  
  void DispatchGameEvents::GameFocusIn()
  {
    auto event = new Events::GameFocusIn();
    Daisy->getGameSession()->Dispatch<Events::GameFocusIn>(event);
    delete event;
  }

  void DispatchGameEvents::GameFocusOut()
  {
    auto event = new Events::GameFocusOut();
    Daisy->getGameSession()->Dispatch<Events::GameFocusOut>(event);
    delete event;
  }

  void DispatchGameEvents::GameLoad()
  {
    auto event = new Events::GameLoad();
    Daisy->getGameSession()->Dispatch<Events::GameLoad>(event);
    delete event;
  }
  void DispatchGameEvents::GameRequestQuit()
  {
    auto event = new Events::GameRequestQuit();
    Daisy->getGameSession()->Dispatch<Events::GameRequestQuit>(event);
    delete event;
  }
  void DispatchGameEvents::GameStarted()
  {
    auto event = new Events::GameStarted();
    Daisy->getGameSession()->Dispatch<Events::GameStarted>(event);
    delete event;
  }

  void DispatchGameEvents::GameEnded()
  {
    auto event = new Events::GameEnded();
    Daisy->getGameSession()->Dispatch<Events::GameEnded>(event);
    delete event;
  }
  
  void DispatchGameEvents::GameSetup()
  {
    auto event = new Events::GameSetup();
    Daisy->getGameSession()->Dispatch<Events::GameSetup>(event);
    delete event;
  }
}


