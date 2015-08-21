#pragma once
#include "..\..\Core\Objects\Resources\Level.h"

// Testing
#include "..\..\Tutorials\OpenGL\GLCameraTutorial.h"

namespace DCEngine {
  class DollHouse : public Level {
  public:
    DollHouse() : Level("Dollhouse") { 
      // TESTING: A simple entity, "Doll"
      doll.reset(new Entity("Doll"));
      doll->AddComponent(ComponentPtr(new Tutorial::GLCameraTutorial)); 
      AddEntity(doll);
    }

  //private:
    EntityPtr doll;

  };


}