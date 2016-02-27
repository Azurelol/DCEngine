/******************************************************************************/
/*
  File:    SceneModule.cpp
  Author:  Alejandro Hitti
  Date:    11/19/14
  Brief:   The Scene Module is in charge of managing the scene and the levels.
           It'll let you create, save and load levels.

  All content © 2014-2015 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

// Includes
//------------------------------------------------------------------------------
#include "stdinc.h"

#include "SceneModule.h"
#include "Filesystem.h"
#include "Graphics.h"
#include "ServiceLocator.h"

namespace Editor
{
  const unsigned kBufferSize = 256;

  bool SceneModule::m_IsActive = false;
  bool SceneModule::m_IsVisible = true;

  char* SceneModule::m_SceneName = new char[kBufferSize];
  bool SceneModule::m_IsFirstTime = true;

  // Initializes the Module
  bool SceneModule::SceneModuleInitialize()
  {
    strcpy(m_SceneName, g_Levels.GetCurrentLevelName().c_str());

    return true;
  }

  // Checks if the module is currently active
  bool SceneModule::SceneModuleIsActive()
  {
    return m_IsActive;
  }

  // Sets the module active flag to the state passed in
  void SceneModule::SceneModuleSetActive(bool state)
  {
    m_IsActive = state;
  }

  // Checks if the module is currently visible
  bool SceneModule::SceneModuleIsVisible()
  {
    return m_IsVisible;
  }

  // Sets the module visible flag to the state passed in
  void SceneModule::SceneModuleSetVisible(bool state)
  {
    m_IsVisible = state;
  }

  // Updates the module
  void SceneModule::SceneModuleUpdate()
  {
    if (!m_IsVisible || !m_IsActive)
      return;

    // Padding to the right of the button
    float padding = 26;
    // Height
    float buttonHeight = 12;

    // Begin the Scene module
    ImGui::Begin("Scene", &m_IsActive, ImVec2(0, 0), EDITOR->GetFlags().fillAlpha, EDITOR->GetLayoutFlags());

    // Positions the window the first time you run it
    if (m_IsFirstTime)
    {
      EDITOR->SetModulePosition((int)ImGui::GetWindowSize().x, (int)ImGui::GetWindowSize().y, Editor::RefPoint::BottomLeft, 5, -5);
      m_IsFirstTime = false;
      strcpy(m_SceneName, g_Levels.GetCurrentLevelName().c_str());
      ImGui::End();
      return;
    }

    bool saveable = EDITOR->GetFreshlyLoaded();

    // Scene Name
    ImGui::Text("Scene Name: ");
    SetHelpTooltip("Current Scene name.");
    ImGui::SameLine();
    ImGui::Text(m_SceneName);

    // New Level
    if (ImGui::Button("New Level", ImVec2(ImGui::GetWindowWidth() - padding, buttonHeight)))
    {
      g_Levels.NewLevel();
      g_Levels.SyncLevelToWorld();
      strcpy(m_SceneName, g_Levels.GetCurrentLevelName().c_str());

      g_Messaging.BroadcastMessage(Core::PauseGame, nullptr, nullptr);
      EDITOR->SetFreshlyLoaded(true);
    }
    SetHelpTooltip("Create a new level at\nthe specified location.");

    // Save Level
    std::string buttonName;
    if (saveable) 
      buttonName = "Save Level";
    else 
      buttonName = "<<Saving Disabled>>";

    if (ImGui::Button(buttonName.c_str(), ImVec2(ImGui::GetWindowWidth() - padding, buttonHeight)) && saveable)
    {
      std::string save = g_Levels.GetCurrentLevelName();
      save = std::string("./levels/") + save;
      if (save == "untitled")
      {
        save = Utilities::Filesystem::SaveFileName();
        if (!save.empty())
          save = Utilities::Filesystem::StripExtension(save);
      }

      if (!save.empty())
      {
        g_Levels.SyncWorldToLevel();
        g_Levels.SaveLevel(save);
        strcpy(m_SceneName, g_Levels.GetCurrentLevelName().c_str());
      }
    }
    SetHelpTooltip("Saves the level at\nthe specified location.");

    // "Save as"
    if (saveable)
      buttonName = "Save Level As";
    else
      buttonName = "<<Saving Disabled>>";

    if (ImGui::Button(buttonName.c_str(), ImVec2(ImGui::GetWindowWidth() - padding, buttonHeight)) && saveable)
    {
      std::string save = Utilities::Filesystem::SaveFileName();
      if (!save.empty())
      {
        save = Utilities::Filesystem::StripExtension(save);

        g_Levels.SyncWorldToLevel();
        g_Levels.SaveLevel(save);
        strcpy(m_SceneName, g_Levels.GetCurrentLevelName().c_str());
      }
    }
    SetHelpTooltip("Saves the level at\nthe specified location.");

    // Load Level
    if (ImGui::Button("Load Level", ImVec2(ImGui::GetWindowWidth() - padding, buttonHeight)))
    {
      std::string load = Utilities::Filesystem::OpenFileName();
      if (!load.empty())
      {
        load = Utilities::Filesystem::StripExtension(load);

        g_Levels.LoadLevel(load);
        g_Levels.SyncLevelToWorld();
        strcpy(m_SceneName, g_Levels.GetCurrentLevelName().c_str());

        g_Messaging.BroadcastMessage(Core::PauseGame, nullptr, nullptr);
        EDITOR->SetFreshlyLoaded(true);
      }
    }
    SetHelpTooltip("Loads a specific level.");

    // Load a level from a file
    ImGui::Text("Load Level from the list");
    
    // Load all level names in the folder
    std::vector<std::string> levelsList = Utilities::Filesystem::GetFilepathsInDirectory("./levels/");
    static int index = 0;

    for (unsigned i = 0; i < levelsList.size(); ++i)
    {
      // Strips the extension
      levelsList[i] = Utilities::Filesystem::StripPathAndExtension(levelsList[i]);

      // Starts the combo box at th currently selected level
      if (m_SceneName == levelsList[i])
        index = i;
    }

    // Sets the new scene.
    if (ImGui::Combo(std::string("Levels##" + index).c_str(), &index, VectorOfStringsItemGettor, &levelsList, levelsList.size(), 8))
    {
      g_Levels.LoadLevel(levelsList[index]);
      g_Levels.SyncLevelToWorld();
      strcpy(m_SceneName, levelsList[index].c_str());

      g_Messaging.BroadcastMessage(Core::PauseGame, nullptr, nullptr);
      EDITOR->SetFreshlyLoaded(true);
    }
    SetHelpTooltip("Loads list of levels in ./assets/levels/");

    // Reload Level
    if (ImGui::Button("Reload", ImVec2(ImGui::GetWindowWidth() - padding, buttonHeight)))
    {
      g_Levels.SyncLevelToWorld();
      strcpy(m_SceneName, g_Levels.GetCurrentLevelName().c_str());

      g_Messaging.BroadcastMessage(Core::PauseGame, nullptr, nullptr);
      EDITOR->SetFreshlyLoaded(true);
    }
    SetHelpTooltip("Reloads the currently loaded level.");

    // End of the Scene module
    ImGui::End();
  }

}
