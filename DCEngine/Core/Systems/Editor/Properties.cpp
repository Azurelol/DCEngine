/******************************************************************************/
/*!
@file   Properties.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/02/2015
@brief  This file includes the implementation for the Editor's properties widget.
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
    @brief  Displays the transform component's properties on ImGui
    @todo   Soon-to-be deprecated!
    */
    /**************************************************************************/
    void DisplayTransform(Transform* transform) {

        ImGui::Text("Translation");
        float trans[3] = { transform->Translation.x,
          transform->Translation.y,
          transform->Translation.z };
        ImGui::InputFloat3("", trans);

        ImGui::Text("");
        float rot[3] = { transform->Rotation.x,
          transform->Rotation.y,
          transform->Rotation.z };
        ImGui::InputFloat3("", rot);

        ImGui::Text("Scale");
        float scale[3] = { transform->Scale.x,
          transform->Scale.y,
          transform->Scale.z };
        ImGui::InputFloat3("", scale);

      }


    /**************************************************************************/
    /*!
    \brief  Displays the properties of the currently selected object.
    */
    /**************************************************************************/
    void Editor::WidgetProperties()
    {
      if (!WidgetPropertiesEnabled)
        return;
      
      ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiSetCond_FirstUseEver);
      ImGui::Begin("Properties", &WidgetPropertiesEnabled);

      // If there's an object selected, display its properties.
      if (SelectedObject != nullptr) {
        // 1. Display the object's name
        auto objBoundType = SelectedObject->ZilchGetDerivedType();
        //auto objNameFn = objBoundType->FindFunction("Name", objBoundType, Zilch::Array<Zilch::Type*>(), ZilchTypeId(void), Zilch::FindMemberOptions::None);
        ImGui::TextColored(ImVec4(0, 0.5, 1, 1), "Name: ");
        ImGui::Text(SelectedObject->Name().c_str());
        // 2. Display its components
        ImGui::TextColored(ImVec4(0, 0.5, 1, 1), "Components: ");
        for (auto component : *SelectedObject->AllComponents()) {
          if (ImGui::TreeNode(component->Name().c_str())) {
            // 3. If the user clicks on a tree-node, display the commponent's properties
            //    through reflection
            DisplayProperties(component);
            ImGui::TreePop();
          }
                  
        }
      }

      ImGui::End();
    }



    /**************************************************************************/
    /*!
    @brief  Displays the component's properties, allowing them to be
            seen and edited.
    @param  component A pointer to the Component.
    @note   Properties are listed from the order they were defined.
    */
    /**************************************************************************/
    void Editor::DisplayProperties(ComponentPtr component) {
      // 1. Get the component's BoundType, which has a wealth of reflected data
      auto componentBoundType = component->ZilchGetDerivedType();
      if (componentBoundType == nullptr)
        return;
      // 2. Get a list of all properties on the object
      for (auto property : componentBoundType->AllProperties) {
        // Display the name of the property
        ImGui::Text(property->Name.c_str());

        // If the bound field/property does not have the Property Attribute, do
        // nothing.
        //if (!property->HasAttribute("Property"))
        //  continue;

        auto state = Daisy->getSystem<Reflection>()->Handler()->getState();
        
        //auto componentHdl = Zilch::Handle(state, component->ZilchGetDerivedType(),
        //                                         component->ZilchGetDerivedType()->);
          
        

        // Property-getter
        Zilch::Call getCall(property->Get, Daisy->getSystem<Reflection>()->Handler()->getState());
        Component *aPointer = component.get();
        getCall.SetHandle(Zilch::Call::This, aPointer);
        // Property-setter
        Zilch::Call setCall(property->Set, Daisy->getSystem<Reflection>()->Handler()->getState());
        setCall.SetHandle(Zilch::Call::This, aPointer);
              

        // Property: Boolean
        if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Boolean))) {
          auto boolean = getCall.Get<Zilch::Boolean>(Zilch::Call::Return);
          ImGui::Checkbox(property->Name.c_str(), &boolean);
          //setCall.Set(0, boolean);
        }
        // Property: String
        if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::String))) {
          auto string = getCall.Get<Zilch::String>(Zilch::Call::Return);
          char buf[32];
          strcpy(buf, string.c_str());
          ImGui::InputText(property->Name.c_str(), buf, IM_ARRAYSIZE(buf));
          //setCall.Set(0, buf);
        }
        // Property: Int
        if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Integer))) {
          auto integer = getCall.Get<Zilch::Integer>(Zilch::Call::Return);
          ImGui::InputInt(property->Name.c_str(), &integer);
          //setCall.Set(0, integer);
        }
        // Property: Int2
        if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Integer2))) {
          auto integer2 = getCall.Get<Zilch::Integer2>(Zilch::Call::Return);
          int int2[2] = { integer2.x, integer2.y };
          ImGui::InputInt2(property->Name.c_str(), int2);
          //setCall.Set(0, Zilch::Integer2(int2[0], int2[1]));
        }
        // Property: Int3
        if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Integer3))) {
          auto integer3 = getCall.Get<Zilch::Integer3>(Zilch::Call::Return);
          int int3[3] = { integer3.x, integer3.y, integer3.z };
          ImGui::InputInt3(property->Name.c_str(), int3);
          //setCall.Set(0, int3);
        }
        // Property: Int4
        if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Integer4))) {
          auto integer4 = getCall.Get<Zilch::Integer4>(Zilch::Call::Return);
          int int4[4] = { integer4.x, integer4.y, integer4.z, integer4.w};
          ImGui::InputInt4(property->Name.c_str(), int4);
          //setCall.Set(0, int4);
        }
        // Property: Real (float)
        if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Real))) {
          auto real = getCall.Get<Zilch::Real>(Zilch::Call::Return);
          ImGui::InputFloat(property->Name.c_str(), &real, 0.1f);
          //setCall.Set(0, real);
        }
        // Property: Real2 (Vec2)
        if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Real2))) {
          auto vec2 = getCall.Get<Zilch::Real2>(Zilch::Call::Return);
          float vec2f[2] = { vec2.x, vec2.y };
          ImGui::InputFloat2(property->Name.c_str(), vec2f);
         // setCall.Set(0, vec2f);
        }
        // Property: Real3 (Vec3)
        if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Real3))) {
          auto vec3 = getCall.Get<Zilch::Real3>(Zilch::Call::Return);
          float vec3f[3] = { vec3.x, vec3.y, vec3.z };
          ImGui::InputFloat2(property->Name.c_str(), vec3f);
          //setCall.Set(0, vec3f);
        }
        // Property: Real4 (Vec4)
        if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Real4))) {
          auto vec4 = getCall.Get<Zilch::Real4>(Zilch::Call::Return);
          float vec4f[4] = { vec4.x, vec4.y, vec4.z, vec4.w};
          ImGui::InputFloat2(property->Name.c_str(), vec4f);
          //setCall.Set(0, vec4f);
        }
      }
    }


  }
}