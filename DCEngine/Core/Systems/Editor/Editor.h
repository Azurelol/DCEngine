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
    // Snapping
    bool Snapping = true;
    float SnapDistance = 1.0;
    float SnapAngle = 15;

    bool Dragging = false;
    bool Panning = false;
    Vec3 CamStartPos;
    Vec3 MouseStartPos;
    //Vec2 Mouse
    //Vec2 PositionRecord;

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
      SystemPtr ReflectionSystem;
      Space* CurrentSpace;
      GameObjectPtr EditorCamera;
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
      void DisplayProperties(ObjectPtr);
      void DisplayEntityProperties();
      void DisplayResourceProperties();
      void AddComponent(EntityPtr);
      void SelectResource(Zilch::Property*, ObjectPtr, unsigned int&);
      // Library
      void WidgetLibrary();
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
      void ShowProjectFolder();
      void PlayGame();
      void StopGame();
      void ExportGame();
      void Exit();
      // Select
      void SelectObjectFromSpace(GameObject*);
      GameObject* FindObjectFromSpace(Vec2 pos);
      GameObjectPtr IsSelectableGameObject(ObjectPtr);
      void SelectObject(GameObject* obj);
      void SelectSpace();
      void DragObject(Vec2);
      void ReleaseObject();
      // Resources
      void WindowAddResource();
      void ResourceCreate(std::string& name, ResourceType type);
      void ResourceAddFromFile(std::string& name, ResourceType type);
      bool CreateLevel(std::string&);
      ResourcePtr CreateCollisionGroup(std::string& name);
      ResourcePtr CreateCollisionTable(std::string& name);
      ResourcePtr CreatePhysicsMaterial(std::string& name);
      ResourcePtr CreateSpriteSource(std::string& name, std::string& assetPath);
      ResourcePtr CreateSoundCue(std::string& name, std::string& assetPath);
      ResourcePtr CreateZilchScript(std::string& name);
      template <typename ResourceMap>
      void SelectResource(std::string resourceType, ResourceMap* map, Zilch::Property * resource, ObjectPtr component, unsigned int propertyID);

      bool LoadLevel(std::string level);
      bool SaveLevel(std::string level);
      bool ReloadLevel();
      void LoadDollhouse();
      // Object Selection
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
      // Create
      void CreateTransform();
      void CreateSprite();
      void CreateFromArchetype(std::string&);
      void MoveToViewportCenter(GameObject* gameobject);
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
    template<typename ResourceMap>
    inline void Editor::SelectResource(std::string resourceType, ResourceMap* map, Zilch::Property * resource, ObjectPtr component, unsigned int propertyID)
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
      }
    }

}


}