/******************************************************************************/
/*
  File:    ResourcesModule.cpp
  Author:  Alejandro Hitti
  Date:    11/19/14
  Brief:   The Resources Module handles the inspection of all the components
           inside of the selected object.

  All content © 2014-2015 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

// Includes
//------------------------------------------------------------------------------
#include "stdinc.h"

#include "ResourcesModule.h"
#include "InspectorModule.h"

namespace Editor
{
  bool ResourcesModule::m_IsActive = false;
  bool ResourcesModule::m_IsVisible = true;
  bool ResourcesModule::m_IsFirstTime = true;
  Core::Entity* ResourcesModule::m_PreviewTexture = nullptr;
  Core::Entity* ResourcesModule::m_ArchetypeEdit = nullptr;

  // Initializes the Module
  bool ResourcesModule::ResourcesModuleInitialize()
  {
    // Create the entity that will be used to preview textures
    m_PreviewTexture = g_Memory.Allocate<Core::Entity>();

    m_PreviewTexture->CreateComponent<Core::TransformComponent>();
    m_PreviewTexture->CreateComponent<Graphics2DComponent>();

    m_PreviewTexture->Initialize();

    return true;
  }

  // Checks if the module is currently active
  bool ResourcesModule::ResourcesModuleIsActive()
  {
    return m_IsActive;
  }

  // Sets the module active flag to the state passed in
  void ResourcesModule::ResourcesModuleSetActive(bool state)
  {
    m_IsActive = state;
  }

  // Checks if the module is currently visible
  bool ResourcesModule::ResourcesModuleIsVisible()
  {
    return m_IsVisible;
  }

  // Sets the module visible flag to the state passed in
  void ResourcesModule::ResourcesModuleSetVisible(bool state)
  {
    m_IsVisible = state;
  }

  ResourcesModule::~ResourcesModule(void)
  {
    // Cleanup both unmanaged entities
    if (m_PreviewTexture != nullptr)
    {
      m_PreviewTexture->Uninitialize();
      g_Memory.Free(m_PreviewTexture);
      m_PreviewTexture = nullptr;
    }

    if (m_ArchetypeEdit != nullptr)
    {
      m_ArchetypeEdit->Uninitialize();
      g_Memory.Free(m_ArchetypeEdit);
      m_ArchetypeEdit = nullptr;
    }

  }

  // Updates the module
  void ResourcesModule::ResourcesModuleUpdate()
  {
    if (!m_IsVisible || !m_IsActive)
      return;

    m_PreviewTexture->Update(0.1f);
    if (m_ArchetypeEdit != nullptr)
      m_ArchetypeEdit->Update(0.1f);

    ImGui::Begin("Resources", &m_IsActive);

    // Positions the window the first time you run it
    if (m_IsFirstTime)
    {
      EDITOR->SetModulePosition((int)ImGui::GetWindowSize().x, (int)ImGui::GetWindowSize().y, Editor::RefPoint::TopRight, -5, 5);
      m_IsFirstTime = false;
      ImGui::End();
      return;
    }

    if (ImGui::CollapsingHeader("Models"))
      DisplayModelResources();

    if (ImGui::CollapsingHeader("Textures"))
      DisplayTextureResources();

    if (ImGui::CollapsingHeader("Archetypes"))
      DisplayArchetypeResources();

    ImGui::End();
  }

  void ResourcesModule::DisplayFilterList(std::vector<std::string>& resourceNames, EditFnSig editFn, bool hasPreview)
  {
    ImGui::Text("Search");

    // Creates the filter text box
    static ImGuiTextFilter filter;
    filter.Draw("");
    SetHelpTooltip("Search Help:\n"
                   "  \"\"         Display all objects.\n"
                   "  \"xxx\"      Display objects containing \"xxx\".\n"
                   "  \"xxx,yyy\"  Display objects containing \"xxx\" or \"yyy\".\n"
                   "  \"-xxx\"     Hide objects containing \"xxx\".\n");

    ImGui::Separator();

    ImGui::Columns(3, resourceNames[0].c_str());
    ImGui::SetColumnOffset(1, (ImGui::GetWindowSize().x) - (ImGui::GetWindowSize().x / 2.5f));
    ImGui::SetColumnOffset(2, (ImGui::GetWindowSize().x) - (ImGui::GetWindowSize().x / 4.0f));
    ImGui::Text("Name");
    ImGui::NextColumn();
    ImGui::Text("");
    ImGui::NextColumn();
    ImGui::Text("");
    ImGui::NextColumn();
    ImGui::Separator();

    // Filters the objects
    bool isSearchEmpty = true;
    for (unsigned i = 0; i < resourceNames.size(); ++i)
    {
      // Erases the directory part
      std::string strippedName = Utilities::Filesystem::StripPathAndExtension(resourceNames[i]);

      // Filter names
      if (filter.PassFilter(strippedName.c_str()))
      {
        // Display name
        ImGui::Text(strippedName.c_str());

        // Call the specific Edit Function for the type of resource
        editFn(resourceNames, i);

        isSearchEmpty = false;
      }
    }

    ImGui::Columns(1);

    if (isSearchEmpty)
      ImGui::Text("No resources match your criteria.");

    ImGui::Separator();
  }

  // Edit functions per type of resource
  void ResourcesModule::EditModel(std::vector<std::string>& resources, int index)
  {
    ImGui::NextColumn();

    if (ImGui::Button(std::string("Edit##" + resources[index]).c_str()), ImVec2(), true)
    {
      //ShowConfirmationBox();
    }

    ImGui::NextColumn();

    if (ImGui::Button(std::string("Remove##" + resources[index]).c_str()))
    {
      std::remove(resources[index].c_str());
    }
    SetHelpTooltip("Removes the .model file from the directory.");

    ImGui::NextColumn();
  }

  void ResourcesModule::EditTexture(std::vector<std::string>& resources, int index)
  {
    ImGui::NextColumn();

    if (ImGui::Button(std::string("Edit##" + resources[index]).c_str()))
    {
      std::string path = resources[index];
      
      for (unsigned i = 0; i < resources[index].size(); ++i)
      {
        if (resources[index][i] == '/')
          resources[index][i] = '\\';
      }

      SHELLEXECUTEINFO shellInfo = { 0 };
      shellInfo.cbSize = sizeof(SHELLEXECUTEINFO);
      shellInfo.lpVerb = "open";
      shellInfo.lpFile = resources[index].c_str();
      shellInfo.nShow = SW_SHOW;
      //shellInfo.hProcess = OpenProcess()

      BOOL error = ShellExecuteEx(&shellInfo);
    }

    ImGui::NextColumn();

    Graphics2DComponent* currGraphics2D = m_PreviewTexture->GetComponentAs<Graphics2DComponent>();
    if (ImGui::Button(std::string("Preview##" + resources[index]).c_str(), ImVec2(0, 0), true))
    {
      //std::remove(resources[index].c_str());
      //SetHelpTooltip("Removes the texture file from the directory.");
    }

    if (ImGui::IsHovered())
    {
      EZ::float2 pos(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y);
      pos.Normalize();
      currGraphics2D->SetPosition(EZ::float2(0, 0));
      currGraphics2D->SetScale(EZ::float2(20, 20));
      currGraphics2D->SetTextureName(Utilities::Filesystem::GetFilenameFromPath(resources[index]));
      currGraphics2D->SetVisible(true);
    }
    else
    {
      currGraphics2D->SetVisible(false);
    }

    ImGui::NextColumn();
  }

  void ResourcesModule::EditArchetype(std::vector<std::string>& resources, int index)
  {
    ImGui::NextColumn();

    if (ImGui::Button(std::string("Edit##" + resources[index]).c_str()))
    {
      // Creates a temp entity to inspect the archetype for edit
      m_ArchetypeEdit = g_Memory.Allocate<Core::Entity>();
      m_ArchetypeEdit = g_Factory.CreateEntityUnmanaged(Utilities::Filesystem::StripPathAndExtension(resources[index]));
      m_ArchetypeEdit->Initialize();
      m_ArchetypeEdit->GetComponentAs<Core::TransformComponent>()->SetPosition(Math::Vector3(10000.0f, 10000.0f, 10000.0f));
    }

    ImGui::NextColumn();

    if (ImGui::Button(std::string("Remove##" + resources[index]).c_str()))
    {
      std::remove(resources[index].c_str());
      SetHelpTooltip("Removes the archetype file from the directory.");
    }

    ImGui::NextColumn();
  }

  bool ResourcesModule::IsArchetypeNameUnique(std::string& archName)
  {
    // Get all archetype names
    std::vector<std::string> archetypeNames = Utilities::Filesystem::GetFilepathsInDirectory("./archetypes/");
    bool isNameUnique = true;

    // Checks for name uniqueness
    for (unsigned i = 0; i < archetypeNames.size(); ++i)
    {
      if (archName == Utilities::Filesystem::StripPathAndExtension(archetypeNames[i]))
      {
        isNameUnique = false;
        break;
      }
    }

    return isNameUnique;
  }

  void ResourcesModule::DisplayModelResources(void)
  {
    // Makes a button to add a new model
    if (ImGui::Button("Add Model"))
    {
      std::string modelName = Utilities::Filesystem::OpenFileName("FBX (.fbx)\0*.fbx\0Model (.model)\0*.model\0");
      std::string extension = Utilities::Filesystem::GetFileExtension(modelName);
      std::string newFile = "./models/";
      newFile += Utilities::Filesystem::GetFilenameFromPath(modelName);

      // If it's a model file (own type) then just copy the file
      if (extension == "model")
      {
        CopyFile(modelName.c_str(), newFile.c_str(), true);
      }

      // If it's an fbx, transform it into .model first into the correct folder
      else if (extension == "fbx")
      {
        std::string command = "..\\ModelConverter\\ModelConverter.exe -o .\\models\\ ";
        command += modelName;
        system(command.c_str());
      }

    }
    SetHelpTooltip("Add a .model or .fbx file to the game's resources.");

    // Runs the batch file to convert all FBX files in the ModelConverter folder
    if (ImGui::Button("Reload Models"))
      system("..\\ModelConverter\\ConvertAll.bat");
    SetHelpTooltip("Converts and loads all FBX files in the ModelConverter folder.");

    ImGui::Separator();

    // Gets all model names
    std::vector<std::string> modelList = Utilities::Filesystem::GetFilepathsInDirectory("./models/");

    DisplayFilterList(modelList, EditModel, false);
  }

  void ResourcesModule::DisplayTextureResources(void)
  {
    if (ImGui::Button("Add Texture"))
    {
      std::string textureName = Utilities::Filesystem::OpenFileName("Image File (.png, .jpg)\0*.png;*.jpg\0");
      std::string newFile = "./textures/";
      newFile += Utilities::Filesystem::GetFilenameFromPath(textureName);

      if (textureName != "*InvalidName*")
        CopyFile(textureName.c_str(), newFile.c_str(), true);
    }
    SetHelpTooltip("Add a texture file from your system into the assets folder.");

    ImGui::Separator();

    // Gets all texture names
    std::vector<String> texList = Utilities::Filesystem::GetFilepathsInDirectory("./textures/");

    DisplayFilterList(texList, EditTexture, true);
  }

  void ResourcesModule::DisplayArchetypeResources(void)
  {
    static bool isAddingArchetype = false;
    static bool isFirstTime = true;
    static bool isConfirmationOn = false;

    // TODO(AleHitti): Finish confirmation of destructive action
    //if (isConfirmationOn)
    //{
    //  if (ImGui::Button("Cancel"))
    //    isConfirmationOn = false;

    //  return;
    //}

    if (ImGui::Button("Add Archetype"))
      isAddingArchetype = true;
    SetHelpTooltip("Creates a new archetype.");

    ImGui::Separator();

    static bool isChanged = false;

    // Initialize the buffer
    static char *archName;
    if (archName == nullptr)
    {
      archName = new char[256];
      memset(archName, 0, 256);
    }

    if (isAddingArchetype)
    {
      ImGui::Text("Chose a name: ");
      ImGui::SameLine();
      ImGui::InputText("##ArchetypeName", archName, 256, ImGuiInputTextFlags_AutoSelectAll);

      if (ImGui::Button("Create"))
      {
        // We check here because we don't want to overwrite an existing archetype
        if (IsArchetypeNameUnique(std::string(archName)))
        {
          // Create a dummy entity
          Core::Entity* dummyEntity = g_Factory.CreateEntity();
          // Add a transform component
          dummyEntity->CreateComponent<Core::TransformComponent>();
          // Add the archetype just with the name
          g_Factory.AddArchetypeFromEntity(archName, dummyEntity);
          // Destroy the dummy
          g_Factory.DestroyEntity(dummyEntity->GetHandle());

          memset(archName, 0, 256);
          isAddingArchetype = false;
        }
      }

      ImGui::SameLine();

      // Don't create the new archetype
      if (ImGui::Button("Cancel"))
      {
        memset(archName, 0, 256);
        isAddingArchetype = false;
      }

      // Warning text if the name is not unique
      if (!IsArchetypeNameUnique(std::string(archName)))
        ImGui::TextColored(ImVec4(255, 0, 0, 1), "That name already exists! Pick another.");
    }

    else if (m_ArchetypeEdit == nullptr)
    {
      // Get all archetype names
      std::vector<std::string> archetypeNames = Utilities::Filesystem::GetFilepathsInDirectory("./archetypes/");

      // Shows a table with all the archetypes
      DisplayFilterList(archetypeNames, EditArchetype, false);

      isChanged = false;
    }

    else
    {
      // Counts number of changes detected
      int changedCounter = 0;

      // Inspect the current archetype
      if (InspectorModule::Inspect(m_ArchetypeEdit))
        ++changedCounter;

      if (isFirstTime)
      {
        std::strcpy(archName, m_ArchetypeEdit->GetName().c_str());
        isFirstTime = false;
      }

      if (ImGui::InputText("Change Name", archName, 256, ImGuiInputTextFlags_::ImGuiInputTextFlags_AutoSelectAll))
        ++changedCounter;
      SetHelpTooltip("Change the name of the Archetype.");

      if ((archName != m_ArchetypeEdit->GetName()) && !IsArchetypeNameUnique(std::string(archName)))
        ImGui::TextColored(ImVec4(255, 0, 0, 1), "That name already exists! Pick another.");

      // Allow to add components to the archetype
      if (InspectorModule::AddComponent(m_ArchetypeEdit))
        ++changedCounter;

      ImGui::Separator();

      // Checks if here are any changes
      if (changedCounter > 0)
        isChanged = true;

      // If there are any changes (property, name or more/less components) show a warning text
      if (isChanged)
      {
        ImGui::TextColored(ImVec4(255.0f, 255.0f, 0.0f, 1.0f), "There are unsaved changes!");
      }

      // Save the current archetype
      if (ImGui::Button("Save"))
      {
        // If the name of the archetype changed, we need to remove the previous one and create the new one
        if (archName != m_ArchetypeEdit->GetArchetypeName() && IsArchetypeNameUnique(std::string(archName)))
        {
          std::string oldArchName = "";
          oldArchName += "./archetypes/";
          oldArchName += m_ArchetypeEdit->GetArchetypeName();
          oldArchName += ".arch";
          std::rename(oldArchName.c_str(), archName);
        }

        // Save the current archetype
        if (IsArchetypeNameUnique(std::string(archName)) || (archName == m_ArchetypeEdit->GetName()))
        {
          g_Factory.AddArchetypeFromEntity(archName, m_ArchetypeEdit);

          // Cleanup the temp entity
          m_ArchetypeEdit->Uninitialize();
          g_Memory.Free(m_ArchetypeEdit);
          m_ArchetypeEdit = nullptr;
          isFirstTime = true;
        }
      }
      SetHelpTooltip("Saves the current archetype.");

      ImGui::SameLine();

      if (ImGui::Button("Cancel"))
      {
        // Cleanup the temp entity
        m_ArchetypeEdit->Uninitialize();
        g_Memory.Free(m_ArchetypeEdit);
        m_ArchetypeEdit = nullptr;
        isFirstTime = true;
      }
      SetHelpTooltip("Discard changes.");
    }
  }
}
