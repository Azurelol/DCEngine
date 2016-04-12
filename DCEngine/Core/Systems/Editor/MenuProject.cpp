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
        Projects.LoadProject(std::string(Settings.ProjectsPath + Settings.RecentProject + ".dceproj"));
        //LoadProject(Settings.ProjectsPath + Settings.RecentProject + ".dceproj");
      }
      // If there is not, enable the editor right away!
      else {
        if (Active) {
          Launcher.Launch();
          Projects.ReloadLevel();
        }
      }
    }

    /**************************************************************************/
    /*!
    @brief  Loads the selected project into the engine.
    @param  path The path of the project.
    */
    /**************************************************************************/
    void Editor::LoadProject(std::string path)
    {

      // Redirect ho!
      Projects.LoadProject(path);
      return;

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
              << Settings.ProjectProperties->ProjectName << "'\n";
      
      // If the Editor is not enabled, do nothing
      if (!Settings.Enabled)
        return;

      // If on editor mode, save the currently-loaded level before exiting the editor
      if (Active)      
        Access().Projects.SaveCurrentLevel();

      // Turn the editor on/off
      ToggleEditor();
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
      Deselect();
      // Save the currently-loaded level before exiting the editor
      Access().Projects.SaveCurrentLevel();
      // Save the current project's settings
      Projects.SaveProject();
      DispatchSystemEvents::EngineExit();
    }



  }
}