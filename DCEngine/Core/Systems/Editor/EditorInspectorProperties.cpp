/******************************************************************************/
/*!
@file   EditorInspectorProperties.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   2/28/2016
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "EditorInspector.h"

#include "../../Engine/Engine.h"


namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    @brief  Displays properties for an object.
    @param  object A pointer to the Component.
    @note   Properties are listed from the order they were defined.
    @todo   Refactor the property-setting so I DONT COPY PASTE THE SAME
    3 LINES.
    Dios mio, 200 line function!
    */
    /**************************************************************************/
    bool EditorInspector::DisplayProperties(ObjectPtr object, Zilch::Handle handle)
    {
      // 1. Get the object's BoundType, which has a wealth of reflected data
      auto componentBoundType = object->ZilchGetDerivedType();
      
      // 2. If it'a ZilchComponent, we will be doing slightly different operation
      if (Zilch::Type::IsSame(componentBoundType, ZilchTypeId(ZilchComponent))) {
        componentBoundType = Daisy->getSystem<Reflection>()->Handler()->ScriptLibrary->BoundTypes.findValue(object->getObjectName().c_str(), nullptr);        
      }

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
        
        // Only reflect properties with the "Property" attribute
        if (!property->HasAttribute("Property")) {
          continue;
        }

        // If property is marked as hidden...
        if (property->HasAttribute("Hidden"))
          continue;

        // If the property is marked as an image...
        if (property->HasAttribute("Image")) {
          PreviewImage(property, object);
          continue;
        }

        // If the property is marked as a sound...
        if (property->HasAttribute("Sound")) {
          PreviewSound(property, object);
          continue;
        }

        // If the property is marked as an enumeration..
        if (property->HasAttribute("Enumeration")) {
          //modified = SelectEnumeration(property, object, propertyID);
          continue;
        }

        // If it's a resource... 
        if (property->HasAttribute("Resource")) {
          //if (!property->Attributes.empty()) {
          modified = InspectResource(property, object, propertyID);
          continue;
        }

        // Create an exception report object
        Zilch::ExceptionReport report;
        // Grab the current property
        Zilch::Call getCall(property->Get, Daisy->getSystem<Reflection>()->Handler()->GetState());        
        // If it has a handle...
        if (handle.Dereference())
          getCall.SetHandle(Zilch::Call::This, handle); // Pass in a handle here
        // Else use the pointer
        else
          getCall.SetHandleVirtual(Zilch::Call::This, object); // Pass in a handle here        
        getCall.Invoke(report);

        /*=======================
        // Property: Enumeration
        =======================*/
        if (Zilch::Type::IsEnumType(property->PropertyType)) {
          ImGui::PushID(propertyID++);

          auto currentVal = getCall.Get<Zilch::Integer>(Zilch::Call::Return);
          int currentItem = currentVal;

          // Fill in a list of all possible enum values by name
          std::vector<const char *> enums;
          auto enumType = Zilch::Type::GetBoundType(property->PropertyType);
          for (auto& enumProperty : enumType->AllProperties) {            
            enums.push_back(enumProperty->Name.c_str());
          }

          // If the user selects an item... 
          ImGui::Text(property->Name.c_str());
          if (ImGui::Combo("##propertyID", &currentItem, enums.data(), static_cast<int>(enums.size()))) {
            // Set the selected item as the current resource
            auto selectedEnum = enums.at(currentItem);
            // Retrieve...
            auto enumValue = enumType->GetStaticProperty(selectedEnum);
            Zilch::Call retriever(enumValue->Get, Daisy->getSystem<Reflection>()->Handler()->GetState());
            retriever.Invoke(report);
            auto valueSet = retriever.Get<Zilch::Integer>(Zilch::Call::Return);
            // Set the property
            Set(ZilchInterface::GetState(), object, handle, property, valueSet);
            modified = true;
          }
          ImGui::PopID();
        }

        /*=======================
        // Property: Boolean
        =======================*/
        else if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Boolean))) {
          auto boolean = getCall.Get<Zilch::Boolean>(Zilch::Call::Return);

          // If the user modifies it
          ImGui::PushID(propertyID++);
          //ImGui::Text(property->Name.c_str());
          if (ImGui::Checkbox(property->Name.c_str(), &boolean)) {
            Set(ZilchInterface::GetState(), object, handle, property, boolean);
            modified = true;
          }
          ImGui::PopID();
        }

        /*=======================
        // Property: String
        =======================*/
        else if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::String))) {
          auto string = getCall.Get<Zilch::String>(Zilch::Call::Return);
          char buf[128];
          strcpy(buf, string.c_str());

          // If the user has given input, set the property
          ImGui::PushID(propertyID++);
          ImGui::Text(property->Name.c_str());
          if (ImGui::InputText("##propertyID", buf, IM_ARRAYSIZE(buf))) {
            Set(ZilchInterface::GetState(), object, handle, property, Zilch::String(buf));
            modified = true;
          }
          ImGui::PopID();
        }

        /*=======================
        // Property: Integer
        =======================*/
        else if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Integer))) {
          auto integer = getCall.Get<Zilch::Integer>(Zilch::Call::Return);

          // If the user has given input, set the property
          ImGui::PushID(propertyID++);
          ImGui::Text(property->Name.c_str());
          // If it's of a given range..
          if (property->HasAttribute("Range")) {
            auto range = Systems::ZilchInterface::Get().getAttribute(property, "Range");
            auto& min = range->Parameters.front().NumberValue;
            auto& max = range->Parameters.back().NumberValue;
            if (ImGui::SliderInt("##propertyID", &integer, static_cast<int>(min), static_cast<int>(max))) {
              CheckUnsigned(property, integer);
              Set(ZilchInterface::GetState(), object, handle, property, integer);
              modified = true;
            }
          }
          else {
            if (ImGui::InputInt("##propertyID", &integer)) {
              CheckUnsigned(property, integer);
              Set(ZilchInterface::GetState(), object, handle, property, integer);
              modified = true;
            }
          }
          ImGui::PopID();
        }

        /*=======================
        // Property: Integer2
        =======================*/
        else if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Integer2))) {
          auto integer2 = getCall.Get<Zilch::Integer2>(Zilch::Call::Return);
          int int2[2] = { integer2.x, integer2.y };

          // If the user has given input, set the property
          ImGui::PushID(propertyID++);
          ImGui::Text(property->Name.c_str());
          if (ImGui::InputInt2("##propertyID", int2)) {
            Set(ZilchInterface::GetState(), object, handle, property, Zilch::Integer2(int2[0], int2[1]));
            modified = true;
          }
          ImGui::PopID();
        }

        /*=======================
        // Property: Integer3
        =======================*/
        else if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Integer3))) {
          auto integer3 = getCall.Get<Zilch::Integer3>(Zilch::Call::Return);
          int int3[3] = { integer3.x, integer3.y, integer3.z };

          // If the user has given input, set the property
          ImGui::PushID(propertyID++);
          ImGui::Text(property->Name.c_str());
          if (ImGui::InputInt3("##propertyID", int3)) {
            Set(ZilchInterface::GetState(), object, handle, property, Zilch::Integer3(int3[0], int3[1], int3[2]));
            modified = true;
          }
          ImGui::PopID();
        }

        /*=======================
        // Property: Integer4
        =======================*/
        else if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Integer4))) {
          auto integer4 = getCall.Get<Zilch::Integer4>(Zilch::Call::Return);
          int int4[4] = { integer4.x, integer4.y, integer4.z, integer4.w };

          // If the user has given input, set the property
          ImGui::PushID(propertyID++);
          ImGui::Text(property->Name.c_str());
          if (ImGui::InputInt4("##propertyID", int4)) {
            Set(ZilchInterface::GetState(), object, handle, property, Zilch::Integer4(int4[0], int4[1], int4[2], int4[3]));
            modified = true;
          }

          ImGui::PopID();
        }

        /*=======================
        // Property: Real
        =======================*/
        else if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Real))) {
          auto real = getCall.Get<Zilch::Real>(Zilch::Call::Return);
          // If the user has given input, set the property
          ImGui::PushID(propertyID++);
          ImGui::Text(property->Name.c_str());

          // If it's of a given range..
          if (property->HasAttribute("Range")) {
            auto range = Systems::ZilchInterface::Get().getAttribute(property, "Range");
            auto& min = range->Parameters.front().NumberValue;
            auto& max = range->Parameters.back().NumberValue;
            if (ImGui::SliderFloat("##propertyID", &real, static_cast<Real>(min),
              static_cast<Real>(max))) {

              // Unsigned
              if (property->HasAttribute("Unsigned")) {
                if (real < 0.0f)
                  real = 0.0f;
              }
              Set(ZilchInterface::GetState(), object,  handle, property, real);
              modified = true;
            }
          }
          else {
            if (ImGui::InputFloat("##propertyID", &real, 0.5f, 0.5f, 3)) {

              // Unsigned
              if (property->HasAttribute("Unsigned")) {
                if (real < 0.0f)
                  real = 0.0f;
              }
              Set(ZilchInterface::GetState(), object, handle, property, real);
              modified = true;
            }
          }

          ImGui::PopID();
        }

        /*=======================
        // Property: Real2
        =======================*/
        else if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Real2))) {
          auto vec2 = getCall.Get<Zilch::Real2>(Zilch::Call::Return);
          float vec2f[2] = { vec2.x, vec2.y };
          ImGui::Text(property->Name.c_str());
          ImGui::PushID(propertyID++);

          // If the user has given input, set the property
          if (ImGui::InputFloat2("##propertyID", vec2f, 3)) {
            Set(ZilchInterface::GetState(), object, handle, property, Zilch::Real2(vec2f));
            modified = true;
          }
          ImGui::PopID();
        }

        /*=======================
        // Property: Real3
        =======================*/
        else if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Real3))) {
          auto vec3 = getCall.Get<Zilch::Real3>(Zilch::Call::Return);
          float vec3f[3] = { vec3.x, vec3.y, vec3.z };
          // If the user has given input, set the property
          ImGui::Text(property->Name.c_str());
          ImGui::PushID(propertyID++);

          if (ImGui::InputFloat3("##propertyID", vec3f, 3)) {
            Set(ZilchInterface::GetState(), object, handle, property, Zilch::Real3(vec3f));
            modified = true;
          }
          ImGui::PopID();
        }

        /*=======================
        // Property: Real4
        =======================*/
        else if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Real4))) {
          auto vec4 = getCall.Get<Zilch::Real4>(Zilch::Call::Return);
          float vec4f[4] = { vec4.x, vec4.y, vec4.z, vec4.w };
          // If the user has given input, set the property          
          ImGui::Text(property->Name.c_str());
          ImGui::PushID(propertyID++);

          // If it's of a given range..
          if (property->HasAttribute("Range")) {
            auto range = Systems::ZilchInterface::Get().getAttribute(property, "Range");
            auto& min = range->Parameters.front().NumberValue;
            auto& max = range->Parameters.back().NumberValue;
            if (ImGui::SliderFloat4(property->Name.c_str(), vec4f, static_cast<float>(min),
              static_cast<float>(max))) {
              Set(ZilchInterface::GetState(), object, handle, property, Zilch::Real4(vec4f));
              modified = true;
            }
          }
          else {
            if (ImGui::InputFloat4("##propertyID", vec4f, 3)) {
              Set(ZilchInterface::GetState(), object, handle, property, Zilch::Real4(vec4f));
              modified = true;
            }
          }
          ImGui::PopID();
        }
      }

      // If the object was modified...
      return modified;
    }

  }
}