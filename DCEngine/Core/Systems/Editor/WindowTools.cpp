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
      ImGui::Separator();
      if (ImGui::Checkbox("Snap Active", &Settings.Snapping)) {
        DCTrace << "Editor::WindowTools - Snapping \n";
      }
      if (ImGui::InputFloat("Snap Distance", &Settings.SnapDistance, 0.5f)) {
      }

      ImGui::Separator();
      ImGui::Text("Grid");
      ImGui::Separator();
      if (ImGui::Checkbox("Grid Active", &Settings.GridActive)) {}
      if (ImGui::InputFloat("Grid Length", &Settings.GridLength, 0.5f)) {}
      
      auto& colorVec = Settings.GridColor;
      float vec4f[4] = { colorVec.x, colorVec.y, colorVec.z, colorVec.w };
      if (ImGui::InputFloat4(" Grid Color", vec4f)) {
        Settings.GridColor = Vec4(vec4f[0], vec4f[1], vec4f[2], vec4f[3]);
      }
      ImGui::Separator();


      if (ImGui::Button("Select")) {
        ActiveTool = EditorTool::Select;
      }
      ImGui::SameLine();
      if (ImGui::Button("Translate")) {
        ActiveTool = EditorTool::Translate;
      }
      ImGui::SameLine();
      if (ImGui::Button("Rotate")) {
        ActiveTool = EditorTool::Rotate;
      }
      ImGui::SameLine();
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