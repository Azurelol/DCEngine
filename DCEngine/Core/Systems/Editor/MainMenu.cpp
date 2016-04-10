/******************************************************************************/
/*!
@file   MainMenu.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/02/2015
@brief  This file includes the implementation for the Editor's main menu bar.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "Editor.h"
#include "../../Engine/Engine.h"
#include "../Input/InputSFML.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    \brief  Displays the menu bar for the editor, which contains all available
    widgets. It allows them to be toggled on and off.
    */
    /**************************************************************************/
    void Editor::DisplayMainMenuBar()
    {
      bool IsFullScreenTriggered = false;
      if (ImGui::BeginMainMenuBar()) {

        // Project
        if (ImGui::BeginMenu("Project")) {
          if (ImGui::MenuItem("New Project")) NewProject();
          if (ImGui::MenuItem("Archive Project")) ArchiveProject();
          if (ImGui::MenuItem("Open Project")) OpenProject();
          if (ImGui::MenuItem("Save Project")) Projects.SaveProject();
          if (ImGui::MenuItem("Show Project Folder")) LaunchProjectFolder();
          if (ImGui::MenuItem("Project Properties")) Projects.Toggle();
          ImGui::Separator();
          if (ImGui::MenuItem("Play Game", "F5")) PlayGame();
          if (ImGui::MenuItem("Stop Game")) StopGame();
          if (ImGui::MenuItem("Exit")) Exit();
          ImGui::EndMenu();
        }

        // Edit
        if (ImGui::BeginMenu("Edit")) {
          if (ImGui::MenuItem("Undo", "CTRL+Z")) { Undo(); }
          if (ImGui::MenuItem("Redo", "CTRL+Y")) { Redo(); }
          ImGui::Separator();
          if (ImGui::MenuItem("Cut", "CTRL+X")) { Cut(); }
          if (ImGui::MenuItem("Copy", "CTRL+C")) { Copy(); }
          if (ImGui::MenuItem("Paste", "CTRL+V")) { Paste(); }
          ImGui::Separator();
          if (ImGui::MenuItem("Duplicate", "CTRL+D")) { Duplicate(); }
          if (ImGui::MenuItem("Delete", "Del")) { DeleteObject(); }
          ImGui::Separator();
          if (ImGui::MenuItem("Lock All")) { Selector.LockAll(); }
          if (ImGui::MenuItem("Unlock All")) { Selector.UnlockAll(); }
          ImGui::Separator();
          if (ImGui::MenuItem("Preferences", "F10")) { Preferences.Toggle(true); }
          ImGui::EndMenu();
        }

        // Create
        if (ImGui::BeginMenu("Create")) {
          if (ImGui::MenuItem("Create Transform")) { Creator.CreateTransform(); }
          ImGui::Separator();
          if (ImGui::MenuItem("Create Sprite")) { Creator.CreateSprite(); }
          if (ImGui::MenuItem("Create SpriteText")) { Creator.CreateSpriteText(); }
          ImGui::Separator();
          if (ImGui::MenuItem("Create Camera")) { Creator.CreateCamera(); }
          if (ImGui::MenuItem("Create Light")) { Creator.CreateLight(); }
          if (ImGui::MenuItem("Create Particle System")) { Creator.CreateParticleSystem(); }
          ImGui::Separator();
          if (ImGui::MenuItem("Create from Archetype"))
            Windows.CreateFromArchetypeEnabled = true;
          ImGui::EndMenu();
        }

        // Select
        if (ImGui::BeginMenu("Select")) {
          if (ImGui::MenuItem("Previous Selection", false)) {}
          if (ImGui::MenuItem("Next Selection", false)) {}
          ImGui::Separator();
          if (ImGui::MenuItem("Select None", false)) {};
          if (ImGui::MenuItem("Select All", false)) {};
          ImGui::Separator();
          if (ImGui::MenuItem("Center Selected", "F")) {};
          if (ImGui::MenuItem("Frame Selected", false)) {};
          ImGui::Separator();
          if (ImGui::MenuItem("Group Selected", false)) {};
          ImGui::Separator();
          if (ImGui::MenuItem("Select Space", "Shift+S")) Inspector.SelectSpace();
          if (ImGui::MenuItem("Select Game", "Shift+G")) Inspector.SelectGameSession(); 
          ImGui::EndMenu();
        }


        // Resources
        if (ImGui::BeginMenu("Resources")) {
          if (ImGui::MenuItem("Add", "Ctrl+N")) {
            Windows.AddResourceEnabled = !Windows.AddResourceEnabled;
          }
          ImGui::Separator();
          if (ImGui::MenuItem("Load Level"))
            Windows.LoadLevelEnabled = !Windows.LoadLevelEnabled;
          if (ImGui::MenuItem("Duplicate Level"))
            Resources.ToggleDuplicateLevel();
          if (ImGui::MenuItem("Save Level"))
            Windows.SaveLevelEnabled = !Windows.SaveLevelEnabled;
          if (ImGui::MenuItem("Reload Level"))
            Projects.ReloadLevel();
          ImGui::EndMenu();
        }

        // Windows
        if (ImGui::BeginMenu("Windows")) {
          if (ImGui::MenuItem("Properties"))
            Inspector.Toggle(true);
          if (ImGui::MenuItem("Library"))
            Windows.LibraryEnabled = !Windows.LibraryEnabled;
          if (ImGui::MenuItem("Objects"))
            Objects.Toggle(true);
          if (ImGui::MenuItem("Tools"))
            Windows.ToolsEnabled = !Windows.ToolsEnabled;
          if (ImGui::MenuItem("Diagnostics"))
            Diagnostics.WindowEnabled = !Diagnostics.WindowEnabled;
          if (ImGui::MenuItem("Commands"))
            Windows.CommandsEnabled = !Windows.CommandsEnabled;
          if (ImGui::MenuItem("Console", "~"))
            Windows.ConsoleEnabled = !Windows.ConsoleEnabled;
          ImGui::Separator();
          if (ImGui::MenuItem("Full Screen/Windows. mode"))
          {
            IsFullScreenTriggered = true;
          }

          ImGui::EndMenu();
        }

        // Help
        if (ImGui::BeginMenu("Help")) {
          if (ImGui::MenuItem("Documentation")) { LaunchDocumentation(); }
          if (ImGui::MenuItem("Report Bug", false)) {}  // Disabled item
          if (ImGui::MenuItem("Controls", false)) {}  // Disabled item
          ImGui::Separator();
          if (ImGui::MenuItem("Release Notes")) {}
          if (ImGui::MenuItem("About")) {}

          ImGui::EndMenu();
        }
                
        if (ImGui::Button("Lightning")) {
          DispatchSystemEvents::GraphicsToggleLightning();
        }

        ImGui::EndMainMenuBar();

      }

      if (IsFullScreenTriggered)
      {
        auto fsevent = new Events::WindowFullScreenToggle();
        Daisy->Dispatch<Events::WindowFullScreenToggle>(fsevent);
        delete fsevent;
        return;
      }
    }

  }
}