/*****************************************************************************/
/*!
\file   EditorDiagnostics.cpp
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   3/3/2016
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "EditorDiagnostics.h"

#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {
    
    /**************************************************************************/
    /*!
    \brief  EditorDiagnostics constructor.
    */
    /**************************************************************************/
    EditorDiagnostics::EditorDiagnostics() : EditorModule(false)
    {
    }

    /**************************************************************************/
    /*!
    \brief  EditorDiagnostics destructor.
    */
    /**************************************************************************/
    EditorDiagnostics::~EditorDiagnostics()
    {
    }

    /**************************************************************************/
    /*!
    \brief  Displays diagnostics window.
    */
    /**************************************************************************/
    void EditorDiagnostics::Display()
    {
      if (!WindowEnabled)
        return;

      ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiSetCond_FirstUseEver);
      ImGui::Begin("Diagnostics", &WindowEnabled);

      // Display the engine's systems histogram.
      auto& systemTimes = Daisy->Profiler().SystemTimes();
      DisplaySystemsHistogram("Systems", systemTimes);

      if (ImGui::CollapsingHeader("Physics")) {
        Physics();
      }
      if (ImGui::CollapsingHeader("Graphics")) {
        Graphics();
      }
      if (ImGui::CollapsingHeader("GameObjects")) {
        GameObjects();
      }
      if (ImGui::CollapsingHeader("Events")) {
        Events();
      }
      if (ImGui::CollapsingHeader("Actions")) {
        Actions();
      }



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
    void EditorDiagnostics::Graphics()
    {
      auto& times = Daisy->Profiler().Graphics();
      DisplaySystemsHistogram("Graphics", times, true); 
      ImGui::Separator();
      auto particlesCreated = std::string("Particles allocated: ") +
        std::to_string(Components::SpriteParticleSystem::Particle::Created);
      ImGui::Text(particlesCreated.c_str());
      auto particlesDestroyed = std::string("Particles deallocated: ") +
        std::to_string(Components::SpriteParticleSystem::Particle::Destroyed);
      ImGui::Text(particlesDestroyed.c_str());

    }

    /**************************************************************************/
    /*!
    \brief  Displays diagnostics for Physics.
    */
    /**************************************************************************/
    void EditorDiagnostics::Physics()
    {
      auto& times = Daisy->Profiler().Physics();
      DisplaySystemsHistogram("Physics", times, true);

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
    \brief  Displays diagnostics for Events.
    */
    /**************************************************************************/
    void EditorDiagnostics::Events()
    {
      auto eventsCreated = std::string("Events allocated: ") +
        std::to_string(Event::EventsCreated);
      ImGui::Text(eventsCreated.c_str());
      auto eventsDeleted = std::string("Events deleted: ") +
        std::to_string(Event::EventsDestroyed);
      ImGui::Text(eventsDeleted.c_str());
      
      // Grab a pointer to the GameSession
      auto gamesession = GameSession::Get();

      // For the GameSession
      if (ImGui::TreeNode("GameSession")) {
        DisplayEvents(std::string("GameSession"), gamesession->PeekEvents());
        ImGui::TreePop();
      }

      // For every Space
      if (ImGui::TreeNode("Spaces"))
      {
        for (auto& space : gamesession->AllSpaces()) {
          auto name = space.first.c_str();
          // Display the events in the space
          if (ImGui::TreeNode(name)) {
            DisplayEvents(name, space.second->PeekEvents());
            ImGui::TreePop();
            // Display the events for individual objects on the space
            for (auto& object : *space.second->AllObjects()) {
              auto objectName = object->Name().c_str();
              if (ImGui::TreeNode(objectName)) {
                DisplayEvents(objectName, object->PeekEvents());
                ImGui::TreePop();
              }
            }
          }    
        }

        ImGui::TreePop();
      }



    }

    /**************************************************************************/
    /*!
    \brief  Displays diagnostics for the events of a specific entity.
    \param info The information on the event.
    */
    /**************************************************************************/
    void EditorDiagnostics::DisplayEvents(const std::string& name, EventDelegatesInfo & info)
    {
      if (info.Events.empty())
        return;

      ImGui::BeginGroup();
      ImGui::Columns(2, name.c_str());
           
      EventDelegatesInfo::EventDelegateInfo* currentEvent = nullptr;

      ImGui::TextColored(ImVec4(1, 0, 1, 1), "Events");
      ImGui::NextColumn();
      ImGui::TextColored(ImVec4(0, 0, 1, 1), "Observers");
      ImGui::Separator();

      ImGui::Columns(2);      
      for (auto& event : info.Events) {
        ImGui::Text(event.Name.c_str());
        ImGui::NextColumn();
        for (auto& observer : event.Observers) {
          ImGui::Text(observer.c_str());
        }      
        ImGui::Columns(2);
      }
      
      
      // Left column: The name of every event
      //ImGui::TextColored(ImVec4(1, 0, 1, 1), "Events");
      //for (auto& event : info.Events) {
      //  if (ImGui::Selectable(event.Name.c_str())) {
      //    currentEvent = &event;
      //
      //  }
      //}
      // Right column: Every observer to this event
      //ImGui::NextColumn();
      //ImGui::TextColored(ImVec4(0, 0, 1, 1), "Observers");
      //ImGui::Separator();
      //if (currentEvent) {
      //  for (auto& observer : currentEvent->Observers) {
      //    ImGui::Text(observer.c_str());
      //  }
      //}

      ImGui::Columns(1);
      ImGui::Separator();
      ImGui::EndGroup();
    }

    /**************************************************************************/
    /*!
    \brief  Displays diagnostics for Actions.
    */
    /**************************************************************************/
    void EditorDiagnostics::Actions()
    {
      auto actionsCreated = std::string("Actions created: ") +
        std::to_string(Action::Created);
      ImGui::Text(actionsCreated.c_str());
      auto actionsDeleted = std::string("Actions deleted: ") +
        std::to_string(Action::Destroyed);
      ImGui::Text(actionsDeleted.c_str());
    }

    /**************************************************************************/
    /*!
    \brief  Displays diagnostics for GameObj ects.
    */
    /**************************************************************************/
    void EditorDiagnostics::GameObjects()
    {
    }
    


    /**************************************************************************/
    /*!
    @brief Displays the histogram for a system.
    @param title The title of the histogram.
    @param data The container of data.
    */
    /**************************************************************************/
    void EditorDiagnostics::DisplaySystemsHistogram(std::string title, DCEngine::Time::FunctionTimeSliceVec& data, bool calls)
    {
      if (data.empty())
        return;
        
        auto frameRate = 60;
        static int slowDown = 15;
        static int refreshTime = 15;

        ++slowDown;        

        // Minimum and max edge caes for the histogram chart
        static float minTime = 0.0f;
        static float maxTime = 1.0f / 60.0f;// Daisy->Dt();

        ImGui::PushItemWidth(90);

        // Parse the times and names to a format dear imgui can read
        std::string names;
        for (unsigned i = 0; i < data.size(); ++i) {
          names += std::to_string(i);
          names += ": ";
          names += data[i].Name;
          // If displaying the number of calls..
          if (calls) {
            names += " '";
            names += std::to_string(data[i].Calls) + "'";
          }
          names += "\n";
        }

        int textSize = 14;
        int height = static_cast<int>(data.size() * textSize);

        // Display the histogram through 
        ImGui::PushItemWidth(290);
        ImGui::PlotHistogram("", &data[0].Time, data.size(), 0, title.c_str(), minTime, maxTime, ImVec2(0, height), sizeof(data[0]));
        ImGui::SameLine();
        // Print the legend 
        ImGui::Text(names.c_str());

        
    }

    void EditorDiagnostics::Update()
    {
    }
  }
}