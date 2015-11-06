#include "Editor.h"
#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {
    
    void Editor::Serialize(Json::Value & root)
    {
    }
    void Editor::Deserialize(Json::Value & root)
    {
    }

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
      // Set it's current space to work on
      CurrentSpace = Daisy->getGameSession()->getDefaultSpace();

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

    /**************************************************************************/
    /*!
    @brief  Receives a MouseDown event.
    @param  event The MouseDown event.
    */
    /**************************************************************************/
    void Editor::OnMouseDownEvent(Events::MouseDown* event)
    {
      if (!EditorEnabled)
        return;

      // Look for an object that matches the translation
      auto posOnSpace = CurrentSpace->getComponent<CameraViewport>()->ScreenToViewport(event->Position);
      SelectObjectFromSpace(posOnSpace);

      //trace << "Editor::OnMouseDownEvent - \n";
    }

    /**************************************************************************/
    /*!
    @brief  Receives a MouseUp event.
    @param  event The MouseUp event.
    */
    /**************************************************************************/
    void Editor::OnMouseUpEvent(Events::MouseUp* event)
    {
      if (!EditorEnabled)
        return;
      //trace << "Editor::OnMouseUpEvent - \n";
    }

    /**************************************************************************/
    /*!
    @brief  Checks if there's an object at the current position in the space.
    @param  pos The position of the mouse relative to world space.
    @todo   Not have the camera's current direction vector hardcoded?
            Find the camera's forward through an orientation component.
            Use std::sort rather than some hacky algorithm.
    */
    /**************************************************************************/
    void Editor::SelectObjectFromSpace(Vec2 pos)
    {   
      // 1. Find all objects on the current mouse position
      auto objsAtPos = Daisy->getSystem<Physics>()->FindAllObjectsAtPosition(Vec3(pos, 0), *CurrentSpace);  
      if (objsAtPos.empty())
        return;

      // 2.1 Find the camera's position in space.
      auto camPos = CurrentSpace->getComponent<CameraViewport>()->getCamera()->TransformComponent->Translation;
      // 2.2 Find the camera's forward direction vector.
      auto camDir = Vec3(0, 0, -1);

      // 3. Sort them in the order of the ones closest to the front of the camera.
      GameObject* closestObj = objsAtPos.front().get();
      for (auto obj : objsAtPos) {
        // 3.1 Get the Z-pos of the current object in the container
        auto objName = obj->Name();
        auto closestObjName = closestObj->Name();

        auto objZ = obj->getComponent<Transform>()->Translation.z;
        auto closestZ = closestObj->getComponent<Transform>()->Translation.z;

        if (objZ < camPos.z && objZ > closestZ) {
          closestObj = obj.get();
        }          
      }

      // 4. Pick the object closest to the camera.
      SelectObject(closestObj);
    }

    void Editor::SelectObject(GameObject* obj)
    {
      trace << "Editor::SelectObject - " << obj->Name() << "\n";
      WidgetPropertiesEnabled = true;
      SelectedObject = obj;
    }

  }
}