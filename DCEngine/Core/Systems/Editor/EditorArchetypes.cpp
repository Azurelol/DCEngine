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
    EditorArchetypes::EditorArchetypes(Editor & editor) : EditorModule(editor, false), CurrentArchetype(nullptr),
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
      // Instantiate the archetype
      CurrentArchetype = ArchetypeSpace->CreateObject(archetype);
	    EditorRef.Select(CurrentArchetype);      
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
    void EditorArchetypes::UploadArchetype(ArchetypeHandle archetype)
    {
      // Get the current project's archetype path
      auto path = EditorRef.Settings.ProjectProperties->ProjectPath + EditorRef.Settings.ProjectProperties->ResourcePath
                   + archetype + Archetype::Extension();

      // Create the archetype
      auto archetypePtr = Daisy->getSystem<Factory>()->BuildArchetype(path, dynamic_cast<GameObjectPtr>(EditorRef.SelectedObject()));
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

    GameObject* EditorArchetypes::Current()
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
    @brief Updates all instances of the selected Archetype on the project.
    @param archetypeName The instance of the Archetype.
    /**************************************************************************/
    void EditorArchetypes::UpdateArchetypeInstances(ArchetypeHandle archetypeName)
    {
      // For every level in the project...
      for (auto& gameObject : *EditorRef.CurrentSpace->AllObjects()) {
        // If the GameObject is of the same archetype
        if (gameObject->getArchetype() == archetypeName)
          RevertToArchetype(gameObject);
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