/******************************************************************************/
/*!
@file   EditorInspector.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   2/28/2016
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include "EditorInspector.h"

#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {
    /**************************************************************************/
    /*!
    \brief  Displays the  inspector window.
    */
    /**************************************************************************/
    void EditorInspector::Display()
    {
      if (!WindowEnabled)
        return;

      ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiSetCond_FirstUseEver | ImGuiWindowFlags_ShowBorders);
      ImGui::Begin("Properties", &WindowEnabled);

      // If there's an object selected, display its properties.
      if (EditorRef.SelectedObject() != nullptr) {
        // If the object is an entity
        if (auto entity = dynamic_cast<EntityPtr>(EditorRef.SelectedObject())) {
          DisplayEntityProperties(entity);
        }
        // If the object is a resource
        else if (auto resource = dynamic_cast<ResourcePtr>(EditorRef.SelectedObject())) {
          DisplayResourceProperties(resource);
        }
      }

      ImGui::End();
    }

    /**************************************************************************/
    /*!
    \brief  Displays the properties of the currently selected object.
    */
    /**************************************************************************/
    void EditorInspector::DisplayEntityProperties(EntityPtr selectedEntity)
    {
      //auto selectedEntity = dynamic_cast<EntityPtr>(SelectedObject());

      ////////////////////////////////
      // 1. Display the object's name
      ////////////////////////////////
      char name[32]; strcpy(name, selectedEntity->Name().c_str());
      // If the user has given input, change the name
      ImGui::PushItemWidth(ImGui::GetWindowWidth() / 4.0f);
      if (ImGui::InputText("Name", name, IM_ARRAYSIZE(name))) {
        selectedEntity->setObjectName(name);
      }
      ImGui::PopItemWidth();
      /////////////////////////////////////
      // 2. Display the entity's archetype
      /////////////////////////////////////
      char archetypeName[32];
      strcpy(archetypeName, selectedEntity->getArchetype().c_str());
      // If the user has given input, change the archetype
      if (ImGui::InputText("Archetype", archetypeName, IM_ARRAYSIZE(archetypeName))) {
        selectedEntity->setArchetype(archetypeName);
      }
      // Upload to Archetype
      if (ImGui::Button("Upload to Archetype")) {
        DCTrace << "Editor::WindowProperties - Uploading to Archetype \n";
        EditorRef.Archetypes.UploadArchetype(selectedEntity->getArchetype());
        //SaveArchetype(selectedEntity->getArchetype());
      }
      ImGui::SameLine();
      // Revert to Archetype
      if (ImGui::Button("Revert to Archetype")) {
        DCTrace << "Editor::WindowProperties - Reverting to Archetype \n";
        EditorRef.Archetypes.RevertToArchetype(selectedEntity);
        return;
      }

      /////////////////////////////
      // 3. Display its components
      /////////////////////////////
      // Keep track of whether the entity was modified
      bool modified = false;
      ImGui::Separator();
      ImGui::TextColored(ImVec4(0, 0.5, 1, 1), "Components: ");
      unsigned int inputID = 0;
      for (auto& componentHandle : selectedEntity->AllComponentsByHandle()) {
        auto component = Component::Dereference(componentHandle);
        if (ImGui::TreeNode(component->Name().c_str())) {
          // 3. If the user clicks on a tree-node, display the commponent's properties
          //    through reflection
          modified = DisplayProperties(component, componentHandle);
          ImGui::TreePop();
        }
        ImGui::SameLine();

        // Remove component
        ImGui::PushID(inputID++);
        if (ImGui::Button("X")) {
          DCTrace << "Editor::DisplayEntityProperties - Removing component: '" << component->Name()
            << "' from " << selectedEntity->Name() << "\n";
          component->Destroy(); // Need to destroy the Zilch-component too.
          //selectedEntity->RemoveComponentByName(component->getObjectName());
        }
        ImGui::PopID();
      }

      ///////////////////////////////////////////
      // 4. Allow the user to add new components
      ///////////////////////////////////////////
      bool componentAdded = AddComponent(selectedEntity);
      // If the entity was modified or a componen was added, save the level
      if (modified || componentAdded)
        EditorRef.SaveCurrentLevel();
    }

    /**************************************************************************/
    /*!
    @brief  Displays a resource's properties
    */
    /**************************************************************************/
    void EditorInspector::DisplayResourceProperties(ResourcePtr selectedResource)
    {
      // 1. Display the Resource's name
      ImGui::TextColored(ImVec4(0, 0.5, 1, 1), "Name: ");
      ImGui::Text(selectedResource->getObjectName().c_str());
      // 2. Display all its reflected properties (Zilch)
      ImGui::Separator();
      auto modified = DisplayProperties(selectedResource);
      // 3. Display additional properties (C++)
      DisplayAdditionalProperties(selectedResource);
      // If the resource was modified...
      if (modified)
        selectedResource->Build();
    }

    /**************************************************************************/
    /*!
    @brief Displays additional properties of the resource that are not
    being reflected by Zilch.
    @param resource A pointer to the resource.
    */
    /**************************************************************************/
    void EditorInspector::DisplayAdditionalProperties(ResourcePtr resource)
    {
      if (auto shader = dynamic_cast<Shader*>(resource)) {
        if (!shader->Read(Shader::Type::Vertex).empty())
          if (ImGui::Button("Vertex")) EditorRef.TextEditor.Load(shader, Shader::Type::Vertex);

        if (!shader->Read(Shader::Type::Fragment).empty())
          if (ImGui::Button("Fragment")) EditorRef.TextEditor.Load(shader, Shader::Type::Fragment);

        if (!shader->Read(Shader::Type::Geometry).empty())
          if (ImGui::Button("Geometry")) EditorRef.TextEditor.Load(shader, Shader::Type::Geometry);
      }

      if (auto bank = dynamic_cast<Bank*>(resource)) {
        // VCAs
        auto vcas = bank->AllVCAs();
        if (!vcas.empty()) {
          if (ImGui::TreeNode("VCAs")) {
            for (auto& vca : vcas) {
              ImGui::BulletText(vca.first.c_str());
            }
            ImGui::TreePop();
          }
        }

        // Buses
        auto buses = bank->AllBuses();
        if (!buses.empty()) {
          if (ImGui::TreeNode("Buses")) {
            for (auto& bus : buses) {
              ImGui::BulletText(bus.first.c_str());
            }
            ImGui::TreePop();
          }
        }
      }

      // SoundCue
      else if (auto soundCue = dynamic_cast<SoundCue*>(resource)) {
        auto parameters = soundCue->Data.Parameters;
        if (parameters.empty())
          return;

        // Parameters
        if (ImGui::TreeNode("Parameters")) {
          ImGui::Columns(3, "paramColumns");
          ImGui::Separator();
          ImGui::Text("Name"); ImGui::NextColumn();
          ImGui::Text("Minimum"); ImGui::NextColumn();
          ImGui::Text("Maximum"); ImGui::NextColumn();
          ImGui::Separator();
          for (auto& parameter : soundCue->Data.Parameters) {
            ImGui::Text(parameter.Name.c_str()); ImGui::NextColumn();
            ImGui::Text(std::to_string(parameter.Minimum).c_str()); ImGui::NextColumn();
            ImGui::Text(std::to_string(parameter.Maximum).c_str()); ImGui::NextColumn();
          }
          ImGui::TreePop();
        }
      }
    }

    /**************************************************************************/
    /*!
    @brief  Allows the user to add a component to the entity.
    @todo   If the component has been marked as skipped, do not attempt to add it.
    */
    /**************************************************************************/
    bool EditorInspector::AddComponent(EntityPtr selectedEntity)
    {
      static bool Scanned = false;

      // Grab a container of all bound components.. 
      static std::vector<Zilch::BoundType*> components;
      static std::vector<const char*> componentNames;
      // Scan for components only when need be
      if (1) {
      //if (!Scanned) {
        componentNames.clear();
        components = Daisy->getSystem<Systems::Reflection>()->AllComponents();
        for (auto component : components) {
          // Do not display components marked as hidden
          if (component->HasAttribute("Hidden"))
            continue;

          // Components to skip
          bool skip = false;
          std::vector<std::string> skippableComponents{ "Component", "Collider", "Graphical", "ZilchComponent" };
          //skippableComponents.push_back(std::string("ZilchComponet"));
          for (auto& name : skippableComponents) {
            auto componentName = std::string(component->Name.c_str());
            if (componentName == name) {
              skip = true;
              break;
            }
          }
          if (skip)
            continue;

          // If it's a Zilch component, we need to do more to get its underlying type
          if (Zilch::TypeBinding::IsA(component, ZilchComponent::ZilchGetStaticType())) {
            //componentNames.push_back(component->Name.c_str());
            componentNames.push_back(component->Name.c_str());
          }

          // If it's a C++ component, it's less cumbersome...
          else
            componentNames.push_back(component->Name.c_str());
        }

        Scanned = true;
      }

      // Sort the component names alphabetically
      std::sort(componentNames.begin(), componentNames.end(),
        [](const char* lhs, const char* rhs) -> bool
      {
        return *lhs < *rhs;
      });

      int currentComponent = 0;
      ImGui::Separator();
      ImGui::TextColored(ImVec4(1, 0, 0.5, 1), "Add Components: ");

      if (ImGui::Combo("##components", &currentComponent, componentNames.data(), componentNames.size())) {
        auto componentName = std::string(componentNames.at(currentComponent));
        auto component = selectedEntity->AddComponentByName(componentName, false);

        DCTrace << "Editor::AddComponent - " << componentName << "\n";

        // If there's a missing dependency...
        if (!component->HasDependencies()) {
          auto missingDependencies = component->MissingDependencies();
          // Send a popup!
          Windows::PopUpData data;
          data.Title = "Missing Dependencies";
          data.List = missingDependencies;
          data.Confirmation = "Back";
          auto popUp = WindowPtr(new Windows::PopUpComponentDependencies(data, component));
          GUI::Add(popUp);
          // Remove the component
          //component->Destroy();
          return false;
        }
        // A component was added
        return true;
        Scanned = false;
      }

      // No component was added
      return false;
    }

    /**************************************************************************/
    /*!
    @brief  Constructor.
    */
    /**************************************************************************/
    EditorInspector::EditorInspector(Editor & editor) : EditorModule(editor, true)
    {
    }
  }
}