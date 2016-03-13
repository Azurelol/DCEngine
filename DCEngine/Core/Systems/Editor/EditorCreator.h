/******************************************************************************/
/*!
@file   EditorCreator.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   1/20/2016
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "EditorModule.h"

namespace DCEngine {
  namespace Systems {

    class Editor;
    class EditorCreator : public EditorModule {
    public:
      EditorCreator(Editor& editor);
      void CreateTransform();
      void CreateSprite();
      void CreateSpriteText();
      void CreateParticleSystem();
      void CreateLight();
      void CreateCamera();      
      void CreateFromArchetype(const std::string& archetypeName);
      void RebuildAllObjectsOnSpace();

    private:
      void Create(std::string name, std::vector<std::string>& components);
      void OnEditorRebuildZilchComponents(Events::EditorRebuildZilchComponents* event);
      void OnScriptingLibraryPatched(Events::ScriptingLibraryPatched* event);

    };


  }
}
