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

    void DispatchSystemEvents::EngineSaveConfigurations()
    {
      auto exitEvent = new Events::EngineSaveConfigurations();
      Daisy->Dispatch<Events::EngineSaveConfigurations>(exitEvent);
      delete exitEvent;
    }

    void DispatchSystemEvents::EditorInitialize(Editor& editor)
    {
      auto event = new Events::EditorInitialize;
      event->Reference = &editor;
      Daisy->Dispatch<Events::EditorInitialize>(event);
      delete event;
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

    void DispatchSystemEvents::EditorSave()
    {
      auto event = new Events::EditorSave;
      Daisy->Dispatch<Events::EditorSave>(event);
      delete event;
    }

    void DispatchSystemEvents::EditorRebuildZilchComponents()
    {
      auto event = new Events::EditorRebuildZilchComponents;
      Daisy->Dispatch<Events::EditorRebuildZilchComponents>(event);
      delete event;
    }

    void DispatchSystemEvents::SetWindowCaption(std::string caption)
    {
      auto event = new Events::SetWindowCaption;      
      event->Caption = caption;
      Daisy->Dispatch<Events::SetWindowCaption>(event);
      delete event;
    }

    void DispatchSystemEvents::WindowResize(const Vec2 & dimensions)
    {
      auto event = new Events::WindowResize;
      event->Dimensions = dimensions;
      Daisy->Dispatch<Events::WindowResize>(event);
      delete event;
    }

		void DispatchSystemEvents::WindowRecreate()
		{
			auto event = new Events::WindowRecreate;
			Daisy->Dispatch<Events::WindowRecreate>(event);
			delete event;
		}

    void DispatchSystemEvents::WindowFullScreenToggle(bool toggled)
    {
      auto event = new Events::WindowFullScreenToggle();
      event->Enabled = toggled;
      Daisy->Dispatch<Events::WindowFullScreenToggle>(event);
      delete event;
    }

    void DispatchSystemEvents::WindowFullScreenEnabled()
    {
      auto event = new Events::WindowFullScreenEnabled();
      Daisy->Dispatch<Events::WindowFullScreenEnabled>(event);
      delete event;
    }

    void DispatchSystemEvents::WindowFullScreenDisabled()
    {
      auto event = new Events::WindowFullScreenDisabled();
      Daisy->Dispatch<Events::WindowFullScreenDisabled>(event);
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

		void DispatchSystemEvents::WindowResizeToNative()
		{
			auto event = new Events::WindowResizeToNative();
			Daisy->Dispatch<Events::WindowResizeToNative>(event);
			delete event;
		}

    void DispatchSystemEvents::GraphicsCompileShaders()
    {
      auto event = new Events::GraphicsCompileShaders();
      Daisy->Dispatch<Events::GraphicsCompileShaders>(event);
      delete event;
    }

    void DispatchSystemEvents::GraphicsCompileShadersError(const std::string& message)
    {
      auto event = new Events::GraphicsCompileShadersError();
      event->ErrorMessage = message;
      Daisy->Dispatch<Events::GraphicsCompileShadersError>(event);
      delete event;
    }

    void DispatchSystemEvents::GraphicsToggleLightning()
    {
      auto event = new Events::GraphicsToggleLightning();
      Daisy->Dispatch<Events::GraphicsToggleLightning>(event);
      delete event;
    }

    void DispatchSystemEvents::ScriptingCompile()
    {
      auto event = new Events::ScriptingCompileScripts();
      Daisy->Dispatch<Events::ScriptingCompileScripts>(event);
      delete event;
    }

    void DispatchSystemEvents::ScriptingLibraryPatched()
    {
      auto event = new Events::ScriptingLibraryPatched();
      Daisy->Dispatch<Events::ScriptingLibraryPatched>(event);
      delete event;
    }

    void DispatchSystemEvents::ScriptingLibraryAboutToCompile()
    {
      auto event = new Events::ScriptingLibraryAboutToCompile();
      Daisy->Dispatch<Events::ScriptingLibraryAboutToCompile>(event);
      delete event;
    }

    void DispatchSystemEvents::ScriptingLibraryCompiled()
    {
      auto event = new Events::ScriptingLibraryCompiled();
      Daisy->Dispatch<Events::ScriptingLibraryCompiled>(event);
      delete event;
    }

    void DispatchSystemEvents::ScriptingLibraryCompilationFailure()
    {
      auto event = new Events::ScriptingLibraryCompilationFailure();
      Daisy->Dispatch<Events::ScriptingLibraryCompilationFailure>(event);
      delete event;
    }

    void DispatchSystemEvents::ScriptingErrorMessage(const std::string & message)
    {
      auto event = new Events::ScriptingErrorMessage();
      event->Message = message;
      Daisy->Dispatch<Events::ScriptingErrorMessage>(event);
      delete event;
    }

    void DispatchSystemEvents::ContentFileMoved(const std::string & fileName)
    {
      auto event = new Events::ContentFileMoved();
      event->FileName = FileSystem::FileNoExtension(fileName);
      event->Extension = FileSystem::FileExtension(fileName);
      Daisy->Dispatch<Events::ContentFileMoved>(event);
      delete event;
    }

    void DispatchSystemEvents::ContentFileUpdated(const std::string & fileName)
    {
      auto event = new Events::ContentFileUpdated();
      event->FileName = FileSystem::FileNoExtension(fileName);
      event->Extension = FileSystem::FileExtension(fileName);
      Daisy->Dispatch<Events::ContentFileUpdated>(event);
      delete event;
    }

    void DispatchSystemEvents::ContentFileFound(const std::string & fileName)
    {
      auto event = new Events::ContentFileFound();
      event->FileName = FileSystem::FileNoExtension(fileName);
      event->Extension = FileSystem::FileExtension(fileName);
      Daisy->Dispatch<Events::ContentFileFound>(event);
      delete event;
    }

    void DispatchSystemEvents::ContentFileDeleted(const std::string & fileName)
    {
      auto event = new Events::ContentFileDeleted();
      event->FileName = FileSystem::FileNoExtension(fileName);
      event->Extension = FileSystem::FileExtension(fileName);
      Daisy->Dispatch<Events::ContentFileDeleted>(event);
      delete event;
    }

    void DispatchSystemEvents::ContentFileScanComplete()
    {
      auto event = new Events::ContentFileScanComplete();;
      Daisy->Dispatch<Events::ContentFileScanComplete>(event);
      delete event;
    }

    void DispatchSystemEvents::ContentProjectLoaded()
    {
      auto event = new Events::ContentProjectLoaded();;
      Daisy->Dispatch<Events::ContentProjectLoaded>(event);
      delete event;
    }


  }
}


