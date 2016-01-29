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

    void DisplayImage(Zilch::Property* property, ObjectPtr object);
    void PreviewSound(Zilch::Property* property, ObjectPtr object);
    
    /**************************************************************************/
    /*!
    \brief  Displays the properties of the currently selected object.
    */
    /**************************************************************************/
    void Editor::WindowProperties()
    {
      if (!WindowPropertiesEnabled)
        return;

      ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiSetCond_FirstUseEver);
      ImGui::Begin("Properties", &WindowPropertiesEnabled);

      // If there's an object selected, display its properties.
      if (SelectedObject != nullptr) {

        // If the object is an entity
        if (dynamic_cast<EntityPtr>(SelectedObject)) {
          DisplayEntityProperties();
        }
        // If the object is a resource
        else if (dynamic_cast<ResourcePtr>(SelectedObject)) {
          DisplayResourceProperties();
        }
      }

      ImGui::End();
    }


    /**************************************************************************/
    /*!
    @brief  Displays an entity's properties
    */
    /**************************************************************************/
    void Editor::DisplayEntityProperties()
    {
      auto selectedEntity = dynamic_cast<EntityPtr>(SelectedObject);

      ////////////////////////////////
      // 1. Display the object's name
      ////////////////////////////////
      char name[32]; strcpy(name, SelectedObject->Name().c_str());
      // If the user has given input, change the name
      ImGui::PushItemWidth(ImGui::GetWindowWidth() / 4.0f);
      if (ImGui::InputText("Name", name, IM_ARRAYSIZE(name))) {
        SelectedObject->setObjectName(name);
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
        SaveArchetype(selectedEntity->getArchetype());
      }
      ImGui::SameLine();
      // Revert to Archetype
      if (ImGui::Button("Revert to Archetype")) {
        DCTrace << "Editor::WindowProperties - Reverting to Archetype \n";
        RevertToArchetype();
      }

      /////////////////////////////
      // 3. Display its components
      /////////////////////////////
      // Keep track of whether the entity was modified
      bool modified = false;
      ImGui::Separator();
      ImGui::TextColored(ImVec4(0, 0.5, 1, 1), "Components: ");
      unsigned int inputID = 0;      
      for (auto &component : selectedEntity->AllComponents()) {
        if (ImGui::TreeNode(component->Name().c_str())) {
          //ImGui::SetWindowFocus();
          // 3. If the user clicks on a tree-node, display the commponent's properties
          //    through reflection
          modified = DisplayProperties(component);
          ImGui::TreePop();
        }
        ImGui::SameLine();

        // Remove component
        ImGui::PushID(inputID++);
        if (ImGui::Button("X")) {          
          DCTrace << "Editor::DisplayEntityProperties - Removing component: '" << component->Name() 
                  << "' from " << selectedEntity->Name() << "\n";
          component->Destroy();
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
        SaveCurrentLevel();
    }

    /**************************************************************************/
    /*!
    @brief  Displays a resource's properties
    */
    /**************************************************************************/
    void Editor::DisplayResourceProperties()
    {
      auto selectedResource = dynamic_cast<ResourcePtr>(SelectedObject);

      // 1. Display the Resource's name
      ImGui::TextColored(ImVec4(0, 0.5, 1, 1), "Name: ");
      ImGui::Text(selectedResource->getObjectName().c_str());
      // 2. Display all its properties
      ImGui::Separator();
      auto modified = DisplayProperties(selectedResource);
      // If the resource was modified...
      if (modified)
        selectedResource->Build();
    }

    /**************************************************************************/
    /*!
    @brief  Displays properties for an object.
    @param  object A pointer to the Component.
    @note   Properties are listed from the order they were defined.
    @todo   Refactor the property-setting so I DONT COPY PASTE THE SAME
            3 LINES.
    */
    /**************************************************************************/
    bool Editor::DisplayProperties(ObjectPtr object) {
      
      // 1. Get the object's BoundType, which has a wealth of reflected data
      auto componentBoundType = object->ZilchGetDerivedType();
      if (componentBoundType == nullptr)
        return false;
      
      // ImGui uses an unique handle for each Input Widget. We will use a
      // a counter to have generate unique IDs for each of them.
      unsigned int propertyID = 0;

      // Keeps track whether any properties were modified. If so,
      // we will make sure to save and serialize the new changes.
      bool modified = false;

      // 2. Get a list of all properties on the object      
      for (auto& property : componentBoundType->AllProperties) {
        
        // If property is marked as hidden...
        if (property->HasAttribute("Hidden"))
          continue;

        // If the property is marked as an image...
        if (property->HasAttribute("Image")) {
          DisplayImage(property, object);
          continue;
        }

        // If the property is marked as a sound...
        if (property->HasAttribute("Sound")) {
          PreviewSound(property, object);
          continue;
        }
        
        // If the property is marked as an enumeration..
        if (property->HasAttribute("Enumeration")) {
          modified = SelectEnumeration(property, object, propertyID);
          continue;
        }

        // If it's a resource... 
        if (property->HasAttribute("Resource")) {
        //if (!property->Attributes.empty()) {
          modified = SelectResource(property, object, propertyID);
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
        getCall.SetHandleVirtual(Zilch::Call::This, object);        
        getCall.Invoke(report);        
                
        // Property: Enumeration
        if (Zilch::Type::IsEnumType(property->PropertyType)) {
          ImGui::PushID(propertyID++);
          std::vector<const char *> enums;
          auto enumType = Zilch::Type::GetBoundType(property->PropertyType);
          for (auto& enumProperty : enumType->AllProperties) {;
            enums.push_back(enumProperty->Name.c_str());            
          }
          // If the user selects an item... 
          static int currentItem = 0;
          ImGui::Text(property->Name.c_str());
          static bool thisModified = false;
          if (ImGui::Combo("##propertyID", &currentItem, enums.data(), enums.size())) {
            // Set the selected item as the current resource
            auto selectedEnum = enums.at(currentItem);
            // Retrieve...
            auto enumValue = enumType->GetStaticProperty(selectedEnum);
            Zilch::Call retriever(enumValue->Get, Daisy->getSystem<Reflection>()->Handler()->getState());
            retriever.Invoke(report);
            auto valueSet = retriever.Get<Zilch::Integer>(Zilch::Call::Return);
            // Set the property
            Zilch::ExceptionReport report;
            Zilch::Call setCall(property->Set, Daisy->getSystem<Reflection>()->Handler()->getState());
            setCall.SetHandleVirtual(Zilch::Call::This, object);
            setCall.Set(0, valueSet);
            setCall.Invoke(report);
          }
          if (thisModified && ImGui::GetIO().WantCaptureKeyboard == false)
          {
            modified = true;
            thisModified = false;
          }
          //if (ImGui::Selectable)
          ImGui::PopID();
        }
        

        // Property: Boolean
        else if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Boolean))) {
          auto boolean = getCall.Get<Zilch::Boolean>(Zilch::Call::Return);

          static bool thisModified = false;
          // If the user modifies it
          ImGui::PushID(propertyID++);
          if (ImGui::Checkbox(property->Name.c_str(), &boolean)) {
            Zilch::Call setCall(property->Set, Daisy->getSystem<Reflection>()->Handler()->getState());
            setCall.SetHandleVirtual(Zilch::Call::This, object);
            setCall.Set(0, boolean);
            setCall.Invoke(report);
          }
          if (thisModified && ImGui::GetIO().WantCaptureKeyboard == false)
          {
            modified = true;
            thisModified = false;
          }
          ImGui::PopID();
        }

        // Property: String
        else if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::String))) {
          auto string = getCall.Get<Zilch::String>(Zilch::Call::Return);
          char buf[128];
          strcpy(buf, string.c_str());          

          static bool thisModified = false;
          // If the user has given input, set the property
          ImGui::PushID(propertyID++);
          if (ImGui::InputText(property->Name.c_str(), buf, IM_ARRAYSIZE(buf))) {
            Zilch::Call setCall(property->Set, Daisy->getSystem<Reflection>()->Handler()->getState());
            setCall.SetHandleVirtual(Zilch::Call::This, object);
            setCall.Set(0, Zilch::String(buf));
            setCall.Invoke(report);
          }
          if (thisModified && ImGui::GetIO().WantCaptureKeyboard == false)
          {
            modified = true;
            thisModified = false;
          }
          ImGui::PopID();
        }
        
        // Property: Integer
        else if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Integer))) {
          auto integer = getCall.Get<Zilch::Integer>(Zilch::Call::Return);

          static bool thisModified = false;
          // If the user has given input, set the property
          ImGui::PushID(propertyID++);
          if (ImGui::InputInt(property->Name.c_str(), &integer)) {
            
            // Unsigned
            if (property->HasAttribute("Unsigned")) {
              if (integer < 0)
                integer = 0;
            }

            Zilch::Call setCall(property->Set, Daisy->getSystem<Reflection>()->Handler()->getState());
            setCall.SetHandleVirtual(Zilch::Call::This, object);
            setCall.Set(0, integer);
            setCall.Invoke(report);
          }
          if (thisModified && ImGui::GetIO().WantCaptureKeyboard == false)
          {
            modified = true;
            thisModified = false;
          }
          ImGui::PopID();
        }

        // Property: Integer2
        else if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Integer2))) {
          auto integer2 = getCall.Get<Zilch::Integer2>(Zilch::Call::Return);
          int int2[2] = { integer2.x, integer2.y };

          static bool thisModified = false;
          // If the user has given input, set the property
          ImGui::PushID(propertyID++);
          if (ImGui::InputInt2(property->Name.c_str(), int2)) {
            Zilch::Call setCall(property->Set, Daisy->getSystem<Reflection>()->Handler()->getState());
            setCall.SetHandleVirtual(Zilch::Call::This, object);
            setCall.Set(0, Zilch::Integer2(int2[0], int2[1]));
            setCall.Invoke(report);
          }
          if (thisModified && ImGui::GetIO().WantCaptureKeyboard == false)
          {
            modified = true;
            thisModified = false;
          }
          ImGui::PopID();
        }

        // Property: Integer3
        else if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Integer3))) {
          auto integer3 = getCall.Get<Zilch::Integer3>(Zilch::Call::Return);
          int int3[3] = { integer3.x, integer3.y, integer3.z };

          static bool thisModified = false;
          // If the user has given input, set the property
          ImGui::PushID(propertyID++);
          if (ImGui::InputInt3(property->Name.c_str(), int3)) {
            Zilch::Call setCall(property->Set, Daisy->getSystem<Reflection>()->Handler()->getState());
            setCall.SetHandleVirtual(Zilch::Call::This, object);
            setCall.Set(0, Zilch::Integer3(int3[0], int3[1], int3[2]));
            setCall.Invoke(report);
          }
          if (thisModified && ImGui::GetIO().WantCaptureKeyboard == false)
          {
            modified = true;
            thisModified = false;
          }
          ImGui::PopID();
        }

        // Property: Integer4
        else if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Integer4))) {
          auto integer4 = getCall.Get<Zilch::Integer4>(Zilch::Call::Return);
          int int4[4] = { integer4.x, integer4.y, integer4.z, integer4.w};

          static bool thisModified = false;
          // If the user has given input, set the property
          ImGui::PushID(propertyID++);
          if (ImGui::InputInt4(property->Name.c_str(), int4)) {
            Zilch::Call setCall(property->Set, Daisy->getSystem<Reflection>()->Handler()->getState());
            setCall.SetHandleVirtual(Zilch::Call::This, object);
            setCall.Set(0, Zilch::Integer4(int4[0], int4[1], int4[2], int4[3]));
            setCall.Invoke(report);    
          }
          if (thisModified && ImGui::GetIO().WantCaptureKeyboard == false)
          {
            modified = true;
            thisModified = false;
          }

          ImGui::PopID();
        }

        // Property: Real (float)
        else if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Real))) {
          auto real = getCall.Get<Zilch::Real>(Zilch::Call::Return);
          // If the user has given input, set the property
          ImGui::Text(property->Name.c_str());
          ImGui::PushID(propertyID++);
          
          static bool thisModified = false;
          if (ImGui::InputFloat(property->Name.c_str(), &real, 0.1f, 0.5f, 3)) {

            // Unsigned
            if (property->HasAttribute("Unsigned")) {
              if (real < 0.0f)
                real = 0.0f;
            }

            Zilch::Call setCall(property->Set, Daisy->getSystem<Reflection>()->Handler()->getState());
            auto set = property->Set;
            setCall.SetHandleVirtual(Zilch::Call::This, object);
            setCall.Set(0, real);
            setCall.Invoke(report);
          }
          if (thisModified && ImGui::GetIO().WantCaptureKeyboard == false)
          {
            modified = true;
            thisModified = false;
          }
          ImGui::PopID();
        }

        // Property: Real2 (Vec2)
        else if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Real2))) {
          auto vec2 = getCall.Get<Zilch::Real2>(Zilch::Call::Return);
          float vec2f[2] = { vec2.x, vec2.y };
          ImGui::Text(property->Name.c_str());
          ImGui::PushID(propertyID++);

          static bool thisModified = false;
          // If the user has given input, set the property
          if (ImGui::InputFloat2("##propertyID", vec2f, 3)) {
            Zilch::Call setCall(property->Set, Daisy->getSystem<Reflection>()->Handler()->getState());
            setCall.SetHandleVirtual(Zilch::Call::This, object);
            setCall.Set(0, Zilch::Real3(vec2f));
            setCall.Invoke(report);
          }
          if (thisModified && ImGui::GetIO().WantCaptureKeyboard == false)
          {
            modified = true;
            thisModified = false;
          }
          ImGui::PopID();
        }

        // Property: Real3 (Vec3)
        else if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Real3))) {
          auto vec3 = getCall.Get<Zilch::Real3>(Zilch::Call::Return);
          float vec3f[3] = { vec3.x, vec3.y, vec3.z };
          // If the user has given input, set the property
          ImGui::Text(property->Name.c_str());
          ImGui::PushID(propertyID++); 

          static bool thisModified = false;
          if (ImGui::InputFloat3("##propertyID", vec3f, 3) ) {
            //DCTrace << "Setting " << property->Name.c_str() << "\n";
            Zilch::Call setCall(property->Set, Daisy->getSystem<Reflection>()->Handler()->getState());
            setCall.SetHandleVirtual(Zilch::Call::This, object);
            setCall.Set(0, Zilch::Real3(vec3f));
            setCall.Invoke(report);
          }
          if (thisModified && ImGui::GetIO().WantCaptureKeyboard == false)
          {
            modified = true;
            thisModified = false;
          }
          ImGui::PopID(); 
        }        

        // Property: Real4 (Vec4)
        else if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Real4))) {
          auto vec4 = getCall.Get<Zilch::Real4>(Zilch::Call::Return);
          float vec4f[4] = { vec4.x, vec4.y, vec4.z, vec4.w};
          // If the user has given input, set the property          
          ImGui::Text(property->Name.c_str());
          ImGui::PushID(propertyID++);

          static bool thisModified = false;
          if (ImGui::InputFloat4("##propertyID", vec4f, 3)) {
            Zilch::Call setCall(property->Set, Daisy->getSystem<Reflection>()->Handler()->getState());
            setCall.SetHandleVirtual(Zilch::Call::This, object);
            setCall.Set(0, Zilch::Real4(vec4f[0], vec4f[1], vec4f[2], vec4f[3]));
            setCall.Invoke(report);
          }
          if (thisModified && ImGui::GetIO().WantCaptureKeyboard == false)
          {
            modified = true;
            thisModified = false;
          }
          ImGui::PopID();
        }
      }

      // If the object was modified...
      return modified;
    }

    /**************************************************************************/
    /*!
    @brief  Allows the user to add a component to the entity.
    @todo   If the component has been marked as skipped, do not attempt to add it.
    */
    /**************************************************************************/
    bool Editor::AddComponent(EntityPtr selectedEntity)
    {
      static bool Scanned = false;

      // Grab a container of all bound components.. 
      static std::vector<Zilch::BoundType*> components;
      static std::vector<const char*> componentNames;
      // Scan for components only when need be
      if (!Scanned) {
        componentNames.clear();
        components = Daisy->getSystem<Systems::Reflection>()->AllComponents();
        for (auto component : components) {
          // Do not display components marked as hidden
          if (component->HasAttribute("Hidden"))
            continue;

          // Components to skip
          bool skip = false;
          std::vector<std::string> skippableComponents{ "Component", "Collider", "Graphical" };
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
            componentNames.push_back(component->Name.c_str());
            auto name = component->Name.c_str();
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
        DCTrace << "Editor::AddComponent - " << componentName << "\n";
        auto& dependencies = Components::Sprite::mDependencies;
        selectedEntity->AddComponentByName(componentName, false);
        Scanned = false;
        // A component was added
        return true;
      }

      // No component was added
      return false;
    }




    /**************************************************************************/
    /*!
    @brief  Reverts a GameObject to its Archetype from file.
    */
    /**************************************************************************/
    void Editor::RevertToArchetype()
    {

    }

    /**************************************************************************/
    /*!
    @brief  Displays an image on the Editor's properties inspector.
    @param  property A pointer to the property.
    @param  object A pointer to the object the property belongs to.
    @todo   Currently we are pulling the image from the Content system..
    */
    /**************************************************************************/
    void DisplayImage(Zilch::Property * property, ObjectPtr object)
    {
      // Create an exception report object
      Zilch::ExceptionReport report;      
      Zilch::Call getCall(property->Get, Daisy->getSystem<Reflection>()->Handler()->getState());
      getCall.SetHandleVirtual(Zilch::Call::This, object);
      getCall.Invoke(report);
      // Grab the image's path
      auto imagePath = getCall.Get<Zilch::String>(Zilch::Call::Return);
      
      // Grab a pointer to the texture data of the image from the SpriteSource it's on
      auto spriteSource = dynamic_cast<SpriteSource*>(object);
      auto textureData = spriteSource->getTexture();
      //ImTextureID texID = textureData;
      ImGui::Image((void*)(textureData.TextureID), ImVec2(textureData.Width, textureData.Height), 
                   ImVec2(0, 1), ImVec2(1, 0), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));

    }

    /**************************************************************************/
    /*!
    @brief  Previews a sound on the Editor's properties inspector.
    @param  property A pointer to the property.
    @param  object A pointer to the object the property belongs to.
    @todo   Currently we are pulling the sound from the Content system..
    */
    /**************************************************************************/
    void PreviewSound(Zilch::Property * property, ObjectPtr object)
    {
      // Create an exception report object
      Zilch::ExceptionReport report;
      Zilch::Call getCall(property->Get, Daisy->getSystem<Reflection>()->Handler()->getState());
      getCall.SetHandleVirtual(Zilch::Call::This, object);
      getCall.Invoke(report);
      // Grab the sound's path
      auto soundPath = getCall.Get<Zilch::String>(Zilch::Call::Return);

      // Grab the reference to the sound this SoundCue holds
      auto soundCue = dynamic_cast<SoundCue*>(object);

      if (ImGui::Button("Preview")) {
        Daisy->getSystem<Audio>()->PlaySound(std::string(soundCue->getObjectName()));
      }
      ImGui::SameLine();
      if (ImGui::Button("Stop Preview")) {
        Daisy->getSystem<Audio>()->StopSound(std::string(soundCue->getObjectName()));
      }

    }

}
}