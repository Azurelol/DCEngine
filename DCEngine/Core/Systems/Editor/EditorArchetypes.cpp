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
    EditorArchetypes::EditorArchetypes(Editor & editor) : Module("EditorArchetypes"), 
                                                          EditorRef(editor),  CurrentArchetype(nullptr),
                                                          ArchetypeSpace(nullptr)
    {
      Daisy->Connect<Events::EditorDeselectObject>(&EditorArchetypes::OnEditorDeselectObjectEvent, this);
    }

	/**************************************************************************/
	/*!
	@brief EditorArchetypes destructor.
	*/
	/**************************************************************************/
    EditorArchetypes::~EditorArchetypes()
    {
      if (ArchetypeSpace)      
        ArchetypeSpace->Destroy();
    }

	/**************************************************************************/
	/*!
	@brief Selects an Archetype, instantiating it.
	*/
	/**************************************************************************/
    void EditorArchetypes::Select(ArchetypeHandle archetype)
    {    
      // Instantiate the archetype
      CurrentArchetype = ArchetypeSpace->CreateObject(archetype);
	    EditorRef.Select(CurrentArchetype);      
    }

    void EditorArchetypes::Preview()
    {
      // Previews the currently selected archetype on the a separate viewport/space
    }

    void EditorArchetypes::ConstructSpace()
    {
      ArchetypeSpace = Daisy->getGameSession()->CreateSpace("ArchetypeSpace");
    }

    void EditorArchetypes::OnEditorDeselectObjectEvent(Events::EditorDeselectObject * event)
    {
      Deselect();
      DCTrace << "received event!!! \n";
    }

    void EditorArchetypes::Deselect()
    {
      if (!CurrentArchetype)
        return;

      // Destroy the current archetype object
      CurrentArchetype->Destroy();
    }

  }


}