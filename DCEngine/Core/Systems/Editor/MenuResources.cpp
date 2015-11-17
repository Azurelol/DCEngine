/******************************************************************************/
/*!
@file   MenuResources.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/15/2015
@brief  This file includes the implementation for the Editor's resource menu.
*/
/******************************************************************************/
#include "Editor.h"
#include "../../Engine/Engine.h"
#include "../Testing.h" // Dollhouse

namespace DCEngine {
  namespace Systems {

    void Editor::AddResource()
    {
    }

    void Editor::LoadLevel()
    {
    }

    void Editor::SaveLevel()
    {
    }

    void Editor::ReloadLevel()
    {
    }

    void Editor::LoadDollhouse()
    {
      // !!! TESTING: Level loading
      LevelPtr dollhouse = LevelPtr(new DollHouse(*CurrentSpace, CurrentSpace->getGameSession()));
      CurrentSpace->LoadLevel(dollhouse);
    }

  }
}