/******************************************************************************/
/*!
\file   GUI.cpp
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   10/23/2015
\brief  The GUI interface class.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/

#include "GUI.h"
#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {
    
    /**************************************************************************/
    /*!
    @brief  Constructor.
    */
    /**************************************************************************/
    GUI::GUI() : System(std::string("GUISystem"), EnumeratedSystem::GUI)
    {
      GUIHandler.reset(new ImGuiSFML());
    }

    /**************************************************************************/
    /*!
    @brief  Initializes the GUI system.
    */
    /**************************************************************************/
    void GUI::Initialize()
    {
      //if (!GUI_ENABLED)
      //  return;

      if (TRACE_INITIALIZE)
        DCTrace << "GUI::Initialize \n";
     GUIHandler->Initialize();
    }

    /**************************************************************************/
    /*!
    @brief  Starts the frame for the GUI.
    */
    /**************************************************************************/
    void GUI::StartFrame()
    {
      if (!GUI_ENABLED)
        return;
      if (TRACE_UPDATE)
        DCTrace << "GUI::StartFrame \n";
      GUIHandler->PollEvents();
      GUIHandler->StartFrame();
    }

    /**************************************************************************/
    /*!
    @brief  Calls the rendering functions on ImGui.
    */
    /**************************************************************************/
    void GUI::Render()
    {
      if (!GUI_ENABLED)
        return;

      GUIHandler->Render();
    }

    /**************************************************************************/
    /*!
    @brief  Toggles the GUI on and off.
    */
    /**************************************************************************/
    void GUI::Toggle()
    {
      GUI_ENABLED = !GUI_ENABLED;
    }

    /**************************************************************************/
    /*!
    @brief  Updates the GUI system.
    */
    /**************************************************************************/
    void GUI::Update(float dt)
    {
      SystemTimer profile(this->Name());
      // Clear inactive windows
      ClearInactive();
      // Draw every active window
      for (auto& window : ActiveWindows) {
        window->Display();
      }
    }

    /**************************************************************************/
    /*!
    @brief  Terminates the GUI system.
    */
    /**************************************************************************/
    void GUI::Terminate()
    {
      if (!GUI_ENABLED)
        return;

      DCTrace << "GUI::Terminate \n";
      GUIHandler->Terminate();
    }

    /**************************************************************************/
    /*!
    @brief  Displays a Tooltip.
    */
    /**************************************************************************/
    void GUI::SetToolTip(std::string tooltip)
    {
      ImGui::SetTooltip(tooltip.c_str());
    }

    /**************************************************************************/
    /*!
    @brief  Displays a PopUp.
    */
    /**************************************************************************/
    void GUI::PopUp(Windows::PopUpData & data)
    {
      ImGui::OpenPopup(data.Title.c_str());
      if (ImGui::BeginPopupModal(data.Title.c_str())) {
        ImGui::Text(data.Message.c_str());
        if (ImGui::Button(data.Confirmation.c_str())) {
          ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
      }
    }

    /**************************************************************************/
    /*!
    @brief Adds a window to the GUI system.
    @param A pointer to the Window.
    */
    /**************************************************************************/
    void GUI::Add(WindowPtr window)
    {
      Daisy->getSystem<GUI>()->ActiveWindows.push_back(window);
    }

    /**************************************************************************/
    /*!
    @brief Checks whether the mouse has been double clicked or not.
    @return True if the mouse was double clicked recently.
    */
    /**************************************************************************/
    bool GUI::IsMouseDoubleClicked()
    {
      return ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0);
    }

    /**************************************************************************/
    /*!
    @brief Removes an active window from the GUI system.
    @param A pointer to the Window.
    */
    /**************************************************************************/
    void GUI::Remove(WindowPtr window)
    {
      InactiveWindows.push_back(window);      
    }

    void GUI::ClearInactive()
    {
      if (InactiveWindows.empty())
        return;

      auto iter = std::find(ActiveWindows.begin(), ActiveWindows.end(), InactiveWindows.back());
      InactiveWindows.pop_back();
      ActiveWindows.erase(iter);

      // Recursionlol
      ClearInactive();
    }

  }
}