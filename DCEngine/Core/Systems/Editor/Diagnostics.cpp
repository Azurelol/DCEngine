/******************************************************************************/
/*!
@file   Diagnostics.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/02/2015
@brief  This file includes the implementation for the Editor's library widget.
*/
/******************************************************************************/
#include "Editor.h"
#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    \brief  Displays the properties of the currently selected object.
    */
    /**************************************************************************/
    void Editor::WidgetDiagnostics()
    {
      if (!WidgetDiagnosticsEnabled)
        return;

      ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiSetCond_FirstUseEver);
      ImGui::Begin("Diagnostics", &WidgetDiagnosticsEnabled);
      if (ImGui::TreeNode("Object Allocation")) {
        ////////////////
        // GameObjects
        ///////////////
        auto gameObjsCreated = std::string("GameObjects allocated: ") +
                               std::to_string(GameObject::GameObjectsCreated);
        ImGui::Text(gameObjsCreated.c_str());
        auto gameObjsDeleted = std::string("GameObjects deleted: ") +
                               std::to_string(GameObject::GameObjectsDestroyed);
        ImGui::Text(gameObjsDeleted.c_str());
        // If the diagnostics flag for components has been turned on...
        if (GameObject::DiagnosticsEnabled) {
          auto gameObjectsLastCreated = std::string("Last GameObject created: ") +
            GameObject::GameObjectLastCreated;
          ImGui::Text(gameObjectsLastCreated.c_str());
          auto gameObjectsLastDestroyed = std::string("Last GameObject destroyed: ") +
            GameObject::GameObjectLastDestroyed;
          ImGui::Text(gameObjectsLastDestroyed.c_str());
        }
        ImGui::Separator();

        //////////////
        // Components
        //////////////
        auto componentsCreated = std::string("Components allocated: ") + 
                                 std::to_string(Component::ComponentsCreated);
        ImGui::Text(componentsCreated.c_str());
        auto componentsDeleted = std::string("Components deleted: ") +
                                 std::to_string(Component::ComponentsDestroyed);
        ImGui::Text(componentsDeleted.c_str());
        // If the diagnostics flag for components has been turned on...
        if (Component::DiagnosticsEnabled) {
          auto componentsLastCreated = std::string("Last component created: ") +
            Component::ComponentLastCreated;
          ImGui::Text(componentsLastCreated.c_str());
          auto componentsLastDestroyed = std::string("Last component destroyed: ") +
            Component::ComponentLastDestroyed;
          ImGui::Text(componentsLastDestroyed.c_str());
        }
        ImGui::Separator();

        ///////////
        // Events
        ///////////
        auto eventsCreated = std::string("Events allocated: ") +
                             std::to_string(Event::EventsCreated);
        ImGui::Text(eventsCreated.c_str());
        auto eventsDeleted = std::string("Events deleted: ") +
                             std::to_string(Event::EventsDestroyed);
        ImGui::Text(eventsDeleted.c_str());
        ImGui::TreePop();
      }    


      ImGui::End();
    }
  }
}
