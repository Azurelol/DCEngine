#include "EditorDiagnostics.h"

#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {

    void DiagnosticsGameObjects();
    void DiagnosticsEventsActions();

    /**************************************************************************/
    /*!
    \brief  EditorDiagnostics constructor.
    */
    /**************************************************************************/
    EditorDiagnostics::EditorDiagnostics(Editor & editor) : EditorModule(editor, false)
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

      if (ImGui::TreeNode("GameObjects")) {
        DiagnosticsGameObjects();
        ImGui::TreePop();
      }

      if (ImGui::TreeNode("Events & Actions")) {
        DiagnosticsEventsActions();
        ImGui::TreePop();
      }

      if (ImGui::TreeNode("Physics")) {
        Physics();
        ImGui::TreePop();
      }

      if (ImGui::TreeNode("Graphics")) {
        Graphics();
        ImGui::TreePop();
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
      DisplaySystemsHistogram("Graphics", times);
    }

    /**************************************************************************/
    /*!
    \brief  Displays diagnostics for Physics.
    */
    /**************************************************************************/
    void EditorDiagnostics::Physics()
    {
      auto& times = Daisy->Profiler().Physics();
      DisplaySystemsHistogram("Physics", times);

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

    void EditorDiagnostics::DisplaySystemsHistogram(std::string title, DCEngine::Time::FunctionTimeSliceVec& data)
    {
      //int systemsProfiled = static_cast<int>(Daisy->Profiler().SystemTimes.size());
      //auto num = std::string("Systems profiled: ") + std::to_string(systemsProfiled);
      //ImGui::Text(num.c_str());
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

        // Slider float to adjust the minimum time
        //ImGui::SliderFloat("Min Time", &minTime, 0.0f, 0.001f, "%.4f");
        //GUI::SetToolTip("Minimum time displayed in the histogram");
        //ImGui::SameLine();
        //// Slider float to adjust the max time
        //ImGui::SliderFloat("Max Time", &maxTime, 0.001f, 0.033f, "%.4f");
        //GUI::SetToolTip("Max time displayed in the histogram.");

        // Parse the times and names to a format dear imgui can read
        float* times = new float[data.size()];
        std::string names;
        for (unsigned i = 0; i < data.size(); ++i) {
          names += std::to_string(i);
          names += ": ";
          names += data[i].first;
          names += "\n";
          times[i] = data[i].second;
        }

        int height = data.size() * 14;

        // Display the histogram through 
        ImGui::PushItemWidth(290);
        ImGui::PlotHistogram("", times, data.size(), 0, title.c_str(), minTime, maxTime, ImVec2(0, height));
        //GUI::SetToolTip("Shows the amount of time taken by each system.");
        //ImGui::PushItemWidth(0);
        ImGui::SameLine();
        // Print the legend 
        ImGui::Text(names.c_str());
        // Clean up!
        delete[] times;
      
    }

    void EditorDiagnostics::Update()
    {
    }
  }
}