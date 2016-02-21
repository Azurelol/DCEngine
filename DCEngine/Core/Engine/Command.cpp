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

  /**************************************************************************/
  /*!
  @brief  Copies the selected GameObjects.
  @param  A container containing all the game objects to copy.
  */
  /**************************************************************************/
  void CommandManager::Copy(GameObjectRawVec gameObjects)
  {
    // Clear the data on previously copied objects
    ObjectCopyData.clear();

    for (auto& gameObj : gameObjects) {
      auto copyName = gameObj->Name() + " Copy";
      ObjectCopyData.push_back(Daisy->getSystem<Systems::Factory>()->BuildArchetype(copyName, gameObj));
    }
  }

  /**************************************************************************/
  /*!
  @brief  Pastes the selected GameObjects.
  @todo   Need to create all at once!
  */
  /**************************************************************************/
  void CommandManager::Paste(Space * space)
  {
    if (!ObjectCopyData.empty()) {
      auto createCommand = CommandPtr(new CommandObjectCreation(ObjectCopyData, space));
      // Recreate the object on the current space
      createCommand->Execute();
      // Add this command to the stack
      Add(createCommand);
    }    
  }

  /*===================*
  *     Creation       *
  *===================*/
  /**************************************************************************/
  /*!
  @brief  CommandObjectCreation constructor.
  */
  /**************************************************************************/
  CommandObjectCreation::CommandObjectCreation(GameObjectPtr gameObject, Space * space, Setting setting)
                                              : Command("Creation - " + gameObject->Name()),
                                               SpaceRef(space), CurrentSetting(setting)
  {
    GameObjectReferences.push_back(gameObject);
    Copy();
    DCTrace << "CommandObjectCreation - GameObject: " << GameObjectReferences.front()->Name() << "\n";
  }


  CommandObjectCreation::CommandObjectCreation(GameObjectRawVec objects, Space* space, Setting setting) 
                                               : Command("Creation - " + objects.front()->Name()), 
                                                GameObjectReferences(objects), SpaceRef(space), CurrentSetting(setting)
  {
    // Save the data of the selected GameObject
    Copy();
    DCTrace << "CommandObjectCreation - GameObject: " << GameObjectReferences.front()->Name() << "\n";
  }

  CommandObjectCreation::CommandObjectCreation(ArchetypeContainer copyData, Space * space)
                                               : Command("Creation - " + copyData.front()->Name()),
                                                 GameObjectData(copyData), SpaceRef(space), CurrentSetting(Setting::Create)
  {

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
  @brief (Re)creates the GameObject(s).
  */
  /**************************************************************************/
  void CommandObjectCreation::Create()
  {
    for (auto& gameObjectData : GameObjectData) {
      GameObjectReferences.push_back(SpaceRef->CreateObject(ArchetypePtr(gameObjectData)));
      DCTrace << "CommandObjectDelete::Create - Created GameObject: " << gameObjectData->Name() << "\n";
    }    
  }

  /**************************************************************************/
  /*!
  @brief Destroys the GameObjects.
  */
  /**************************************************************************/
  void CommandObjectCreation::Destroy()
  {
    for (auto& gameObj : GameObjectReferences) {
      DCTrace << "CommandObjectDelete::Destroy - Deleted GameObject: " << gameObj->Name() << "\n";
      gameObj->Destroy();
    }

    GameObjectReferences.clear();

  }

  /**************************************************************************/
  /*!
  @brief Copies the GameObject's data into an Archtype.
  */
  /**************************************************************************/
  void CommandObjectCreation::Copy()
  {
    for (auto& gameObj : GameObjectReferences) {
      GameObjectData.push_back(Daisy->getSystem<Systems::Factory>()->BuildArchetype("object", gameObj));
    }    
  }

  /*===================*
  *     Transform     *
  *===================*/ 
  CommandObjectTransform::CommandObjectTransform(TransformDataPairVec & previous, TransformDataPairVec & current)
    : Command("Transform"), PreviousTransforms(previous), CurrentTransforms(current)
  {
  }

  /**************************************************************************/
  /*!
  @brief  Undoes the last Transform command, reverting transformations.
  */
  /**************************************************************************/
  void CommandObjectTransform::Undo()
  { 
    ApplyTransform(PreviousTransforms);
  }

  /**************************************************************************/
  /*!
  @brief Executes the Transform command.
  */
  /**************************************************************************/
  void CommandObjectTransform::Execute()
  {
    ApplyTransform(CurrentTransforms);
  }

  /**************************************************************************/
  /*!
  @brief Applies the currently-stored transforms to each transform component
         on the pair.
  @param transforms A container of transform component-transform data pairs.
  */
  /**************************************************************************/
  void CommandObjectTransform::ApplyTransform(TransformDataPairVec & transforms)
  {
    for (auto& transform : transforms) {
      transform.first->setTranslation(transform.second.Translation);
      transform.first->setRotation(transform.second.Rotation);
      transform.first->setScale(transform.second.Scale);
    }
  }








}