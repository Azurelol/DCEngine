/******************************************************************************/
/*!
@file   MenuEdit.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/15/2015
@brief  This file includes the implementation for the Editor's edit menu
        operations.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "Editor.h"
#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    @brief  Deletes the currently selected GameObject.
    */
    /**************************************************************************/
    void Editor::DeleteObject()
    {
      if (!SelectedObject) {
        DCTrace << "Editor::DeleteObject - No object selected! \n";
        return;
      }
      
      // Destroy the currently selected GameObject
      if (auto gameObject = dynamic_cast<GameObject*>(SelectedObject)) {
        gameObject->Destroy();
        SelectedObject = nullptr;
        DCTrace << "Editor::DeleteObject - " << SelectedObject->Name() << "\n";
      }
    }


    /**************************************************************************/
    /*!
    @brief  Duplicates the currently selected GameObject.
    */
    /**************************************************************************/
    void Editor::DuplicateObject()
    {
      DCTrace << "Editor::DuplicateObject - " << SelectedObject->Name() << "\n";
    }




  }
}