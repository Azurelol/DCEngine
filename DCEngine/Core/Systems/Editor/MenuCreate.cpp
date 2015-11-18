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
    @brief  Creates a GameObject with a Transform component on the selected
            space.
    */
    /**************************************************************************/
    void Editor::CreateTransform()
    {      
      // Create the empty transform
      auto transform = CurrentSpace->CreateObject();
      transform->setName("Transform");
      //transform->setArchetype = "Transform";
      DCTrace << "Editor::CreateTransform - Created 'Transform' \n";
    }

    /**************************************************************************/
    /*!
    @brief  Creates a GameObject with Transform and Sprite components on the 
            selected space.
    @todo   Create the GameObject from a "Sprite' archetype rather than
            hard-coded here.
    */
    /**************************************************************************/
    void Editor::CreateSprite()
    {
      // Create the Sprite GameObject
      auto sprite = CurrentSpace->CreateObject();
      sprite->setName("Sprite");
      // Add the Sprite component
      sprite->AddComponent<Sprite>();
      DCTrace << "Editor::CreateTransform - Created 'Transform' \n";
    }


  }
}