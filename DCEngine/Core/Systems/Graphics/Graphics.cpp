#include "Graphics.h"

namespace DCEngine {
  namespace Systems {

    Graphics::Graphics() : System(std::string("GraphicsSystem"), EnumeratedSystem::Graphics) {    
    }

    void Graphics::Initialize() {
      if (TRACE_ON && TRACE_INITIALIZE)
      trace << "Graphics::Initialize \n";

    }

    void Graphics::Update(float dt) {
      if (TRACE_UPDATE)
      trace << "Graphics::Update \n";

    }

    void Graphics::Terminate() {
      trace << "Graphics::Terminate \n";
    }

  }


}


