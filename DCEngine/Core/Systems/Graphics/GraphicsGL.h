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

namespace DCEngine {
  
  class GraphicsSpace;
  class Camera;

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

      void ConfigureSpriteVAO();
      void SetShaderProjectionUniform(Camera& camera);
      void DrawSprite(GameObject& gameObj, Camera& camera);

      void DrawModel(GameObject& gameObj);

      void DrawCircle(Real3 pos, Real radius, Real4 color);
      void DrawRectangle(Real3 pos, Real width, Real height, Real4 color);
      void DrawLineSegment(Real3 startPos, Real3 endPos, Real length, Real4 color);


    private:
      // TEMP: Change these two to const
      GLuint screenwidth_;
      GLuint screenheight_;
      glm::vec4 ClearColor = glm::vec4(0.2f, 0.2f, 0.3f, 1.0f);

      // Used for drawing sprites
      ShaderPtr SpriteShader;
      GLuint SpriteVAO; // A Quad VAO

      // Testing
      ShaderPtr SimpleShader;
      void SetUpTest();
      void DrawTest();
      GLuint testVAO, testVBO;
      


    }; // GraphicsGL 

  } // Systems
} //DCEngine
