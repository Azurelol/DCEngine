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
    EditorCreator::EditorCreator(Editor & editor) : EditorRef(editor)
    {
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
    @brief  Creates a GameObject from an Archetype into the current space.
    @param archetypeName The name of the Archetype.
    */
    /**************************************************************************/
    void EditorCreator::CreateFromArchetype(const std::string & archetypeName)
    {
      auto gameObject = EditorRef.CurrentSpace->CreateObject(Daisy->getSystem<Content>()->getArchetype(archetypeName));
      EditorRef.Select(gameObject);
      EditorRef.Windows.PropertiesEnabled = true;
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
      EditorRef.Windows.PropertiesEnabled = true;
      EditorRef.MoveToViewportCenter(object);
      // Save the command
      auto command = CommandPtr(new CommandObjectCreation(object, EditorRef.CurrentSpace,
        CommandObjectCreation::Setting::Create));
      EditorRef.Add(command);
      DCTrace << "EditorCreator::Create - Created '" << name << "'\n";
    }

  }
}