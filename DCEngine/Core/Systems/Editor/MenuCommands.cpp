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
    @brief  Displays the Command stack.
    */
    /**************************************************************************/
    void Editor::WindowCommands()
    {
      if (!WindowCommandsEnabled)
        return;

      ImGui::SetNextWindowSize(ImVec2(200, 400), ImGuiSetCond_FirstUseEver);
      ImGui::Begin("Commands", &WindowCommandsEnabled);

      for (auto& command : Settings.Commands.CommandsCurrent) {
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
      Settings.Commands.Undo();
    }

    /**************************************************************************/
    /*!
    @brief  Redoes the last command.
    */
    /**************************************************************************/
    void Editor::Redo()
    {
      Settings.Commands.Redo();
    }

    /**************************************************************************/
    /*!
    @brief  Deletes the currently selected GameObject.
    @todo   
    */
    /**************************************************************************/
    void Editor::DeleteObject()
    {
      if (!SelectedObject) {
        DCTrace << "Editor::DeleteObject - No object selected! \n";
        return;
      }
      
      if (SelectedObject->Name() == "EditorCamera") {        
        DCTrace << "Editor::DeleteObject - Cannot delete the editor camera! Y-y-you trying to crash or sumthin?? \n";
        return;        
      }

      //ImGui::OpenPopup("#ProtectTheEditorCamera");
      //if (ImGui::BeginPopupModal("#ProtectTheEditorCamera")) {
      //  ImGui::Text("Cannot delete the editor camera! Y-y-you trying to crash or sumthin??");

      //  if (ImGui::Button("I am sorry")) {
      //    ImGui::CloseCurrentPopup();
      //  }

      //  ImGui::EndPopup();
      //  
      //}


      // Destroy the currently selected GameObject
      if (auto gameObject = dynamic_cast<GameObject*>(SelectedObject)) {
        // Save the command
        auto deleteCommand = CommandPtr(new CommandObjectCreation(gameObject, CurrentSpace, 
                                            CommandObjectCreation::Setting::Destroy));
        deleteCommand->Execute();
        Settings.Commands.Add(deleteCommand);

        // Destroy the object
        //gameObject->Destroy();
        DCTrace << "Editor::DeleteObject - Deleting gameobject: " << SelectedObject->Name() << "\n";
        SelectedObject = nullptr;        
      }

      // Destroy the currently selected Resource
      if (auto resource = dynamic_cast<Resource*>(SelectedObject)) {               
        DeleteResource(resource);
        SelectedObject = nullptr;
      }

      // Turn off the Properties window
      WindowPropertiesEnabled = false;


    }

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
    @brief  Duplicates the currently selected GameObject.
    */
    /**************************************************************************/
    void Editor::DuplicateObject()
    {
      DCTrace << "Editor::DuplicateObject - " << SelectedObject->Name() << "\n";
    }




  }
}