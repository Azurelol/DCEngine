#include "GUI.h"
#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {
    
    GUI::GUI() : System(std::string("GUI"), EnumeratedSystem::GUI)
    {
      GUIHandler.reset(new ImGuiSFML());
    }

    void GUI::Initialize()
    {
      if (!GUI_ENABLED)
        return;

      if (TRACE_INITIALIZE)
        trace << "GUI::Initialize \n";
     GUIHandler->Initialize();
    }

    void GUI::StartFrame()
    {
      if (!GUI_ENABLED)
        return;

      if (TRACE_UPDATE)
        trace << "GUI::StartFrame \n";
      GUIHandler->StartFrame();
    }

    void GUI::Render()
    {
      if (!GUI_ENABLED)
        return;

      GUIHandler->Render();
    }

    void GUI::Update(float dt)
    {
    }

    void GUI::Terminate()
    {
      if (!GUI_ENABLED)
        return;

      trace << "GUI::Terminate \n";
      GUIHandler->Terminate();
    }

    void GUI::Serialize(Json::Value & root)
    {
    }

    void GUI::Deserialize(Json::Value & root)
    {
    }



  }
}