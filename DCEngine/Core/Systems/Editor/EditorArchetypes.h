/******************************************************************************/
/*!
@file   EditorArchetypes.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   3/12/2016
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once

#include "../../Resources/Archetype.h"
#include "EditorModule.h"
// Entities
//#include "../../Objects/Entities/EntitiesInclude.h"

namespace DCEngine {

  class Entity;
  class Space;
  class GameSession;

  using SpacePtr = Space*;
  using GameSessionPtr = GameSession*;

  namespace Systems {

    class Editor;
    class EditorArchetypes : public EditorModule {
    public:
      EditorArchetypes();
      ~EditorArchetypes();
      void Select(ArchetypeHandle archetype);
      void Preview();      
      void UploadArchetype(EntityPtr entity);      
      void RevertToArchetype(EntityPtr entity);
      void RevertSpace(SpacePtr space);
      void RevertGameSession(GameSessionPtr gameSession);
      EntityPtr Current();


    private:
      void Display() {}
      void Deselect();
      void ConstructSpace();
      void UpdateArchetypeInstances(ArchetypeHandle archetypeName);

      void OnEngineInitializedEvent(Events::EngineInitialized* event);
      void OnEditorSelectObjectEvent(Events::EditorSelectObject* event);
      void OnEditorDeselectObjectEvent(Events::EditorDeselectObject* event);

      EntityPtr CurrentArchetype;
      Space* ArchetypeSpace;


    };

  }
}