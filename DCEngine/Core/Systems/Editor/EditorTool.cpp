#include "EditorTool.h"

// Engine
#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    @brief  EditorTool constructor.
    @param  editor A reference to the editor.
    */
    /**************************************************************************/
    EditorTool::EditorTool(Editor & editor) : EditorRef(editor)
    {
    }

    /**************************************************************************/
    /*!
    @brief  Returns the editor's currently selected object.
    */
    /**************************************************************************/
    ObjectPtr EditorTool::SelectedObject()
    {
      return EditorRef.SelectedObject();
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

    /**************************************************************************/
    /*!
    @brief  Returns a pointer to the current space the editor is working on.
    @param  A pointer to the Space.
    */
    /**************************************************************************/
    SpacePtr EditorTool::CurrentSpace()
    {
      return EditorRef.CurrentSpace;
    }

  }
}