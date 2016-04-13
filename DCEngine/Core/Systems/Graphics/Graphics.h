/*****************************************************************************/
/*!
\file   Graphics.h
\author Christian Sagel, Chen Shu, William Mao
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
#include <ZILCH\Zilch.hpp>

namespace DCEngine {
  class Engine;

  class Camera;
  class Sprite;
  class SpriteText;
  class DebugDrawObject;
	

  namespace Systems {
    class WindowSFML;
		
    class Graphics : public System {
      friend class Engine;
      friend class WindowSFML;
      friend class GraphicsGL;
			struct DeferredObject;
    public:
      // Registration
      void Register(Components::GraphicsSpace& graphicsSpace);
      void Deregister(Components::GraphicsSpace& graphicsSpace);
      // Draw
      void DrawSprite(Components::Sprite& sprite, Components::Camera& camera, float dt);
      void DrawSpriteText(Components::SpriteText& st, Components::Camera& cam);
      void DrawParticles(Components::SpriteParticleSystem& particles, Components::Camera& cam, double dt);
      //void DrawModel(Components::Model& model, Components::Camera& cam);
      void DrawDebug(DebugDrawObject& debugDraw);
      // DebugDraw
      void DrawCircle(const Vec3& pos, Real& radius, const Vec4& color, Components::Camera& cam, bool fill = false);
      void DrawRectangle(const Vec3& pos, Real& width, Real& height, const Vec4& color, Components::Camera& cam, bool fill = false);
      void DrawLineSegment(const Vec3& startPos, const Vec3& endPos, const Vec4& color, Components::Camera& cam);

			//graphicsSettings
			void SetResolution(unsigned x, unsigned y);
			void SetAntiAliasingLevel(unsigned samples);
			void ToggleFullscreen();
			//Setting Gettors
			unsigned GetScreenWidth() const;
			unsigned GetScreenHeight() const;

			void ScreenSpaceRectangle(float width, float height, SpriteSourcePtr sprite);


    private:

      GraphicsConfig& Settings;		
      static std::unique_ptr<GraphicsGL> GraphicsHandler;
      std::vector<Components::GraphicsSpace*> ActiveGraphicsSpaces;			
      // Matrixes
      Mat4 ProjMatrix;
      Mat4 ViewMatrix;
      Mat4 ViewProjMatrix;		
      // Base methods
      void StartFrame();
      void EndFrame();
      void BackupState();
      void RestoreState();
      // Events
      void OnGraphicsCompileShadersEvent(Events::GraphicsCompileShaders* event);      
      void OnWindowFullScreenEnabledEvent(Events::WindowFullScreenEnabled* event);
      void OnWindowFullScreenDisabledEvent(Events::WindowFullScreenDisabled* event);
      void OnWindowResizeEvent(Events::WindowResize* event);
			void OnWindowRecreateEvent(Events::WindowRecreate* event);
      void OnGraphicsToggleLightningEvent(Events::GraphicsToggleLightning* event);
      // CTOR/ DTOR, Initialize
      Graphics(GraphicsConfig& settings);
      void Initialize();
      void Subscribe();
      // Main Methods
      void Update(float dt);
      void UpdateObjects(float dt);
      void RenderShadows(Components::Camera* camera, Components::Light* light);
      void RenderScene(Components::Camera* camera, Components::Light* light = 0, ShaderPtr shader = 0);
      void RenderBackground(ShaderPtr shader, Components::Camera* camera);
      void RenderZ0Scene(Components::Camera* camera, Components::Light* light, ShaderPtr shader = 0);
      void DrawDebug(Components::Camera* camera);
      void Terminate();
      // Assets
      void LoadGraphicalResources();

      //2D draw list
      //int TotalObjNumG = 0;
      //int TotalObjTranspNumG = 0;
      std::vector<std::vector<Components::Graphical*>> mDrawList;
			std::vector<SpriteSourcePtr> mDeferredObjects;
      void SendCountToGL(int TotalObjNumG, int TotalObjTransNumG);

			/*============
			Deferred OBJECT
			=============*/
			struct DeferredObject
			{
				DeferredObject(unsigned width, unsigned height, SpriteSourcePtr sprite)
					: width(width), height(height), sprite(sprite) {}
				unsigned width, height;
				SpriteSourcePtr sprite;
			};
      /*============
       DEBUG OBJECT
      =============*/
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
      // Draw Lists
      std::vector<DebugLine> mDebugLineList;
      std::vector<DebugRectangle> mDebugRectangleList;
      std::vector<DebugCircle> mDebugCircleList;
    };


  }

}