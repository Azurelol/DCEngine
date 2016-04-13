/*****************************************************************************/
/*!
\file   Graphics.cpp
\author Chen Shu, Christian Sagel, William Mao
\par    email: c.sagel\@digipen.edu
\date   9/16/2015
\brief  Graphics system of DCEngine.

@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Graphics.h"
#include "../../Components/CameraViewport.h"
#include "../../Engine/Engine.h"
#include "../../Debug/DebugGraphics.h"

namespace DCEngine {
  namespace Systems {

    std::unique_ptr<GraphicsGL> Graphics::GraphicsHandler;

    /**************************************************************************/
    /*!
    \brief Default constructor for the Graphics System.
    \note  It sets several values for the OpenGL interface...
    */
    /**************************************************************************/
    Graphics::Graphics(GraphicsConfig& settings) : System(std::string("GraphicsSystem"), EnumeratedSystem::Graphics), Settings(settings) {
      //Settings.MaxDrawLayers = 5;
      mDrawList.resize(Settings.MaxDrawLayers);
      DCTrace << "*Using OpenGL for Graphics \n";
      GraphicsHandler.reset(new GraphicsGL(Settings));
    }

    /**************************************************************************/
    /*!
    \brief Initializes the Graphics system.
    */
    /**************************************************************************/
    void Graphics::Initialize() {
      if (TRACE_ON && TRACE_INITIALIZE)
        DCTrace << "Graphics::Initialize \n";
      GraphicsHandler->Initialize();
      GraphicsHandler->mDrawList = &mDrawList;
			GraphicsHandler->mDeferredObjects = &mDeferredObjects;
      // Subscribe to events
      Subscribe();
    }

    /**************************************************************************/
    /*!
    \brief Subscribe to events.
    */
    /**************************************************************************/
    void Graphics::Subscribe()
    {
      Daisy->Connect<Events::WindowResize>(&Graphics::OnWindowResizeEvent, this);
      Daisy->Connect<Events::WindowFullScreenEnabled>(&Graphics::OnWindowFullScreenEnabledEvent, this);
      Daisy->Connect<Events::WindowFullScreenDisabled>(&Graphics::OnWindowFullScreenDisabledEvent, this);
      Daisy->Connect<Events::GraphicsCompileShaders>(&Graphics::OnGraphicsCompileShadersEvent, this);
      Daisy->Connect<Events::GraphicsToggleLightning>(&Graphics::OnGraphicsToggleLightningEvent, this);
      Daisy->Connect<Events::WindowRecreate>(&Graphics::OnWindowRecreateEvent, this);
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
        DCTrace << "Graphics::Update \n";
          
      // Start the profiler
      SystemTimer profile(this->Name());

      // For every Space with a 'GraphicsSpace' component...
      for (Components::GraphicsSpace* gfxSpace : ActiveGraphicsSpaces) {

        // Get the default camera from the 'CameraViewport' component
        Components::Camera* camera = gfxSpace->Owner()->getComponent<Components::CameraViewport>()->getCamera();

        // Do not update the space if no camera has been set
        if (camera == nullptr)
          continue;

        std::vector<Components::Graphical*> graphicalComponents = gfxSpace->getGraphicsComponents();
        for (auto graphicalComponent : graphicalComponents) {
          auto drawLayer = graphicalComponent->getDrawLayer(); 
          //DCTrace << "DrawLayer: " << drawLayer << "\n";
          mDrawList[drawLayer].push_back(graphicalComponent);
        }

        std::vector<Components::Light*> lightComponents;
        if (Settings.LightningEnabled)
          lightComponents = gfxSpace->getLightComponents();

        lightComponents.erase(std::remove_if(lightComponents.begin(), lightComponents.end(), 
          [] (Components::Light* light) { return !light->getVisible(); }), lightComponents.end());

        UpdateObjects(dt);

        GraphicsHandler->PreRender(camera);

        if (!lightComponents.empty())
        {
          for (const auto& light : lightComponents)
          {
            if (light->getCastShadows())
              GraphicsHandler->RenderShadows(camera, light);
            GraphicsHandler->RenderLights(light);
          }
          GraphicsHandler->RenderScene(camera->getExposure(), true);
        }
        else
        {
          GraphicsHandler->RenderLights(0);
          GraphicsHandler->RenderScene(camera->getExposure(), false);
        }

        DrawDebug(camera);

        for (auto&& drawList : mDrawList)
          drawList.clear();

        GraphicsHandler->ClearFrameBufferObjects();
      }
      GraphicsHandler->FinalRender();
			if(!mDeferredObjects.empty())
				GraphicsHandler->ScreenSpaceRectangle();
			mDeferredObjects.clear();
      // Load any graphical assets
      LoadGraphicalResources();

    }


    /**************************************************************************/
    /*!
    @brief Registers a space to this graphics system.
    @param A reference to the 'GraphicsSpace' component in the Space.
    */
    /**************************************************************************/
    void Graphics::Register(Components::GraphicsSpace& graphicsSpace) {
      ActiveGraphicsSpaces.push_back(&graphicsSpace);
      DCTrace << "Graphics::Register -  " << graphicsSpace.Owner()->Name()
        << " has registered to the Graphics system\n";
    }

    /**************************************************************************/
    /*!
    @brief Deregisters a space to this graphics system.
    @param A reference to the 'GraphicsSpace' component in the Space.
    */
    /**************************************************************************/
    void Graphics::Deregister(Components::GraphicsSpace & graphicsSpace)
    {
      DCTrace << "Graphics::Deregister -  " << graphicsSpace.Owner()->Name()
        << " has deregistered from the Graphics system\n";
      auto graphicsSpacePtr = &graphicsSpace;
      ActiveGraphicsSpaces.erase(std::remove(ActiveGraphicsSpaces.begin(),
        ActiveGraphicsSpaces.end(), graphicsSpacePtr),
        ActiveGraphicsSpaces.end());
    }

    /**************************************************************************/
    /*!
    @brief Draws a circle at the specified position.
    @param pos The center of the circle.
    @param radius The radius of the circle.
    @param color The color of the circle.
    @param camera A reference to the camera component.
    @param fill Whether the circle should be filled.
    */
    /**************************************************************************/
    void Graphics::DrawCircle(const Vec3& pos, Real& radius, const Vec4& color, Components::Camera& camera, bool fill)
    {
      GraphicsHandler->SetShaderProjViewUniforms(GraphicsHandler->SpriteShader, camera);
      mDebugCircleList.push_back(DebugCircle(color, pos, radius));
    }

    /**************************************************************************/
    /*!
    @brief Draws a rectangle at the specified position.
    @param pos The center of the rectangle.
    @param radius The width of the rectangle.
    @param height The height of the rectangle.
    @param color The color of the rectangle.
    @param camera A reference to the camera component.
    @param fill Whether the rectangle should be filled.
    */
    /**************************************************************************/
    void Graphics::DrawRectangle(const Vec3& pos, Real& width, Real& height, const Vec4& color, Components::Camera& camera, bool fill)
    {
      GraphicsHandler->SetShaderProjViewUniforms(GraphicsHandler->SpriteShader, camera);
      mDebugRectangleList.push_back(DebugRectangle(color, pos, Vec2(width, height), fill));
    }

    /**************************************************************************/
    /*!
    @brief Draws a line from the specified starting position to the end position.
    @param startPos The starting position of the line.
    @param endPos The ending position of the line.
    @param color  The color of the line.
    */
    /**************************************************************************/
    void Graphics::DrawLineSegment(const Vec3& startPos, const Vec3& endPos, const Vec4& color, Components::Camera& camera)
    {
      GraphicsHandler->SetShaderProjViewUniforms(GraphicsHandler->SpriteShader, camera);
      mDebugLineList.push_back(DebugLine(color, startPos, endPos));
    }

    /**************************************************************************/
    /*!
    \brief Terminates the Graphics System.
    */
    /**************************************************************************/
    void Graphics::Terminate() {
      DCTrace << "Graphics::Terminate \n";
      GraphicsHandler->Terminate();
    }

    /**************************************************************************/
    /*!
    \brief Loads all currently loaded graphical assets from the Content system
           into the GPU.
    */
    /**************************************************************************/
    void Graphics::LoadGraphicalResources()
    {
      auto& resources = Daisy->getSystem<Content>()->LoadedGraphicalResources();
      std::lock_guard<std::mutex> lock(resources.AssetsLock);

      // If there's no resources to load..
      if (resources.Assets.empty())
        return;

      // Load the latest resource
      auto& resource = resources.Assets.front();

      // Update the statistics (A bit prematurely!)
      resources.LastLoaded = resource->Name();
      resources.NumLoaded++;

      // SpriteSource
      if (auto spriteSource = dynamic_cast<SpriteSource*>(resource)) {
        spriteSource->GenerateTexture();
      }
      // Font
      else if (auto font = dynamic_cast<Font*>(resource)) {
        font->GenerateFont();
      }

      // Remove it from the queue
      resources.Assets.pop();

      // If the queue is now empty..
      if (resources.NumLoaded == resources.NumTotal) {
        std::lock_guard<std::mutex> lock(resources.LockFinished);
        resources.Finished = true;
      }


    }

		void Graphics::ScreenSpaceRectangle(float width, float height, SpriteSourcePtr sprite)
		{
			mDeferredObjects.push_back(sprite);
		}

    /**************************************************************************/
    /*!
    \brief Starts the current frame.
    */
    /**************************************************************************/
    void Graphics::StartFrame() {
      GraphicsHandler->StartFrame();
    }

    /**************************************************************************/
    /*!
    \brief Ends the current frame.
    */
    /**************************************************************************/
    void Graphics::EndFrame() {
      GraphicsHandler->EndFrame();
    }

    /**************************************************************************/
    /*!
    \brief Saves the current OpenGL state.
    */
    /**************************************************************************/
    void Graphics::BackupState()
    {
      GraphicsHandler->BackupState();
    }

    /**************************************************************************/
    /*!
    \brief Restores the previous OpenGL state.
    */
    /**************************************************************************/
    void Graphics::RestoreState()
    {
      GraphicsHandler->RestoreState();
      //GraphicsHandler->ConfigureSpriteVAO();
    }

    /**************************************************************************/
    /*!
    \brief Recompiles the shaders.
    */
    /**************************************************************************/
    void Graphics::OnGraphicsCompileShadersEvent(Events::GraphicsCompileShaders * event)
    {
      GraphicsHandler->CompileShaders();
    }

    /**************************************************************************/
    /*!
    \brief Event received whenever fullscreen has been enabled.
    */
    /**************************************************************************/
    void Graphics::OnWindowFullScreenEnabledEvent(Events::WindowFullScreenEnabled * event)
    {
      std::string willNoticeMe = "Will I am enabled";
      GraphicsHandler->FreeFBO();
      GraphicsHandler->Initialize();
    }

    /**************************************************************************/
    /*!
    \brief Event received whenever fullscreen has been disabled.
    */
    /**************************************************************************/
    void Graphics::OnWindowFullScreenDisabledEvent(Events::WindowFullScreenDisabled * event)
    {
      std::string willNoticeMe = "Will I am disabled";
      GraphicsHandler->FreeFBO();
      GraphicsHandler->Initialize();
    }

    /**************************************************************************/
    /*!
    \brief Event received whenever the window has been resized.
    */
    /**************************************************************************/
    void Graphics::OnWindowResizeEvent(Events::WindowResize * event)
    {
      //Settings.ViewportScale = event->Dimensions;
      Settings.ScreenWidth = event->Dimensions.x;
      Settings.ScreenHeight = event->Dimensions.y;
      GraphicsHandler->FreeFBO();
      GraphicsHandler->Initialize();
      DCTrace << "Graphics::OnWindowResizeEvent - Width: " << event->Dimensions.x
        << " Height " << event->Dimensions.y << "\n";
    }

    void Graphics::OnWindowRecreateEvent(Events::WindowRecreate * event)
    {
      GraphicsHandler->FreeFBO();
      GraphicsHandler->Initialize();
    }

    /**************************************************************************/
    /*!
    \brief Event received whenever lightning is enabled or disaled.
    */
    /**************************************************************************/
    void Graphics::OnGraphicsToggleLightningEvent(Events::GraphicsToggleLightning * event)
    {
      Settings.LightningEnabled = !Settings.LightningEnabled;
    }

    void Graphics::SendCountToGL(int TotalObjNumG, int TotalObjTranspNumG)
    {
      //GraphicsHandler->TotalObjNum = TotalObjNumG;
      //GraphicsHandler->TotalTranspObjNum = TotalObjTranspNumG;
    }


  
  }
}