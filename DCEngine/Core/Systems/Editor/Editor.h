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
      void ToggleTest();

    private:

      /////////////////
      //  Settings 
      ////////////////
      // Widgets
      bool EditorStart = false;
      bool EditorEnabled = false;
      bool ShowTestWindow = false;
      bool WidgetMenuBarEnabled = false;
      bool WidgetLevelEnabled = false;
      bool WidgetResourceAddEnabled = false;
      bool WidgetObjectsEnabled = false;
      bool WidgetPropertiesEnabled = false;
      bool WidgetLibraryEnabled = false;
      bool WidgetDiagnosticsEnabled = false;
      bool WindowSaveLevelEnabled = false;
      bool WindowLoadLevelEnabled = false;
      bool WindowConsoleEnabled = false;
      SystemPtr ReflectionSystem;
      Space* CurrentSpace;
      GameObject* SelectedObject = nullptr;      
      EditorTool ActiveTool = EditorTool::None;
      Vec2 ViewportResize = Vec2(0.75, 0.75);
      float SnapDistance = 1.0;
      float SnapAngle = 15; 

      /////////////////
      //  Methods 
      ////////////////
      void DisplayEditor();
      // Windows
      void DisplayMainMenuBar();
      void WidgetLevel();
      void WidgetResourceAdd();
      void WidgetObjects();
      // Properties
      void WindowProperties();
      void DisplayProperties(ComponentPtr);
      void AddComponent();
      void SelectResource(Zilch::Property*, ComponentPtr);

      void WidgetLibrary();
      void WidgetDiagnostics();
      void WindowSaveLevel();
      void WindowLoadLevel();
      void WindowConsole();
      // Project
      void NewProject();
      void ArchiveProject();
      void OpenProject();
      void SaveProject();
      void ShowProjectFolder();
      void PlayGame();
      void StopGame();
      void ExportGame();
      void Exit();
      // Resources
      void AddResource();
      void LoadLevel(std::string level);
      void SaveLevel(std::string level);
      void ReloadLevel();
      void LoadDollhouse();
      // Object Selection
      void SelectObjectFromSpace(Vec2 pos);
      void SelectObject(GameObject* obj);
      void DeleteObject();
      void DuplicateObject();
      // Tools
      void UseTool();
      void SelectTool();
      void TranslateTool();
      void RotateTool();
      void ScaleTool();
      // Actions
      void MoveObject(Vec3);
      // Window, Input
      void ApplyEditorWindowLayout();
      void SetEditorCamera();
      void Hotkeys(Events::KeyDown* event);
      // Create
      void CreateTransform();
      void CreateSprite();

      /* Functions */
      Editor(bool enabled);
      void Initialize();
      void Subscribe();
      void Update(float dt);
      void Terminate();
      /* Events */
      void OnEditorEnabledEvent(Events::EditorEnabled* event);
      void OnKeyDownEvent(Events::KeyDown* event);
      void OnMouseDownEvent(Events::MouseDown* event);
      void OnMouseUpEvent(Events::MouseUp* event);
      /* Test functions */
      void DisplayGUITest();
      void LoadSampleLevel();

    };

  }


}