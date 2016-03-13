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
    @brief  Loads the level, deserializing a level file's binary data and
    constructing gameobjects along with their components.
    */
    /**************************************************************************/
    bool Editor::LoadLevel(std::string level)
    {
      DCTrace << "Editor::LoadLevel - Loading " << level << "\n";

      // Save the currently-loaded level before loading a new one
      if (CurrentSpace->CurrentLevel) {
        auto currentLevelName = CurrentSpace->CurrentLevel->Name();
        SaveLevel(currentLevelName);
      }

      // Query the Content system for the Level resource 
      auto levelPtr = Daisy->getSystem<Systems::Content>()->getLevel(level);
      Deselect();

      if (levelPtr == nullptr) {
        DCTrace << "Editor::LoadLevel - Could not find " << level << "\n";
        return false;
      }

      // Load the level
      CurrentSpace->LoadLevel(level);
      // Load the editor camera
      SetEditorCamera(true);
      // Sets the current window's caption
      UpdateCaption();
      // Set it as the default level
      Settings.ProjectProperties->DefaultLevel = level;
      return true;
    }

    /**************************************************************************/
    /*!
    @brief  Saves the current level, serializing all of the active gameobjects
    along with their components in the space.
    @param  level The name of the level.
    @todo   The level path is currently hardcoded. Change that.
    */
    /**************************************************************************/
    bool Editor::SaveLevel(std::string level)
    {
      DCTrace << "Editor::SaveLevel - Saving " << level << "\n";

      // Get the current project's path
      std::string LevelPath = Settings.ProjectProperties->ProjectPath + Settings.ProjectProperties->ResourcePath;
      // ("Projects/Rebound/Resources/Levels/");
      auto levelResource = CurrentSpace->SaveLevel(LevelPath + level + Level::Extension());

      // If the level was saved successfully
      if (levelResource) {
        // Set it as the current level on the space
        CurrentSpace->CurrentLevel = levelResource;
        // Scan for resources again
        //Daisy->getSystem<Content>()->ScanResources();
        Daisy->getSystem<Content>()->ScanForLevels();
        // Update the caption
        UpdateCaption();

        return true;
      }
      else
        return false;
    }

    /**************************************************************************/
    /*!
    @brief Reloads the currently loaded level.
    */
    /**************************************************************************/
    bool Editor::ReloadLevel()
    {
      // Reload the current level
      CurrentSpace->ReloadLevel();
      // Reload the editor camera
      SetEditorCamera(true);
      // Clear the currently selected object
      Deselect();
      // Eh?
      return true;
    }

    /**************************************************************************/
    /*!
    @brief  Window for saving levels.
    */
    /**************************************************************************/
    void Editor::WindowSaveLevel()
    {
      if (!Windows.SaveLevelEnabled)
        return;

      ImGui::SetNextWindowSize(ImVec2(150, 150), ImGuiSetCond_FirstUseEver);
      ImGui::Begin("Save Level", &Windows.SaveLevelEnabled);

      // Query the Content system for the current list of levels
      static bool ScannedForLevels;
      if (!ScannedForLevels) {
        Daisy->getSystem<Content>()->ScanForLevels();
        ScannedForLevels = true;
      }

      // Query the Content system for the current list of levels
      static char filename[32];      
      for (auto& level : *Daisy->getSystem<Content>()->AllLevels()) {
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
            Windows.SaveLevelEnabled = false;
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
          Windows.SaveLevelEnabled = false;
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
          Windows.SaveLevelEnabled = false;
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
      if (!Windows.LoadLevelEnabled)
        return;

      
      ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiWindowFlags_AlwaysAutoResize);
      ImGui::Begin("Load Level", &Windows.LoadLevelEnabled);
           
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
        Windows.LoadLevelEnabled = !Windows.LoadLevelEnabled;
        ScannedForLevels = false;
      }
      ImGui::SameLine();  

      // Close the window
      if (ImGui::Button("Cancel")) {
        Windows.LoadLevelEnabled = !Windows.LoadLevelEnabled;
        ScannedForLevels = false;
      }
      ImGui::End();
    }
  }
}