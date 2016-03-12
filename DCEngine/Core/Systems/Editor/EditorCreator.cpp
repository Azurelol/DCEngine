#include "EditorCreator.h"

#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    @brief EditorCreator constructor.
    @param editor A reference to the Editor system.
    */
    /**************************************************************************/
    EditorCreator::EditorCreator(Editor & editor) : EditorModule(editor, false)
    {
      Daisy->Connect<Events::ScriptingLibraryPatched>(&EditorCreator::OnScriptingLibraryPatched, this);
    }

    void EditorCreator::CreateTransform()
    {
      std::vector<std::string> components;
      Create("Transform", components);
    }

    void EditorCreator::CreateSprite()
    {
      std::vector<std::string> components;
      components.push_back("Sprite");
      Create("Sprite", components);
    }

    void EditorCreator::CreateSpriteText()
    {
      std::vector<std::string> components;
      components.push_back("SpriteText");
      Create("SpriteText", components);
    }

    void EditorCreator::CreateParticleSystem()
    {
      std::vector<std::string> components;
      components.push_back("SpriteParticleSystem");
      components.push_back("ParticleEmitter");
      components.push_back("LinearParticleAnimator");
      Create("Sprite", components);
    }

    /**************************************************************************/
    /*!
    @brief Creates a light on the editor's current level.
    */
    /**************************************************************************/
    void EditorCreator::CreateLight()
    {
      std::vector<std::string> components;
      components.push_back("Light");
      Create("Light", components);

    }

    /**************************************************************************/
    /*!
    @brief Creates a camera on the editor's current level.
    */
    /**************************************************************************/
    void EditorCreator::CreateCamera()
    {
      std::vector<std::string> components;
      components.push_back("Camera");
      Create("Camera", components);
    }

    /**************************************************************************/
    /*!
    @brief  Creates a GameObject from an Archetype into the current space.
    @param archetypeName The name of the Archetype.
    */
    /**************************************************************************/
    void EditorCreator::CreateFromArchetype(const std::string & archetypeName)
    {
      auto gameObject = EditorRef.CurrentSpace->CreateObject(Daisy->getSystem<Content>()->getArchetype(archetypeName));
      EditorRef.Select(gameObject);
      EditorRef.Inspector.Toggle(true);
      EditorRef.MoveToViewportCenter(gameObject);
      // Save the command
      auto command = CommandPtr(new CommandObjectCreation(gameObject, EditorRef.CurrentSpace,
        CommandObjectCreation::Setting::Create));
      EditorRef.Add(command);
    }

    /**************************************************************************/
    /*!
    @brief Creates a GameObject on the editor's currently selected space.
    @param name The name of the object
    @param components A vector of the components the object requires.
    */
    /**************************************************************************/
    void EditorCreator::Create(std::string name, std::vector<std::string>& components)
    {
      // Create the object
      auto object = EditorRef.CurrentSpace->CreateObject();
      object->setObjectName(name);
      // Add the components
      for (auto& componentName : components) {
        object->AddComponentByName(componentName);
      }
      EditorRef.Select(object);
      EditorRef.Inspector.Toggle(true);
      EditorRef.MoveToViewportCenter(object);
      // Save the command
      auto command = CommandPtr(new CommandObjectCreation(object, EditorRef.CurrentSpace,
        CommandObjectCreation::Setting::Create));
      EditorRef.Add(command);
      DCTrace << "EditorCreator::Create - Created '" << name << "'\n";
    }

    /**************************************************************************/
    /*!
    @brief Whenever the scripting library has been patched, reconstructs
           all entities that have zilch components.
    @todo  This could be optimized by having the entity flagged as having
           script components.
    */
    /**************************************************************************/
    void EditorCreator::OnScriptingLibraryPatched(Events::ScriptingLibraryPatched * event)
    {
      /*
      bool patching = true;
      if (patching)
        return;
*/
      // For every GameObject in the current space..
      for (auto& gameObject : *EditorRef.CurrentSpace->AllObjects()) {
        // Check if it has a Zilch component
        for (auto& component : gameObject->AllComponents()) {
          // If it's a zilch component, reconstruct this gameobject and look to the next one..
          if (ZilchComponent::IsZilchComponent(component)) {
            gameObject->Rebuild();
            break;
          }
        }
      }

    }
    
  }
}