/******************************************************************************/
/*!
@file   EditorArchetypes.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   2/20/2016
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once

#include "../../Resources/Archetype.h"

namespace DCEngine {

  class GameObject;
  class Space;

  namespace Systems {

    class Editor;
    class EditorArchetypes {
    public:
      EditorArchetypes(Editor& editor);
      ~EditorArchetypes();
      void Select(ArchetypeHandle archetype);
      void Deselect();
      void Preview();

    private:
      Editor& EditorRef;
      GameObject* CurrentArchetype;      
      Space* ArchetypeSpace;


    };

  }
}