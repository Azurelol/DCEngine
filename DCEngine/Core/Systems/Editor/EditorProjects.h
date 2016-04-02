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
      void Display();
      void LoadProject(const std::string& path);
      void OpenRecentProject();
      void SaveProject();

      EditorProjects();
      ~EditorProjects();

    protected:
      

    private:
      bool Ready;
      void OnScriptingLibraryCompiled(Events::ScriptingLibraryCompiled* event);
      void OnScriptingLibraryCompilationFailure(Events::ScriptingLibraryCompilationFailure* event);
      bool WindowProjectsPropertiesEnabled;
      void Update();

    };


  }
}