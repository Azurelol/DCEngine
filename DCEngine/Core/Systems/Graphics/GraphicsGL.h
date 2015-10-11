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
#include "..\..\Resources\Shader.h"
// DebugDraw
#include "../../Objects/DebugDraw.h"
#include "../../Objects/DebugDraw/DebugDrawInclude.h"

namespace DCEngine {
  
  // Forward declarations
  class GraphicsSpace;
  class Camera;
  class Sprite;
  class SpriteText;

  namespace Systems {

    class GraphicsGL {
      friend class Graphics;
          
    public:
      GraphicsGL();      
      void Initialize();
      void Update(float dt);
      void Terminate();
      void StartFrame();
      void EndFrame();      

      void SetShaderProjViewUniforms(ShaderPtr shader, Camera& camera);

      /* Sprite */
      void ConfigureSpriteVAO();      
      void SetSpriteShader(Camera& camera);
      void DrawSprite(Sprite& sprite, Camera& camera);
      /* SpriteText */
      void ConfigureSpriteTextVAO();
      void SetSpriteTextShader(Camera& camera);
      void DrawSpriteText(SpriteText& st, Camera& camera);

      /* Model */
      void DrawModel(GameObject& gameObj);

      /* DebugDraw */
      void ConfigureLineVAO();
      void ConfigureCircleVAO();
      void ConfigureRectangleVAO();

      void SetDebugDrawShaderProjViewUniform(Camera& camera);

      void DrawDebug(DebugDrawObject& debugDrawObj, Camera& cam); 
      void DrawCircle(DrawCircleObj& obj);
      void DrawRectangle(DrawRectObj& obj);
      
      void DrawLineSegment(DrawLineObj& obj);
      void DrawRectangle(Real3& pos, Real& width, Real& height, Real4& color, Camera& cam);
      void DrawCircle(Real3& pos, Real& radius, Real4& color, Camera& cam);
      void DrawLineSegment(Real3& startPos, Real3& endPos, Real& length, Real4& color, Camera& cam);
      
    private:
      // TEMP: Change these two to const
      GLuint screenwidth_;
      GLuint screenheight_;
      glm::vec4 ClearColor = glm::vec4(0.2f, 0.2f, 0.3f, 1.0f);

      // Draw function
      //void Draw(glm::mat4  glm::vec3 scale, glm::vec3 rotation, Camera& camera, GLenum drawMode);

      // Sprites
      ShaderPtr SpriteShader;
      GLuint SpriteVAO; 
      // SpriteText
      ShaderPtr SpriteTextShader;
      GLuint SpriteTextVAO, SpriteTextVBO;
      // DebugDraw
      ShaderPtr DebugDrawShader;
      GLuint LineVAO, CircleVAO, RectVAO;

      // Testing
      ShaderPtr SimpleShader;

      // Temporary
	    void BufferCleaner();

    }; // GraphicsGL 

  } // Systems
} //DCEngine
