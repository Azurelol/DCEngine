/******************************************************************************/
/*!
@file   WindowTools.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/02/2015
@brief  This editor window allows the user to change settings regarding their
        tools such as Translate, Scale, Rotate.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "Editor.h"
#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {
    
    void Editor::WindowTools()
    {
      if (!WindowToolsEnabled)
        return;

      ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiWindowFlags_AlwaysAutoResize);
      ImGui::Begin("Tools", &WindowToolsEnabled);
      // The console needs to read the DCTrace lines while active.
      // Or it could read the log file?

      ImGui::Text("Snapping");
      if (ImGui::Checkbox("##snappingBool", &Settings.Snapping)) {
        DCTrace << "Editor::WindowTools - Snapping \n";
      }

      ImGui::Text("Snap Distance");
      if (ImGui::InputFloat("##Snapping", &Settings.SnapDistance, 0.5f)) {
      }

      //ImGui::Separator();
      //ImGui::Text("Grid");
      //if (ImGui::InputFloat4("##GridColor", &Settings.GridColor, 0.5f)) {
      //}
      //ImGui::Text("Distance");
      //if (ImGui::InputFloat("##Snapping", &Settings.GridActive, 0.5f)) {
      //}


      if (ImGui::Button("Select")) {
        ActiveTool = EditorTool::Select;
      }

      if (ImGui::Button("Translate")) {
        ActiveTool = EditorTool::Translate;
      }

      if (ImGui::Button("Rotate")) {
        ActiveTool = EditorTool::Rotate;
      }

      if (ImGui::Button("Scale")) {
        ActiveTool = EditorTool::Scale;
      }
      //if (ImGui::Button()
      //
      //ImGui::IsMouseDoubleClicked()

      ImGui::End();

    }

  }
}