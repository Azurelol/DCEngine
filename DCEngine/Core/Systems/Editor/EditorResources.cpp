/******************************************************************************/
/*!
@file   EditorResources.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   4/7/2016
@brief  This file includes the implementation for the Editor's library widget.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "EditorResources.h"

#include "../../Engine/Engine.h"

// Static variables
static char LevelName[32];

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    \brief  Displays the duplicate level Window.
    */
    /**************************************************************************/
    void EditorResources::DisplayDuplicateLevelWindow()
    {
            
      if (!ActiveWindows.DuplicateLevels) {
        return;
      }

      ImGui::SetNextWindowSize(ImVec2(150, 150), ImGuiSetCond_FirstUseEver);
      ImGui::Begin("Duplicate Level", &ActiveWindows.DuplicateLevels);
      
      ImGui::InputText("Name:", LevelName, IM_ARRAYSIZE(LevelName));
      if (ImGui::Button("Duplicate")) {
        DuplicateLevel(LevelName);
        ActiveWindows.DuplicateLevels = false;
      }


      ImGui::End();
    }

    /**************************************************************************/
    /*!
    \brief Duplicates the current level with the selected name.
    \param name The name of the duplicated level.
    */
    /**************************************************************************/
    void EditorResources::DuplicateLevel(const std::string & name)
    {
      // Saves the new level using the current level's data  
      //Daisy->getSystem<Content>()->Add
    }

    /**************************************************************************/
    /*!
    \brief  Displays the duplicate level Window.
    */
    /**************************************************************************/
    void EditorResources::ToggleDuplicateLevel()
    {
      std::strcpy(LevelName, Access().CurrentSpace->getCurrentLevel()->Name().c_str());
      ActiveWindows.DuplicateLevels = true;
    }

    /**************************************************************************/
    /*!
    \brief  EditorResources constructor.
    */
    /**************************************************************************/
    EditorResources::EditorResources() : EditorModule(true)
    {

    }



  }
}