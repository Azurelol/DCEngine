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

      /* Widget Enable Booleans */
      bool EditorEnabled = false;
      bool ShowTestWindow = false;
      bool WidgetMenuBarEnabled = false;
      bool WidgetLevelEnabled = false;
      bool WidgetResourceAddEnabled = false;
      bool WidgetObjectsEnabled = false;
      bool WidgetPropertiesEnabled = false;
      bool WidgetLibraryEnabled = false;
      /* Reflection System */
      SystemPtr ReflectionSystem;
      /* Space */
      Space* CurrentSpace;
      GameObject* SelectedObject = nullptr;
      
      /* Window Settings */
      Vec2 ViewportResize = Vec2(0.75, 0.75);

      void DisplayEditor();
      /* Widgets Functions */
      void DisplayMainMenuBar();
      void WidgetLevel();
      void WidgetResourceAdd();
      void WidgetObjects();
      void WidgetProperties();
      void Editor::DisplayProperties(ComponentPtr component);
      void WidgetLibrary();

      /* Serialization functions*/
      //void SaveLevel();
      //void ReloadLevel();
            
      /* Test functions */
      void DisplayGUITest();

      /* Functions */
      Editor();
      void Initialize();
      void Subscribe();
      void Update(float dt); //!< Delete all objects in the to-be-deleted list
      void Terminate();
      /* Events */
      void OnEditorEnabledEvent(Events::EditorEnabled* event);
      void OnMouseDownEvent(Events::MouseDown* event);
      void OnMouseUpEvent(Events::MouseUp* event);

      // Modification of the rendering window
      void ApplyEditorWindowLayout();

      void SelectObjectFromSpace(Vec2 pos);
      void SelectObject(GameObject* obj);
      

    };

  }


}