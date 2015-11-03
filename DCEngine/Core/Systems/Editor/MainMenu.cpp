/******************************************************************************/
/*!
@file   MainMenu.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/02/2015
@brief  This file includes the implementation for the Editor's main menu bar.
*/
/******************************************************************************/
#include "Editor.h"
#include "../../Engine/Engine.h"

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
      if (ImGui::BeginMainMenuBar()) {

        // Project
        if (ImGui::BeginMenu("Project")) {
          if (ImGui::MenuItem("New Project")) {}
          if (ImGui::MenuItem("Archive Project")) {}
          if (ImGui::MenuItem("Open Project")) {}
          if (ImGui::MenuItem("Save Project")) {}
          if (ImGui::MenuItem("Show Project Folder")) {}
          ImGui::Separator();
          if (ImGui::MenuItem("Load Level")) {}
          if (ImGui::MenuItem("Save Level")) {}
          if (ImGui::MenuItem("Reload Level")) {}
            //ReloadLevel();
          ImGui::EndMenu();
        }

        // Create
        if (ImGui::BeginMenu("Create")) {
          if (ImGui::MenuItem("Create Transform")) {}
          ImGui::Separator();
          if (ImGui::MenuItem("Create Sprite")) {}
          if (ImGui::MenuItem("Create SpriteText")) {}
          ImGui::Separator();
          if (ImGui::MenuItem("Create Particle System")) {}
          ImGui::EndMenu();
        }

        // Edit
        if (ImGui::BeginMenu("Edit")) {
          if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
          if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
          ImGui::Separator();
          if (ImGui::MenuItem("Cut", "CTRL+X")) {}
          if (ImGui::MenuItem("Copy", "CTRL+C")) {}
          if (ImGui::MenuItem("Paste", "CTRL+V")) {}
          ImGui::EndMenu();
        }

        // Windows
        if (ImGui::BeginMenu("Windows")) {
          if (ImGui::MenuItem("Properties"))
            WidgetPropertiesEnabled = !WidgetPropertiesEnabled;
          if (ImGui::MenuItem("Library"))
            WidgetLibraryEnabled = !WidgetLibraryEnabled;
          if (ImGui::MenuItem("Objects"))
            WidgetObjectsEnabled = !WidgetObjectsEnabled;
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

        ImGui::EndMainMenuBar();

      }
    }


    /**************************************************************************/
    /*!
    \brief  Loads a level from the currently tracked levels in the library.
    */
    /**************************************************************************/
    void LoadLevel() {

    }

    /**************************************************************************/
    /*!
    \brief  Loads a level from the currently tracked levels in the library.
    */
    /**************************************************************************/
    void SaveLevel() {

    }

    /**************************************************************************/
    /*!
    \brief  Reloads the currently loaded level.
    */
    /**************************************************************************/
    void ReloadLevel() {
      
    }


    void ShowWindowsMenu() {

    }



  }
}