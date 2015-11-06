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

      /*
      Next, we simply send the vertices to the GPU and configure the vertex attributes,
      which in this case is a single vertex attribute.
      */
      glGenVertexArrays(1, &this->SpriteVAO);
      glGenBuffers(1, &VBO);

      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
      glBindVertexArray(this->SpriteVAO);
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