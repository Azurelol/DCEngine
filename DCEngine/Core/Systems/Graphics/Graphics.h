#pragma once
#include "../System.h"

// OpenGL rendering
#include "GraphicsGL.h"

namespace DCEngine {
  namespace Systems {

    class Graphics : public System {
    public:
      Graphics();

      void Initialize();
      void Update(float dt);
      void Terminate();

    private:

    };


  }

}