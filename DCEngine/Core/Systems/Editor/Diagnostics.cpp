/******************************************************************************/
/*!
@file   Diagnostics.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/02/2015
@brief  This file includes the implementation for the Editor's library widget.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "Editor.h"

#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {
    
    // Different diagnostics tabs
    void DiagnosticsGameObjects();
    void DiagnosticsGraphics();
    void DiagnosticsPhysics();
    void DiagnosticsEventsActions();

    /**************************************************************************/
    /*!
    \brief  Diagnostics Window.
    */
    /**************************************************************************/
    void Editor::WindowDiagnostics()
    {
      if (!Windows.DiagnosticsEnabled)
        return;

      ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiSetCond_FirstUseEver);
      ImGui::Begin("Diagnostics", &Windows.DiagnosticsEnabled);

      if (ImGui::TreeNode("GameObjects")) {        
        DiagnosticsGameObjects();
        ImGui::TreePop();
      }    

      if (ImGui::TreeNode("Events & Actions")) {
        DiagnosticsEventsActions();
        ImGui::TreePop();
      }

      if (ImGui::TreeNode("Physics")) {
        DiagnosticsPhysics();
        ImGui::TreePop();
      }

      if (ImGui::TreeNode("Graphics")) {
        DiagnosticsGraphics();
        ImGui::TreePop();
      }

      auto a = 14;

      ImGui::End();
    }

    /**************************************************************************/
    /*!
    \brief  Displays diagnostics for GameObjects.
    */
    /**************************************************************************/
    void DiagnosticsGameObjects()
    {
      ////////////////
      // GameObjects
      ///////////////
      auto gameObjectsActive = std::string("Active GameObjects ") +
        std::to_string(GameObject::GameObjectsActive);
      ImGui::Text(gameObjectsActive.c_str());
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
    }

    /**************************************************************************/
    /*!
    \brief  Displays diagnostics for Physics.
    */
    /**************************************************************************/
    void DiagnosticsGraphics()
    {
    }

    /**************************************************************************/
    /*!
    \brief  Displays diagnostics for Physics.
    */
    /**************************************************************************/
    void DiagnosticsPhysics()
    {
      /*============
         COLLIDERS
      ============*/
      auto collidersActive = std::string("Colliders Active: ") +
        std::to_string(Components::Collider::Active);
      ImGui::Text(collidersActive.c_str());
      auto collidersCreated = std::string("Colliders allocated: ") +
        std::to_string(Components::Collider::Created);
      ImGui::Text(collidersCreated.c_str());
      auto collidersDestroyed = std::string("Colliders deleted: ") +
        std::to_string(Components::Collider::Destroyed);
      ImGui::Text(collidersDestroyed.c_str());

      ImGui::Separator();

      /*============
        RIGIDBODIES
      ============*/
      auto rigidBodiesActive = std::string("RigidBodies Active: ") +
        std::to_string(Components::RigidBody::Active);
      ImGui::Text(rigidBodiesActive.c_str());
      auto rigidBodiesCreated = std::string("RigidBodies allocated: ") +
        std::to_string(Components::RigidBody::Created);
      ImGui::Text(rigidBodiesCreated.c_str());
      auto rigidBodiesDestroyed = std::string("RigidBodies deleted: ") +
        std::to_string(Components::RigidBody::Destroyed);
      ImGui::Text(rigidBodiesDestroyed.c_str());
    }

    /**************************************************************************/
    /*!
    \brief  Displays diagnostics for Events and Actions.
    */
    /**************************************************************************/
    void DiagnosticsEventsActions()
    {
      ///////////
      // Events
      ///////////
      auto eventsCreated = std::string("Events allocated: ") +
        std::to_string(Event::EventsCreated);
      ImGui::Text(eventsCreated.c_str());
      auto eventsDeleted = std::string("Events deleted: ") +
        std::to_string(Event::EventsDestroyed);
      ImGui::Text(eventsDeleted.c_str());      

      ///////////
      // Events
      ///////////
      auto actionsCreated = std::string("Actions created: ") +
        std::to_string(Action::Created);
      ImGui::Text(actionsCreated.c_str());
      auto actionsDeleted = std::string("Actions deleted: ") +
        std::to_string(Action::Destroyed);
      ImGui::Text(actionsDeleted.c_str());

    }

  }
}
