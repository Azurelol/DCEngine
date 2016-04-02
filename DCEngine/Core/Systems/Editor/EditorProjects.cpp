/******************************************************************************/
/*!
@file   EditorProjects.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   2/20/2016
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "EditorProjects.h"

#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    @brief EditorProjects constructor.
    */
    /**************************************************************************/
    EditorProjects::EditorProjects() : EditorModule(true), WindowProjectsPropertiesEnabled(false), 
                                       Ready(false), InitializingProject(false)
    {
    }

    /**************************************************************************/
    /*!
    @brief EditorProjects destructor.
    */
    /**************************************************************************/
    EditorProjects::~EditorProjects()
    {
    }

    /**************************************************************************/
    /*!
    @brief EditorProjects initializer.
    */
    /**************************************************************************/
    void EditorProjects::Initialize()
    {
      Daisy->Connect<Events::ScriptingLibraryCompiled>(&EditorProjects::OnScriptingLibraryCompiled, this);
      Daisy->Connect<Events::ScriptingLibraryCompilationFailure>(&EditorProjects::OnScriptingLibraryCompilationFailure, this);
    }

    /**************************************************************************/
    /*!
    @brief Displays the Project Properties' window.
    */
    /**************************************************************************/
    void EditorProjects::Display()
    {
      if (!WindowEnabled)
        return;

      auto title = Access().Settings.ProjectProperties->ProjectName + " Project Properties";
      ImGui::SetNextWindowSize(ImVec2(500, 300), ImGuiSetCond_FirstUseEver);
      ImGui::Begin(title.c_str(), &WindowEnabled);
      if (ImGui::TreeNode("Game")) {
        ImGui::Checkbox("Play", &Access().Settings.ProjectProperties->Play);
        ImGui::TreePop();
      }

      ImGui::End();
    }

    /**************************************************************************/
    /*!
    @brief Initializes the current project. This will first attempt to compile
           all scripts, then if they compile successfully load the default level.
    */
    /**************************************************************************/
    void EditorProjects::InitializeProject()
    {
      InitializingProject = true;
      // Request scripts to be compiled
      DispatchSystemEvents::ScriptingCompile();
    }

    /**************************************************************************/
    /*!
    @brief  Loads the selected project into the engine.
    @param  path The path of the project.
    */
    /**************************************************************************/
    void EditorProjects::LoadProject(const std::string& path)
    {
      // Load the project's data into the Content system. This will
      // automatically load its resources/assets for use.
      Daisy->getSystem<Content>()->LoadProject(path);
      // Save a pointer to the project data
      Access().Settings.ProjectProperties = &Daisy->getSystem<Content>()->getProjectInfo();
      // Update the window caption to display the current project
      auto projectName = Access().Settings.ProjectProperties->ProjectName;
      DCTrace << "Editor::LoadProject - Opening: " << projectName << "\n";
      DispatchSystemEvents::SetWindowCaption(projectName + "- Daisy Chain Engine");
      
      // Check that the scripting library has been compiled successfully before
      // trying to load the default level.
      InitializeProject();
    }

    /**************************************************************************/
    /*!
    @brief Saves the current project's properties..
    */
    /**************************************************************************/
    void EditorProjects::SaveProject()
    { 
      // Serialize the current properties to a string.
      std::string serializedData;
      if (!Serialization::Serialize(Access().Settings.ProjectProperties, serializedData)) {
        DCTrace << "EditorProjects::SaveProject: Failed to save the serialized data! \n";
        return;
      }
      // Write to the file!
      auto& path = Access().Settings.ProjectProperties->ProjectPath + Access().Settings.ProjectProperties->ProjectName + ".dceproj";
      if (!FileSystem::FileWriteString(path, serializedData)) {
        DCTrace << "EditorProjects::SaveProject: Failed to write to the project properties file! \n";
        return;
      }
      DCTrace << "EditorProjects::SaveProject: Saved the current project! \n";
    }
    
    /**************************************************************************/
    /*!
    @brief If the scripting library compiles, load the default level.
    */
    /**************************************************************************/
    void EditorProjects::OnScriptingLibraryCompiled(Events::ScriptingLibraryCompiled * event)
    {
      Ready = true;

      // If the project is being initialized...
      if (InitializingProject) {
        // Load its default level 
        auto play = Access().Settings.ProjectProperties->Play;
        auto load = Access().LoadLevel(Access().Settings.ProjectProperties->DefaultLevel);

        // If the level loaded successfully
        if (load) {
          // If set to play mode, disable the editor
          if (play)
            Access().ToggleEditor(false);
          // Otherwise, load the editor right away
          else {
            DCTrace << "Editor::LoadProject - Default level found editor turned on \n";
            Access().ToggleEditor(true);
          }

        }

        // No default level set, turn on the editor!
        else
          Access().ToggleEditor(true);

        InitializingProject = false;
      }

    }

    /**************************************************************************/
    /*!
    @brief If the scripting library fails to compile, do nothing!
    */
    /**************************************************************************/
    void EditorProjects::OnScriptingLibraryCompilationFailure(Events::ScriptingLibraryCompilationFailure * event)
    {
      Ready = false;
      // If the project is being initialized...
      if (InitializingProject) {
        DCTrace << "Editor::LoadProject - Default level found editor turned on \n";
        Access().ToggleEditor(true);
        InitializingProject = false;
      }
    }

    void EditorProjects::Update()
    {
    }
  }
}

