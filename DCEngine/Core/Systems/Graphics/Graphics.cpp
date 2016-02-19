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
				Components::Camera* camera = gfxSpace->Owner()->getComponent<Components::CameraViewport>()->getCamera();

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

				lightComponents.erase(std::remove_if(lightComponents.begin(), lightComponents.end(), 
					[] (Components::Light* light) { return !light->getVisible(); }), lightComponents.end());

				GraphicsHandler->SetParticleSystemShader(*camera);
				GraphicsHandler->SetSpriteTextShader(*camera);
				GraphicsHandler->SetSpriteShader(*camera, lightComponents);


				for (auto& drawList : mDrawList)
				{
					for (auto&& obj : drawList)
					{
						std::sort(drawList.begin(), drawList.end(),
							[](Components::Graphical* a, Components::Graphical* b)
						{
							return a->Owner()->getComponent<Components::Transform>()->Translation.z
								< b->Owner()->getComponent<Components::Transform>()->Translation.z;
						});
					}
					//drawList.clear();
				}

				//RenderDepths(dt, camera);

				//glEnable(GL_STENCIL_TEST);

				//RenderShadows(dt, camera, lightComponents);

				RenderScene(dt, camera);

				//glDisable(GL_STENCIL_TEST);

				DrawDebug();
				
				for (auto&& drawList : mDrawList)
				{
					drawList.clear();
				}

				SendCountToGL(TotalObjNumG, TotalObjTranspNumG);
			}
		}

		void Graphics::RenderDepths(float dt, Components::Camera* camera)
		{
			glDrawBuffer(GL_NONE);
			RenderObjects(dt, camera);
		}

		void Graphics::RenderShadows(float dt, Components::Camera* camera, const std::vector<Components::Light*>& lightComponents)
		{
			//glDepthMask(GL_FALSE);
			glEnable(GL_DEPTH_CLAMP);
			glDisable(GL_CULL_FACE);

			glStencilFunc(GL_ALWAYS, 0, 0xff);

			glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
			glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

			GraphicsHandler->SetShadowingShaders(*camera, lightComponents);

			for (auto&& drawList : mDrawList)
			{
				for (auto&& obj : drawList)
				{
					if (dynamic_cast<Components::Sprite*>(obj))
					{
						obj->SetModelMatrix();
						obj->Draw(*camera);
					}
				}
			}

			// Restore local stuff
			glDisable(GL_DEPTH_CLAMP);
			glEnable(GL_CULL_FACE);
		}

		void Graphics::RenderScene(float dt, Components::Camera* camera)
		{
			glDrawBuffer(GL_BACK);

			// Draw only if the corresponding stencil value is zero
			glStencilFunc(GL_EQUAL, 0x0, 0xFF);

			// prevent update to the stencil buffer
			glStencilOpSeparate(GL_BACK, GL_KEEP, GL_KEEP, GL_KEEP);

			RenderObjects(dt, camera);

			glDepthMask(GL_TRUE);
		}

		void Graphics::RenderObjects(float dt, Components::Camera* camera)
		{
			for (auto&& drawList : mDrawList)
			{
				for (auto&& obj : drawList)
				{
					obj->Update(dt);
					obj->SetModelMatrix();
					obj->Draw(*camera);
				}
				//drawList.clear();
			}
		}

		void Graphics::DrawDebug()
		{
			glDisable(GL_BLEND);
			GraphicsHandler->SpriteShader->SetInteger("numLights", 0);
			GraphicsHandler->SpriteShader->Use();
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