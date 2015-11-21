/******************************************************************************/
/*!
@file   MenuResources.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/15/2015
@brief  This file includes the implementation for the Editor's resource menu.
*/
/******************************************************************************/
#include "Editor.h"
#include "../../Engine/Engine.h"
#include "../Testing.h" // Dollhouse

namespace DCEngine {
  namespace Systems {

    void Editor::AddResource()
    {
    }

    /**************************************************************************/
    /*!
    @brief  Loads the level, deserializing a level file's binary data and
            constructing gameobjects along with their components.
    */
    /**************************************************************************/
    void Editor::LoadLevel(std::string level)
    {
      DCTrace << "Editor::LoadLevel - Loading " << level << "\n";
      
      // Query the Content system for the Level resource 
      auto levelPtr = Daisy->getSystem<Systems::Content>()->getLevel(level);
      DCTrace << "Hey you loaded me man here you go: " << levelPtr->Get() << " \n";

      //CurrentSpace->LoadLevel(level);

      //std::string levelData = 
    }

    /**************************************************************************/
    /*!
    @brief  Saves the current level, serializing all of the active gameobjects
            along with their components in the space.
    @todo   The level path is currently hardcoded. Change that.
    */
    /**************************************************************************/
    void Editor::SaveLevel(std::string level)
    {
      DCTrace << "Editor::SaveLevel - Saving " << level << "\n";
      // Request the space to serialize the current level and return that string.
      //CurrentSpace->SaveLevel(level);
      
      //Zilch::JsonBuilder builder;
      std::string levelData = CurrentSpace->SaveLevel(level);
      //std::string levelData = "Haha I am totally serialized now dude my name is " + level;      

      // Get the current project's path
      std::string LevelPath("Projects/Rebound/Resources/Levels/");
      FileSystem::FileWriteString(LevelPath + level + std::string(".lvl"), levelData);

    }

    void Editor::ReloadLevel()
    {
    }

    void Editor::LoadDollhouse()
    {
      // !!! TESTING: Level loading
      LevelPtr dollhouse = LevelPtr(new DollHouse(*CurrentSpace, CurrentSpace->getGameSession()));
      CurrentSpace->LoadLevel(dollhouse);
    }

  }
}