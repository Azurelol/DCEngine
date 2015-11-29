/******************************************************************************/
/*!
@file   ResourcesAdd.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/28/2015
@brief  Methods that add various resources to the currently active project.
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
    @brief  Creates a level on the currently selected project
    @param  The name of the level to create.
    */
    /**************************************************************************/
    bool Editor::CreateLevel(std::string & name)
    {
      DCTrace << "Editor::CreateLevel - Creating " << name << "\n";
      // Save the current level
      SaveLevel(CurrentSpace->CurrentLevel->Name());
      // Destroy the objects in the space
      CurrentSpace->DestroyAll();
      // Add a default camera
      CurrentSpace->CreateObject("Camera");      
      // Save the level
      SaveLevel(name);
      // Add the editor camera right away
      SetEditorCamera(true);
      return true;
    }

  }
}
