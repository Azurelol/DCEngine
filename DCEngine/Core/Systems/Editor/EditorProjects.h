/******************************************************************************/
/*!
@file   EditorProjects.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   2/20/2016
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once

#include "../../Resources/Archetype.h"
//#include "../System.h"
#include "EditorModule.h"

namespace DCEngine {
  namespace Systems {

    class Editor;
    class EditorProjects : public EditorModule {
    public:
      void Initialize();
      void Display();
      void LoadProject(const std::string& path);
      void OpenRecentProject();
      void SaveProject();
      void AutoSave();
      void SaveLevelBackup();
      // Levels
      bool LoadLevel(std::string level);
      bool SaveLevel(std::string level);
      bool ReloadLevel();
      void SaveCurrentLevel();
      

      bool Ready;
      EditorProjects();
      ~EditorProjects();

    protected:      

    private:
      std::unique_ptr<Time::Timer> AutoSaveTimer;
      bool WindowProjectsPropertiesEnabled;
      // Load
      void LoadDefaultGameSession();
      void LoadDefaultSpace();
      // Initialize
      bool InitializingProject;
      void InitializeProject();
      // Events
      void OnContentProjectLoadedEvent(Events::ContentProjectLoaded* event);
      void OnScriptingLibraryCompiled(Events::ScriptingLibraryCompiled* event);
      void OnScriptingLibraryCompilationFailure(Events::ScriptingLibraryCompilationFailure* event);

    };


  }
}