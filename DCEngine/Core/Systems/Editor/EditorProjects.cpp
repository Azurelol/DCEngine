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

    EditorProjects::EditorProjects() : EditorModule(true), WindowProjectsPropertiesEnabled(false), Ready(false)
    {
    }

    EditorProjects::~EditorProjects()
    {
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


      // Load its default level 
      auto play = Access().Settings.ProjectProperties->Play;
      auto load = Access().LoadLevel(Access().Settings.ProjectProperties->DefaultLevel);
      //
      if (load) {
        if (play) {
          Access().ToggleEditor(false);
        }

        else {
          DCTrace << "Editor::LoadProject - Default level found editor turned on \n";
          Access().ToggleEditor(true);
        }
      }
      // No default level set, turn on the editor!
      else
        Access().ToggleEditor(true);

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
    

    void EditorProjects::OnScriptingLibraryCompiled(Events::ScriptingLibraryCompiled * event)
    {
    }

    void EditorProjects::OnScriptingLibraryCompilationFailure(Events::ScriptingLibraryCompilationFailure * event)
    {
    }

    void EditorProjects::Update()
    {
    }
  }
}

