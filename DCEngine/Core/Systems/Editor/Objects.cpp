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
      ImGui::SetNextWindowSize(ImVec2(200, 400), ImGuiSetCond_FirstUseEver);
      // Title
      ImGui::Begin("Objects", &Windows.ObjectsEnabled);
      
      ImGui::SameLine();
      if (ImGui::Button("Up")) {
        if (auto gameObject = dynamic_cast<GameObjectPtr>(SelectedObject()))
          ObjectsListSwapPosition(gameObject, Direction::Up);
      }
      ImGui::SameLine();
      if (ImGui::Button("Down")) {
        if (auto gameObject = dynamic_cast<GameObjectPtr>(SelectedObject()))
          ObjectsListSwapPosition(gameObject, Direction::Down);
      }

      // Display every object's name
      unsigned objID = 0;
      auto objects = Daisy->getGameSession()->getDefaultSpace()->AllObjects();
      for (auto& object : *objects) {
        ImGui::PushID(objID++);
        auto objectName = object->Name().c_str();   
        // If the user has selected the GameObject.
        //if (ImGui::Selectable(objectName)) {
        //auto selectedObject = SelectedObject();
        //static unsigned selectedObjectID = 0;
        //if (selectedObject)
        //  selectedObjectID = selectedObject->getObjectID();
        //unsigned objectID = object->getObjectID();
        //if (ImGui::Selectable(objectName, selectedObjectID == objectID)) {


        if (ImGui::Selectable(objectName, SelectedObject() && SelectedObject()->getObjectID() == object->getObjectID())) {
          SelectObject(object);
          Windows.PropertiesEnabled = true;
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