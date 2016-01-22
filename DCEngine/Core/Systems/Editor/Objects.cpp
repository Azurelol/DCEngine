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
      if (!WidgetObjectsEnabled)
        return;

      // Set the position of the window
      ImGui::SetNextWindowSize(ImVec2(200, 400), ImGuiSetCond_FirstUseEver);
      // Title
      ImGui::Begin("Objects", &WidgetObjectsEnabled);
      
      ImGui::SameLine();
      if (ImGui::Button("Up")) {
        if (auto gameObject = dynamic_cast<GameObjectPtr>(SelectedObject))
          ObjectsListSwapPosition(gameObject, Direction::Up);
      }
      ImGui::SameLine();
      if (ImGui::Button("Down")) {
        if (auto gameObject = dynamic_cast<GameObjectPtr>(SelectedObject))
          ObjectsListSwapPosition(gameObject, Direction::Down);
      }

      // Print every name
      auto objects = Daisy->getGameSession()->getDefaultSpace()->AllObjects();
      
      // Allocate a dynamically allocated array for selectable
      //static std::vector<bool> selected(objects->size());
      //int i = 0;
      for (auto& object : *objects) {
        auto ObjectName = object->Name().c_str();
        // If the user has selected the GameObject.
        if (ImGui::Selectable(ObjectName)) {
        //if (ImGui::Selectable(ObjectName), SelectedObject && (SelectedObject->Name() == object->Name())) {
        //if (ImGui::Selectable(ObjectName, &selected[i++])) {
        //if (ImGui::Button(ObjectName)) {
          SelectedObject = object;
          WindowPropertiesEnabled = true;
        }          
        //ImGui::Text(object->Name().c_str());
      }

      // Ends the window
      ImGui::End();

    }

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