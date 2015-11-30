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

namespace DCEngine {
  
  /* Forward declarations*/
  class Engine;  
  class Space;

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

      void ToggleEditor();
      void ToggleEditor(bool);
      void ToggleTest();

    private:

      /////////////////
      //  Settings 
      ////////////////
      // Widgets
      //bool EditorStart = false;
      bool EditorEnabled = false;
      std::string RecentProject;
      EditorConfig Settings;
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
      void WidgetObjects();
      // Tools
      void WindowTools();
      // Properties
      void WindowProperties();
      void DisplayProperties(ObjectPtr);
      void DisplayEntityProperties();
      void DisplayResourceProperties();
      void AddComponent(EntityPtr);
      void SelectResource(Zilch::Property*, ObjectPtr);
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
      void ShowProjectFolder();
      void PlayGame();
      void StopGame();
      void ExportGame();
      void Exit();
      // Select
      void SelectObjectFromSpace(GameObject*);
      GameObject* FindObjectFromSpace(Vec2 pos);
      void SelectObject(GameObject* obj);
      void SelectSpace();
      void DragObject(Vec2);
      void ReleaseObject();
      // Resources
      void WindowAddResource();
      void AddResource(std::string& name, ResourceType type);
      bool CreateLevel(std::string&);
      bool LoadLevel(std::string level);
      bool SaveLevel(std::string level);
      bool ReloadLevel();
      void LoadDollhouse();
      // Object Selection

      void DeleteObject();
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

  }


}