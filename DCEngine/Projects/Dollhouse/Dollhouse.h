#pragma once
#include "..\..\Core\Objects\Resources\Level.h"

// Testing
#include "..\..\Tutorials\OpenGL\GLCameraTutorial.h"
#include "..\..\Core\Objects\Entity.h"
//#include "..\..\Core\Objects\Entities\GameObject.h"

namespace DCEngine {

  class Space;
  //class Entity;
  class GameSession;

  class DollHouse : public Level {
  public:
    DollHouse(Space& space, GameSession& gamesession) : Level("Dollhouse"), 
                            space_(&space), gamesession_(&gamesession) {
      // TESTING: A simple entity, "Doll"
      // The space will create an entity
      doll.reset(new GameObject("Doll", *space_, *gamesession_));     
      AddGameObject(doll);      
      // UGLY
      Entity &dollRef = *doll;
      ComponentPtr glTut = ComponentPtr(new Tutorial::GLCameraTutorial(dollRef));
      doll->AddComponent(glTut);
      //doll->AddComponent(ComponentPtr(new Tutorial::GLCameraTutorial()));
    }

  //private:
    GameObjectPtr doll;
    

    // (TEMP) Resources should not contain data of their owners
    Space* space_;
    GameSession* gamesession_;

  };


}