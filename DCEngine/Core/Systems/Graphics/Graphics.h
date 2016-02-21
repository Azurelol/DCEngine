/*****************************************************************************/
/*!
\file   Graphics.h
\author Christian Sagel, Chen Shu
\par    email: c.sagel\@digipen.edu
\date   9/12/2015
\brief  The graphics system is the bridge between the rendering calls from the
entity components and the underlying low-level OpenGL renderer. It
receives rendering requests and draws on the window.

@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "../System.h"

// OpenGL rendering
#include "GraphicsGL.h"
#include "../../Components/GraphicsSpace.h"
#include "../../Components/SpriteParticleSystem.h"

namespace DCEngine {
	class Engine;

	class Camera;
	class Sprite;
	class SpriteText;
	class DebugDrawObject;

  struct GraphicsConfig {
    int MaxDrawLayers;
  };


	namespace Systems {
		class WindowSFML;

		class Graphics : public System {
			friend class Engine;
			friend class WindowSFML;
			friend class GraphicsGL;
		public:
      // Registration
			void Register(Components::GraphicsSpace& graphicsSpace);
			void Deregister(Components::GraphicsSpace& graphicsSpace);
      // Draw
			void DrawSprite(Components::Sprite& sprite, Components::Camera& camera, float dt);
			void DrawSpriteText(Components::SpriteText& st, Components::Camera& cam);
			void DrawParticles(Components::SpriteParticleSystem& particles, Components::Camera& cam, double dt);
			void DrawModel(GameObject& gameObj);
			void DrawDebug(DebugDrawObject& debugDraw);
			// DebugDraw
			void DrawCircle(const Vec3& pos, Real& radius, const Vec4& color, Components::Camera& cam, bool fill = false);
			void DrawRectangle(const Vec3& pos, Real& width, Real& height, const Vec4& color, Components::Camera& cam, bool fill = false);
			void DrawLineSegment(const Vec3& startPos, const Vec3& endPos, const Vec4& color, Components::Camera& cam);

		private:

			static std::unique_ptr<GraphicsGL> GraphicsHandler;

      GraphicsConfig Settings;
			
			const int screenwidth_ = 1024;
			const int screenheight_ = 768;
			Vec4 ClearColor = Vec4(0.0f, 0.5f, 1.0f, 1.0f);
			Vec2 ViewportScale = Vec2(1, 1);
			Mat4 ProjMatrix;
			Mat4 ViewMatrix;
			Mat4 ViewProjMatrix;
			std::vector<Components::GraphicsSpace*> graphicsSpaces_;

			
			

			// Base methods
			void StartFrame();
			void EndFrame();
			void BackupState();
			void RestoreState();
			// Events
			void OnFullscreenEnabledEvent(Events::FullscreenEnabledEvent* event);
			void OnResizeViewportEvent(Events::ResizeViewportEvent* event);
      // CTOR/ DTOR, Initialize
			Graphics(GraphicsConfig settings);
			void Initialize();
			void Subscribe();

			//Main Methods
			void Update(float dt);
			void RenderDepths(float dt, Components::Camera* camera);
			void RenderShadows(float dt, Components::Camera* camera, const std::vector<Components::Light*>& lightComponents);
			void RenderScene(float dt, Components::Camera* camera);
			void RenderObjects(float dt, Components::Camera* camera);
			void DrawDebug();
			void Terminate();

			//2D draw list
			//int TotalObjNumG = 0;
			//int TotalObjTranspNumG = 0;
			std::vector<std::vector<Components::Graphical*>> mDrawList;
			//std::vector<Components::Light*> mLightList;
			
			//std::vector<Components::Sprite*>  NonTextureObjNontransp;
			//std::vector<Components::Sprite*>  TextureObjNontransp;
			//std::vector<Components::Sprite*> NonTextureObjtransp;
			//std::vector<Components::Sprite*> TextureObjtransp;
			void SendCountToGL(int TotalObjNumG, int TotalObjTransNumG);
			class DebugObject
			{
			public:
				DebugObject(const Vec4&, const Vec3&);
				virtual void Draw(void) const = 0;
			protected:
				Vec4 color;
				Vec3 position;
			};
			class DebugLine : public DebugObject
			{
			public:
				DebugLine(const Vec4&, const Vec3&, const Vec3&);
				virtual void Draw(void) const;
			private:
				Vec3 endPosition;
			};
			class DebugRectangle : public DebugObject
			{
			public:
				DebugRectangle(const Vec4&, const Vec3&, const Vec2&, bool fill);
				virtual void Draw(void) const;
			private:
				Vec2 scale;
				bool fill;
			};
			class DebugCircle : public DebugObject
			{
			public:
				DebugCircle(const Vec4&, const Vec3&, float);
				virtual void Draw(void) const;
			private:
				float radius;
			};
			std::vector<DebugLine> mDebugLineList;
			std::vector<DebugRectangle> mDebugRectangleList;
			std::vector<DebugCircle> mDebugCircleList;
		};


	}

}