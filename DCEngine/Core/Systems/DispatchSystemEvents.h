/*****************************************************************************/
/*!
@file   DispatchSystemEvents.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   12/01/2015
@note   This file includes all the headers for the components that Rebound
uses.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once

namespace DCEngine {
  namespace Systems {

    class DispatchSystemEvents {
    public:

      static void EngineResume();
      static void EnginePause();
      static void EngineExit();
      
      static void SetWindowCaption(std::string name);
      static void WindowLostFocus();
      static void WindowGainedFocus();

    };

  }

 

}