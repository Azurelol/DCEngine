/******************************************************************************/
/*!
@file   Properties.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/02/2015
@brief  This file includes the implementation for the Editor's properties widget.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "Editor.h"
#include "../../Engine/Engine.h"
#include "../../ComponentsInclude.h"
#include "../../Systems/Reflection/ZilchInterface.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    \brief  Displays the properties of the currently selected object.
    */
    /**************************************************************************/
    void Editor::WindowProperties()
    {
      if (!WidgetPropertiesEnabled)
        return;





      ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiSetCond_FirstUseEver);
      ImGui::Begin("Properties", &WidgetPropertiesEnabled);

      //int inputID = 0;
      //{
      //  float vec1f[3] = { 1, 2, 3 };
      //  ImGui::Text("Testy");
      //  ImGui::PushID(inputID++);   

      //  if (ImGui::InputFloat3("##inputID", vec1f)) {
      //    DCTrace << "boo \n";
      //  }
      //  ImGui::PopID();
      //}
      //{
      //  float vec2f[3] = { 1, 2, 3 };
      //  ImGui::Text("Testy");
      //  ImGui::PushID(inputID++);
      //  if (ImGui::InputFloat3("##inputID", vec2f)) {
      //    DCTrace << "boo \n";
      //  }
      //  ImGui::PopID();
      //}
      //{
      //  float vec3f[3] = { 1, 2, 3 };
      //  ImGui::Text("Testy");
      //  ImGui::PushID(inputID++);        
      //  if (ImGui::InputFloat3("##inputID", vec3f)) {
      //    DCTrace << "boo \n";
      //  }
      //  ImGui::PopID();
      //}


      // If there's an object selected, display its properties.
      if (SelectedObject != nullptr) {
        // 1. Display the object's name
        char name[32]; strcpy(name, SelectedObject->Name().c_str());
        // If the user has given input, change the name
        if (ImGui::InputText("Name", name, IM_ARRAYSIZE(name))) {
          SelectedObject->setObjectName(name);
        }
                
        // 2. Display the object's archetype
        char archetypeName[32]; 
        strcpy(archetypeName, SelectedObject->getArchetype().c_str());
        // If the user has given input, change the archetype
        if (ImGui::InputText("Archetype", archetypeName, IM_ARRAYSIZE(archetypeName))) {
          SelectedObject->setArchetype(archetypeName);
        }
        if (ImGui::Button("Upload to Archetype")) {
          DCTrace << "Editor::WindowProperties - Uploading to Archetype \n";
        }
        ImGui::SameLine();
        if (ImGui::Button("Revert to Archetype")) {
          DCTrace << "Editor::WindowProperties - Reverting to Archetype \n";
        }

        // 3. Display its components
        ImGui::Separator();
        ImGui::TextColored(ImVec4(0, 0.5, 1, 1), "Components: ");
        for (auto &component : *SelectedObject->AllComponents()) {
          if (ImGui::TreeNode(component->Name().c_str())) {
            //ImGui::SetWindowFocus();
            // 3. If the user clicks on a tree-node, display the commponent's properties
            //    through reflection
            DisplayProperties(component.get());
            ImGui::TreePop();
          }                  
        }

        // 4. Allow the user to add new components
        AddComponent();

      }

      ImGui::End();
    }

    

    /**************************************************************************/
    /*!
    @brief  Displays the component's properties, allowing them to be
            seen and edited.
    @param  component A pointer to the Component.
    @note   Properties are listed from the order they were defined.
    @todo   Refactor the property-setting so I DONT COPY PASTE THE SAME
            3 LINES.
    */
    /**************************************************************************/
    void Editor::DisplayProperties(ComponentPtr component) {
      
      // 1. Get the component's BoundType, which has a wealth of reflected data
      auto componentBoundType = component->ZilchGetDerivedType();
      if (componentBoundType == nullptr)
        return;

      // ImGui uses an unique handle for each Input Widget. We will use a
      // a counter to have generate unique IDs for each of them.
      unsigned int inputID = 0;

      // 2. Get a list of all properties on the object      
      for (auto property : componentBoundType->AllProperties) {
        
        // If it is a resource property..
        if (property->HasAttribute("Resource")) {
          SelectResource(property, component);
          continue;
        }

        // If the bound field/property does not have the Property Attribute, do
        // nothing.
        //if (!property->HasAttribute("Property"))
        //  continue;              

        
        // Create an exception report object
        Zilch::ExceptionReport report;
        // Grab the current property
        Zilch::Call getCall(property->Get, Daisy->getSystem<Reflection>()->Handler()->getState());
        getCall.SetHandleVirtual(Zilch::Call::This, component);        
        getCall.Invoke(report);

        // Property: Boolean
        if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Boolean))) {
          auto boolean = getCall.Get<Zilch::Boolean>(Zilch::Call::Return);
          // If the user modifies it
          if (ImGui::Checkbox(property->Name.c_str(), &boolean)) {
            Zilch::Call setCall(property->Set, Daisy->getSystem<Reflection>()->Handler()->getState());
            setCall.SetHandleVirtual(Zilch::Call::This, component);
            setCall.Set(0, boolean);
            setCall.Invoke(report);
          }
        }

        // Property: String
        else if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::String))) {
          auto string = getCall.Get<Zilch::String>(Zilch::Call::Return);
          char buf[32];
          strcpy(buf, string.c_str());
          // If the user has given input, set the property
          if (ImGui::InputText(property->Name.c_str(), buf, IM_ARRAYSIZE(buf)), ImGuiInputTextFlags_EnterReturnsTrue) {
            Zilch::Call setCall(property->Set, Daisy->getSystem<Reflection>()->Handler()->getState());
            setCall.SetHandleVirtual(Zilch::Call::This, component);
            setCall.Set(0, Zilch::String(buf));
            setCall.Invoke(report);
          }
        }

        // Property: Integer
        else if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Integer))) {
          auto integer = getCall.Get<Zilch::Integer>(Zilch::Call::Return);
          // If the user has given input, set the property
          if (ImGui::InputInt(property->Name.c_str(), &integer)) {
            Zilch::Call setCall(property->Set, Daisy->getSystem<Reflection>()->Handler()->getState());
            setCall.SetHandleVirtual(Zilch::Call::This, component);
            setCall.Set(0, integer);
            setCall.Invoke(report);
          }
        }

        // Property: Integer2
        else if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Integer2))) {
          auto integer2 = getCall.Get<Zilch::Integer2>(Zilch::Call::Return);
          int int2[2] = { integer2.x, integer2.y };
          // If the user has given input, set the property
          if (ImGui::InputInt2(property->Name.c_str(), int2)) {
            Zilch::Call setCall(property->Set, Daisy->getSystem<Reflection>()->Handler()->getState());
            setCall.SetHandleVirtual(Zilch::Call::This, component);
            setCall.Set(0, Zilch::Integer2(int2[0], int2[1]));
            setCall.Invoke(report);
          }
        }

        // Property: Integer3
        else if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Integer3))) {
          auto integer3 = getCall.Get<Zilch::Integer3>(Zilch::Call::Return);
          int int3[3] = { integer3.x, integer3.y, integer3.z };
          // If the user has given input, set the property
          if (ImGui::InputInt3(property->Name.c_str(), int3)) {
            Zilch::Call setCall(property->Set, Daisy->getSystem<Reflection>()->Handler()->getState());
            setCall.SetHandleVirtual(Zilch::Call::This, component);
            setCall.Set(0, Zilch::Integer3(int3[0], int3[1], int3[2]));
            setCall.Invoke(report);
          }
        }

        // Property: Integer4
        else if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Integer4))) {
          auto integer4 = getCall.Get<Zilch::Integer4>(Zilch::Call::Return);
          int int4[4] = { integer4.x, integer4.y, integer4.z, integer4.w};
          // If the user has given input, set the property
          if (ImGui::InputInt4(property->Name.c_str(), int4)) {
            Zilch::Call setCall(property->Set, Daisy->getSystem<Reflection>()->Handler()->getState());
            setCall.SetHandleVirtual(Zilch::Call::This, component);
            setCall.Set(0, Zilch::Integer4(int4[0], int4[1], int4[2], int4[3]));
            setCall.Invoke(report);
          }
        }

        // Property: Real (float)
        else if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Real))) {
          auto real = getCall.Get<Zilch::Real>(Zilch::Call::Return);
          // If the user has given input, set the property
          //ImGui::Text(property->Name.c_str());
          //ImGui::PushID(inputID++);
          if (ImGui::InputFloat(property->Name.c_str(), &real, 1.0f)) {
          //if (ImGui::InputFloat(property->Name.c_str(), &real, 1.0f)) {
            Zilch::Call setCall(property->Set, Daisy->getSystem<Reflection>()->Handler()->getState());
            setCall.SetHandleVirtual(Zilch::Call::This, component);
            setCall.Set(0, real);
            setCall.Invoke(report);
          }
        }

        // Property: Real2 (Vec2)
        else if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Real2))) {
          auto vec2 = getCall.Get<Zilch::Real2>(Zilch::Call::Return);
          float vec2f[2] = { vec2.x, vec2.y };
          ImGui::Text(property->Name.c_str());
          ImGui::PushID(inputID++);
          // If the user has given input, set the property
          if (ImGui::InputFloat2("##inputID", vec2f)) {
            Zilch::Call setCall(property->Set, Daisy->getSystem<Reflection>()->Handler()->getState());
            setCall.SetHandleVirtual(Zilch::Call::This, component);
            setCall.Set(0, Zilch::Real3(vec2f));
            setCall.Invoke(report);
          }
          ImGui::PopID();
        }

        // Property: Real3 (Vec3)
        else if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Real3))) {
          auto vec3 = getCall.Get<Zilch::Real3>(Zilch::Call::Return);
          float vec3f[3] = { vec3.x, vec3.y, vec3.z };
          // If the user has given input, set the property
          ImGui::Text(property->Name.c_str());
          ImGui::PushID(inputID++); 
          if (ImGui::InputFloat3("##inputID", vec3f)) {
            //DCTrace << "Setting " << property->Name.c_str() << "\n";
            Zilch::Call setCall(property->Set, Daisy->getSystem<Reflection>()->Handler()->getState());
            setCall.SetHandleVirtual(Zilch::Call::This, component);
            setCall.Set(0, Zilch::Real3(vec3f));
            //setCall.Set(0, Zilch::Real3(vec3f[0], vec3f[1], vec3f[2]));
            setCall.Invoke(report);
          }
          ImGui::PopID(); 
        }        

        // Property: Real4 (Vec4)
        else if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Real4))) {
          auto vec4 = getCall.Get<Zilch::Real4>(Zilch::Call::Return);
          float vec4f[4] = { vec4.x, vec4.y, vec4.z, vec4.w};
          // If the user has given input, set the property          
          ImGui::Text(property->Name.c_str());
          ImGui::PushID(inputID++);
          if (ImGui::InputFloat4("##inputID", vec4f)) {
            Zilch::Call setCall(property->Set, Daisy->getSystem<Reflection>()->Handler()->getState());
            setCall.SetHandleVirtual(Zilch::Call::This, component);
            setCall.Set(0, Zilch::Real4(vec4f[0], vec4f[1], vec4f[2], vec4f[3]));
            setCall.Invoke(report);
          }
          ImGui::PopID();
        }
      }
    }

    /**************************************************************************/
    /*!
    @brief  Allows the user to add a component to the entity.
    */
    /**************************************************************************/
    void Editor::AddComponent()
    {
      static bool Scanned = false;

      // Grab a container of all bound components.. 
      static std::vector<Zilch::BoundType*> components;
      static std::vector<const char*> componentNames;
      // Scan for components only when need be
      if (!Scanned) {
        components = Daisy->getSystem<Systems::Reflection>()->AllComponents();
        for (auto component : components) {
          componentNames.push_back(component->Name.c_str());
        }
        Scanned = true;
      }

      int currentComponent = 0;
      // If the user selects the combo box...
      //if (ImGui::Button("Add Component"), ImGuiAlign_Center) {
      ImGui::Separator();
      //ImGui::TextColored(ImVec4(0.5, 0, 0, 1), "Add Component");
      ImGui::TextColored(ImVec4(1, 0, 0.5, 1), "Add Components: ");
      //ImGui::Text("Add Component..");
        if (ImGui::Combo("##components", &currentComponent, componentNames.data(), componentNames.size())) {
          auto componentName = std::string(componentNames.at(currentComponent));
          SelectedObject->AddComponentByName(componentName, true);
          Scanned = false;
        }
      //}
        
    }

    void SelectSpriteSource(Zilch::Property * resource, ComponentPtr component);
    void SelectSoundCue(Zilch::Property * resource, ComponentPtr component);

    /**************************************************************************/
    /*!
    @brief  Allows the user to select the resource for the property.
    @param  resource A pointer to the BoundType of the resource.
    @param  component A pointer to the component that holds the resource.
    */
    /**************************************************************************/
    void Editor::SelectResource(Zilch::Property * resource, ComponentPtr component)
    {
      // Get the type of the resource      
      auto resourceType = std::string(resource->Name.c_str());
      ImGui::Text(resourceType.c_str());
      // SpriteSource 
      if (resourceType == std::string("SpriteSource")) {
        SelectSpriteSource(resource, component);
      }
      // SoundCue
      else if (resourceType == std::string("SoundCue")) {

      }

    }

    /**************************************************************************/
    /*!
    @brief  Allows the user to select the SpriteSource for the component.
    @param  resource A pointer to the BoundType of the resource.
    @param  component A pointer to the component that holds the resource.
    */
    /**************************************************************************/
    void SelectSpriteSource(Zilch::Property * resource, ComponentPtr component) {
      // Get a container of all active spritesources        
      auto container = Daisy->getSystem<Content>()->AllSpriteSources();
      
      std::vector<const char *> spriteSourceNames;
      for (auto spriteSource : *container) {
        // Push the name of it into the vector of strings
        spriteSourceNames.push_back(spriteSource.second->Name().c_str());
      }
      // Start at the current item
      static int currentItem = 0;
      // If the user selects an item... 
      if (ImGui::Combo("##spritenames", &currentItem, spriteSourceNames.data(), spriteSourceNames.size())) {
        // Set the selected item as the current resource
        auto selectedSpriteSource = spriteSourceNames.at(currentItem);
        Zilch::ExceptionReport report;
        Zilch::Call setCall(resource->Set, Daisy->getSystem<Reflection>()->Handler()->getState());
        setCall.SetHandleVirtual(Zilch::Call::This, component);
        setCall.Set(0, Zilch::String(selectedSpriteSource));
        setCall.Invoke(report);
      }
    }
    
    /**************************************************************************/
    /*!
    @brief  Allows the user to select the SoundCue for the component.
    @param  resource A pointer to the BoundType of the resource.
    @param  component A pointer to the component that holds the resource.
    */
    /**************************************************************************/
    void SelectSoundCue(Zilch::Property * resource, ComponentPtr component)
    {
      // Get a container of all active spritesources        
      auto container = Daisy->getSystem<Content>()->AllSoundCues();
      std::vector<const char *> soundCueNames;
      for (auto soundCue : *container) {
        // Push the name of it into the vector of strings
        soundCueNames.push_back(soundCue.second->Name().c_str());
      }
      // Start at the current item
      static int currentItem = 0;
      // If the user selects an item... 
      if (ImGui::Combo("##soundCueNames", &currentItem, soundCueNames.data(), soundCueNames.size())) {
        // Set the selected item as the current resource
        auto selectedSoundCue = soundCueNames.at(currentItem);
        Zilch::ExceptionReport report;
        Zilch::Call setCall(resource->Set, Daisy->getSystem<Reflection>()->Handler()->getState());
        setCall.SetHandleVirtual(Zilch::Call::This, component);
        setCall.Set(0, Zilch::String(selectedSoundCue));
        setCall.Invoke(report);
      }
    }


  }
}