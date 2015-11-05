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

      void DisplayEditor();

      /* Widgets Functions */
      void DisplayMainMenuBar();
      void WidgetLevel();
      void WidgetResourceAdd();
      void WidgetObjects();
      void WidgetProperties();
      void WidgetLibrary();

      /* Serialization functions*/
      //void SaveLevel();
      //void ReloadLevel();
            
      /* Test functions */
      void DisplayGUITest();

      /* Functions */
      Editor();
      void Initialize();
      void Update(float dt); //!< Delete all objects in the to-be-deleted list
      void Terminate();
      virtual void Serialize(Json::Value& root);
      virtual void Deserialize(Json::Value& root);      
      
      /* Widget Enable Booleans */
      bool EditorEnabled = false;
      bool ShowTestWindow = false;
      bool WidgetMenuBarEnabled = false;
      bool WidgetLevelEnabled = false;
      bool WidgetResourceAddEnabled = false;
      bool WidgetObjectsEnabled = false;
      bool WidgetPropertiesEnabled = false;
      bool WidgetLibraryEnabled = false;
      
      Space* CurrentSpace;
      GameObject* SelectedObject = nullptr;

      // Mouse input onto the space
      void OnMouseDownEvent(Events::MouseDown* event);
      void OnMouseUpEvent(Events::MouseUp* event);
      void SelectObjectFromSpace(Vec2 pos);
      

    };

  }


}