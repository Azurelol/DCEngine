/******************************************************************************/
/*!
@file   MenuProject.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/15/2015
@brief  This file includes the implementation for the Editor's project menu.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "Editor.h"
#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {

    void Editor::NewProject()
    {
    }

    void Editor::ArchiveProject()
    {
    }

    /**************************************************************************/
    /*!
    @brief  Displays a window allowing a project to be opened.
    @note   This function will scan all the files with the .dce extension.
    */
    /**************************************************************************/
    void Editor::OpenProject()
    {
      // Scan the Projects folder for DCE files
      DCTrace << "Editor::OpenProject - Opening project \n";
      // Open the projects path
      auto executablePath = boost::filesystem::initial_path().string();
      auto projectsPath = executablePath + "\\Projects";
      // If the projects directory doesn't exist, start from the executable
      if (!FileSystem::Exists(projectsPath)) {
        projectsPath = executablePath;
      }
      // Grab the path of the file selected
      auto fileOpenedPath = FileSystem::FileOpenDialog(projectsPath, std::string("dceproj"));
      // Do something with this path:

    }

    /**************************************************************************/
    /*!
    @brief  Opens the last known recent project... if there is one.
    */
    /**************************************************************************/
    void Editor::OpenRecentProject()
    {
      // If there is a recent project, load it! 
      if (!Settings.RecentProject.empty()) {
        LoadProject(Settings.ProjectsPath + Settings.RecentProject + ".dceproj");
      }
      // If there is not, enable the editor right away!
      else {
        if (Settings.EditorEnabled)
          ToggleEditor(true);
      }
    }

    /**************************************************************************/
    /*!
    @brief  Loads the selected project into the engine.
    @param  path The path of the project.
    */
    /**************************************************************************/
    void Editor::LoadProject(std::string& path)
    {

      // Load the project's data into the Content system. This will
      // automatically load its resources/assets for use.
      Daisy->getSystem<Content>()->LoadProject(path);
      // Update the window caption to display the current project
      auto projectName = Daisy->getSystem<Content>()->ProjectInfo->ProjectName;
      DCTrace << "Editor::LoadProject - Opening: " << projectName << "\n";
      DispatchSystemEvents::SetWindowCaption("Daisy Chain Engine - " + projectName);
      // Load its default level 
      auto play = Daisy->getSystem<Content>()->ProjectInfo->Play;
      auto load = LoadLevel(Daisy->getSystem<Content>()->ProjectInfo->DefaultLevel);

      if (load) {
        if (play) {       
          ToggleEditor(false);
          //PlayGame();
        }
          
        else {
          DCTrace << "Editor::LoadProject - Default level found editor turned on \n";
          ToggleEditor(true);
        }
      }
        // No default level set, turn on the editor!
      else
        ToggleEditor(true);
     }
    

    /**************************************************************************/
    /*!
    @brief  Saves the active project.
    @note   This function will serialize back into the file.
    */
    /**************************************************************************/
    void Editor::SaveProject()
    {
      // Serialize all resources, scripts
    }

    /**************************************************************************/
    /*!
    @brief  Saves the currently-loaded level.
    */
    /**************************************************************************/
    void Editor::SaveCurrentLevel()
    {      
      if (CurrentSpace->CurrentLevel) {
        auto currentLevelName = CurrentSpace->CurrentLevel->Name();
        SaveLevel(currentLevelName);
      }
    }

    /**************************************************************************/
    /*!
    @brief  Opens the folder where the project is stored.
    */
    /**************************************************************************/
    void Editor::ShowProjectFolder()
    {
      // Open the projects path
      auto executablePath = boost::filesystem::initial_path().string();
      auto documentationPath = executablePath + "\\Documentation\\html\\index.html";
      // If the projects directory doesn't exist, start from the executable
      if (FileSystem::Exists(documentationPath)) {
        FileSystem::Execute(documentationPath);
      }
    }

    /**************************************************************************/
    /*!
    @brief  Starts the game, reloading the level from the last settings saved
            and unpausing it.
    */
    /**************************************************************************/
    void Editor::PlayGame()
    {
      DCTrace << "Editor::PlayGame - Playing: /"
              << Daisy->getSystem<Content>()->ProjectInfo->ProjectName << "'\n";
      
      // If on editor mode, save the currently-loaded level before exiting the editor
      if (Settings.EditorEnabled)      
        SaveCurrentLevel();

      // Turn the editor on/off
      ToggleEditor(!Settings.EditorEnabled);
    }

    /**************************************************************************/
    /*!
    @brief  Stops the game, pausing it and reloading the level.
    */
    /**************************************************************************/
    void Editor::StopGame()
    {
      DCTrace << "Editor::StopGame \n";
      // Reload the level

      // Pause it
    }

    void Editor::ExportGame()
    {
    }

    /**************************************************************************/
    /*!
    @brief  Exits the engine through the editor.
    */
    /**************************************************************************/
    void Editor::Exit()
    {
      // Save the currently-loaded level before exiting the editor
      SaveCurrentLevel();

      DispatchSystemEvents::EngineExit();
    }



  }
}