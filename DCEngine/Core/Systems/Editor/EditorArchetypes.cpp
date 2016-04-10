/******************************************************************************/
/*!
@file   EditorArchetypes.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   2/20/2016
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "EditorArchetypes.h"

#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {

  /**************************************************************************/
  /*!
  @brief EditorArchetypes constructor.
  @param editor A reference to the Editor system.
  */
  /**************************************************************************/
    EditorArchetypes::EditorArchetypes() : EditorModule(false), CurrentArchetype(nullptr),
                                                          ArchetypeSpace(nullptr)
    {
      Daisy->Connect<Events::EditorDeselectObject>(&EditorArchetypes::OnEditorDeselectObjectEvent, this);
      Daisy->Connect<Events::EngineInitialized>(&EditorArchetypes::OnEngineInitializedEvent, this);
    }

  /**************************************************************************/
  /*!
  @brief EditorArchetypes destructor.
  */
  /**************************************************************************/
    EditorArchetypes::~EditorArchetypes()
    {
      //if (ArchetypeSpace)      
      //  ArchetypeSpace->Destroy();
    }

  /**************************************************************************/
  /*!
  @brief Selects an Archetype, instantiating it.
  @param archetype The name of the archetype.
  */
  /**************************************************************************/
    void EditorArchetypes::Select(ArchetypeHandle archetype)
    {    
      // Create a dummy object

      // Instantiate the archetype
      CurrentArchetype = ArchetypeSpace->CreateObject(archetype);
      Access().Select(CurrentArchetype);
    }

    /**************************************************************************/
    /*!
    @brief Deselects the currently selected Archetype.
    */
    /**************************************************************************/
    void EditorArchetypes::Deselect()
    {
      if (!CurrentArchetype)
        return;

      // Destroy the current archetype object
      CurrentArchetype->Destroy();
      CurrentArchetype = nullptr;
    }

    /**************************************************************************/
    /*!
    @brief Previews the currenty-selected archetype by switching to its space,
           probably by making it block the other.
    */
    /**************************************************************************/
    void EditorArchetypes::Preview()
    {
      // Previews the currently selected archetype on the a separate viewport/space
    }

    /**************************************************************************/
    /*!
    @brief Uploads the selected Archetype.
    @param archetype The name of the archetype which to upload.
    @note This will overwrite any Archetypes named after it.
    */
    /**************************************************************************/
    void EditorArchetypes::UploadArchetype(EntityPtr entity)
    {
      // Get the entity's archetype
      auto archetype = entity->getArchetype();

      // Get the current project's archetype path
      auto path = Access().Settings.ProjectProperties->ProjectPath + Access().Settings.ProjectProperties->ResourcePath
                   + archetype + Archetype::Extension();

      // Create the archetype
      auto archetypePtr = Daisy->getSystem<Factory>()->BuildArchetype(path, dynamic_cast<GameObjectPtr>(entity));
      // Save it
      archetypePtr->Save();
      // Scan for archetypes again
      Daisy->getSystem<Content>()->ScanForArchetypes();
      // Update all Archetype instances
      UpdateArchetypeInstances(archetype);
    }

    /**************************************************************************/
    /*!
    @brief Reverts the currently selected entity to its Archetype.
    @param entity A pointer to the entity.
    */
    /**************************************************************************/
    void EditorArchetypes::RevertToArchetype(EntityPtr entity)
    {
      // Request the factory system to rebuild the object's components
      // from its archetype.
      Daisy->getSystem<Systems::Factory>()->RebuildFromArchetype(entity);
    }

    /**************************************************************************/
    /*!
    @brief Returns the currently selected archetype.
    @return A pointer to the archetype.
    */
    /**************************************************************************/
    EntityPtr EditorArchetypes::Current()
    {
      return CurrentArchetype;
    }

    /**************************************************************************/
    /*!
    @brief Constructs the Archetype space.
    */
    /**************************************************************************/
    void EditorArchetypes::ConstructSpace()
    {
      ArchetypeSpace = Daisy->getGameSession()->CreateSpace("ArchetypeSpace");
    }



    /**************************************************************************/
    /*!
    @brief Updates all instances of the selected Archetype on the project.
    @param archetypeName The instance of the Archetype.
    /**************************************************************************/
    void EditorArchetypes::UpdateArchetypeInstances(ArchetypeHandle archetypeName)
    {
      // If the current archetype is a GameObject
      if (GameObject::IsA(CurrentArchetype)) {
        // For every level in the project...
        for (auto& gameObject : *Access().CurrentSpace->AllObjects()) {
          // If the GameObject is of the same archetype and has not been modified
          bool modified = gameObject->getModifiedFromArchetype();
          if (gameObject->getArchetype() == archetypeName && !modified) {
            RevertToArchetype(gameObject);
          }
        }
      }

      // If it's a Space
      if (auto space = dynamic_cast<SpacePtr>(CurrentArchetype)) {
        RevertToArchetype(space);
      }

      // If it's the GameSession
      if (auto gameSession = dynamic_cast<GameSession*>(CurrentArchetype)) {
        RevertToArchetype(gameSession);
      }

    }


    void EditorArchetypes::OnEngineInitializedEvent(Events::EngineInitialized * event)
    {
      ConstructSpace();
    }

    void EditorArchetypes::OnEditorDeselectObjectEvent(Events::EditorDeselectObject * event)
    {
      Deselect();
    }


  }


}