/******************************************************************************/
/*
  File:    EntityModule.cpp
  Author:  Alejandro Hitti
  Date:    11/21/14
  Brief:   The Entity Module handles the creation of new entities and archetypes.
           It also holds a list of objects currently in the scene.

  All content © 2014-2015 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

// Includes
//------------------------------------------------------------------------------
#include "stdinc.h"

#include "EntityModule.h"
#include "ServiceLocator.h"
#include "Entity.h"
#include "Input.h"

namespace Editor
{
  bool EntityModule::m_IsActive = false;
  bool EntityModule::m_IsVisible = true;
  bool EntityModule::m_IsFirstTime = true;

  // Initializes the Module
  bool EntityModule::EntityModuleInitialize()
  {
    return true;
  }

  // Checks if the module is currently active
  bool EntityModule::EntityModuleIsActive()
  {
    return m_IsActive;
  }

  // Sets the module active flag to the state passed in
  void EntityModule::EntityModuleSetActive(bool state)
  {
    m_IsActive = state;
  }

  // Checks if the module is currently visible
  bool EntityModule::EntityModuleIsVisible()
  {
    return m_IsVisible;
  }

  // Sets the module visible flag to the state passed in
  void EntityModule::EntityModuleSetVisible(bool state)
  {
    m_IsVisible = state;
  }

  // Updates the module
  void EntityModule::EntityModuleUpdate()
  {
    if (!m_IsVisible || !m_IsActive)
      return;

    // Start the Main module
    ImGui::Begin("Entity", &m_IsActive, ImVec2(0,0), EDITOR->GetFlags().fillAlpha, EDITOR->GetLayoutFlags());

    // Positions the window the first time you run it
    if (m_IsFirstTime)
    {
      EDITOR->SetModulePosition((int)ImGui::GetWindowSize().x, (int)ImGui::GetWindowSize().y, Editor::RefPoint::TopLeft, 5, 55);
      m_IsFirstTime = false;
      ImGui::End();
      return;
    }

    if (ImGui::CollapsingHeader("Objects List"))
    {
      static int sortBy = SortBy::Archetype;
      static const char* sortByStrings[] = { "Search...", "Archetypes", "Name" };

      ImGui::Combo("Sort by", &sortBy, sortByStrings, SortBy::NumSorts);
      SetHelpTooltip("Choose between multiple sorting methods.");
      SortObjectsBy((SortBy::SortByEnum)sortBy);
    }

    if (ImGui::CollapsingHeader("Factory"))
    {
      CreateEmptyEntity();
      ImGui::Separator();
      CreateEntityFromArchetype();
      ImGui::Separator();
    }

    // End Main Module
    ImGui::End();
  }

  // Calls the proper sort method
  void EntityModule::SortObjectsBy(SortBy::SortByEnum sortType)
  {
    switch (sortType)
    {
      case Editor::EntityModule::SortBy::Search:
        SearchObjectsList();
        break;

      case Editor::EntityModule::SortBy::Archetype:
        SortByArchetype();
        break;

      case Editor::EntityModule::SortBy::Name:
        SortByName();
        break;

      default:
        break;
    }
  }

  // Organizes the objects list by archetype name
  void EntityModule::SortByArchetype(void)
  {
    ImGui::Separator();

    std::vector<Core::Entity*> entities = g_Factory.GetAllEntities();
    std::vector<std::string> archetypeNames = Utilities::Filesystem::GetFilepathsInDirectory("./archetypes/");

    // For every archetype name
    for (auto names = archetypeNames.begin(); names != archetypeNames.end(); ++names)
    {
      // Flag to see if the tree is empty
      bool isEmpty = true;

      if (ImGui::TreeNode(Utilities::Filesystem::StripPathAndExtension(*names).c_str()))
      {
        // Check all entities
        for (unsigned i = 0; i < entities.size(); ++i)
        {
          // If the name of the archetype matches the element's archetype
          if (Utilities::Filesystem::StripPathAndExtension((*names)) == entities[i]->GetArchetypeName())
          {
            MakeObjectButtons(entities, i);

            isEmpty = false;
          }
        }

        // Show "Empty" if there are no entities with this archetype
        if (isEmpty)
          ImGui::BulletText("Empty");

        ImGui::TreePop();
      }
    }
  }

  // Organizes the objects list alphabetically
  void EntityModule::SortByName(void)
  {
    ImGui::Separator();

    std::vector<Core::Entity*> entities = g_Factory.GetAllEntities();

    // For every archetype name
    for (char letter = 'A'; letter <= 'Z'; ++letter)
    {
      // Flag to see if the tree is empty
      bool isEmpty = true;
      const char temp[2] = { letter, '\0' };

      if (ImGui::TreeNode(temp))
      {
        // Check all entities
        for (unsigned i = 0; i < entities.size(); ++i)
        {
          char upper = (entities[i]->GetName())[0];
          if (upper >= 'a' && upper <= 'z')
            upper -= ('A' - 'a');

          if (letter == upper)
          {
            MakeObjectButtons(entities, i);

            isEmpty = false;
          }
        }

        // Show "Empty" if there are no entities that begin with this letter
        if (isEmpty)
          ImGui::BulletText("Empty");

        ImGui::TreePop();
      }
    }
  }

  // Does object searching within the objects list
  void EntityModule::SearchObjectsList(void)
  {
    // Creates the filter text box
    static ImGuiTextFilter filter;
    filter.Draw("");
    SetHelpTooltip("Search Help:\n"
                   "  \"\"         Display all objects.\n"
                   "  \"xxx\"      Display objects containing \"xxx\".\n"
                   "  \"xxx,yyy\"  Display objects containing \"xxx\" or \"yyy\".\n"
                   "  \"-xxx\"     Hide objects containing \"xxx\".\n");

    ImGui::Separator();

    // Filters the objects
    std::vector<Core::Entity*> entities = g_Factory.GetAllEntities();
    bool isSearchEmpty = true;
    for (unsigned i = 0; i < entities.size(); ++i)
    {
      if (filter.PassFilter(entities[i]->GetName().c_str()))
      {
        MakeObjectButtons(entities, i);
        isSearchEmpty = false;
      }
    }

    if (isSearchEmpty)
      ImGui::Text("No entities match your criteria.");

    ImGui::Separator();
  }

  // Makes the buttons for the objects
  void EntityModule::MakeObjectButtons(std::vector<Core::Entity*>& entities, int i)
  {
    // Padding to the right of the button
    float padding = 120;
    // Height
    float buttonHeight = 12;

    // Used to create unique identifiers with the same label
    ImGui::PushID(i);
    if (ImGui::Button(std::string(entities[i]->GetName()).c_str(), ImVec2(ImGui::GetWindowWidth() - padding, buttonHeight)))
      EDITOR->SetSelectedEntity(entities[i]->GetHandle());
    SetHelpTooltip("Inspect this entity.");

    ImGui::SameLine();

    if (ImGui::Button("F", ImVec2(0, buttonHeight)))
    {
      EDITOR->SetSelectedEntity(entities[i]->GetHandle());
      GraphicsSystem::ScreenManager::CameraID cameraID = EDITOR->GetEditorCameraID();

      if (EDITOR->GetGFX()->GetScreenManager().GetActiveCameraID() == cameraID)
      {
        EZ::Camera& camera = EDITOR->GetGFX()->GetScreenManager().GetActiveCamera();
        Core::TransformComponent* currTransform = g_Factory.GetEntity(  EDITOR->GetSelectedEntity())->GetComponentAs<Core::TransformComponent>();
        Math::Matrix4& camMatrix = EDITOR->GetEditorCameraMatrix();

        EZ::float3 camPos = *(EZ::float3*)(&(currTransform->GetPosition()));
        camPos -= (*(EZ::float3*)(&(camMatrix.GetColumn(3) * 30.0f)));

        camera.SetPosition(camPos);
        camera.LookAt(*(EZ::float3*)(&(currTransform->GetPosition())));
      }
    }
    SetHelpTooltip("Focus the camera on this entity.");

    ImGui::SameLine();

    // X to delete the entity
    if (ImGui::Button("X", ImVec2(0, buttonHeight)))
    {
      g_Factory.DestroyEntity(entities[i]->GetHandle());

      // Invalidate the handle so that the inspector module doesn't break
      EDITOR->SetSelectedEntity(Core::c_InvalidHandle);
    }
    ImGui::PopID();
    SetHelpTooltip("Deletes this entity.");
  }

  // Creates a new empty entity (no components)
  void EntityModule::CreateEmptyEntity(void)
  {
    static bool isNewEntityActive = false;

    // Buttom to prompt the creation of a new entity
    if (ImGui::Button("Create Empty Entity"))
    {
      isNewEntityActive = true;
    }
    SetHelpTooltip("Creates a new empty Entity.\n"
                   "Promts to input a name.\n");

    // Don't draw the name window if not active
    if (!isNewEntityActive)
      return;

    // New window to put the name of the new entity
    ImGui::Begin("Create Entity", &isNewEntityActive, ImVec2(0, 0), EDITOR->GetFlags().fillAlpha, EDITOR->GetLayoutFlags());

    static bool entityFirstTime = true;

    // Positions the window the first time you run it
    if (entityFirstTime)
    {
      EDITOR->SetModulePosition((int)ImGui::GetWindowSize().x, (int)ImGui::GetWindowSize().y, Editor::RefPoint::Center, 0, -((int)(EDITOR->GetGFX()->GetResolution().height / 2.5f)));
      entityFirstTime = false;
      ImGui::End();
      return;
    }

    // Initialize the buffer
    static char* entityName;
    if (entityName == nullptr)
    {
      entityName = new char[256];
      memset(entityName, 0, 256);
    }

    ImGui::InputText("Name", entityName, 256, ImGuiInputTextFlags_::ImGuiInputTextFlags_AutoSelectAll);
    SetHelpTooltip("Name of the new entity.");

    if (ImGui::Button("  Create  "))
    {
      Core::Entity* newEntity = g_Factory.CreateEntity();
      newEntity->SetName(entityName);
      EDITOR->SetSelectedEntity(newEntity->GetHandle());
      isNewEntityActive = false;
    }
    SetHelpTooltip("Create a new entity with the selected name.");

    ImGui::SameLine();

    if (ImGui::Button("  Cancel  "))
    {
      isNewEntityActive = false;
    }
    SetHelpTooltip("Closes this window.");

    ImGui::End();
  }

  // Creates a new entity from an archetype
  void EntityModule::CreateEntityFromArchetype(void)
  {
    std::vector<std::string> archetypeNames = Utilities::Filesystem::GetFilepathsInDirectory("./archetypes/");
    static int index = 0;

    for (unsigned i = 0; i < archetypeNames.size(); ++i)
      archetypeNames[i] = Utilities::Filesystem::StripPathAndExtension(archetypeNames[i]);

    ImGui::Combo("Archetypes", &index, VectorOfStringsItemGettor, &archetypeNames, archetypeNames.size(), 12);

    if (ImGui::Button("Create From Archetype"))
    {
      Core::Entity* newEntity = g_Factory.CreateEntity(archetypeNames[index]);
      EDITOR->SetSelectedEntity(newEntity->GetHandle());
    }
  }
}
