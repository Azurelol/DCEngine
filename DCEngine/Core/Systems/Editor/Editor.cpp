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
      EditorEnabled = !EditorEnabled;
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
      if (EditorEnabled) {
        ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
        ImGui::Begin("Another Window", &EditorEnabled);
        ImGui::Text("Hello");
        ImGui::End();
      }
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

  }
}