/******************************************************************************/
/*!
\file   GraphicsGL_Draw.cpp
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   7/30/2015
\brief  The implementation of the Sprite drawing functions.

@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/

#include "GraphicsGL.h"

#include "../../Debug/DebugGraphics.h"
// (!) Should these be included? Perhaps only the data needed should be passed in.
#include "../../Components/EngineReference.h"
#include "../../Components/Sprite.h"
#include "../../Components/SpriteText.h"
#include "../../Components/Transform.h"
#include "../../Components/GraphicsSpace.h"
#include "../../Components/CameraViewport.h"
#include "../../Components/Camera.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    \brief  Draws geometry using Arrays.
    \param  The handle to the Vertex Array Object.
    \param  The number of vertices.
    \param  The mode with which to draw.
    */
    /**************************************************************************/
    void GraphicsGL::DrawArrays(GLuint vao, GLuint numVertices, GLenum drawMode)
    {
      // Bind the vertex array
      glBindVertexArray(vao);
      // Draw the array
      glDrawArrays(drawMode, 0, numVertices);
      // Unbind the vertex array
      glBindVertexArray(0);
    }

    /**************************************************************************/
    /*!
    \brief  Draws geometry using Elements.
    \param  The handle to the Vertex Array Object.
    \param  The number of vertices.
    \param  The mode with which to draw.
    */
    /**************************************************************************/
    void GraphicsGL::DrawElements(GLuint vao, GLuint numVertices, GLenum drawMode)
    {
      // Bind the vertex array
      glBindVertexArray(vao);
      // Draw the elements
      glDrawElements(drawMode, numVertices, GL_UNSIGNED_INT, 0);
      // Unbind the vertex array
      glBindVertexArray(0);
    }

    /**************************************************************************/
    /*!
    @brief Draws a sprite on screen.
    @param A reference to the GameObject with the sprite component.
    @note
    */
    /**************************************************************************/
    void GraphicsGL::DrawSprite(Components::Sprite& sprite, Components::Camera& camera, float dt) {
      AnimationUpdate(sprite, dt);
      this->SpriteShader->SetInteger("isTexture", 1);
      //DCTrace << "GraphicsGL::DrawSprite - Drawing " << gameObj.Name() << "\n";
      //glEnable(GL_CULL_FACE);
      //glEnable(GL_BLEND);
      //glEnable(GL_TEXTURE_2D);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      this->SpriteShader->Use();

      // Retrieve the 'SpriteSource' resource from the content system
      auto spriteSrc = Daisy->getSystem<Content>()->getSpriteSrc(sprite.SpriteSource);


      // We use transform data for drawing:
      auto transform = sprite.TransformComponent;

      // Create the matrix of the transform
      GLfloat verticesOffset = 0.5f;
      glm::mat4 modelMatrix;

      // Matrices
      modelMatrix = glm::translate(modelMatrix, glm::vec3(transform->Translation.x,
        transform->Translation.y,
        transform->Translation.z));
	  if (sprite.FlipX == true)
	  {
		  this->SpriteShader->SetInteger("flipx", 1);
	  }
	  else
	  {
		  this->SpriteShader->SetInteger("flipx", 0);
	  }

	  if (sprite.FlipY == true)
	  {
		  this->SpriteShader->SetInteger("flipy", 1);
	  }
	  else
	  {
		  this->SpriteShader->SetInteger("flipy", 0);
	  }
      modelMatrix = glm::rotate(modelMatrix, transform->Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
      modelMatrix = glm::rotate(modelMatrix, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
      modelMatrix = glm::scale(modelMatrix, glm::vec3(transform->Scale.x,
        transform->Scale.y,
        0.0f));


      // Update the uniforms in the shader to this particular sprite's data 
      this->SpriteShader->SetMatrix4("model", modelMatrix);
      this->SpriteShader->SetVector4f("spriteColor", sprite.Color);
      this->SpriteShader->SetFloat("CutMinX", (float)spriteSrc->MinX / spriteSrc->PicWidth);
      this->SpriteShader->SetFloat("CutMaxX", (float)spriteSrc->MaxX / spriteSrc->PicWidth);
      this->SpriteShader->SetFloat("CutMinY", (float)spriteSrc->MinY / spriteSrc->PicHeight);
      this->SpriteShader->SetFloat("CutMaxY", (float)spriteSrc->MaxY / spriteSrc->PicHeight);


      // Set the active texture
      glActiveTexture(GL_TEXTURE0); // Used for 3D???
      spriteSrc->getTexture().Bind();
      //this->SpriteShader->SetInteger("image", spriteSrc->getTexture().TextureID); // WHAT DO?
      DrawArrays(this->SpriteVAO, 6, GL_TRIANGLES);
    }

    /**************************************************************************/
    /*!
    @brief Draws a SpriteText on screen.
    @param A reference to the SpriteText component.
    @param A reference to the space's viewport camera.
    @note  To render each character, we extract the corresponding Character
    struct from the Characters map and calculate the quad dimensions
    using the character's metrics.
    */
    /**************************************************************************/
    void GraphicsGL::DrawSpriteText(Components::SpriteText & st, Components::Camera & camera)
    {
      return;
      // Enable alpha blending for opacity.
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      // Activate the SpriteText shader
      this->SpriteTextShader->Use();
      this->SpriteTextShader->SetVector4f("textColor", st.getColor());

      glActiveTexture(GL_TEXTURE0);
      if (auto a = Debug::CheckOpenGLError())
        DCTrace << "GraphicsGL::DrawSpriteText - Failed to set active texture!\n";
      glBindVertexArray(SpriteTextVAO);
      if (Debug::CheckOpenGLError())
        DCTrace << "GraphicsGL::DrawSpriteText - Failed to bind vertex array!\n";

      // Retrieve the Font resource from the content system
      auto font = Daisy->getSystem<Content>()->getFont(std::string(st.getFont()));

      // (!) This is used to advance cursors
      GLfloat x = static_cast<GLfloat>(st.TransformComponent->Translation.x);


      // Iterate through all the characters
      std::string::const_iterator c;
      for (c = st.getText().begin(); c != st.getText().end(); ++c) {
        // Access a character glyph from the characters map
        Character ch = font->Characters[*c];

        // Calculate the origin position of the quad 
        GLfloat xPos = x + ch.Bearing.x * st.getFontSize();
        GLfloat yPos = st.TransformComponent->Translation.y - (ch.Size.y - ch.Bearing.y) * st.getFontSize();
        // Calculate the quad's size
        GLfloat w = ch.Size.x * static_cast<GLfloat>(st.getFontSize());
        GLfloat h = ch.Size.y * static_cast<GLfloat>(st.getFontSize());
        // Generate a set of 6 vertices to form the 2D quad
        GLfloat vertices[6][4] = {
          { xPos    , yPos + h, 0.0, 0.0 },
          { xPos    , yPos    , 0.0, 1.0 },
          { xPos + w, yPos    , 1.0, 1.0 },

          { xPos    , yPos + h, 0.0, 0.0 },
          { xPos + w, yPos    , 1.0, 1.0 },
          { xPos + w, yPos + h, 1.0, 0.0 },
        };

        // Update glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.CharacterTextureID);
        if (Debug::CheckOpenGLError())
          DCTrace << "GraphicsGL::DrawSpriteText - Failed to bind texture!\n";

        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, SpriteTextVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Update quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Advance cursors for next glyph (Advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * st.getFontSize();
      }

      // Unbind
      glBindVertexArray(0);
      glBindTexture(GL_TEXTURE_2D, 0);
    }


    /**************************************************************************/
    /*!
    \brief Sets the shader's camera projection matrix and view uniforms.
    \param A (smart) pointer to the shader object.
    \param A reference to camera object.
    \note
    */
    /**************************************************************************/
    void GraphicsGL::SetShaderProjViewUniforms(ShaderPtr shader, Components::Camera& camera) {

      auto camTrans = camera.Owner()->getComponent<Components::Transform>();

      // (???) Sets the "image" uniform to 0
      shader->SetInteger("image", 0);
      // Set the projection matrix
      shader->SetMatrix4("projection", camera.GetProjectionMatrix());
      // Set the view matrix
      shader->SetMatrix4("view", camera.GetViewMatrix());
    }

    /*!************************************************************************\
    @brief  Sets the Sprite's Shaders Uniforms.
    \**************************************************************************/
    void GraphicsGL::SetSpriteShader(Components::Camera& camera)
    {
      SetShaderProjViewUniforms(this->SpriteShader, camera);
      // Enable alpha blending for opacity.
    }

    /*!************************************************************************\
    @brief  Sets the SpriteText's Shader Uniforms.
    \**************************************************************************/
    void GraphicsGL::SetSpriteTextShader(Components::Camera& camera)
    {
      SetShaderProjViewUniforms(this->SpriteTextShader, camera);
    }

    /**************************************************************************/
    /*!
    @brief  Runs the current animation update.
    @todo   Currently hard-coded the ColumnCount.
    */
    /**************************************************************************/
    void GraphicsGL::AnimationUpdate(Components::Sprite& sprite, float dt)
    {
      auto spriteSrc = Daisy->getSystem<Content>()->getSpriteSrc(sprite.SpriteSource);
      // CHANGE THIS
      Daisy->getSystem<Content>()->getSpriteSrc(sprite.SpriteSource)->ColumnCount = 8;
      Daisy->getSystem<Content>()->getSpriteSrc(sprite.SpriteSource)->RowCount = 1;
      //Animation update
      this->SpriteShader->SetInteger("isAnimaitonActivated", 0);
      if (sprite.HaveAnimation == true)//Check whether it has animation
      {
        if (spriteSrc->ColumnCount == 0 || spriteSrc->RowCount == 0)//Check whether the number of frames if 0
        {
          if (spriteSrc->ColumnCount == 0)
          {
            DCTrace << "GraphicsGL::DrawSprite - Sprite - Animation - Total Column is 0, but still enabled HaveAnimation" << "\n";
          }
          else
          {
            DCTrace << "GraphicsGL::DrawSprite - Sprite - Animation - Total Row is 0, but still enabled HaveAnimation" << "\n";
          }
        }
        else
        {
          if (sprite.UpdateAnimationSpeed())//Check whether the animation speed is 0
          {
            if (sprite.CheckAnimationIntialized() == false)
            {
              this->SpriteShader->SetInteger("isAnimaitonActivated", 1);
              this->SpriteShader->SetFloat("columnLength", (float)1 / spriteSrc->ColumnCount);
              this->SpriteShader->SetFloat("rowHeight", (float)1 / spriteSrc->RowCount);
              this->SpriteShader->SetInteger("currentColumn", sprite.StartColumn);
              this->SpriteShader->SetInteger("currentRow", sprite.StartRow);
            }
            else
            {
              if (sprite.AnimationActive == true)
              {
                sprite.IncreaseAnimationCounter(dt);
                IsNextFrame(sprite);
              }
              this->SpriteShader->SetInteger("isAnimaitonActivated", 1);
              this->SpriteShader->SetFloat("columnLength", (float)1 / spriteSrc->ColumnCount);
              this->SpriteShader->SetFloat("rowHeight", (float)1 / spriteSrc->RowCount);
              //DCTrace << (float)1 / sprite.TotalFrames << "\n";
              //DCTrace << sprite.currentColumn << "\n";
              this->SpriteShader->SetInteger("currentColumn", sprite.CurrentColumn);
              //DCTrace << sprite.CurrentColumn << sprite.CurrentRow << "\n";
              this->SpriteShader->SetInteger("currentRow", sprite.CurrentRow);
            }
          }
        }
      }
    }

    int GraphicsGL::IsNextFrame(Components::Sprite& sprite)
    {
      if (sprite.GetAnimationSpeedFPSCounter() >= sprite.GetAnimationSpeedFPS())
      {
        sprite.ResetSpeedCounter();
        sprite.CurrentColumn++;
        //Check whether it reaches the next line.
        if (sprite.CurrentColumn >= Daisy->getSystem<Content>()->getSpriteSrc(sprite.SpriteSource)->ColumnCount)
        {
          sprite.CurrentRow++;
          sprite.CurrentColumn = 0;
          if (sprite.CurrentRow >= Daisy->getSystem<Content>()->getSpriteSrc(sprite.SpriteSource)->RowCount)
          {
            sprite.CurrentRow = 0;
          }
        }
        //Current frame started from 0
        return 1;
      }
      else
      {
        return 0;
      }
    }


  }
}
