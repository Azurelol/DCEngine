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
    GUI::GUI(GUIConfig& config) : System(std::string("GUISystem"), EnumeratedSystem::GUI), Settings(config), FontTexture(0)
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
      // Update the curent styles
      UpdateStyle();

      if (TRACE_INITIALIZE)
        DCTrace << "GUI::Initialize \n";
     GUIHandler->Initialize();

     // Set the current font
     SetFont(std::string("DroidSans.ttf"), 14);    
		 GUIHandler->StartFrame();
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
    @brief  Loads the currently serialized Style into ImGui.
    */
    /**************************************************************************/
    void GUI::UpdateStyle()
    {
    //  auto style = ImGui::GetStyle();

    //  // Rendering
    //  style.Alpha = Settings.Alpha;

    //  // Colors

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

    bool GUI::SliderFloat4(std::string label, ImVec4 & vec4, float min, float max)
    {
      bool modified = false;
      //float f4[4] = { vec4.x, vec4.y, vec4.z, vec4.w };
      //if (ImGui::SliderFloat4("##propertyID", f4, min, max)) {
      //  vec4.x = f4[0]; vec4.y = f4[1]; vec4.z = f4[2]; vec4.w = f4[3];
      //  modified = true;
      //}

      int id = 0;
      ImGui::Text(label.c_str());
      ImGui::PushID(id++);
      if (ImGui::SliderFloat("##label", &vec4.x, min, max))
        modified = true;
      ImGui::PopID();
      ImGui::SameLine();
      ImGui::PushID(id++);
      if (ImGui::SliderFloat("##label", &vec4.y, min, max))
        modified = true;
      ImGui::PopID();
      ImGui::SameLine();
      ImGui::PushID(id++);
      if (ImGui::SliderFloat("##label", &vec4.z, min, max))
        modified = true;
      ImGui::PopID();
      ImGui::SameLine();
      ImGui::PushID(id++);
      if (ImGui::SliderFloat("##label", &vec4.w, min, max))
        modified = true;
      ImGui::PopID();

      return modified;        
    }

    /**************************************************************************/
    /*!
    @brief Sets the current font for the GUI.
    @param fontPath The path for the font to use.
    @param pixelSize What size of the font to use
    */
    /**************************************************************************/
    void GUI::SetFont(const std::string & fontPath, unsigned pixelSize)
    {
      auto io = ImGui::GetIO();
      // Clear previous fonts
      io.Fonts->ClearFonts();
      io.Fonts->AddFontFromFileTTF(fontPath.c_str(), pixelSize);
      unsigned char* pixels;
      int width, height;
      // Generate a font texture
      //GLuint FontTexture;
			io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
			if (!FontTexture)
			{
				
				glGenTextures(1, &FontTexture);
				glBindTexture(GL_TEXTURE_2D, FontTexture);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
				// Set the font
			}
      io.Fonts->TexID = (void *)(intptr_t)FontTexture;
      // Clear the buffer stuff??
      io.Fonts->ClearInputData();
      io.Fonts->ClearTexData();
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