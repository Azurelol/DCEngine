#include "Editor.h"
#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {
    
    Editor::Editor() : System(std::string("EditorSystem"), EnumeratedSystem::Editor)
    {      
    }

    void Editor::Initialize()
    {
      if (TRACE_INITIALIZE)
        trace << "Editor::Initialize \n";
      //GUIHandler->Initialize();
    }

    void Editor::Update(float dt)
    {
      if (!GUI_ENABLED)
        return;
     
      if (TRACE_UPDATE)
        trace << "Editor::Update \n";
      GUITest();
    }

    void Editor::Terminate()
    {
        trace << "Editor::Terminate \n";
      //GUIHandler->Terminate();
    }

    void Editor::GUITest()
    {
      // TEMP IMGUI TEST
      ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
      ImGui::Begin("Another Window", &ShowTest);
      ImGui::BulletText("Hello, hello, hello");
      ImGui::Text("Hello");
      ImGui::End();
    }

    void Editor::Serialize(Json::Value & root)
    {
    }

    void Editor::Deserialize(Json::Value & root)
    {
    }

  }
}