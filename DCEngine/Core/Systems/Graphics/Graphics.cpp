#include "Graphics.h"
#include "../../Components/CameraViewport.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    \brief Default constructor for the Graphics System.
    \note  It sets several values for the OpenGL interface...
    */
    /**************************************************************************/
    Graphics::Graphics() : System(std::string("GraphicsSystem"), EnumeratedSystem::Graphics) {    
      trace << "*Using OpenGL for Graphics \n";
      GraphicsHandler.reset(new GraphicsGL());
      GraphicsHandler->screenwidth_ = screenwidth_;
      GraphicsHandler->screenheight_ = screenheight_;
      GraphicsHandler->ClearColor = ClearColor;
    }

    /**************************************************************************/
    /*!
    \brief Initializes the Graphics system.
    */
    /**************************************************************************/
    void Graphics::Initialize() {
      if (TRACE_ON && TRACE_INITIALIZE)
      trace << "Graphics::Initialize \n";
      GraphicsHandler->Initialize();
      // Connect to graphics space registration events
    }

    /**************************************************************************/
    /*!
    \brief Updates the graphics system, rendering each graphical object..
    \param The delta time.
    \note  The projection/view uniforms are set once for each shader, 
           while the others change depending on the object.
    */
    /**************************************************************************/
    void Graphics::Update(float dt) {
      if (TRACE_UPDATE)
      trace << "Graphics::Update \n";

      // For every Space with a 'GraphicsSpace' component...
      for (auto gfxSpace : graphicsSpaces_) {
        // Get the default camera from the 'CameraViewport' component
        auto camera = gfxSpace->Owner()->getComponent<CameraViewport>()->getCamera();

        // Render every 'Sprite'
        GraphicsHandler->SetSpriteShader(*camera);
        for (auto gameObj : gfxSpace->getSprites()) {
         DrawSprite(*gameObj, *camera);
        }

        /* IF DRAW SPRITE TEXT IS CALLED, BREAKS T_T */

        // Render every 'SpriteText'
        GraphicsHandler->SetSpriteTextShader(*camera);
        //for (auto spriteText : gfxSpace->getSpriteTextContainer()) {
        //  DrawSpriteText(*spriteText, *camera);
        //}

        // Render every 'DebugDrawObject'

        
        // Render all models. Load the model shader.

        // 3. Render al particles. Load different shader.
      }

    }

    /**************************************************************************/
    /*!
    \brief Registers a space to this graphics system.
    \param A reference to the 'GraphicsSpace' component in the Space.
    \note  
    */
    /**************************************************************************/
    void Graphics::Register(GraphicsSpace& graphicsSpace) {
      graphicsSpaces_.push_back(&graphicsSpace);
      trace << "Graphics::Register -  " << graphicsSpace.Owner()->Name()
        << " has registered to the Graphics system\n";
    }

    /**************************************************************************/
    /*!
    \brief Draws a sprite, by forwarding the data to OpenGL.
    \param A reference to the GameObject.
    \param A reference to the camera object in the Space.
    \note
    */
    /**************************************************************************/
    void Graphics::DrawSprite(Sprite & sprite, Camera& cam) {
      if (TRACE_UPDATE)
        trace << "Graphics::DrawSprite - Drawing " << sprite.Owner()->Name() << "\n";
      GraphicsHandler->DrawSprite(sprite, cam);
    }

    /**************************************************************************/
    /*!
    \brief Draws a 'SpriteText', by forwarding the data to OpenGL.
    \param A reference to the SpriteText component.
    \param A reference to the camera object in the Space.
    \note
    */
    /**************************************************************************/
    void Graphics::DrawSpriteText(SpriteText & st, Camera & cam)
    {
      if (TRACE_UPDATE)
        trace << "Graphics::DrawSpriteText - Drawing " << st.Owner()->Name() << "\n";
      GraphicsHandler->DrawSpriteText(st, cam);
    }

    /**************************************************************************/
    /*!
    \brief Draws a 'DebugDrawObject', by forwarding the data to OpenGL.
    \param A reference to the DebugDrawObject.
    \note
    */
    /**************************************************************************/
    void Graphics::DrawDebug(DebugDrawObject & debugDraw)
    {
    }

    ///////////////
    // DEBUG DRAW
    ///////////////

    void Graphics::DrawCircle(Real3& pos, Real& radius, Real4& color, Camera& cam)
    {
      GraphicsHandler->DrawCircle(pos, radius, color, cam);
    }

    void Graphics::DrawRectangle(Real3& pos, Real& width, Real& height, Real4& color, Camera& cam)
    {
      GraphicsHandler->DrawRectangle(pos, width, height, color, cam);
    }

    void Graphics::DrawLineSegment(Real3& startPos, Real3& endPos, Real& length, Real4& color, Camera& cam)
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


