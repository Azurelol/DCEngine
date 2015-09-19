#include "Graphics.h"

#include "../../Objects/Components/GraphicsSpace.h" // sprites_, vectors_

namespace DCEngine {
  namespace Systems {

    Graphics::Graphics() : System(std::string("GraphicsSystem"), EnumeratedSystem::Graphics) {    
      trace << "*Using OpenGL for Graphics \n";
      GraphicsHandler.reset(new GraphicsGL());
      GraphicsHandler->screenwidth_ = screenwidth_;
      GraphicsHandler->screenheight_ = screenheight_;
    }

    void Graphics::Initialize() {
      if (TRACE_ON && TRACE_INITIALIZE)
      trace << "Graphics::Initialize \n";
      GraphicsHandler->Initialize();

      // Connect to graphics space registration events
    }

    void Graphics::Update(float dt) {
      if (TRACE_UPDATE)
      trace << "Graphics::Update \n";

      // For every graphics space component
      for (auto gfxSpace : graphicsSpaces_) {
        // Draw sprites
        for (auto gameObj : gfxSpace->getSprites()) {
          // If the sprite is visible
          DrawSprite(*gameObj);
        }          
      }

    }

    void Graphics::Register(GraphicsSpace& graphicsSpace) {
      graphicsSpaces_.push_back(&graphicsSpace);
      trace << "Graphics::Register -  " << graphicsSpace.Owner()->Name()
        << " has registered to the Graphics system\n";

      

    }

    void Graphics::DrawSprite(GameObject & gameObj) {
      if (TRACE_UPDATE)
        trace << "Graphics::DrawSprite - Drawing " << gameObj.Name() << "\n";
      GraphicsHandler->DrawSprite(gameObj);
    }

    void Graphics::Terminate() {
      trace << "Graphics::Terminate \n";
      GraphicsHandler->Terminate();
    }


  }


}


