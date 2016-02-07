#include "EditorTool.h"

// Engine
#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {

    //EditorTool::EditorTool()
    EditorTool::EditorTool(Systems::Editor & editor) : Editor(editor)
    {
    }

    ObjectPtr EditorTool::SelectedObject()

    {
      return Editor.SelectedObject();
    }


    /**************************************************************************/
    /*!
    @brief  Checks if the object is a valid selectable GameObject. That is,
    one with an active transform component.
    @param  object An Object*
    */
    /**************************************************************************/
    GameObjectPtr EditorTool::IsSelectable(ObjectPtr object)
    {
      if (auto gameObject = dynamic_cast<GameObjectPtr>(object)) {
        if (gameObject->HasComponent(std::string("Transform"))) {
          return gameObject;
        }
      }
      return nullptr;
    }

  }
}