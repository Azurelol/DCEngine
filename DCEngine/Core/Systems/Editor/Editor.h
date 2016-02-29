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

// Utilities
#include "EditorUtilities.h"
// Modules
#include "EditorModulesInclude.h"
// Tools
//#include "EditorTool.h"
#include "EditorToolsInclude.h"
// Engine
#include "../../Objects/Object.h"
#include "../../Objects/ObjectsInclude.h"
#include "../../Objects/Entities/EntitiesInclude.h"

namespace DCEngine {

  // Forward declarations
  class Engine;
  class Space;

  namespace Systems {

    class Editor : public System {
      friend class Engine;
      // Modules. @toodo I would prefer not to friend all my modules to give them private access.
      friend class EditorCreator;
      friend class EditorArchetypes;
      friend class EditorResources;
      friend class EditorProjects;
      friend class EditorInspector;

    public:

      SelectionData Selection;
      ObjectPtr SelectedObject();      
      ObjectContainer& AllSelectedObjects();
      void Add(EditorModulePtr module);
      void Add(CommandPtr command);
      bool IsEnabled();
      void ToggleEditor();
      void ToggleEditor(bool);
      SpacePtr CurrentSpace;
      static Editor& Access();

    private:

      // Tools
      enum class EditorTools {
        None,
        Translate,
        Rotate,
        Scale,
        Tilemap,
      };

      void SwitchTool(EditorTools tool);
      EditorTools ActiveTool;
      bool IsToolRegion(GameObjectPtr);
           
      //EditorToolPtr ActiveToolHandle;
      //TransformToolPtr TransformToolHandle;
      void DisplayTool();
      void UseTool(GameObjectPtr gameObject, Vec2& position);
      void DrawTranslateTool();
      void DrawRotateTool();
      void DrawScaleTool();
      void MoveObject(const Vec3&);
      void TransformStartDragging();
      void TransformDrag(Vec2&);
      void TransformDragRelease();      
      // Settings
      EditorConfig Settings;
      // Modules
      EditorCreator Creator;
      EditorResources Resources;
      EditorArchetypes Archetypes;
      EditorWindows Windows;
      EditorProjects Projects;
      EditorDiagnostics Diagnostics;
      EditorTextEditor TextEditor;
      EditorInspector Inspector;
      std::vector<EditorModulePtr> ActiveModules;

      TransformToolData Transformation;
      // Objects
      SystemPtr ReflectionSystem;
      GameObjectPtr EditorCamera = nullptr;
      //GameObjectPtr TransformTool = nullptr;      
      // Editor
      void setEnabled(bool);
      void DisplayEditor();
      // Windows
      void DisplayMainMenuBar();
      void WidgetLevel();
      // Objects
      void WindowObjects();
      void ObjectsListSwapPosition(GameObjectPtr, Direction);
      // Tools
      void WindowTools();
      // Library
      void WindowLibrary();
      //template <typename ResourceMap> void DisplayResourceTree(std::string resourceName, ResourceMap resourceMap, Delegate* function);
      void WindowDiagnostics();
      void WindowSaveLevel();
      void WindowLoadLevel();
      void WindowConsole();
      // Archetypes
      void WindowCreateFromArchetype();
      // Project
      void NewProject();
      void ArchiveProject();
      void OpenProject();
      void OpenRecentProject();
      void LoadProject(std::string path);
      void AutoSave();
      void SaveCurrentLevel();
      void PlayGame();
      void StopGame();
      void ExportGame();
      bool LoadLevel(std::string level);
      bool SaveLevel(std::string level);
      bool ReloadLevel();
      void Exit();
      // Select
      ObjectContainer SelectedObjects;
      void DrawSelection();
      void SelectObjectFromSpace(GameObject*);
      GameObject* FindObjectFromSpace(Vec2 pos);
      GameObjectPtr IsSelectableGameObject(ObjectPtr);
      void SelectObject(GameObject* obj);
      void Select(ObjectPtr);
      void SelectionAddOrRemoveToMultiple(GameObjectPtr);
      void Deselect();
      void SelectSpace();
      void CenterSelected();
      void SelectMultiple(Vec2&);
      bool IsSelected(GameObjectPtr);
      void CalculateSelectionBounding();
      void DrawMultiSelect();
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
      void WindowCollisionTableEditor();
      void WindowSpriteLayerOrderEditor();
      ResourcePtr SelectedCollisionTable;
      SpriteLayerOrderPtr SelectedSpriteLayerOrder;

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
      void MoveToViewportCenter(GameObject* gameobject);
      // Window, Input
      void ApplyEditorWindowLayout();
      void SetEditorCamera(bool);
      void Hotkeys(Events::KeyDown* event);
      void UpdateCaption();
      void PanCamera(Vec2&);
      void DrawGrid();
      // Create
      // Processes      
      void LaunchProjectFolder();
      void LaunchDocumentation();
      // CTOR
      void WindowSplashScreen();
      Editor(EditorConfig settings);
      void Initialize();
      void Subscribe();
      void Update(float dt);
      void Terminate();
      // Events
      void OnEditorEnabledEvent(Events::EditorEnabled* event);
      void OnKeyDownEvent(Events::KeyDown* event);
      void OnMouseDownEvent(Events::MouseDown* event);
      void OnMouseUpEvent(Events::MouseUp* event);
      void OnMouseUpdateEvent(Events::MouseUpdate* event);


    };
    
  }
}

#include "Editor.hpp"
