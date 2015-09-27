#include "Graphics.h"
#include "../../Components/CameraViewport.h"

namespace DCEngine {
  namespace Systems {

    Graphics::Graphics() : System(std::string("GraphicsSystem"), EnumeratedSystem::Graphics) {    
      trace << "*Using OpenGL for Graphics \n";
      GraphicsHandler.reset(new GraphicsGL());
      GraphicsHandler->screenwidth_ = screenwidth_;
      GraphicsHandler->screenheight_ = screenheight_;
      GraphicsHandler->ClearColor = ClearColor;
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
        // Get the default camera from the viewport component
        auto camera = gfxSpace->Owner()->getComponent<CameraViewport>()->getCamera();
        // Set the sprite shader's projection/view matrix uniform once per frame
        GraphicsHandler->SetShaderProjectionUniform(*camera);
        // Draw every sprite
        for (auto gameObj : gfxSpace->getSprites()) {
         DrawSprite(*gameObj, *camera);
        }

        
        // Set the debugdraw shader's projection/view matrix uniform once per frame

        // Draw every debug object

        
        // 2. Render all models. Load the model shader.

        // 3. Render al particles. Load different shader.
      }

    }

    void Graphics::Register(GraphicsSpace& graphicsSpace) {
      graphicsSpaces_.push_back(&graphicsSpace);
      trace << "Graphics::Register -  " << graphicsSpace.Owner()->Name()
        << " has registered to the Graphics system\n";
    }

    void Graphics::DrawSprite(GameObject & gameObj, Camera& cam) {
      if (TRACE_UPDATE)
        trace << "Graphics::DrawSprite - Drawing " << gameObj.Name() << "\n";
      GraphicsHandler->DrawSprite(gameObj, cam);
    }

    void Graphics::DrawDebug(DebugDrawObject & debugDraw)
    {
    }

    ///////////////
    // DEBUG DRAW
    ///////////////

    void Graphics::DrawCircle(Real3 pos, Real radius, Real4 color, Camera& cam)
    {
      GraphicsHandler->DrawCircle(pos, radius, color, cam);
    }

    void Graphics::DrawRectangle(Real3 pos, Real width, Real height, Real4 color, Camera& cam)
    {
      GraphicsHandler->DrawRectangle(pos, width, height, color, cam);
    }

    void Graphics::DrawLineSegment(Real3 startPos, Real3 endPos, Real length, Real4 color, Camera& cam)
    {
      GraphicsHandler->DrawLineSegment(startPos, endPos, length, color, cam);
    }

    /////////////////
    // FRAME UPDATE
    /////////////////

    void Graphics::Terminate() {
      trace << "Graphics::Terminate \n";
      GraphicsHandler->Terminate();
    }

    void Graphics::StartFrame() {
      GraphicsHandler->StartFrame();
    }

    void Graphics::EndFrame() {
      GraphicsHandler->EndFrame();
    }


  }


}


