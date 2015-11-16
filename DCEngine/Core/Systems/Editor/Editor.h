/******************************************************************************/
/*!
\file   Editor.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   10/24/2015
\brief  The editor allows the real-time editing of gameobjects in levels ,
        then doing serialization of the levels.
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
      void ToggleEditor();
      void ToggleTest();

    private:

      /////////////////
      //  Settings 
      ////////////////
      // Widgets
      bool EditorEnabled = false;
      bool ShowTestWindow = false;
      bool WidgetMenuBarEnabled = false;
      bool WidgetLevelEnabled = false;
      bool WidgetResourceAddEnabled = false;
      bool WidgetObjectsEnabled = false;
      bool WidgetPropertiesEnabled = false;
      bool WidgetLibraryEnabled = false;
      bool WidgetDiagnosticsEnabled = false;
      SystemPtr ReflectionSystem;
      Space* CurrentSpace;
      GameObject* SelectedObject = nullptr;      
      Vec2 ViewportResize = Vec2(0.75, 0.75);

      /////////////////
      //  Methods 
      ////////////////
      void DisplayEditor();
      // Widgets
      void DisplayMainMenuBar();
      void WidgetLevel();
      void WidgetResourceAdd();
      void WidgetObjects();
      void WidgetProperties();
      void DisplayProperties(ComponentPtr);
      void WidgetLibrary();
      void WidgetDiagnostics();
      // Project
      void NewProject();
      void ArchiveProject();
      void OpenProject();
      void SaveProject();
      void ShowProjectFolder();
      void PlayGame();
      void ExportGame();
      void Exit();
      // Resources
      void AddResource();
      void LoadLevel();
      void SaveLevel();
      void ReloadLevel();
      void LoadDollhouse();
      // Object Selection
      void SelectObjectFromSpace(Vec2 pos);
      void SelectObject(GameObject* obj);
      void DeleteObject();
      void DuplicateObject();
      // Window
      void ApplyEditorWindowLayout();
      /* Serialization functions*/
      //void SaveLevel();
      //void ReloadLevel(); 


      //////////////////
      // CREATE
      /////////////////
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
      void OnMouseDownEvent(Events::MouseDown* event);
      void OnMouseUpEvent(Events::MouseUp* event);
      /* Test functions */
      void DisplayGUITest();
      void LoadSampleLevel();

    };

  }


}