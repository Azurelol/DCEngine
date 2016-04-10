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
    EditorCreator::EditorCreator() : EditorModule(false)
    {
      Daisy->Connect<Events::ScriptingLibraryPatched>(&EditorCreator::OnScriptingLibraryPatched, this);
    }

    GameObjectPtr EditorCreator::CreateTransform()
    {
      std::vector<std::string> components;
      return Create("Transform", components);
    }

    GameObjectPtr EditorCreator::CreateSprite()
    {
      std::vector<std::string> components;
      components.push_back("Sprite");
      return Create("Sprite", components);
    }

    GameObjectPtr EditorCreator::CreateSpriteText()
    {
      std::vector<std::string> components;
      components.push_back("SpriteText");
      return Create("SpriteText", components);
    }

    GameObjectPtr EditorCreator::CreateParticleSystem()
    {
      std::vector<std::string> components;
      components.push_back("SpriteParticleSystem");
      components.push_back("ParticleEmitter");
      components.push_back("LinearParticleAnimator");
      return Create("ParticleSystem", components);
    }

    /**************************************************************************/
    /*!
    @brief Creates a light on the editor's current level.
    */
    /**************************************************************************/
    GameObjectPtr EditorCreator::CreateLight()
    {
      std::vector<std::string> components;
      components.push_back("Light");
      return Create("Light", components);

    }

    /**************************************************************************/
    /*!
    @brief Creates a camera on the editor's current level.
    */
    /**************************************************************************/
    GameObjectPtr EditorCreator::CreateCamera()
    {
      std::vector<std::string> components;
      components.push_back("Camera");
      return Create("Camera", components);
    }

    /**************************************************************************/
    /*!
    @brief  Creates a GameObject from an Archetype into the current space.
    @param archetypeName The name of the Archetype.
    */
    /**************************************************************************/
    GameObjectPtr EditorCreator::CreateFromArchetype(const std::string & archetypeName)
    {
      auto gameObject = Access().CurrentSpace->CreateObject(Daisy->getSystem<Content>()->getArchetype(archetypeName));
      // Begin as not modified from the archetype
      gameObject->setModifiedFromArchetype(false);
      // Select the object
      Access().Select(gameObject);
      Access().Inspector.Toggle(true);
      Access().MoveToViewportCenter(gameObject);
      // Save the command
      auto command = CommandPtr(new CommandObjectCreation(gameObject, Access().CurrentSpace,
        CommandObjectCreation::Setting::Create));
      Access().Add(command);

      return gameObject;
    }

    GameObjectPtr EditorCreator::CreateEditorCamera()
    {
      return GameObjectPtr();
    }

    /**************************************************************************/
    /*!
    @brief Creates the default LevelSettings object.
    @return A pointer to the GameObject.
    */
    /**************************************************************************/
    GameObjectPtr EditorCreator::CreateLevelSettings()
    {
      std::vector<std::string> components;
      auto levelSettings = Create("LevelSettings", components);
      levelSettings->RemoveComponentByName("Transform");
      levelSettings->setLocked(true);
      levelSettings->setProtected(true);
      return levelSettings;
    }

    /**************************************************************************/
    /*!
    @brief Creates a GameObject on the editor's currently selected space.
    @param name The name of the object
    @param components A vector of the components the object requires.
    */
    /**************************************************************************/
    GameObjectPtr EditorCreator::Create(std::string name, std::vector<std::string>& components)
    {
      // Create the object
      auto object = Access().CurrentSpace->CreateObject();
      object->setObjectName(name);
      // Add the components
      for (auto& componentName : components) {
        object->AddComponentByName(componentName);
      }
      Access().Select(object);
      Access().Inspector.Toggle(true);
      Access().MoveToViewportCenter(object);
      // Save the command
      auto command = CommandPtr(new CommandObjectCreation(object, Access().CurrentSpace,
        CommandObjectCreation::Setting::Create));
      Access().Add(command);
      DCTrace << "EditorCreator::Create - Created '" << name << "'\n";

      return object;
    }

    void EditorCreator::OnEditorRebuildZilchComponents(Events::EditorRebuildZilchComponents * event)
    {
      RebuildAllObjectsOnSpace();
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
    }

    /**************************************************************************/
    /*!
    @brief Rebuilds all the GameObjects on the current space
    */
    /**************************************************************************/
    void EditorCreator::RebuildAllObjectsOnSpace()
    {
      // For every GameObject in the current space..
      for (auto& gameObject : *Access().CurrentSpace->AllObjects()) {
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