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
      EditorCreator();
      GameObjectPtr CreateTransform();
      GameObjectPtr CreateSprite();
      GameObjectPtr CreateSpriteText();
      GameObjectPtr CreateParticleSystem();
      GameObjectPtr CreateLight();
      GameObjectPtr CreateCamera();
      GameObjectPtr CreateFromArchetype(const std::string& archetypeName);
      GameObjectPtr CreateEditorCamera();
      GameObjectPtr CreateLevelSettings();
      void RebuildAllObjectsOnSpace();

    private:
      GameObjectPtr Create(std::string name, std::vector<std::string>& components);
      void OnEditorRebuildZilchComponents(Events::EditorRebuildZilchComponents* event);
      void OnScriptingLibraryPatched(Events::ScriptingLibraryPatched* event);

    };


  }
}
