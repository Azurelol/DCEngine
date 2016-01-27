/******************************************************************************/
/*!
@file   MainCreate.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/02/2015
@brief  This file includes the implementation for the Editor's main menu bar.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "Editor.h"
#include "../../Engine/Engine.h"
#include "../../ComponentsInclude.h"

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
      transform->setObjectName("Transform");
      //transform->setArchetype = "Transform";
      DCTrace << "Editor::CreateTransform - Created 'Transform' \n";
      SelectedObject = transform;
      WindowPropertiesEnabled = true;
      MoveToViewportCenter(transform);

      // Save the command
      auto command = CommandPtr(new CommandObjectCreation(transform, CurrentSpace,
                                          CommandObjectCreation::Setting::Create));
      Settings.Commands.Add(command);

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
      sprite->setObjectName("Sprite");
      // Add the Sprite component
      sprite->AddComponentByName(std::string("Sprite"), true);
      DCTrace << "Editor::CreateTransform - Created 'Sprite' \n";
      SelectedObject = sprite;
      WindowPropertiesEnabled = true;
      MoveToViewportCenter(sprite);
      // Save the command
      auto command = CommandPtr(new CommandObjectCreation(sprite, CurrentSpace,
                                    CommandObjectCreation::Setting::Create));
      Settings.Commands.Add(command);

    }

    /**************************************************************************/
    /*!
    @brief  Creates a GameObject with a default SpriteText.
    */
    /**************************************************************************/
    void Editor::CreateSpriteText()
    {
      // Create the SpriteText GameObject
      auto spriteText = CurrentSpace->CreateObject("SpriteText");
      spriteText->setObjectName("SpriteText");
      // Add the Sprite component
      spriteText->AddComponentByName(std::string("SpriteText"), true);
      DCTrace << "Editor::CreateSpriteText - Created 'SpriteText' \n";
      SelectedObject = spriteText;
      WindowPropertiesEnabled = true;
      MoveToViewportCenter(spriteText);
      // Save the command
      auto command = CommandPtr(new CommandObjectCreation(spriteText, CurrentSpace,
                                    CommandObjectCreation::Setting::Create));
      Settings.Commands.Add(command);
    }

    /**************************************************************************/
    /*!
    @brief  Creates a GameObject with a default particle system.
    */
    /**************************************************************************/
    void Editor::CreateParticleSystem()
    {
      // Create the ParticleSystem GameObject
      auto psystem = CurrentSpace->CreateObject("ParticleEffect");
      psystem->setObjectName("ParticleSystem");
      // Add the Sprite component
      //psystem->AddComponentByName(std::string("Particle System"), true);
      DCTrace << "Editor::CreateParticleSystem - Created 'ParticleEffect' \n";
      SelectedObject = psystem;
      WindowPropertiesEnabled = true;
      MoveToViewportCenter(psystem);
      // Save the command
      auto command = CommandPtr(new CommandObjectCreation(psystem, CurrentSpace,
                                    CommandObjectCreation::Setting::Create));
      Settings.Commands.Add(command);
    }

    /**************************************************************************/
    /*!
    @brief  Creates a GameObject from an Archetype into the current space.
    @todo   Create the GameObject from a "Sprite' archetype rather than
    hard-coded here.
    */
    /**************************************************************************/
    void Editor::CreateFromArchetype(std::string & archetype)
    {
      auto gameObject = CurrentSpace->CreateObject(Daisy->getSystem<Content>()->getArchetype(archetype));
      SelectedObject = gameObject;
      WindowPropertiesEnabled = true;
      MoveToViewportCenter(gameObject);
      // Save the command
      auto command = CommandPtr(new CommandObjectCreation(gameObject, CurrentSpace,
                                    CommandObjectCreation::Setting::Create));
      Settings.Commands.Add(command);
    }

    /**************************************************************************/
    /*!
    @brief  Moves the object to the viewport's center.
    */
    /**************************************************************************/
    void Editor::MoveToViewportCenter(GameObject* gameobject)
    {
      auto viewportCenter = CurrentSpace->getComponent<Components::CameraViewport>()->getCamera()->TransformComponent->Translation;
      gameobject->getComponent<Components::Transform>()->setTranslation(Vec3(viewportCenter.x, viewportCenter.y, 0));
    }

    /**************************************************************************/
    /*!
    @brief  Saves a GameObject's configuration into an Archetype to file.
    @todo   Refactor how the path is pulled.
    */
    /**************************************************************************/
    void Editor::SaveArchetype(std::string& archetype)
    {
      // Get the current project's archetype path
      auto path = Settings.ProjectInfo->ProjectPath + Settings.ProjectInfo->ResourcePath
                                                    + archetype + Archetype::Extension();

      // Create the archetype
      auto archetypePtr = Daisy->getSystem<Factory>()->BuildArchetype(path, dynamic_cast<GameObjectPtr>(SelectedObject));
      // Save it
      archetypePtr->Save();
      // Upload the latest archetype

      // Scan for archetypes again
      Daisy->getSystem<Content>()->ScanForArchetypes();
    }

    
  }
}