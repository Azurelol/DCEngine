#pragma once

#include "..\..\Core\\Systems\Gamestate\Gamestate.h"
#include "..\..\Core\Systems\System.h"
#include "..\..\Components\ComponentsInclude.h"

namespace DCEngine {
  namespace Gamestates {

    class Sandbox : public Gamestate {
    public:
      // Engine calls Initialize manually, doing most of a constructor's functions
      Sandbox() : Gamestate("Sandbox", EnumeratedGamestate::Sandbox) {}
      ~Sandbox() {}

      void Initialize();
      void Update(float dt);
      void Terminate();

    private:
      



    };

    using SandboxPtr = std::shared_ptr<Sandbox>;

  } // Gamestates
} // DCEngine