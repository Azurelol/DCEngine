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

  }
}