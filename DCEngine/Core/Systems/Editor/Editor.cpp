#include "Editor.h"
#include <IMGUI\imgui.h>

namespace DCEngine {
  namespace Systems {

    Editor::Editor() : System(std::string("EditorSystem"), EnumeratedSystem::Editor)
    {
      trace << "*Using imgui for Editor GUI \n";
      GUIHandler.reset(new GUI());
    }

    void Editor::Initialize()
    {
      if (TRACE_INITIALIZE)
        trace << "Editor::Initialize \n";
      GUIHandler->Initialize();
    }

    void Editor::Update(float dt)
    {
      if (TRACE_UPDATE)
        trace << "Editor::Update \n";
      GUIHandler->Update(dt);
    }

    void Editor::Terminate()
    {
        trace << "Editor::Terminate \n";
      GUIHandler->Terminate();
    }

    void Editor::Serialize(Json::Value & root)
    {
    }

    void Editor::Deserialize(Json::Value & root)
    {
    }

  }
}