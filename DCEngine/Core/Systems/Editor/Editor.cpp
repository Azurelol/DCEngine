#include "Editor.h"
#include <IMGUI\imgui.h>

namespace DCEngine {
  namespace Systems {

    Editor::Editor() : System(std::string("EditorSystem"), EnumeratedSystem::Editor)
    {

    }

    void Editor::Initialize()
    {
    }

    void Editor::Update(float dt)
    {
    }

    void Editor::Terminate()
    {
    }

    void Editor::Serialize(Json::Value & root)
    {
    }

    void Editor::Deserialize(Json::Value & root)
    {
    }

  }
}