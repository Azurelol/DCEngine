/*****************************************************************************/
/*!
@file   Command.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   12/25/2015
@brief  The Command Class is used by the editor to store a list of all
recently given commands to the Editor. Such as deleting, moving,
creating objects, etc.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Command.h"

#include "../Engine/Engine.h"
#include "../../Core/Components/Transform.h"

namespace DCEngine {

  /*===================*
  *     Manager       *
  *===================*/

  /**************************************************************************/
  /*!
  @brief  CommandManager constructor
  */
  /**************************************************************************/
  CommandManager::CommandManager() : Maximum(10)
  {
  }

  /**************************************************************************/
  /*!
  @brief  Adds the latest command.
  @param  command A pointer to the latest command.
  */
  /**************************************************************************/
  void CommandManager::Add(CommandPtr command)
  {
    // Add the newest, current command
    CommandsCurrent.push_back(command);

    // If the number of commands has gone over the maximum, remove the oldest command
    if (CommandsCurrent.size() > Maximum)
      CommandsCurrent.pop_front();
  }

  /**************************************************************************/
  /*!
  @brief  Undoes the last command.
  */
  /**************************************************************************/
  void CommandManager::Undo()
  {
    if (CommandsCurrent.empty())
      return;

    // Undo the last 'Current' command
    CommandsCurrent.back()->Undo();
    // Move it to the 'Undo' command stack
    CommandsUndo.push_back(CommandsCurrent.back());
    // Remove it from the 'Current' command stack
    CommandsCurrent.pop_back();

    // If the number of 'Undo' commands has gone over the maximum, remove the oldest command
    if (CommandsUndo.size() > Maximum)
      CommandsUndo.pop_front();
  }

  /**************************************************************************/
  /*!
  @brief  Redoes the last command.
  */
  /**************************************************************************/
  void CommandManager::Redo()
  {
    if (CommandsUndo.empty())
      return;

    // Redo the last 'Undo' command
    CommandsUndo.back()->Execute();
    // Move it to the 'Current' command stack
    CommandsCurrent.push_back(CommandsUndo.back());
    // Remove it from the 'Undo' command stack
    CommandsUndo.pop_back();

    // If the number of 'Current' commands has gone over the maximum, remove the oldest command
    if (CommandsCurrent.size() > Maximum)
      CommandsCurrent.pop_front();
  }


  /*===================*
  *     Creation       *
  *===================*/
  /**************************************************************************/
  /*!
  @brief  CommandObjectCreation constructor.
  */
  /**************************************************************************/
  CommandObjectCreation::CommandObjectCreation(GameObject * object, Space* space, Setting setting) : Command("Creation - " + object->Name()),
                                                                                GameObjectRef(object), SpaceRef(space), CurrentSetting(setting)
  {    
    // Save the data of the selected GameObject
    GameObjectData = Daisy->getSystem<Systems::Factory>()->BuildArchetype("object", GameObjectRef);
    DCTrace << "CommandObjectDelete - GameObject: " << object->Name() << "\n";
  }

  /**************************************************************************/
  /*!
  @brief  Undoes the last Creation command, specific to the setting set.
  */
  /**************************************************************************/
  void CommandObjectCreation::Undo()
  {
    switch (CurrentSetting) {
    case Setting::Create:
      Destroy();
      break;

    case Setting::Destroy:
      Create();
      break;

    default:
      break;
    }    
  }

  /**************************************************************************/
  /*!
  @brief  Undoes the last Creation command, specific to the setting set.
  */
  /**************************************************************************/
  void CommandObjectCreation::Execute()
  {
    switch (CurrentSetting) {
    case Setting::Create:
      Create();
      break;

    case Setting::Destroy:
      Destroy();
      break;

    default:
      break;
    }
  }

  /**************************************************************************/
  /*!
  @brief (Re)creates the GameObject.
  */
  /**************************************************************************/
  void CommandObjectCreation::Create()
  {
    auto a = GameObjectData;
    GameObjectRef = SpaceRef->CreateObject(ArchetypePtr(GameObjectData));
    
    DCTrace << "CommandObjectDelete::Create - Created GameObject: " << GameObjectRef->Name() << "\n";
  }

  /**************************************************************************/
  /*!
  @brief Destroys the GameObject.
  */
  /**************************************************************************/
  void CommandObjectCreation::Destroy()
  {
    DCTrace << "CommandObjectDelete::Destroy - Deleted GameObject: " << GameObjectRef->Name() << "\n";
    GameObjectRef->Destroy();
    GameObjectRef = nullptr;

  }


  /*===================*
  *     Transform     *
  *===================*/ 
  CommandObjectTransform::CommandObjectTransform(Components::Transform * transform) : Command("Transform"), 
                                                                                      TransformedObject(transform)
  {
  }


  void CommandObjectTransform::Undo()
  { 
    TransformedObject->setTranslation(PreviousTranslation);
    TransformedObject->setRotation(PreviousRotation);
    TransformedObject->setScale(PreviousScale);
  }

  void CommandObjectTransform::Execute()
  {
    TransformedObject->setTranslation(NewTranslation);
    TransformedObject->setRotation(NewRotation);
    TransformedObject->setScale(NewScale);
  }

  void CommandObjectTransform::SaveNew(Vec3 transform, Vec3 rotation, Vec3 scale)
  {
    NewTranslation = transform;
    NewRotation = rotation;
    NewScale = scale;
  }

  void CommandObjectTransform::SavePrevious(Vec3 transform, Vec3 rotation, Vec3 scale)
  {
    PreviousTranslation = transform;
    PreviousRotation = rotation;
    PreviousScale = scale;
  }






}