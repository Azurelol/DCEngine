#include "EditorCreator.h"

#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {

    EditorCreator::EditorCreator(Editor & editor) : EditorRef(editor)
    {

    }

    /**************************************************************************/
    /*!
    @brief Creates a light on the editor's current level.
    */
    /**************************************************************************/
    void EditorCreator::CreateLight()
    {
      // Create the empty transform
      //auto transform = EditorRef.CurrentSpace->CreateObject();
      //transform->setObjectName("Transform");
      ////transform->setArchetype = "Transform";
      //DCTrace << "Editor::CreateTransform - Created 'Transform' \n";
      //EditorRef.Select(transform);
      //Windows.PropertiesEnabled = true;
      //MoveToViewportCenter(transform);

      //// Save the command
      //auto command = CommandPtr(new CommandObjectCreation(transform, CurrentSpace,
      //  CommandObjectCreation::Setting::Create));
      //EditorRef.Add(command);
    }

  }
}