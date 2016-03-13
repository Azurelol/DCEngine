/******************************************************************************/
/*
  File:    MainModule.cpp
  Author:  Alejandro Hitti
  Date:    11/19/14
  Brief:   The Main Module is in charge of activating and managing all the other
           modules in the GUI Interface.

  All content © 2014-2015 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

// Includes
//------------------------------------------------------------------------------
#include "stdinc.h"

#include "EditorManager.h"
#include "MainModule.h"
#include "MacroHelpers.h"

namespace Editor
{
  bool MainModule::m_IsActive = true;
  bool MainModule::m_IsVisible = true;
  bool MainModule::m_IsFirstTime = true;

  // Initializes the Module
  bool MainModule::MainModuleInitialize()
  {
    return true;
  }

  // Checks if the module is currently active
  bool MainModule::MainModuleIsActive()
  {
    return m_IsActive;
  }

  // Sets the module active flag to the state passed in
  void MainModule::MainModuleSetActive(bool state)
  {
    m_IsActive = state;
  }

  // Checks if the module is currently visible
  bool MainModule::MainModuleIsVisible()
  {
    return m_IsVisible;
  }

  // Sets the module visible flag to the state passed in
  void MainModule::MainModuleSetVisible(bool state)
  {
    m_IsVisible = state;
  }

  // Updates the module
  void MainModule::MainModuleUpdate()
  {
    if (!m_IsVisible || !m_IsActive)
      return;

    // Padding to the right of the button
    float padding = 26;
    // Height
    float buttonHeight = 12;

    // Start the Main module
    ImGui::Begin("Main", 0, ImVec2(0, 0), EDITOR->GetFlags().fillAlpha, ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar |
                                                                        ImGuiWindowFlags_::ImGuiWindowFlags_NoMove     |
                                                                        ImGuiWindowFlags_::ImGuiWindowFlags_NoResize);

    // Positions the window the first time you run it
    if (m_IsFirstTime)
    {
      EDITOR->SetModulePosition((int)ImGui::GetWindowSize().x, (int)ImGui::GetWindowSize().y, Editor::RefPoint::TopLeft, 0, 0);
      ImGui::SetWindowSize(ImVec2(450.0f, 20.0f));
      m_IsFirstTime = false;
      ImGui::End();
      return;
    }

    //ImGui::Text("Select a Module");

    // Add all the buttons to open other modules
    if (ImGui::Button("Scene"))
      EDITOR->SetModuleActive(Modules::Scene, !EDITOR->IsModuleActive(Modules::Scene));
    SetHelpTooltip("Opens the Scene Module.\n"
                   "Create/Save/Load Scenes here.\n");

    ImGui::SameLine();

    if (ImGui::Button("Entity"))
      EDITOR->SetModuleActive(Modules::Entity, !EDITOR->IsModuleActive(Modules::Entity));
    SetHelpTooltip("Opens the Entity Module.\n"
                   "Get the object's list as well as create/remove objects.\n");

    ImGui::SameLine();

    if (ImGui::Button("Resources"))
      EDITOR->SetModuleActive(Modules::Resources, !EDITOR->IsModuleActive(Modules::Resources));
    SetHelpTooltip("Opens the Resources Module.\n"
                   "Manages resources, allowing for adding, deleting and editing them.\n");

    ImGui::SameLine();

    //if (ImGui::Button("Inspector"))
    //  EDITOR->SetModuleActive(Modules::Inspector, !EDITOR->IsModuleActive(Modules::Inspector));
    //SetHelpTooltip("Opens the Inspector Module.\n"
    //               "Displays all the components and properties of the selecter entity.\n");

    //ImGui::SameLine();

    if (ImGui::Button("Profiler"))
      EDITOR->SetModuleActive(Modules::Profiler, !EDITOR->IsModuleActive(Modules::Profiler));
    SetHelpTooltip("Opens the Profiler Module.\n"
                   "Find debug information about the game's performance.\n");

    ImGui::SameLine();

    if (ImGui::Button("Settings"))
      EDITOR->SetModuleActive(Modules::Settings, !EDITOR->IsModuleActive(Modules::Settings));
    SetHelpTooltip("Opens the Settings Module.\n"
                   "Modify editor and game related settings.\n");

    ImGui::SameLine();

    if (ImGui::Button("Exit"))
      g_Messaging.BroadcastMessage(Core::MessageName::ShutDown, nullptr, nullptr);
    SetHelpTooltip("Exits the game.");

    // End Main Module
    ImGui::End();

  }

}
