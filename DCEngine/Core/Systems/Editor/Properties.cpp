/******************************************************************************/
/*!
@file   Properties.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/02/2015
@brief  This file includes the implementation for the Editor's properties widget.
*/
/******************************************************************************/
#include "Editor.h"
#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    \brief  Displays the properties of the currently selected object.
    */
    /**************************************************************************/
    void Editor::WidgetProperties()
    {
      if (!WidgetPropertiesEnabled)
        return;

      ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiSetCond_FirstUseEver);
      ImGui::Begin("Properties", &WidgetPropertiesEnabled);

      // If there's an object selected, display its properties.
      if (SelectedObject != nullptr) {
        // 1. Display the object's name
        ImGui::TextColored(ImVec4(0, 0.5, 1, 1), "Name: ");
        ImGui::Text(SelectedObject->Name().c_str());
        // 2. Display its components
        ImGui::TextColored(ImVec4(0, 0.5, 1, 1), "Components: ");
        for (auto component : *SelectedObject->AllComponents()) {
          ImGui::Text(component->Name().c_str());
        }
      }

      ImGui::End();
    }


  }
}