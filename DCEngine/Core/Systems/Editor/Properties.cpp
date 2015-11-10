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

            // Hardcoded currently for the transform component!
            //if (std::type_index(typeid(*component)) == std::type_index(typeid(Transform))) {
            //  auto transform = SelectedObject->getComponent<Transform>(); // or d.cast instead lol 
            //  DisplayTransform(transform);
            //}

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
        if (!property->HasAttribute("Property"))
          continue;

        auto componentHdl = component.get();
        // Property-getter
        Zilch::Call get(property->Get, Daisy->getSystem<Reflection>()->Handler()->getState());
        get.SetHandle(Zilch::Call::This, componentHdl->ZilchGetDerivedType());
        // Property-setter
        Zilch::Call set(property->Set, Daisy->getSystem<Reflection>()->Handler()->getState());
        get.SetHandle(Zilch::Call::This, componentHdl->ZilchGetDerivedType());
              
        // Property: Float
        if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Real))) {
          auto real = get.Get<Zilch::Real>(Zilch::Call::Return);
          ImGui::InputFloat(property->Name.c_str(), &real, 0.1f);
        }
        // Property: Bool
        if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Boolean))) {
        }
        // Property: String
        if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::String))) {

          //ImGui::InputText(property->Name.c_str(), property->Get->, IM_ARRAYSIZ)
        }
        // Property: Vec2
        if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Real2))) {

        }
        // Property: Vec3
        if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Real3))) {

        }
        // Property: Vec4
        if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Real4))) {

        }
        

        //DCTrace << property->Name.c_str() << "\n";
      }
    }


  }
}