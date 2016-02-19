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
      // Create the light
      auto light = EditorRef.CurrentSpace->CreateObject();
      light->AddComponentByName("Light");
      light->setObjectName("Light");
      //light->setArchetype = "Transform";
      DCTrace << "Editor::CreateTransform - Created 'Light' \n";
      EditorRef.Select(light);
      EditorRef.Windows.PropertiesEnabled = true;
      EditorRef.MoveToViewportCenter(light);

      // Save the command
      auto command = CommandPtr(new CommandObjectCreation(light, EditorRef.CurrentSpace,
                                CommandObjectCreation::Setting::Create));
      EditorRef.Add(command);
    }

  }
}