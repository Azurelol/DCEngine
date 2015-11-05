#include "Editor.h"
#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {
    
    /**************************************************************************/
    /*!
    \brief  Constructor.
    */
    /**************************************************************************/
    Editor::Editor() : System(std::string("EditorSystem"), EnumeratedSystem::Editor)
    {      
    }

    /**************************************************************************/
    /*!
    \brief  Initializes the Editor system.
    */
    /**************************************************************************/
    void Editor::Initialize()
    {
      // Connect to mouse events
      Daisy->Connect<DCEngine::Events::MouseDown>(Daisy->getMouse(), &Editor::OnMouseDownEvent, this);

      if (TRACE_INITIALIZE)
        trace << "Editor::Initialize \n";
      //GUIHandler->Initialize();
    }

    /**************************************************************************/
    /*!
    \brief  Updates the Editor system.
    */
    /**************************************************************************/
    void Editor::Update(float dt)
    {
      // Only enable the editor while the GUI is enabled
      if (!GUI_ENABLED)
        return;
     
      if (TRACE_UPDATE)
        trace << "Editor::Update \n";
      DisplayEditor();
      DisplayGUITest();
      
    }

    /**************************************************************************/
    /*!
    \brief  Toggled the Editor on and off.
    */
    /**************************************************************************/
    void Editor::ToggleEditor()
    {
      // Toggle on the editor
      EditorEnabled = !EditorEnabled;
      // Toggle on the GUI system
      //Daisy->getSystem<GUI>()->Toggle();
      trace << "Editor::ToggleEditor : " << EditorEnabled << "\n";
    }

    /**************************************************************************/
    /*!
    \brief  Toggles the ImGui Test Window on and off.
    */
    /**************************************************************************/
    void Editor::ToggleTest()
    {
      ShowTestWindow = !ShowTestWindow;
      trace << "Editor::ToggleTest : " << ShowTestWindow << "\n";
    }

    void Editor::DisplayEditor()
    {
      if (!EditorEnabled)
        return;

      // Display all known editor widgets
      DisplayMainMenuBar();
      WidgetLevel();
      WidgetObjects();
      WidgetLibrary();
      WidgetProperties();
    }



    /**************************************************************************/
    /*!
    \brief  Allows the user a variety of settings regarding levels, such as
            reloading the current level, loading a new one, or saving the
            current one.
    */
    /**************************************************************************/
    void Editor::WidgetLevel()
    {
    }

    /**************************************************************************/
    /*!
    \brief  Allows the user to add a resource to the current project's library
            of assets.
    */
    /**************************************************************************/
    void Editor::WidgetResourceAdd()
    {
    }

    /**************************************************************************/
    /*!
    \brief  Displays the ImGui Test Window.
    */
    /**************************************************************************/
    void Editor::DisplayGUITest()
    {
      if (ShowTestWindow) {
        ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
        ImGui::ShowTestWindow(&ShowTestWindow);
      }
    }

    /**************************************************************************/
    /*!
    \brief  Terminates the editor system.
    */
    /**************************************************************************/
    void Editor::Terminate()
    {
      trace << "Editor::Terminate \n";
      //GUIHandler->Terminate();
    }

    void Editor::Serialize(Json::Value & root)
    {
    }

    void Editor::Deserialize(Json::Value & root)
    {
    }

    void Editor::OnMouseDownEvent(Events::MouseDown* event)
    {
      if (!EditorEnabled)
        return;

      // Look for an object that matches the translation
      

      trace << "Editor::OnMouseDownEvent - \n";
    }

    void Editor::OnMouseUpEvent(Events::MouseUp* event)
    {
      if (!EditorEnabled)
        return;
      trace << "Editor::OnMouseUpEvent - \n";
    }

  }
}