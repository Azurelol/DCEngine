/******************************************************************************/
/*
  File:    InspectorModule.cpp
  Author:  Alejandro Hitti
  Date:    11/19/14
  Brief:   The Inspector Module handles the inspection of all the components
           inside of the selected object.

  All content © 2014-2015 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

// Includes
//------------------------------------------------------------------------------
#include "stdinc.h"

#include "InspectorModule.h"
#include "EntityModule.h"
#include "Reflex.h"

namespace Editor
{
  bool InspectorModule::m_IsActive = false;
  bool InspectorModule::m_IsVisible = true;
  char InspectorModule::m_TextBuffer[256];
  bool InspectorModule::m_IsFirstTime = true;
  std::vector<std::string> InspectorModule::m_OrderedComponentStrings;

  // Initializes the Module
  bool InspectorModule::InspectorModuleInitialize()
  {
    // Sorts the list of components alphabetically to make it easier to search
    // for a specific component in the combo box

    // Populate the list and store the indices
    for (unsigned i = 0; i < Core::NumOfComponents; ++i)
      m_OrderedComponentStrings.push_back(Core::ComponentStrings[i]);

    // Sort the array alphabetically
    std::sort(m_OrderedComponentStrings.begin(), m_OrderedComponentStrings.end());

    return true;
  }

  // Checks if the module is currently active
  bool InspectorModule::InspectorModuleIsActive()
  {
    return m_IsActive;
  }

  // Sets the module active flag to the state passed in
  void InspectorModule::InspectorModuleSetActive(bool state)
  {
    m_IsActive = state;
  }

  // Checks if the module is currently visible
  bool InspectorModule::InspectorModuleIsVisible()
  {
    return m_IsVisible;
  }

  // Sets the module visible flag to the state passed in
  void InspectorModule::InspectorModuleSetVisible(bool state)
  {
    m_IsVisible = state;
  }

  // Updates the module
  void InspectorModule::InspectorModuleUpdate()
  {
    Core::Entity* entity = g_Factory.GetEntity(EDITOR->GetSelectedEntity());
    if (!entity)
      return;

    if ((EDITOR->GetSelectedEntity() != Core::c_InvalidHandle) ||
        (g_Factory.GetEntity(EDITOR->GetSelectedEntity()) != nullptr))
      m_IsActive = true;
    else
      m_IsActive = false;

    if (!m_IsVisible || !m_IsActive)
      return;

    ImGui::Begin("Inspector", &m_IsActive, ImVec2(0, 0), EDITOR->GetFlags().fillAlpha, EDITOR->GetLayoutFlags());

    // Positions the window the first time you run it
    if (m_IsFirstTime)
    {
      EDITOR->SetModulePosition((int)ImGui::GetWindowSize().x, (int)ImGui::GetWindowSize().y, Editor::RefPoint::TopRight, 
                                -5, (int)((EDITOR->GetGFX()->GetScreenSize().y - ImGui::GetWindowSize().y) / 2.15f));
      m_IsFirstTime = false;
      ImGui::End();
      return;
    }

    if (!m_IsActive)
      EDITOR->SetSelectedEntity(Core::c_InvalidHandle);

    if (ImGui::CollapsingHeader(entity->GetName().c_str(), (const char*)0, false, true))
    {
      if (ImGui::CollapsingHeader("Components", (const char*)0, true, false))
        Inspect(entity);

      if (ImGui::CollapsingHeader("Options", (const char*)0, true, false))
      {
        ChangeName(entity);
        ImGui::Separator();
        AddComponent(entity);
        SaveToArchetype(entity);
      }
    }

    ImGui::End();
  }

  bool InspectorModule::Inspect(Core::Entity* entity)
  {
    bool isChanged = false;
    bool isCurrChanged = false;

    // Get all components from the entity
    const Core::ComponentContainer& components = entity->GetAllComponents();

    // Checks if component is empty
    bool isComponentsEmpty = true;

    // Loops through every component type in the entity
    for (auto iter = components.begin(); iter != components.end(); ++iter)
    {
      const std::vector<Core::Component*>& vec = iter->second;
      if (vec.size() != 0)
        isComponentsEmpty = false;

      // Loop through every individual components
      for (unsigned int i = 0; i < vec.size(); ++i)
      {
        Core::Component* component = vec[i];

        // Start the tree node with he component name
        if (ImGui::TreeNode(std::string(component->GetName() + "##tree").c_str()))
        {
          // Get reflection information
          const Reflex::TypeInfo* compInfo = component->GetTypeInfo();
          Reflex::Variant compVar = component;
          void *compData = compVar.GetData();
          std::vector<Reflex::Member> compMembers = compInfo->GetAllMembers();

          if (compMembers.empty())
          {
            ImGui::Text("This Component is empty");
            ImGui::TreePop();
            break;
          }

          // For every member in the current component
          for (unsigned j = 0; j < compMembers.size(); ++j)
          {
            // Get reflection information
            const Reflex::Member* mem = &compMembers[j];
            const Reflex::TypeInfo* memInfo = mem->GetType();
            void* offsetData = PTR_ADD(compData, mem->GetOffset());

            // Base Case
            if (IsBaseCase(memInfo))
              isCurrChanged = InspectPod(Reflex::Variant(memInfo, offsetData), mem, entity);

            // Recursive Case
            else
            {
              // Start a tree node with the name of the member
              if (ImGui::TreeNode(mem->GetName()))
              {
                // Kickstart a recursive call to get members of members
                isCurrChanged = InspectRec(Reflex::Variant(memInfo, offsetData), entity);

                // Pop member name tree node
                ImGui::TreePop();
              }
              SetHelpTooltip("Open to see members.");
            }

            if (isCurrChanged)
              isChanged = true;
          }

          std::string remCompText = "Remove " + component->GetName() + " Component";
          if (ImGui::Button(remCompText.c_str()))
          {
            entity->RemoveComponent(component);
            --i; // So that the next iteration continues on current i
                 // Without this we accidentally skip a component
            if (isCurrChanged)
              isChanged = true;

            continue;
          }
          SetHelpTooltip("Removes the component from the entity.");

          // Pop component name tree node
          ImGui::TreePop();
        }
        SetHelpTooltip("Open to see members.");
      }
    }

    // If component is empty, show this message
    if (isComponentsEmpty)
    {
      ImGui::Text("No components found on this entity.");
      SetHelpTooltip("Open the options menu to add components to the entity.");
    }

    return isChanged;
  }

  bool InspectorModule::InspectRec(Reflex::Variant var, Core::Entity* entity)
  {
    bool isChanged = false;
    bool isCurrChanged = false;

    // Get reflection information
    const Reflex::TypeInfo* info = var.GetTypeInfo();
    std::vector<Reflex::Member> members = info->GetAllMembers();
    void* data = var.GetData();

    // For every member in the type
    for (unsigned i = 0; i < members.size(); ++i)
    {
      // Get reflection information
      const Reflex::Member* mem = &members[i];
      const Reflex::TypeInfo* memInfo = mem->GetType();
      void* offsetData = PTR_ADD(data, mem->GetOffset());

      // Base Case
      if (IsBaseCase(memInfo))
        isCurrChanged = InspectPod(Reflex::Variant(memInfo, offsetData), mem, entity);

      // Recursive Case
      else
      {
        // Start a tree node with the name of the member
        if (ImGui::TreeNode(mem->GetName()))
        {
          // Kickstart a recursive call to get members of members
          isCurrChanged = InspectRec(Reflex::Variant(memInfo, offsetData), entity);

          // Pop member name tree node
          ImGui::TreePop();
        }
        SetHelpTooltip("Open to see members.");
      }

      if (isCurrChanged)
        isChanged = true;
    }

    return isChanged;
  }

  bool HelperEulerRotation(Math::Vector3& eulerAngles)
  {
    ImGui::PushItemWidth(150);
    int changedCounter = 0;

    // Gets the Pitch euler angle
    if (ImGui::InputFloat("##Pitch", &eulerAngles.X, 1, 10, 2))
      ++changedCounter;
    ImGui::SameLine();
    ImGui::PushAllowKeyboardFocus(false);
    if (ImGui::SliderFloat("Pitch", &eulerAngles.X, 0.0f, 360.0f, "%.2f"))
      ++changedCounter;
    ImGui::PopAllowKeyboardFocus();

    // Gets the Yaw euler angle
    if (ImGui::InputFloat("##Yaw", &eulerAngles.Y, 1, 10, 2))
      ++changedCounter;
    ImGui::SameLine();
    ImGui::PushAllowKeyboardFocus(false);
    if (ImGui::SliderFloat("Yaw", &eulerAngles.Y, 0.0f, 360.0f, "%.2f"))
      ++changedCounter;
    ImGui::PopAllowKeyboardFocus();

    // Gets the Roll euler angle
    if (ImGui::InputFloat("##Roll", &eulerAngles.Z, 1, 10, 2))
      ++changedCounter;
    ImGui::SameLine();
    ImGui::PushAllowKeyboardFocus(false);
    if (ImGui::SliderFloat("Roll", &eulerAngles.Z, 0.0f, 360.0f, "%.2f"))
      ++changedCounter;
    ImGui::PopAllowKeyboardFocus();

    ImGui::PopItemWidth();

    return (changedCounter > 0);
  }

  bool InspectorModule::InspectPod(Reflex::Variant var, const Reflex::Member* mem, const Core::Entity* entity)
  {
    bool isChanged = false;

    // Get reflection information
    const Reflex::TypeInfo* info = var.GetTypeInfo();
    void* data = var.GetData();

    // Deal with all POD cases
    //if (info == TYPE_STR("double"))
    //  ImGui::InputDouble(mem->GetName(), (double*)data, 0.1, 0.5);

    if (info == TYPE_STR("float"))
      isChanged = ImGui::InputFloat(mem->GetName(), (float*)data, 0.1f, 0.5f);

    else if (info == TYPE_STR("int"))
      isChanged = ImGui::InputInt(mem->GetName(), (int*)data);

    else if (info == TYPE_STR("unsigned"))
      isChanged = ImGui::InputInt(mem->GetName(), (int*)data);

    else if (info == TYPE_STR("bool"))
      isChanged = ImGui::Checkbox(mem->GetName(), (bool*)data);

    else if (info == TYPE_STR("std::string"))
    {
      std::strcpy(m_TextBuffer, reinterpret_cast<std::string*>(data)->c_str());

      if (ImGui::InputText(mem->GetName(), m_TextBuffer, 256, ImGuiInputTextFlags_::ImGuiInputTextFlags_AutoSelectAll    |
                                                              ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue))
      {
        *reinterpret_cast<std::string*>(data) = m_TextBuffer;
        isChanged = true;
      }
    }

    else if (info == TYPE_STR("Vector3"))
    {
      float* vec = reinterpret_cast<float*>(data);
      isChanged = ImGui::InputFloat3(mem->GetName(), vec, 2);
    }

    // Special case to display a combo box of textures, read from the file directory
    else if (info == TYPE_STR("Texture"))
    {
      // Gets all texture names
      std::string* texName = reinterpret_cast<std::string*>(data);
      std::vector<String> texList = Utilities::Filesystem::GetFilepathsInDirectory("./textures/");
      static int index = 0;

      for (unsigned i = 0; i < texList.size(); ++i)
      {
        // Strips path and extension
        texList[i] = Utilities::Filesystem::GetFilenameFromPath(texList[i]);

        // Starts the combo box at the currently selected texture
        if (*texName == texList[i])
          index = i;
      }

      // Sets the texture. Loads it into memory if it's not there yet.
      if (ImGui::Combo(std::string("Texures##" + std::to_string(*(int*)entity)).c_str(), &index, VectorOfStringsItemGettor, &texList, texList.size(), 12))
      {
        // Load the texture if it cannot be found
        if (EDITOR->GetGFX()->GetTexture(texList[index]) == EZ::Handle::Null())
          EDITOR->GetGFX()->LoadTexture(std::string("./textures/" + texList[index]));

        // Set the texture
        entity->GetComponentAs<Graphics3DComponent>()->SetTextureName(texList[index]);

        isChanged = true;
      }
    }

    // Special case for Model3D to display a combo box with all models in the file directory
    else if (info == TYPE_STR("Model3D"))
    {
      // Gets all model names
      std::string* modelName = reinterpret_cast<std::string*>(data);
      std::vector<std::string> modelList = Utilities::Filesystem::GetFilepathsInDirectory("./models/");
      static int index = 0;
      
      for (unsigned i = 0; i < modelList.size(); ++i)
      {
        modelList[i] = Utilities::Filesystem::GetFilenameFromPath(modelList[i]);

        // Starts the combo box at the currently selected model
        if (*modelName == modelList[i])
          index = i;
      }

      // Sets the model. Loads it into memory if it's not there yet.
      if (ImGui::Combo(std::string("Models##" + std::to_string(*(int*)entity)).c_str(), &index, VectorOfStringsItemGettor, &modelList, modelList.size(), 12))
      {
        // Load if the model cannot be found
        if (EDITOR->GetGFX()->GetModel(modelList[index]) == nullptr)
          EDITOR->GetGFX()->LoadModel(std::string("./models/" + modelList[index]));

        // Set the new model
        entity->GetComponentAs<Graphics3DComponent>()->SetModel(modelList[index]);

        isChanged = true;
      }

      // Runs the batch file to convert all FBX files in the ModelConverter folder
      if (ImGui::Button("Reload Models"))
        system("..\\ModelConverter\\ConvertAll.bat");
      SetHelpTooltip("Converts and loads all FBX files in the ModelConverter folder.");
    }

    // Position needs to be set after modifying the values so that it updates physics correctly
    else if (info == TYPE_STR("Position"))
    {
      // Gets every value from the data void*
      float* vec = reinterpret_cast<float*>(data);
      vec[0] = entity->GetComponentAs<Core::TransformComponent>()->GetPosition().X;
      vec[1] = entity->GetComponentAs<Core::TransformComponent>()->GetPosition().Y;
      vec[2] = entity->GetComponentAs<Core::TransformComponent>()->GetPosition().Z;

      // Show the values and set the new position in the transform component
      isChanged = ImGui::InputFloat3(mem->GetName(), vec, 2);
      Math::Vector3 newPos = Math::Vector3(vec[0], vec[1], vec[2]);
      entity->GetComponentAs<Core::TransformComponent>()->SetPosition(newPos);
    }

    // Quaternion internals are different than its member variables. The easiest way to se it up
    // is with Pitch, Yaw and Roll. We get those values from the user and create the quaternion
    // from it.
    else if (info == TYPE_STR("Rotation"))
    {
      // Gets current rotation
      Math::Vector3 eulerAngles = entity->GetComponentAs<Core::TransformComponent>()->GetOrientation().GetEuler();
      
      bool eulerChanged = HelperEulerRotation(eulerAngles);
      isChanged = isChanged || eulerChanged;

      // Sets the rotation by generating a new quaternion and passing it to the transform component
      if (eulerChanged)
        entity->GetComponentAs<Core::TransformComponent>()->SetOrientation(Math::Quaternion(eulerAngles.X, eulerAngles.Y, eulerAngles.Z));
    }

    else if (info == TYPE_STR("Quaternion"))
    {
      // Gets current rotation
      Math::Quaternion* quaternion = static_cast<Math::Quaternion*>(data);
      Math::Vector3 eulerAngles = quaternion->GetEuler();

      bool eulerChanged = HelperEulerRotation(eulerAngles);
      isChanged = isChanged || eulerChanged;

      // Sets the rotation by generating a new quaternion and passing it to the transform component
      if (eulerChanged)
        *quaternion = Math::Quaternion(eulerAngles.X, eulerAngles.Y, eulerAngles.Z);
    }

    // Gets color information as RGBA
    else if (info == TYPE_STR("Color"))
    {
      isChanged = ImGui::ColorEdit4(info->GetName(), reinterpret_cast<float*>(data));
    }

    // Used to determine if a certain POD case hasn't been dealt with
    else
    {
      ImGui::Text(mem->GetName());
      ImGui::SameLine();
      ImGui::Text(info->GetName());
    }

    return isChanged;
  }

  // Function that checks if a specific type should be treated as POD, even if it is not inside
  // the reflection system
  bool InspectorModule::IsBaseCase(const Reflex::TypeInfo* info)
  {
    if (info->IsPOD() || info == TYPE_STR("Vector3")  || info == TYPE_STR("Texture") || info == TYPE_STR("Position") ||
                         info == TYPE_STR("Rotation") || info == TYPE_STR("Model3D") || info == TYPE_STR("Color")    ||
                         info == TYPE_STR("Quaternion"))
      return true;

    else
      return false;
  }

  // Add/Remove components
  bool InspectorModule::AddComponent(Core::Entity* entity)
  {
    bool isChanged = false;

    // Creates the combo box and has the logic to add components
    static int index = 0;
    ImGui::Combo("Components", &index, VectorOfStringsItemGettor, &m_OrderedComponentStrings, m_OrderedComponentStrings.size(), 12);
    SetHelpTooltip("Select a component from the list.");

    if (ImGui::Button("Add Component"))
    {
      const Reflex::TypeInfo* info = TYPE_STR(m_OrderedComponentStrings[index].c_str());

      Core::Component* component = nullptr;
      unsigned int size = info->GetSize();
      component = reinterpret_cast<Core::Component*>(g_Memory.Allocate(size));

      Reflex::Variant var(info, component);
      var.PlacementNew();

      std::vector<const Reflex::TypeInfo*> dependencies;
      dependencies = Core::Dependencies()->GetMissingDependencies(entity, component);
      
      entity->CreateComponents(dependencies);
      entity->AddComponent(component);

      //AddComponentBaseCases(entity, component);
      isChanged = true;
    }
    SetHelpTooltip("Adds the selected component into the entity.");

    return isChanged;
  }


  // Handles base cases for adding components
  void InspectorModule::AddComponentBaseCases(Core::Entity* entity, Core::Component* component)
  {
    if (component->GetType() == Core::BoxCollider)
    {
      Core::RigidbodyComponent* rigidBody = entity->GetComponentAs<Core::RigidbodyComponent>();
      //reinterpret_cast<Core::BoxColliderComponent*>(component)->
    }
  }

  // Change entity name
  bool InspectorModule::ChangeName(Core::Entity* entity)
  {
    bool isChanged = false;
    std::strcpy(m_TextBuffer, entity->GetName().c_str());

    if (ImGui::InputText("Change Name", m_TextBuffer, 256, ImGuiInputTextFlags_::ImGuiInputTextFlags_AutoSelectAll    |
                                                           ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue))
    {
      entity->SetName(m_TextBuffer);
      isChanged = true;
    }
    SetHelpTooltip("Change the name of the entity.");

    return isChanged;
  }

  // Saves the current entity as an archetype
  void InspectorModule::SaveToArchetype(Core::Entity* entity)
  {
    static bool isSaveArchetypeActive = false;

    // Button to prompt the window to save the archetype
    if (ImGui::Button("Save to Archetype"))
    {
      isSaveArchetypeActive = true;
    }
    SetHelpTooltip("Saves the current entity as an archetype.\n"
                   "Promts to input a name.\n");

    // Don't draw the window if not active
    if (!isSaveArchetypeActive)
      return;

    // New window to put the name of the new archetype
    ImGui::Begin("Save Archetype", &isSaveArchetypeActive, ImVec2(0, 0), EDITOR->GetFlags().fillAlpha, EDITOR->GetLayoutFlags());

    static bool archFirstTime = true;

    // Positions the window the first time you run it
    if (archFirstTime)
    {
      EDITOR->SetModulePosition((int)ImGui::GetWindowSize().x, (int)ImGui::GetWindowSize().y, Editor::RefPoint::Center, 0, -((int)(EDITOR->GetGFX()->GetResolution().height / 2.5)));
      archFirstTime = false;
      ImGui::End();
      return;
    }

    // Initialize the buffer
    static char *archName;
    if (archName == nullptr)
    {
      archName = new char[256];
      memset(archName, 0, 256);
    }

    ImGui::InputText("Name", archName, 256, ImGuiInputTextFlags_::ImGuiInputTextFlags_AutoSelectAll);
    SetHelpTooltip("Name of the archetype to be saved.");

    if (ImGui::Button("  Save  "))
    {
      g_Factory.AddArchetypeFromEntity(archName, entity);
      isSaveArchetypeActive = false;
    }
    SetHelpTooltip("Save the archetype with selected name.");

    ImGui::SameLine();

    if (ImGui::Button("  Cancel  "))
    {
      isSaveArchetypeActive = false;
    }
    SetHelpTooltip("Closes this window.");

    ImGui::End();
  }
}
