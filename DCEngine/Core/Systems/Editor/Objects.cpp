/******************************************************************************/
/*!
@file   Objects.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/02/2015
@brief  This file includes the implementation for the Editor's objects widget.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "Editor.h"
#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    @brief  Displays a list of objects in the default space.
    @todo   Find a way to swap and parent objects to one another with dragging.
    */
    /**************************************************************************/
    void Editor::WindowObjects()
    {
      if (!Windows.ObjectsEnabled)
        return;

      // Set the position of the window
      ImGui::SetNextWindowSize(ImVec2(200, 400), ImGuiSetCond_FirstUseEver | ImGuiWindowFlags_ShowBorders);
      // Title
      ImGui::Begin("Objects", &Windows.ObjectsEnabled);
      
      // Capture a list of all objects on the space
      auto objects = Daisy->getGameSession()->getDefaultSpace()->AllObjects();
      auto objectIdentifiers = CurrentSpace->IdentifyAllObjects();
      auto names = objectIdentifiers.NamesAsChars();

      ImGui::PushItemWidth(ImGui::GetWindowWidth() / 2.0f);
      ImGui::TextColored(ImVec4(255, 0, 0, 255), "Attach To");
      ImGui::SameLine();
      static int currentAttachable = 0;
      if (ImGui::Combo("##Attach", &currentAttachable, names.data(), static_cast<int>(names.size()))) {
        DCTrace << "Attaching to " << objectIdentifiers.Names[currentAttachable] << ", with ID: " << objectIdentifiers.IDs[currentAttachable] << "\n";
      }
      if (ImGui::Button("Detach")) {      
      }
      ImGui::SameLine();
      //ImGui::Columns(2);
      if (ImGui::Button("Up")) {
        if (auto gameObject = dynamic_cast<GameObjectPtr>(SelectedObject()))
          ObjectsListSwapPosition(gameObject, Direction::Up);
      }
      ImGui::SameLine();
      if (ImGui::Button("Down")) {
        if (auto gameObject = dynamic_cast<GameObjectPtr>(SelectedObject()))
          ObjectsListSwapPosition(gameObject, Direction::Down);
      }



      //ImGui::NextColumn();
      // Display every object's name
      unsigned objID = 0;
      for (auto& object : *objects) {
        ImGui::PushID(objID++);
        auto objectName = object->Name().c_str();   

        // If the object was found among one of those selected..
        bool selected = SelectedObject() && SelectedObject()->getObjectID() == object->getObjectID();
        if (ImGui::Selectable(objectName, selected)) {
        //if (ImGui::Selectable(objectName, SelectedObject() && SelectedObject()->getObjectID() == object->getObjectID())) {
          SelectObject(object);
          Inspector.Toggle(true);
        }
        ImGui::PopID();
      }
      // Ends the window
      ImGui::End();
    }

    /**************************************************************************/
    /*!
    @brief  Swaps the position of the currently selected GameObject.
    @param object A pointer to the object.
    @param dir The direction to which to move it.
    @todo   Find a way to swap and parent objects to one another with dragging.
    */
    /**************************************************************************/
    void Editor::ObjectsListSwapPosition(GameObjectPtr object, Direction dir)
    {
      if (dir == Direction::Up) {
        CurrentSpace->SwapGameObject(object, Direction::Up);
      }
      else if (dir == Direction::Down) {
        CurrentSpace->SwapGameObject(object, Direction::Down);
      }
      
    }




  }
}