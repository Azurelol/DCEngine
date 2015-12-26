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
          if (ImGui::MenuItem("New Project"))
            NewProject();          
          if (ImGui::MenuItem("Archive Project"))
            ArchiveProject();
          if (ImGui::MenuItem("Open Project"))
            OpenProject();
          if (ImGui::MenuItem("Save Project"))
            SaveProject();
          if (ImGui::MenuItem("Show Project Folder"))
            ShowProjectFolder();
          ImGui::Separator();
          if (ImGui::MenuItem("Play Game", "F5"))
            PlayGame();
          if (ImGui::MenuItem("Stop Game"))
            StopGame();
          if (ImGui::MenuItem("Exit"))
            Exit();
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
          ImGui::EndMenu();
        }

        // Create
        if (ImGui::BeginMenu("Create")) {
          if (ImGui::MenuItem("Create Transform")) {
            CreateTransform();
          }
          ImGui::Separator();
          if (ImGui::MenuItem("Create Sprite")) {
            CreateSprite();
          }
          if (ImGui::MenuItem("Create SpriteText")) {}
          ImGui::Separator();
          if (ImGui::MenuItem("Create Particle System")) {}
          ImGui::Separator();
          if (ImGui::MenuItem("Create from Archetype"))
            WindowCreateFromArchetypeEnabled = true;
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
          if (ImGui::MenuItem("Select Space", "Shift+S")) SelectSpace();
          if (ImGui::MenuItem("Select Game", "Shift+G")) {};
          ImGui::EndMenu();
        }


        // Resources
        if (ImGui::BeginMenu("Resources")) {
          if (ImGui::MenuItem("Add", "Ctrl+N")) {
            WindowAddResourceEnabled = !WindowAddResourceEnabled;
          }
          ImGui::Separator();
          if (ImGui::MenuItem("Load Level"))
            WindowLoadLevelEnabled = !WindowLoadLevelEnabled;
          if (ImGui::MenuItem("Save Level"))
            WindowSaveLevelEnabled = !WindowSaveLevelEnabled;
          if (ImGui::MenuItem("Reload Level"))
            ReloadLevel();
          //ImGui::Separator();
          /*if (ImGui::MenuItem("Load Dollhouse"))
            LoadDollhouse();*/
          //ReloadLevel();
          ImGui::EndMenu();
        }

        // Windows
        if (ImGui::BeginMenu("Windows")) {
          if (ImGui::MenuItem("Properties"))
            WindowPropertiesEnabled = !WindowPropertiesEnabled;
          if (ImGui::MenuItem("Library"))
            WidgetLibraryEnabled = !WidgetLibraryEnabled;
          if (ImGui::MenuItem("Objects"))
            WidgetObjectsEnabled = !WidgetObjectsEnabled;
          if (ImGui::MenuItem("Tools"))
            WindowToolsEnabled = !WindowToolsEnabled;
          if (ImGui::MenuItem("Diagnostics"))
            WidgetDiagnosticsEnabled = !WidgetDiagnosticsEnabled;
          if (ImGui::MenuItem("Commands"))
            WindowCommandsEnabled = !WindowCommandsEnabled;
          if (ImGui::MenuItem("Console", "~"))
            WindowConsoleEnabled = !WindowConsoleEnabled;
		  ImGui::Separator();
		  if (ImGui::MenuItem("Full Screen/Window mode"))
		  {
			  IsFullScreenTriggered = true;
		  }

          ImGui::EndMenu();
        }

        // Help
        if (ImGui::BeginMenu("Help")) {
          if (ImGui::MenuItem("Documentation")) {}
          if (ImGui::MenuItem("Report Bug", false)) {}  // Disabled item
          if (ImGui::MenuItem("Controls", false)) {}  // Disabled item
          ImGui::Separator();
          if (ImGui::MenuItem("Release Notes")) {}
          if (ImGui::MenuItem("About")) {}

          ImGui::EndMenu();
        }
        
        //ImGui::PushID(1);
        ImGui::BeginChild("Project2", ImVec2(30, 30));
        ImGui::EndChild();

        ImGui::EndMainMenuBar();

      }
	  if (IsFullScreenTriggered)
	  {
		  auto fsevent = new Events::FullscreenEnabledEvent();
		  Daisy->Dispatch<Events::FullscreenEnabledEvent>(fsevent);
		  delete fsevent;
		  return;
	  }
    }

  }
}