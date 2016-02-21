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

    EditorArchetypes::EditorArchetypes(Editor & editor) : EditorRef(editor)
    {
      ArchetypeSpace = Daisy->getGameSession()->CreateSpace("ArchetypeSpace");
    }

    EditorArchetypes::~EditorArchetypes()
    {
      ArchetypeSpace->Destroy();
    }

    void EditorArchetypes::Select(ArchetypeHandle archetype)
    {
      // Instantiate the archetype
      ArchetypeSpace->CreateObject(archetype);
      
    }

    void EditorArchetypes::Preview()
    {
      // Previews the currently selected archetype on the a separate viewport/space
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