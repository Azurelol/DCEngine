/******************************************************************************/
/*!
\file   GraphicsGL.h
\author Christian Sagel, Chen Shu
\par    email: c.sagel\@digipen.edu
\date   7/30/2015
\brief  The GraphicsGL system handles direct access to the GPU through OpenGL
        interface by GLEW, using the GLM library for arithmetic operations.
        It receives drawing requests directly from the Graphics system, 
        its interface.

@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "..\System.h"

// Libraries
#define GLEW_STATIC
#include "..\..\..\Dependencies\include\GLEW\glew.h"
#define GLM_FORCE_RADIANS // functions taking degrees is deprecated.
#include "..\..\..\Dependencies\include\GLM\glm.hpp"
#include "..\..\..\Dependencies\include\GLM\gtc\matrix_transform.hpp"
#include "..\..\..\Dependencies\include\GLM\gtc\type_ptr.hpp"
// Graphics resources
#include "..\..\Components\SpriteParticleSystem.h"
#include "..\..\Components\Light.h"
#include "..\..\Resources\Shader.h"
// DebugDraw
#include "../../Objects/DebugDraw.h"
#include "../../Objects/DebugDraw/DebugDrawInclude.h"

namespace DCEngine {
  
  // Forward declarations  
  namespace Components {
    class GraphicsSpace;
    class Camera;
    class Sprite;
    class SpriteText;
		class Light;
  }
  
  namespace Systems {



    // More forward declarations!
    class Window;
    class Graphics;

    class GraphicsGL {
      friend class Graphics;
          
    public:
      // Shader Control
			void ConfigureFBO();
      void CompileShaders();
      // Uniforms
      void SetShaderProjViewUniforms(ShaderPtr shader, Components::Camera& camera);
			void SetLightUniforms(ShaderPtr shader, Components::Light* lightComponents);
			void SetAllLightUniforms(ShaderPtr shader, const std::vector<Components::Light*>& lightComponents);
      // Sprites
      void ConfigureSpriteVAO();      
      void SetSpriteShader(Components::Camera& camera, Components::Light* light);
      void DrawSprite(Components::Sprite& sprite, Components::Camera& camera, float dt);
	    void AnimationUpdate(Components::Sprite& sprite, float dt);
	    int IsNextFrame(Components::Sprite& sprite);
      // SpriteText
      void ConfigureSpriteTextVAO();
      void SetSpriteTextShader(Components::Camera& camera);
      void DrawSpriteText(DCEngine::Components::SpriteText& st, DCEngine::Components::Camera& camera);
			// Particle System
			void ConfigureParticleBuffers();
			void SetParticleSystemShader(Components::Camera& camera);
			void DrawParticles(Components::SpriteParticleSystem& particles, Components::Camera & camera, double dt);
			// Shadowing System
			void ConfigureShadowBuffers(void);
			void SetShadowingShaders(Components::Camera& camera, Components::Light* light);
      // Modes
      void DrawModel(GameObject& gameObj);
      // DebugDraw
      void ConfigureLineVAO();
      void ConfigureCircleVAO();
      void ConfigureRectangleVAO();
      void SetDebugDrawShaderProjViewUniform(Components::Camera& camera);
      void DrawDebug(DebugDrawObject& debugDrawObj, Components::Camera& cam);
      void DrawCircle(DrawCircleObj& obj);
      void DrawRectangle(DrawRectObj& obj);      
      void DrawLineSegment(DrawLineObj& obj);
      void DrawRectangle(const Vec3& pos, Real width, Real height, const Vec4& color, bool fill = false);
      void DrawCircle(const Vec3& pos, Real radius, const Vec4& color, bool fill = false);
      void DrawLineSegment(const Vec3& startPos, const Vec3& endPos, const Vec4& color);
      // Lightning
			void RenderDepths(Components::Camera* camera);
      void RenderShadows(Components::Camera* camera, Components::Light* light);
      void RenderObjects(Components::Camera* camera, Components::Light* light = 0, ShaderPtr shader = 0);
      void RenderBackground(ShaderPtr shader, Components::Camera* camera);
      void RenderZ0Scene(Components::Camera* camera, Components::Light* light, ShaderPtr shader = 0);
			void PreRender(Components::Camera* camera);
			void RenderLights(Components::Light* light = 0);
			void RenderScene(float exposure);
      void DrawDebug();
      
    private:
      GraphicsConfig& Settings;
      ShaderPtr FinalRenderShader, DebugShader, SpriteShader, SpriteTextShader, DebugDrawShader, ParticleSystemShader, ShadowingShader, LightingShader;
      GLuint SpriteVAO, SpriteTextVAO, SpriteTextVBO, ParticleVAO, ParticleVBO, ParticleColorInstanceVBO, ParticleTransformInstanceVBO;
      GLuint LineVAO, CircleVAO, RectVAO;
			GLuint FBO, PosTexture, NormalTexture, ColorTexture, FinalColor;
			
			std::vector<std::vector<Components::Graphical*>>* mDrawList;
      OpenGLStateData GLState;
      // OpenGL Drawing functions
      void DrawArrays(GLuint VAO, GLuint numVertices, GLenum drawMode);
      void DrawElements(GLuint VAO, GLuint numVertices, GLenum drawMode);      
      // Main functions
      GraphicsGL(GraphicsConfig& settings);
      void Initialize();
      void ViewportUpdate();
      void ResizeViewport(glm::vec2 ratio);
      void Terminate();
      void StartFrame();
      void EndFrame();
      void BackupState();
      void RestoreState();
      // Temporary until we switch to DebugDraw objects with shaders
	    void CleanBuffer();
    };
  }
}
