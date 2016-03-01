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
    @brief Displays the Project Properties' window.
    */
    /**************************************************************************/
    void EditorProjects::WindowProjectProperties()
    {
      if (!WindowProjectsPropertiesEnabled)
        return;

      auto title = EditorRef.Settings.ProjectProperties->ProjectName + " Project Properties";
      ImGui::SetNextWindowSize(ImVec2(500, 300), ImGuiSetCond_FirstUseEver);
      ImGui::Begin(title.c_str(), &WindowProjectsPropertiesEnabled);
      if (ImGui::TreeNode("Game")) {
        ImGui::Checkbox("Play", &EditorRef.Settings.ProjectProperties->Play);
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
    void EditorProjects::LoadProject(std::string path)
    {
      // Load the project's data into the Content system. This will
      // automatically load its resources/assets for use.
      //Daisy->getSystem<Content>()->LoadProject(path);
      //// Save a pointer to the project data
      //EditorRef.Settings.ProjectProperties = Daisy->getSystem<Content>()->ProjectInfo.get();
      //// Update the window caption to display the current project
      ///auto projectName = Daisy->getSystem<Content>()->ProjectInfo->ProjectName;
      ////DCTrace << "Editor::LoadProject - Opening: " << projectName << "\n";
      //DispatchSystemEvents::SetWindowCaption("Daisy Chain Engine - " + projectName);
      //// Load its default level 
      //auto play = EditorRef.Settings.ProjectProperties->Play;
      //auto load = EditorRef.LoadLevel(EditorRef.Settings.ProjectProperties->DefaultLevel);

      //if (load) {
      //  if (play) {
      //    EditorRef.ToggleEditor(false);
      //    //PlayGame();
      //  }

      //  else {
      //    DCTrace << "Editor::LoadProject - Default level found editor turned on \n";
      //    EditorRef.ToggleEditor(true);
      //  }
      //}
      //// No default level set, turn on the editor!
      //else
      //  EditorRef.ToggleEditor(true);
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
      if (!Serialization::Serialize(EditorRef.Settings.ProjectProperties, serializedData)) {
        DCTrace << "EditorProjects::SaveProject: Failed to save the serialized data! \n";
        return;
      }
      // Write to the file!
      auto& path = EditorRef.Settings.ProjectProperties->ProjectPath + EditorRef.Settings.ProjectProperties->ProjectName + ".dceproj";
      if (!FileSystem::FileWriteString(path, serializedData)) {
        DCTrace << "EditorProjects::SaveProject: Failed to write to the project properties file! \n";
        return;
      }
      DCTrace << "EditorProjects::SaveProject: Saved the current project! \n";
    }

    void EditorProjects::ToggleProperties()
    {
      WindowProjectsPropertiesEnabled = !WindowProjectsPropertiesEnabled;
    }

    void EditorProjects::Display()
    {
    }

    EditorProjects::EditorProjects(Editor & editor) : EditorModule(editor, true), WindowProjectsPropertiesEnabled(false)
    {
    }

    EditorProjects::~EditorProjects()
    {
    }
    void EditorProjects::Update()
    {
    }
  }
}

