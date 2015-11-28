/******************************************************************************/
/*!
@file   MenuResources.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/15/2015
@brief  This file includes the implementation for the Editor's resource menu.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "Editor.h"
#include "../../Engine/Engine.h"
#include "../Testing.h" // Dollhouse

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    @brief  Adds a resource to the project.
    */
    /**************************************************************************/
    void Editor::AddResource()
    {

    }


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
      SelectedObject = nullptr;

      if (levelPtr == nullptr) {
        DCTrace << "Editor::LoadLevel - Could not find " << level << "\n";
        return false;
      }
        
      // Load the level
      CurrentSpace->LoadLevel(level);
      // Load the editor camera
      SetEditorCamera(true);
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
      std::string LevelPath("Projects/Rebound/Resources/Levels/");
      auto levelResource = CurrentSpace->SaveLevel(LevelPath + level + std::string(".lvl"));
      
      // If the level was saved successfully
      if (levelResource) {
        // Scan for levels again
        Daisy->getSystem<Content>()->ScanForLevels();
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
      SelectedObject = nullptr;
      // Eh?
      return true;
    }

    /**************************************************************************/
    /*!
    @brief Loads the hard-coded level, Dollhouse.
    */
    /**************************************************************************/
    void Editor::LoadDollhouse()
    {
      SelectedObject = nullptr;
      // !!! TESTING: Level loading
      LevelPtr dollhouse = LevelPtr(new DollHouse(*CurrentSpace, CurrentSpace->getGameSession()));
      CurrentSpace->LoadSampleLevel(dollhouse);      
      // Load the editor camera
      SetEditorCamera(true);
    }

  }
}