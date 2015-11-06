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
#include "../../ComponentsInclude.h"

namespace DCEngine {
  namespace Systems {

    void DisplayTransform() {

    }

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
          if (ImGui::TreeNode(component->Name().c_str())) {

            // Hardcoded currently for the transform component!
            if (std::type_index(typeid(*component)) == std::type_index(typeid(Transform))) {
              auto transform = SelectedObject->getComponent<Transform>(); // or d.cast instead lol      
              
              ImGui::Text("Translation");
              float trans[3] = { transform->Translation.x, 
                                 transform->Translation.y, 
                                 transform->Translation.z };
              ImGui::InputFloat3("", trans);

              ImGui::Text("");
              float rot[3] = { transform->Rotation.x,
                               transform->Rotation.y,
                               transform->Rotation.z };
              ImGui::InputFloat3("", rot);

              ImGui::Text("Scale");
              float scale[3] = { transform->Scale.x,
                                 transform->Scale.y,
                                 transform->Scale.z };
              ImGui::InputFloat3("", scale);

            }
            ImGui::TreePop();
          }
                  
        }
      }

      ImGui::End();
    }


  }
}