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

namespace DCEngine {
	namespace Systems {

		/**************************************************************************/
		/*!
		\brief Default constructor for the Graphics System.
		\note  It sets several values for the OpenGL interface...
		*/
		/**************************************************************************/
		Graphics::Graphics() : System(std::string("GraphicsSystem"), EnumeratedSystem::Graphics) {
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

			// Update the graphics system
			//GraphicsHandler->ViewportUpdate();

			// For every Space with a 'GraphicsSpace' component...
			for (auto gfxSpace : graphicsSpaces_) {
        
				// Get the default camera from the 'CameraViewport' component
				auto camera = gfxSpace->Owner()->getComponent<CameraViewport>()->getCamera();

				// Do not update the space if no camera has been set
				if (camera == nullptr)
					continue;

				// Update every 'Sprite'
				GraphicsHandler->SetSpriteShader(*camera);

				for (auto gameObj : gfxSpace->getSprites()) {
					//draw list
					++TotalObjNumG;
					if (gameObj->SpriteSource == "Square")
					{
						if (gameObj->Color.a == 1)
						{
							NonTextureObjNontransp.push_back(gameObj);
						}
						else
						{
							NonTextureObjtransp.push_back(gameObj);
						}
					}
					else
					{
						if (gameObj->Color.a == 1)
						{
							TextureObjNontransp.push_back(gameObj);
						}
						else
						{
							TextureObjtransp.push_back(gameObj);
						}
					}
				}
				//sort
				/*std::map<float, Sprite*> sorted;
				for (GLuint i = 0; i < TransparentObj.size(); i++) // windows contains all window positions
				{
				GLfloat distance = glm::length(camera->TransformComponent->Translation - TransparentObj[i]->TransformComponent->Translation);
				sorted[distance] = TransparentObj[i];
				}*/

				//Nontexture object draw first
				for (int i = 0; i < NonTextureObjNontransp.size(); ++i)
				{
					DrawSprite(*(NonTextureObjNontransp[i]), *camera, dt);
				}

				for (int i = 0; i < TextureObjNontransp.size(); ++i)
				{
					DrawSprite(*(TextureObjNontransp[i]), *camera, dt);
				}

				for (int i = 0; i < NonTextureObjtransp.size(); ++i)
				{
					DrawSprite(*(NonTextureObjtransp[i]), *camera, dt);
				}

				for (int i = 0; i < TextureObjtransp.size(); ++i)
				{
					DrawSprite(*(TextureObjtransp[i]), *camera, dt);
				}

				/*for (std::map<float, Sprite*>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
				{
				DrawSprite(*(it->second), *camera, dt);
				}*/
				NonTextureObjNontransp.clear();
				TextureObjNontransp.clear();
				NonTextureObjtransp.clear();
				TextureObjtransp.clear();
				/*/*sorted.clear();
				TransparentObj.clear();*/

				SendCountToGL(TotalObjNumG, TotalObjTranspNumG);

				//Clean the counter
				TotalObjNumG = 0;
				TotalObjTranspNumG = 0;

				/* IF DRAW SPRITE TEXT IS CALLED, BREAKS T_T */
				// Update every 'SpriteText'
				//GraphicsHandler->SetSpriteTextShader(*camera);
				//for (auto spriteText : gfxSpace->getSpriteTextContainer()) {
				//  DrawSpriteText(*spriteText, *camera);
				//}
			}

		}

		/**************************************************************************/
		/*!
		@brief Registers a space to this graphics system.
		@param A reference to the 'GraphicsSpace' component in the Space.
		*/
		/**************************************************************************/
		void Graphics::Register(GraphicsSpace& graphicsSpace) {
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
		void Graphics::Deregister(GraphicsSpace & graphicsSpace)
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
		void Graphics::DrawSprite(Sprite & sprite, Camera& cam, float dt) {
			
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
		void Graphics::DrawSpriteText(SpriteText & st, Camera & cam)
		{
			if (TRACE_UPDATE)
				DCTrace << "Graphics::DrawSpriteText - Drawing " << st.Owner()->Name() << "\n";
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

		void Graphics::DrawCircle(Vec3& pos, Real& radius, Vec4& color, Camera& cam)
		{
			GraphicsHandler->DrawCircle(pos, radius, color, cam);
		}

		void Graphics::DrawRectangle(Vec3& pos, Real& width, Real& height, Vec4& color, Camera& cam)
		{
			GraphicsHandler->DrawRectangle(pos, width, height, color, cam);
		}

		void Graphics::DrawLineSegment(Vec3& startPos, Vec3& endPos, Vec4& color, Camera& cam)
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