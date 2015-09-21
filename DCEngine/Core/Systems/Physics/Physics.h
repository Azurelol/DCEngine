#pragma once
#include "../System.h"

namespace DCEngine {
  class Engine;

  namespace Systems {    
    class Physics : public System {
      friend class Engine;
    public:


    private:
      Physics();
      void Initialize();
      void Update(float dt);
      void Terminate();

    };

  }
}