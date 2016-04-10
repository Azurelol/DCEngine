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
      bool Ready;

      EditorProjects();
      ~EditorProjects();

    protected:      

    private:
      bool InitializingProject;
      void InitializeProject();
      void OnContentProjectLoadedEvent(Events::ContentProjectLoaded* event);
      void OnScriptingLibraryCompiled(Events::ScriptingLibraryCompiled* event);
      void OnScriptingLibraryCompilationFailure(Events::ScriptingLibraryCompilationFailure* event);
      bool WindowProjectsPropertiesEnabled;
      void Update();

    };


  }
}