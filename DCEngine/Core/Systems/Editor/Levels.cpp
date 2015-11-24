/******************************************************************************/
/*!
@file   Levels.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/23/2015
@brief
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/

#include "Editor.h"
#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    @brief  Window for saving levels.
    */
    /**************************************************************************/
    void Editor::WindowSaveLevel()
    {
      if (!WindowSaveLevelEnabled)
        return;

      ImGui::SetNextWindowSize(ImVec2(150, 150), ImGuiSetCond_FirstUseEver);
      ImGui::Begin("Save Level", &WindowSaveLevelEnabled);

      // Query the Content system for the current list of levels
      static bool ScannedForLevels;
      if (!ScannedForLevels) {
        Daisy->getSystem<Content>()->ScanForLevels();
        ScannedForLevels = true;
      }

      // Query the Content system for the current list of levels
      static char filename[32];      
      for (auto level : *Daisy->getSystem<Content>()->AllLevels()) {
        auto levelName = FileSystem::FileNoExtension(level.second->Name());
        if (ImGui::Selectable(levelName.c_str())) {
          strcpy(filename, levelName.c_str());
        }
      }

      // Allow the user to select the current name from either text input
      // or by selecting one of the other levels?
      ImGui::Separator();
      ImGui::InputText("File name:", filename, IM_ARRAYSIZE(filename));
      ImGui::Separator();

      // BUTTON: Save level
      if (ImGui::Button("Save Level")) {

        // Check if the level is already in there
        if (Daisy->getSystem<Content>()->getLevel(std::string(filename))) {
          ImGui::OpenPopup("Confirm Save");
        }
        else {
            DCTrace << "Editor::WindowSaveLevel - Saving new level! \n";
            SaveLevel(filename);
            WindowSaveLevelEnabled = false;
            ScannedForLevels = false;
          }
        }

      if (ImGui::BeginPopupModal("Confirm Save"))
      {
        std::string overwriteName(filename + std::string(" already exists"));
        ImGui::Text(overwriteName.c_str());
        ImGui::Text("Do you wish to overwrite it?");

        if (ImGui::Button("OK", ImVec2(120, 0))) {
          DCTrace << "Editor::WindowSaveLevel - Overwriting level! \n";
          SaveLevel(filename);
          WindowSaveLevelEnabled = false;
          ScannedForLevels = false;
          ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
          ImGui::CloseCurrentPopup();

        ImGui::EndPopup();
      }


        // Button: Cancel
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
          WindowSaveLevelEnabled = false;
          ScannedForLevels = false;
        }

      
      ImGui::End();

    }

    /**************************************************************************/
    /*!
    @brief  Window for loading levels.
    */
    /**************************************************************************/
    void Editor::WindowLoadLevel()
    {
      if (!WindowLoadLevelEnabled)
        return;

      
      ImGui::SetNextWindowSize(ImVec2(200, 200));
      ImGui::Begin("Load Level", &WindowLoadLevelEnabled);
           
      // Request the content system to scan for levels everytime the window is opened
      static bool ScannedForLevels;
      if (!ScannedForLevels) {
        Daisy->getSystem<Content>()->ScanForLevels();
        ScannedForLevels = true;
      }

      // Query the Content system for the current list of levels
      static char filename[32];      
      for (auto level : *Daisy->getSystem<Content>()->AllLevels()) {
        auto levelName = FileSystem::FileNoExtension(level.second->Name());
        if (ImGui::Selectable(levelName.c_str())) {          
          strcpy(filename, levelName.c_str());
        }
      }
      
      // Allow the user to select the file by name
      ImGui::Separator();
      ImGui::InputText("File name:", filename, IM_ARRAYSIZE(filename));
      ImGui::Separator();

      // Load the selected level
      if (ImGui::Button("Load")) {
        LoadLevel(filename);
        WindowLoadLevelEnabled = !WindowLoadLevelEnabled;
        ScannedForLevels = false;
      }
      ImGui::SameLine();  

      // Close the window
      if (ImGui::Button("Cancel")) {
        WindowLoadLevelEnabled = !WindowLoadLevelEnabled;
        ScannedForLevels = false;
      }
      ImGui::End();
    }
  }
}