#include "GUI.h"
#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {
    
    /**************************************************************************/
    /*!
    @brief  Constructor.
    */
    /**************************************************************************/
    GUI::GUI() : System(std::string("GUI"), EnumeratedSystem::GUI)
    {
      GUIHandler.reset(new ImGuiSFML());
    }

    /**************************************************************************/
    /*!
    @brief  Initializes the GUI system.
    */
    /**************************************************************************/
    void GUI::Initialize()
    {
      //if (!GUI_ENABLED)
      //  return;

      if (TRACE_INITIALIZE)
        trace << "GUI::Initialize \n";
     GUIHandler->Initialize();
    }

    /**************************************************************************/
    /*!
    @brief  Starts the frame for the GUI.
    */
    /**************************************************************************/
    void GUI::StartFrame()
    {
      if (!GUI_ENABLED)
        return;
      if (TRACE_UPDATE)
        trace << "GUI::StartFrame \n";
      GUIHandler->PollEvents();
      GUIHandler->StartFrame();
    }

    /**************************************************************************/
    /*!
    @brief  Calls the rendering functions on ImGui.
    */
    /**************************************************************************/
    void GUI::Render()
    {
      if (!GUI_ENABLED)
        return;

      GUIHandler->Render();
    }

    /**************************************************************************/
    /*!
    @brief  Toggles the GUI on and off.
    */
    /**************************************************************************/
    void GUI::Toggle()
    {
      GUI_ENABLED = !GUI_ENABLED;
    }

    /**************************************************************************/
    /*!
    @brief  Updates the GUI system.
    */
    /**************************************************************************/
    void GUI::Update(float dt)
    {
      
    }

    /**************************************************************************/
    /*!
    @brief  Terminates the GUI system.
    */
    /**************************************************************************/
    void GUI::Terminate()
    {
      if (!GUI_ENABLED)
        return;

      trace << "GUI::Terminate \n";
      GUIHandler->Terminate();
    }

    /**************************************************************************/
    /*!
    @brief  Serialize function.
    */
    /**************************************************************************/
    void GUI::Serialize(Json::Value & root)
    {
    }

    void GUI::Deserialize(Json::Value & root)
    {
    }



  }
}