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
  
  class Object;

  namespace Systems {



    class DispatchSystemEvents {
    public:

      static void EngineInitialized();
      static void EngineResume();
      static void EnginePause();
      static void EngineExit();
      static void EngineSaveConfigurations();
      
      static void EditorInitialize();
      static void EditorSelectObject(Object* object);
      static void EditorDeselectObject();
      static void EditorSave();
      static void EditorRebuildZilchComponents();
      
      static void SetWindowCaption(std::string name);
      static void WindowLostFocus();
      static void WindowGainedFocus();
      
      static void GraphicsCompileShaders();
      static void GraphicsToggleLightning();

      static void ScriptingCompile();
      static void ScriptingLibraryPatched();
      static void ScriptingErrorMessage(const std::string& message);
      

    };

  }

 

}