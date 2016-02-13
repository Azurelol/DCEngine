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
		Graphics::Graphics(GraphicsConfig settings) : System(std::string("GraphicsSystem"), EnumeratedSystem::Graphics), Settings(settings) {
      //Settings.MaxDrawLayers = 5;
			mDrawList.resize(Settings.MaxDrawLayers);
			DCTrace << "*Using OpenGL for Graphics \n";
			GraphicsHandler.reset(new GraphicsGL());
			GraphicsHandler->ClearColor = ClearColor;
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

			// For every Space with a 'GraphicsSpace' component...
			for (Components::GraphicsSpace* gfxSpace : graphicsSpaces_) {
        
				int TotalObjNumG = 0, TotalObjTranspNumG = 0;

				// Get the default camera from the 'CameraViewport' component
				auto camera = gfxSpace->Owner()->getComponent<Components::CameraViewport>()->getCamera();

				// Do not update the space if no camera has been set
				if (camera == nullptr)
					continue;

				std::vector<Components::Graphical*> graphicalComponents = gfxSpace->getGraphicsComponents();
				for (auto graphicalComponent : graphicalComponents)
				{
					++TotalObjNumG;
					mDrawList[graphicalComponent->getDrawLayer()].push_back(graphicalComponent);
				}

				std::vector<Components::Light*> lightComponents = gfxSpace->getLightComponents();
				for (auto lightComponent : lightComponents)
				{
					mLightList.push_back(lightComponent);
				}

				GraphicsHandler->SetParticleSystemShader(*camera);
				GraphicsHandler->SetSpriteTextShader(*camera);
				GraphicsHandler->SetSpriteShader(*camera, lightComponents);

				for (auto&& drawList : mDrawList)
				{
					for (auto&& obj : drawList)
					{
						obj->Update(dt);
						obj->Draw(*camera);
					}
					drawList.clear();
				}
				if (Debug::CheckOpenGLError())
					DCTrace << "GraphicsGL::DrawSpriteText - Failed to set active texture!\n";

				glDisable(GL_BLEND);
				GraphicsHandler->SpriteShader->SetInteger("numLights", 0);

				for (const auto& debugObj : mDebugLineList)
				{
					debugObj.Draw();
				}
				for (const auto& debugObj : mDebugRectangleList)
				{
					debugObj.Draw();
				}
				for (const auto& debugObj : mDebugCircleList)
				{
					debugObj.Draw();
				}
				mDebugLineList.clear();
				mDebugRectangleList.clear();
				mDebugCircleList.clear();
				SendCountToGL(TotalObjNumG, TotalObjTranspNumG);
				if (Debug::CheckOpenGLError())
					DCTrace << "GraphicsGL::DrawSpriteText - Failed to set active texture!\n";
			}
		}

		/**************************************************************************/
		/*!
		@brief Registers a space to this graphics system.
		@param A reference to the 'GraphicsSpace' component in the Space.
		*/
		/**************************************************************************/
		void Graphics::Register(Components::GraphicsSpace& graphicsSpace) {
			graphicsSpaces_.push_back(&graphicsSpace);
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
			graphicsSpaces_.erase(std::remove(graphicsSpaces_.begin(),
				graphicsSpaces_.end(), graphicsSpacePtr),
				graphicsSpaces_.end());
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
			GraphicsHandler->DrawSprite(sprite, cam, dt);
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
			GraphicsHandler->DrawSpriteText(st, cam);
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

		///////////////
		// DEBUG DRAW
		///////////////

		void Graphics::DrawCircle(const Vec3& pos, Real& radius, const Vec4& color, Components::Camera& cam, bool fill)
		{
			mDebugCircleList.push_back(DebugCircle(color, pos, radius));
		}

		void Graphics::DrawRectangle(const Vec3& pos, Real& width, Real& height, const Vec4& color, Components::Camera& cam, bool fill)
		{
			mDebugRectangleList.push_back(DebugRectangle(color, pos, Vec2(width, height)));
		}

		void Graphics::DrawLineSegment(const Vec3& startPos, const Vec3& endPos, const Vec4& color, Components::Camera& cam)
		{
			mDebugLineList.push_back(DebugLine(color, startPos, endPos));
		}

		/////////////////
		// FRAME UPDATE
		/////////////////

		void Graphics::Terminate() {
			DCTrace << "Graphics::Terminate \n";
			GraphicsHandler->Terminate();
		}

		void Graphics::StartFrame() {
			GraphicsHandler->StartFrame();
		}

		void Graphics::EndFrame() {
			GraphicsHandler->EndFrame();
		}

		void Graphics::BackupState()
		{
			GraphicsHandler->BackupState();
		}

		void Graphics::RestoreState()
		{
			GraphicsHandler->RestoreState();
			GraphicsHandler->ConfigureSpriteVAO();
		}

		void Graphics::OnFullscreenEnabledEvent(Events::FullscreenEnabledEvent * event)
		{

		}

		void Graphics::OnResizeViewportEvent(Events::ResizeViewportEvent * event)
		{
			ViewportScale = event->viewportScale;
			DCTrace << "Graphics::OnResizeViewportEvent - Width: " << ViewportScale.x
				<< " Height " << ViewportScale.y << "\n";
		}

		void Graphics::SendCountToGL(int TotalObjNumG, int TotalObjTranspNumG)
		{
			GraphicsHandler->TotalObjNum = TotalObjNumG;
			GraphicsHandler->TotalTranspObjNum = TotalObjTranspNumG;
		}


	}
}