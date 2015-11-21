/******************************************************************************/
/*!
@file   MenuProject.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/15/2015
@brief  This file includes the implementation for the Editor's project menu.
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
      //auto projects = FileSystem::
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
    @brief  Opens the folder where the project is stored.
    */
    /**************************************************************************/
    void Editor::ShowProjectFolder()
    {
    }

    /**************************************************************************/
    /*!
    @brief  Starts the game, reloading the level from the last settings saved
            and unpausing it.
    */
    /**************************************************************************/
    void Editor::PlayGame()
    {
      DCTrace << "Editor::PlayGame \n";
      // Reload the level

      // Unpause it
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
    @brief  Exits the engine.
    */
    /**************************************************************************/
    void Editor::Exit()
    {
      auto exitEvent = new Events::EngineExit();
      Daisy->Dispatch<Events::EngineExit>(exitEvent);
      delete exitEvent;
    }

  }
}