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

    GameObjectRawVec ObjectPtrsToGameObjectPtrs(ObjectContainer objects) {
      GameObjectRawVec gameObjects;
      for (auto& objectPtr : objects) {
        gameObjects.push_back(dynamic_cast<GameObjectPtr>(objectPtr));
      }
      return gameObjects;
    }

    /**************************************************************************/
    /*!
    @brief  Displays the Command stack.
    */
    /**************************************************************************/
    void Editor::WindowCommands()
    {
      if (!Windows.CommandsEnabled)
        return;

      ImGui::SetNextWindowSize(ImVec2(200, 400), ImGuiSetCond_FirstUseEver);
      ImGui::Begin("Commands", &Windows.CommandsEnabled);

      ImGui::TextColored(ImVec4(1, 0, 0, 1), "Current");
      for (auto& command : Settings.Commands.CommandsCurrent) {
        ImGui::Text(command->CommandName.c_str());
      }
      ImGui::Separator();
      ImGui::TextColored(ImVec4(1, 0, 0, 1), "Undo");
      for (auto& command : Settings.Commands.CommandsUndo) {
        ImGui::Text(command->CommandName.c_str());
      }

      ImGui::End();

    }

    /**************************************************************************/
    /*!
    @brief  Undoes the last command.
    */
    /**************************************************************************/
    void Editor::Undo()
    {
      // If undoing a creation, delesect the object
      if (Settings.Commands.CommandsCurrent.empty()) // ew
        return;

      auto lastCommand = Settings.Commands.CommandsCurrent.back();
      if (auto creationCommand = dynamic_cast<CommandObjectCreation*>(lastCommand.get()) ) {
        if (!creationCommand->GameObjectReferences.empty() && SelectedObject() == creationCommand->GameObjectReferences.front()) // what
          Deselect();
      }

      Settings.Commands.Undo();

      DCTrace << "Editor::Undo \n";
    }

    /**************************************************************************/
    /*!
    @brief  Redoes the last command.
    */
    /**************************************************************************/
    void Editor::Redo()
    {
      Settings.Commands.Redo();
      DCTrace << "Editor::Redo \n";
    }

    /**************************************************************************/
    /*!
    @brief  Makes a 'Cut' of the selected object. (Copy/Deletion)
    */
    /**************************************************************************/
    void Editor::Cut()
    {
      GameObjectRawVec gameObjects;
      Settings.Commands.Copy(ObjectPtrsToGameObjectPtrs(SelectedObjects));
      DeleteObject();
      DCTrace << "Editor::Cut \n";
    }

    /**************************************************************************/
    /*!
    @brief  Makes a Copy of the selected object.
    */
    /**************************************************************************/
    void Editor::Copy()
    {
      Settings.Commands.Copy(ObjectPtrsToGameObjectPtrs(SelectedObjects));
      DCTrace << "Editor::Copy \n";
    }

    /**************************************************************************/
    /*!
    @brief Recreates the last copied object.
    */
    /**************************************************************************/
    void Editor::Paste()
    {
      Settings.Commands.Paste(CurrentSpace);
      DCTrace << "Editor::Paste \n";
    }

    /**************************************************************************/
    /*!
    @brief  Makes a 'Duplicate' of the selected object. (Copy/Paste)
    */
    /**************************************************************************/
    void Editor::Duplicate()
    {
      Settings.Commands.Copy(ObjectPtrsToGameObjectPtrs(SelectedObjects));
      Settings.Commands.Paste(CurrentSpace);
      DCTrace << "Editor::Duplicate \n";
    }

    /**************************************************************************/
    /*!
    @brief  Adds children to the container??
    */
    /**************************************************************************/
    void AddChildrenToContainer(EntityPtr gameObject, EntityVec& children) {
      for (auto& child : gameObject->Children()) {
        // Add this child to the container
        children.push_back(child);
        // Add all its children
        AddChildrenToContainer(child, children);
      }
    }

    /**************************************************************************/
    /*!
    @brief  Deletes the currently selected GameObject.
    */
    /**************************************************************************/
    void Editor::DeleteObject()
    {
      if (!SelectedObject()) {
        DCTrace << "Editor::DeleteObject - No object selected! \n";
        return;
      }
      
      if (SelectedObject()->Name() == "EditorCamera") {        
        DCTrace << "Editor::DeleteObject - Cannot delete the editor camera! Y-y-you trying to crash or sumthin?? \n";
        return;        
      }

      // Destroy the currently selected GameObjects
      if (dynamic_cast<GameObjectPtr>(SelectedObject())) {

        // The list of objects to be deleted
        auto objectsToDelete = ObjectPtrsToGameObjectPtrs(SelectedObjects);
        // Also add their children
        //GameObjectRawVec childrenToDelete;
        //for (auto& object : objectsToDelete) {
        //  AddChildrenToContainer(object, childrenToDelete);
        //}
        //// Add the children to the list of objects to be deleted
        //objectsToDelete.insert(objectsToDelete.begin(), childrenToDelete.begin(), 
        //                       childrenToDelete.end());

        // Save the command
        auto deleteCommand = CommandPtr(new CommandObjectCreation(objectsToDelete, 
                                                                  CurrentSpace,
                                        CommandObjectCreation::Setting::Destroy));
        deleteCommand->Execute();
        Settings.Commands.Add(deleteCommand);

        // Destroy the object
        DCTrace << "Editor::DeleteObject - Deleting gameobject: " << SelectedObject()->Name() << "\n";
        Deselect();
      }
      // Destroy the currently selected Resource
      if (auto resource = dynamic_cast<Resource*>(SelectedObject())) {               
        DeleteResource(resource);
        Deselect();
      }

      // Turn off the Properties window
      Inspector.Toggle(false);
    }

    /**************************************************************************/
    /*!
    @brief  Deletes the currently selected Resource.
    @todo
    */
    /**************************************************************************/
    void Editor::DeleteResource(ResourcePtr resource)
    {
      DCTrace << "Editor::DeleteResource - Deleting resource: " << resource->Name() << "\n";
      Daisy->getSystem<Content>()->RemoveResource(resource);

      // If the deleted resource is the current level.. 
      if (auto level = dynamic_cast<Level*>(resource)) {
        if (level == CurrentSpace->CurrentLevel.get()) {
          // Clear the current level

        }
      }
    }



    /**************************************************************************/
    /*!
    @brief  Moves the object to the viewport's center.
    @param  A pointer to the GameObject.
    */
    /**************************************************************************/
    void Editor::MoveToViewportCenter(GameObject* gameobject)
    {
      auto viewportCenter = CurrentSpace->getComponent<Components::CameraViewport>()->getCamera()->TransformComponent->Translation;
      gameobject->getComponent<Components::Transform>()->setTranslation(Vec3(viewportCenter.x, viewportCenter.y, 0));
    }


  }
}