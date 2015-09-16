#pragma once
#include "../System.h"

namespace DCEngine {
  namespace Systems {

    class Physics : public System {
    public:
      Physics();

      void Initialize();
      void Update(float dt);
      void Terminate();

    };

  }

}