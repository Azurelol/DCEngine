#include "Windows.h"

#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {


    Windows::Window::~Window()
    {
    }

    void Windows::Window::Remove()
    {
      Daisy->getSystem<GUI>()->Remove(WindowPtr(this));
    }

    void Windows::PopUp::Display()
    {
      // Open the PopUp
      ImGui::SetNextWindowSize(ImVec2(150, 150), ImGuiSetCond_FirstUseEver);
      ImGui::OpenPopup(Data.Title.c_str());

      if (ImGui::BeginPopupModal(Data.Title.c_str())) {
        ImGui::Text(Data.Message.c_str());

        // Close the PopUp
        if (ImGui::Button(Data.Confirmation.c_str())) {
          ImGui::CloseCurrentPopup();
          Remove();
        }

        ImGui::EndPopup();
      }
    }

    void Windows::PopUpComponentDependencies::Display()
    {
      // Open the PopUp
      ImGui::SetNextWindowSize(ImVec2(150, 150), ImGuiSetCond_FirstUseEver);
      ImGui::OpenPopup(Data.Title.c_str());

      if (ImGui::BeginPopupModal(Data.Title.c_str())) {

        // List all missing dependencies.
        for (auto& dependency : Data.List) {
          ImGui::Text(dependency.c_str());
        }
        //ImGui::Columns();
        // Add all missing dependencies
        if (ImGui::Button("Add All")) {

          auto entity = ComponentRef->Owner();
          // Add every missing dependency
          for (auto& dependency : Data.List) {
            entity->AddComponentByName(dependency, false);
          }
          // Move the original component to the back,
          // after all its dependencies
          entity->SwapToBack(ComponentRef);

          ImGui::CloseCurrentPopup();
          Remove();
        }
        ImGui::SameLine();
        // Close the PopUp
        if (ImGui::Button(Data.Confirmation.c_str())) {
          ComponentRef->Destroy();
          ImGui::CloseCurrentPopup();
          Remove();
        }

        ImGui::EndPopup();
      }
    }

  }
}