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

				//GraphicsHandler->SetParticleSystemShader(*camera);
				//for (auto&& particleSystem : gfxSpace->getParticleSystem())
				//{
				//	DrawParticles(*particleSystem, *camera, dt);
				//}

				//draw sprite text
				//GraphicsHandler->SetSpriteTextShader(*camera);
				//for (auto spriteText : gfxSpace->getSpriteTextContainer())
				//{
				//	DrawSpriteText(*spriteText, *camera);
				//}

				// draw sprites
				//GraphicsHandler->SetSpriteShader(*camera);
				if (Debug::CheckOpenGLError())
					DCTrace << "GraphicsGL::DrawSpriteText - Failed to set active texture!\n";
				std::vector<Components::Graphical*> graphicalComponents = gfxSpace->getGraphicsComponents();
				for (auto graphicalComponent : graphicalComponents)
				{
					++TotalObjNumG;
					mDrawList[graphicalComponent->getDrawLayer()].push_back(graphicalComponent);
				}
				if (Debug::CheckOpenGLError())
					DCTrace << "GraphicsGL::DrawSpriteText - Failed to set active texture!\n";

				for (auto&& drawList : mDrawList)
				{
					for (auto&& obj : drawList)
					{
						obj->Update(dt);
						obj->Draw(*camera);
					}
					drawList.clear();
				}

				//for (auto gameObj : gfxSpace->getSprites()) {
				//	++TotalObjNumG;
				//	mDrawList[gameObj->getDrawLayer()].push_back(gameObj);
				//}

				//for(auto&& drawList : mDrawList)
				//{
				//	for (Components::Sprite* sprite : drawList)
				//	{
				//		DrawSprite(*sprite, *camera, dt);
				//	}
				//	drawList.clear();
				//}

				SendCountToGL(TotalObjNumG, TotalObjTranspNumG);
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

		void Graphics::DrawCircle(Vec3& pos, Real& radius, Vec4& color, Components::Camera& cam)
		{
			GraphicsHandler->DrawCircle(pos, radius, color, cam);
		}

		void Graphics::DrawRectangle(Vec3& pos, Real& width, Real& height, Vec4& color, Components::Camera& cam)
		{
			GraphicsHandler->DrawRectangle(pos, width, height, color, cam);
		}

		void Graphics::DrawLineSegment(Vec3& startPos, Vec3& endPos, Vec4& color, Components::Camera& cam)
		{
			GraphicsHandler->DrawLineSegment(startPos, endPos, color, cam);
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