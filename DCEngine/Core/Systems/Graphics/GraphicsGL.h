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

    class Window;
    class Graphics;

    class GraphicsGL {
      friend class Graphics;
          
    public:
      /* Shader Control */
      void CompileShaders();
      /* Uniforms */
      void SetShaderProjViewUniforms(ShaderPtr shader, Components::Camera& camera);
			void SetLightUniforms(ShaderPtr shader, Components::Light* lightComponents);
			void SetAllLightUniforms(ShaderPtr shader, const std::vector<Components::Light*>& lightComponents);

      /* Sprite */
      void ConfigureSpriteVAO();      
      void SetSpriteShader(Components::Camera& camera, Components::Light* light);
      void DrawSprite(Components::Sprite& sprite, Components::Camera& camera, float dt);
	    void AnimationUpdate(Components::Sprite& sprite, float dt);
	    int IsNextFrame(Components::Sprite& sprite);
      /* SpriteText */
      void ConfigureSpriteTextVAO();
      void SetSpriteTextShader(Components::Camera& camera);
      void DrawSpriteText(DCEngine::Components::SpriteText& st, DCEngine::Components::Camera& camera);
			/* Particle System */
			void ConfigureParticleBuffers();
			void SetParticleSystemShader(Components::Camera& camera);
			void DrawParticles(Components::SpriteParticleSystem& particles, Components::Camera & camera, double dt);
			/* Shadowing System */
			void ConfigureShadowBuffers(void);
			void SetShadowingShaders(Components::Camera& camera, Components::Light* light);
      /* Model */
      void DrawModel(GameObject& gameObj);
      /* DebugDraw */
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
      /* Viewport */
      
    private:
      // TEMP: Change these two to const
      glm::vec4 ClearColor = glm::vec4(0.2f, 0.2f, 0.3f, 1.0f);
      glm::ivec2 ViewportRatio;
      ShaderPtr SpriteShader, SpriteTextShader, DebugDrawShader, ParticleSystemShader, ShadowingShader;
      GLuint SpriteVAO, SpriteTextVAO, SpriteTextVBO, ParticleVAO, ParticleVBO, ParticleColorInstanceVBO, ParticleTransformInstanceVBO;
      GLuint LineVAO, CircleVAO, RectVAO;
      OpenGLStateData GLState;

			bool lightingON;

      // OpenGL Drawing functions
      void DrawArrays(GLuint VAO, GLuint numVertices, GLenum drawMode);
      void DrawElements(GLuint VAO, GLuint numVertices, GLenum drawMode);      
      // Temporary until we switch to DebugDraw objects with shaders
	    void CleanBuffer();

      GraphicsGL();
      void Initialize();
      void ViewportUpdate();
      void ResizeViewport(glm::vec2 ratio);
      void Terminate();
      void StartFrame();
      void EndFrame();
      void BackupState();
      void RestoreState();
    };
  }
}
