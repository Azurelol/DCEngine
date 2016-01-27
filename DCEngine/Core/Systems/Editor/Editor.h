/******************************************************************************/
/*!
@file   Editor.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   10/24/2015
@brief  The editor allows the real-time editing of gameobjects in levels ,
        then doing serialization of the levels.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "../System.h"

#include "../../Engine/Data.h"
#include "../../Engine/Command.h"

#include "../../Objects/Object.h"
#include "../../Objects/ObjectsInclude.h"
#include "../../Objects/Entities/EntitiesInclude.h"


namespace DCEngine {

  /* Forward declarations*/
  class Engine;
  class Space;

  /**************************************************************************/
  /*!
  @struct Configuration data for the Editor system.
  */
  /**************************************************************************/
  struct EditorConfig {
    // Project
    bool EditorEnabled = false;
    std::string ProjectsPath;
    std::string RecentProject;
    ProjectData* ProjectInfo;
    // Tools
    bool TransformTool_IsComponent = false;
    bool GridActive = true;
    Real GridLength = 1.0f;
    Vec4 GridColor = Vec4(0.5f, 0.5f, 0.5f, 0.1f);
    // Snapping
    bool Snapping = true;
    float SnapDistance = 1.0;
    float SnapAngle = 15;
    // Dragging
    bool Dragging = false;
    bool DraggingX = false;
    bool DraggingY = false;
    float DragOffset = 0;
    // Rotating
    bool Rotating = false;
    // Scaling
    bool ScalingY = false;
    bool ScalingX = false;
    Vec2 OriginMousePos;
    Vec3 OriginScale;
    //Panning
    bool Panning = false;
    Vec3 CamStartPos;
    Vec3 MouseStartPos;
    // Commands
    CommandManager Commands;
  };

  namespace Systems {

    class Editor : public System {
      friend class Engine;

    public:

      enum class EditorTool {
        None,
        Select,
        Translate,
        Rotate,
        Scale,

      };

      bool IsEnabled();
      void ToggleEditor();
      void ToggleEditor(bool);
      void ToggleTest();

    private:

      /////////////////
      //  Settings 
      ////////////////
      EditorConfig Settings;
      void setEnabled(bool);
      std::string RecentProject;
      bool ShowTestWindow = false;
      bool WidgetMenuBarEnabled = false;
      bool WidgetLevelEnabled = false;
      bool WindowAddResourceEnabled = false;
      bool WidgetObjectsEnabled = false;
      bool WindowPropertiesEnabled = false;
      bool WidgetLibraryEnabled = false;
      bool WidgetDiagnosticsEnabled = false;
      bool WindowSaveLevelEnabled = false;
      bool WindowLoadLevelEnabled = false;
      bool WindowConsoleEnabled = false;
      bool WindowToolsEnabled = false;
      bool WindowCreateFromArchetypeEnabled = false;
      bool WindowCollisionTableEditorEnabled = false;
      bool WindowSpriteLayerOrderEditorEnabled = false;
      bool WindowCommandsEnabled = false;
      SystemPtr ReflectionSystem;
      Space* CurrentSpace;
      GameObjectPtr EditorCamera = nullptr;
      GameObjectPtr TransformTool = nullptr;
      Object* SelectedObject = nullptr;
      EditorTool ActiveTool = EditorTool::None;
      Vec2 ViewportResize = Vec2(0.75, 0.75);

      /////////////////
      //  Methods 
      ////////////////
      void DisplayEditor();
      // Windows
      void DisplayMainMenuBar();
      void WidgetLevel();
      void WidgetResourceAdd();
      // Objects
      void WindowObjects();
      void ObjectsListSwapPosition(GameObjectPtr, Direction);
      // Tools
      void WindowTools();
      // Properties
      void WindowProperties();
      bool DisplayProperties(ObjectPtr);
      void DisplayEntityProperties();
      void DisplayResourceProperties();
      bool AddComponent(EntityPtr);
      // Library
      void WindowLibrary();
      //template <typename ResourceMap> void DisplayResourceTree(std::string resourceName, ResourceMap resourceMap, Delegate* function);
      void WidgetDiagnostics();
      void WindowSaveLevel();
      void WindowLoadLevel();
      void WindowConsole();
      void WindowCreateFromArchetype();
      // Archetypes
      void SaveArchetype(std::string&);
      void RevertToArchetype();
      // Project
      void NewProject();
      void ArchiveProject();
      void OpenProject();
      void OpenRecentProject();
      void LoadProject(std::string& path);
      void SaveProject();
      void SaveCurrentLevel();
      void PlayGame();
      void StopGame();
      void ExportGame();
      void Exit();
      // Select
      void SelectObjectFromSpace(GameObject*);
      GameObject* FindObjectFromSpace(Vec2 pos);
      GameObjectPtr IsSelectableGameObject(ObjectPtr);
      void SelectObject(GameObject* obj);
      void Deselect();
      void SelectSpace();
      void CenterSelected();
      void DragObject(Vec2);
      void RotateObject(Vec2, Vec3);
      void ScaleObject(Vec2 pos);
      void ReleaseObject();
      // Resources
      void WindowAddResource();
      void ResourceCreate(std::string& name, ResourceType type);
      void ResourceAddFromFile(std::string& name, ResourceType type);
      bool CreateLevel(std::string&);
      ResourcePtr CreateBank(std::string& name, std::string& assetPath);
      ResourcePtr CreateCollisionGroup(std::string& name);
      ResourcePtr CreateCollisionTable(std::string& name);
      ResourcePtr CreatePhysicsMaterial(std::string& name);
      ResourcePtr CreateSpriteSource(std::string& name, std::string& assetPath);
      ResourcePtr CreateSoundCue(std::string& name, std::string& assetPath);
      ResourcePtr CreateZilchScript(std::string& name);
      ResourcePtr CreateFont(std::string& name, std::string& assetPath);
      ResourcePtr CreateSpriteLayer(std::string& name);
      ResourcePtr CreateSpriteLayerOrder(std::string& name);
      bool SelectEnumeration(Zilch::Property*, ObjectPtr, unsigned int&);
      bool SelectResource(Zilch::Property*, ObjectPtr, unsigned int&);
      template <typename ResourceMap>
      bool SelectResource(std::string resourceType, ResourceMap* map, Zilch::Property * resource, ObjectPtr component, unsigned int propertyID);

      void WindowCollisionTableEditor();
      void WindowSpriteLayerOrderEditor();
      ResourcePtr SelectedCollisionTable;
      SpriteLayerOrderPtr SelectedSpriteLayerOrder;
      bool LoadLevel(std::string level);
      bool SaveLevel(std::string level);
      bool ReloadLevel();
      // Object Selection
      void WindowCommands();
      void Undo();
      void Redo();
      void Cut();
      void Copy();
      void Paste();
      void Duplicate();
      void DeleteObject();
      void DeleteResource(ResourcePtr);
      void DuplicateObject();
      // Tools
      void UseTool();
      void ShowSelection();
      void TranslateTool();
      void RotateTool();
      void ScaleTool();
      // Actions
      void MoveObject(Vec3);
      void ScaleObject(Vec3);
      // Window, Input
      void ApplyEditorWindowLayout();
      void SetEditorCamera(bool);
      void Hotkeys(Events::KeyDown* event);
      void UpdateCaption();
      void PanCamera(Vec2);
      void DrawGrid();
      // Create
      void CreateTransform();
      void CreateSprite();
      void CreateSpriteText();
      void CreateParticleSystem();
      void CreateFromArchetype(std::string&);
      void MoveToViewportCenter(GameObject* gameobject);
      // Processes      
      void LaunchProjectFolder();
      void LaunchDocumentation();

      /* Functions */
      Editor(EditorConfig settings);
      void Initialize();
      void Subscribe();
      void Update(float dt);
      void Terminate();
      /* Events */
      void OnEditorEnabledEvent(Events::EditorEnabled* event);
      void OnKeyDownEvent(Events::KeyDown* event);
      void OnMouseDownEvent(Events::MouseDown* event);
      void OnMouseUpEvent(Events::MouseUp* event);
      void OnMouseUpdateEvent(Events::MouseUpdate* event);

    };

    //#include "Editor.tcc"

    /////////////
    // Templates
    /////////////
    //template<typename ResourceMap>
    //inline void Editor::DisplayResourceTree(std::string resourceName, ResourceMap resourceMap, EventDelegate* delegate)
    //{
    //  if (ImGui::TreeNode(resourceName.c_str())) {
    //    for (auto& resource : resourceMap) {
    //      if (ImGui::Selectable(resource.second->Name().c_str())) {
    //        SelectedObject = resource.second.get();
    //        WindowPropertiesEnabled = true;
    //      }
    //      if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
    //        
    //        break;
    //      }
    //    }
    //    ImGui::TreePop();
    //  }
    //}

    template<typename ResourceMap>
    inline bool Editor::SelectResource(std::string resourceType, ResourceMap* map, Zilch::Property * resource, ObjectPtr component, unsigned int propertyID)
    {
      // Get a container of all active resources
      std::vector<const char *> resources;
      auto resourceValue = Reflection::PropertyAsString(resource, component);
      static int currentItem = 0;
      for (auto resource : *map) {
        // Push the name of it into the vector of strings
        resources.push_back(resource.second->Name().c_str());
        if (resource.second->getObjectName() == resourceValue)
          currentItem = resources.size() - 1;
      }

      // If the user selects an item... 
      if (ImGui::Combo("##propertyID", &currentItem, resources.data(), resources.size())) {
        // Set the selected item as the current resource
        auto selectedResource = resources.at(currentItem);
        Zilch::ExceptionReport report;
        Zilch::Call setCall(resource->Set, Daisy->getSystem<Reflection>()->Handler()->getState());
        setCall.SetHandleVirtual(Zilch::Call::This, component);
        setCall.Set(0, Zilch::String(selectedResource));
        setCall.Invoke(report);
        // Property was modified..
        return true;
      }
      // Property was not modified
      return false;
    }

  }


}