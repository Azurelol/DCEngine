/******************************************************************************/
/*!
\file   GraphicsGL_Configure.cpp
\author Christian Sagel, Chen Shu
\par    email: c.sagel\@digipen.edu
\date   7/30/2015
\brief  Configuration of VAOs.

@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/

#include "GraphicsGL.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    \brief Configures the QuadVAO, used for drawing sprites.
    \note  Because all sprites share the same vertex data, we only have to
    define a single VAO.
    */
    /**************************************************************************/
    void GraphicsGL::ConfigureSpriteVAO() {

      /*
      We first define a set of vertices with (0,0) coordinate being the top-left
      corner of the quad. This means that when translation or scaling transformations
      are applied onto the quad, they're transformmed from the top-left position
      of the quad.
      This is commonly accepted in 2D graphics/GUI systems where elements' positions
      are correspond to the top-left corner of the elements.
      */
      GLuint VBO;
      GLfloat vertices[]{
        // Position,      Texture
        -1.0f, 1.0f,     0.0f, 1.0f,
        1.0f, -1.0f,      1.0f, 0.0f,
        -1.0f, -1.0f,     0.0f, 0.0f,

        -1.0f, 1.0f,      0.0f, 1.0f,
        1.0f, 1.0f,     1.0f, 1.0f,
        1.0f, -1.0f,      1.0f, 0.0f
      };

			const unsigned MAX_PARTICLES = 10000;

      /*
      Next, we simply send the vertices to the GPU and configure the vertex attributes,
      which in this case is a single vertex attribute.
      */
      glGenVertexArrays(1, &SpriteVAO);
      glGenBuffers(1, &VBO);

      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
      glBindVertexArray(SpriteVAO);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);

      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindVertexArray(0);
    }

    /**************************************************************************/
    /*!
    @brief Configures the VAO used for SpriteText.
    @note  We reserve enough memory when initiating the VBO so that we can
    later update the VBO's memory when rendering characters.
    THe 2D quad requries 6 vertices of 4 floats each so we reserve
    6 * 4 floats of memory.
    Because we will be updating the content of the VBO's memory quite often,
    we allocate with GL_DYNAMIC_DRAW.
    */
    /**************************************************************************/
    void GraphicsGL::ConfigureSpriteTextVAO()
    {
      glGenVertexArrays(1, &SpriteTextVAO);
      glGenBuffers(1, &SpriteTextVBO);
      glBindVertexArray(SpriteTextVAO);
      glBindBuffer(GL_ARRAY_BUFFER, SpriteTextVBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindVertexArray(0);
    }

		void GraphicsGL::ConfigureParticleBuffers()
		{
			GLfloat vertices[]{
				// Position,
				-1.0f, 1.0f,
				1.0f, -1.0f,
				-1.0f, -1.0f,

				-1.0f, 1.0f,
				1.0f, 1.0f,
				1.0f, -1.0f,
			};

			const unsigned MAX_PARTICLES = 10000;

			/*
			Next, we simply send the vertices to the GPU and configure the vertex attributes,
			which in this case is a single vertex attribute.
			*/
			glGenVertexArrays(1, &ParticleVAO);
			glGenBuffers(1, &ParticleVBO);
			glGenBuffers(1, &ParticleColorInstanceVBO);
			glGenBuffers(1, &ParticleTransformInstanceVBO);

			glBindBuffer(GL_ARRAY_BUFFER, ParticleVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
			glBindVertexArray(ParticleVAO);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);

			glBindBuffer(GL_ARRAY_BUFFER, ParticleColorInstanceVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * MAX_PARTICLES, NULL, GL_STREAM_DRAW);

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (GLvoid*)0);
			glVertexAttribDivisor(1, 1);

			glBindBuffer(GL_ARRAY_BUFFER, ParticleTransformInstanceVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * MAX_PARTICLES, NULL, GL_STREAM_DRAW);

			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 4, (GLvoid*)0);
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 4, (GLvoid*)(sizeof(glm::vec4)));
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 4, (GLvoid*)(sizeof(glm::vec4) * 2));
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 4, (GLvoid*)(sizeof(glm::vec4) * 3));
			
			glVertexAttribDivisor(2, 1);
			glVertexAttribDivisor(3, 1);
			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

    /**************************************************************************/
    /*!
    \brief  Backs up the current OpenGL state.
    */
    /**************************************************************************/
    void GraphicsGL::BackupState()
    {
      glGetIntegerv(GL_CURRENT_PROGRAM, &GLState.lastProgram);
      glGetIntegerv(GL_TEXTURE_BINDING_2D, &GLState.lastTexture);
      glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &GLState.lastArrayBuffer);
      glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &GLState.lastElementArrayBuffer);
      glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &GLState.lastVertexArray);
      glGetIntegerv(GL_BLEND_SRC, &GLState.lastBlendSrc);
      glGetIntegerv(GL_BLEND_DST, &GLState.lastBlendDst);
      glGetIntegerv(GL_BLEND_EQUATION_RGB, &GLState.lastBlendEquationRGB);
      glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &GLState.lastBlendEquationAlpha);
      GLState.lastEnableBlend = glIsEnabled(GL_BLEND);
      GLState.lastEnableCullFace = glIsEnabled(GL_CULL_FACE);
      GLState.lastEnableDepthTest = glIsEnabled(GL_DEPTH_TEST);
      GLState.lastEnableScissorTest = glIsEnabled(GL_SCISSOR_TEST);
    }

    void GraphicsGL::RestoreState()
    {
      glUseProgram(GLState.lastProgram);
      glBindTexture(GL_TEXTURE_2D, GLState.lastTexture);
      glBindBuffer(GL_ARRAY_BUFFER, GLState.lastArrayBuffer);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GLState.lastElementArrayBuffer);
      glBindVertexArray(GLState.lastVertexArray);
      glBlendEquationSeparate(GLState.lastBlendEquationRGB, GLState.lastBlendEquationAlpha);
      glBlendFunc(GLState.lastBlendSrc, GLState.lastBlendDst);
      if (GLState.lastEnableBlend) glEnable(GL_BLEND); else glDisable(GL_BLEND);
      if (GLState.lastEnableCullFace) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
      if (GLState.lastEnableDepthTest) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
      if (GLState.lastEnableScissorTest) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST);
    }


  }
}