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

