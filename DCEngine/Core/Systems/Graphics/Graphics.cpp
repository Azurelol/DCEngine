/*****************************************************************************/
/*!
\file   Graphics.cpp
\author Chen Shu, Christian Sagel
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
			Daisy->Connect<Events::ResizeViewportEvent>(&Graphics::OnResizeViewportEvent, this);
			Daisy->Connect<Events::FullscreenEnabledEvent>(&Graphics::OnFullscreenEnabledEvent, this);
      Daisy->Connect<Events::GraphicsCompileShaders>(&Graphics::OnGraphicsCompileShadersEvent, this);
      Daisy->Connect<Events::GraphicsToggleLightning>(&Graphics::OnGraphicsToggleLightningEvent, this);
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
				for (auto graphicalComponent : graphicalComponents)
					mDrawList[graphicalComponent->getDrawLayer()].push_back(graphicalComponent);

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
				//if (!lightComponents.empty())
				//{
				//	glDrawBuffer(GL_NONE);
				//	RenderZ0Scene(camera, 0);
				//	for (const auto& light : lightComponents)
				//	{
				//		if (light->getCastShadows())
				//		{
				//			glDepthFunc(GL_LESS);
				//			glDrawBuffer(GL_NONE);
				//			glEnable(GL_STENCIL_TEST);
				//
				//			RenderShadows(camera, light);
				//
				//			glStencilFunc(GL_GEQUAL, 0x1, 0xFF);
				//			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
				//		}
				//		glDrawBuffer(GL_FRONT_AND_BACK);
				//		glDepthFunc(GL_LEQUAL);
				//
				//		glEnable(GL_BLEND);
				//		glBlendFunc(GL_ONE, GL_ONE);
				//		RenderScene(camera, light);
				//
				//		glClear(GL_STENCIL_BUFFER_BIT);
				//	}
				//}
				//else
				//{
				//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				//	RenderScene(camera);
				//}
				
			}
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
		\brief Draws a sprite, by forwarding the data to OpenGL.
		\param A reference to the GameObject.
		\param A reference to the camera object in the Space.
		\note
		*/
		/**************************************************************************/
		void Graphics::DrawSprite(Components::Sprite & sprite, Components::Camera& cam, float dt) {
			
      if (!sprite.Visible)
        return;

      if (TRACE_UPDATE)
				DCTrace << "Graphics::DrawSprite - Drawing " << sprite.Owner()->Name() << "\n";
			//GraphicsHandler->DrawSprite(sprite, cam, dt);
		}

		/**************************************************************************/
		/*!
		\brief Draws a 'SpriteText', by forwarding the data to OpenGL.
		\param A reference to the SpriteText component.
		\param A reference to the camera object in the Space.
		\note
		*/
		/**************************************************************************/
		void Graphics::DrawSpriteText(Components::SpriteText & st, Components::Camera & cam)
		{
			if (TRACE_UPDATE)
				DCTrace << "Graphics::DrawSpriteText - Drawing " << st.Owner()->Name() << "\n";
			//GraphicsHandler->DrawSpriteText(st, cam);
		}

		void Graphics::DrawParticles(Components::SpriteParticleSystem& particles, Components::Camera & cam, double dt)
		{
			GraphicsHandler->DrawParticles(particles, cam, dt);
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
			GraphicsHandler->ConfigureSpriteVAO();
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

    void Graphics::OnFullscreenEnabledEvent(Events::FullscreenEnabledEvent * event)
		{

		}

		void Graphics::OnResizeViewportEvent(Events::ResizeViewportEvent * event)
		{
			Settings.ViewportScale = event->viewportScale;
			DCTrace << "Graphics::OnResizeViewportEvent - Width: " << Settings.ViewportScale.x
				<< " Height " << Settings.ViewportScale.y << "\n";
		}

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