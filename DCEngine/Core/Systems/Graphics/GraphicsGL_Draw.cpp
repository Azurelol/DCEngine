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
		//void GraphicsGL::DrawSprite(Components::Sprite& sprite, Components::Camera& camera, float dt) {
		//	AnimationUpdate(sprite, dt);
		//	SpriteShader->SetInteger("isTexture", 1);
		//	//DCTrace << "GraphicsGL::DrawSprite - Drawing " << gameObj.Name() << "\n";
		//	//glEnable(GL_CULL_FACE);
		//	glEnable(GL_BLEND);
		//	//glEnable(GL_TEXTURE_2D);
		//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//	//this->SpriteShader->Use();
		//
		//	// Retrieve the 'SpriteSource' resource from the content system
		//	auto spriteSrc = Daisy->getSystem<Content>()->getSpriteSrc(sprite.SpriteSource);
		//
		//
		//	// We use transform data for drawing:
		//	auto transform = sprite.TransformComponent;
		//
		//	// Create the matrix of the transform
		//	GLfloat verticesOffset = 0.5f;
		//	glm::mat4 modelMatrix;
		//
		//	// Matrices
		//	modelMatrix = glm::translate(modelMatrix, glm::vec3(transform->Translation.x,
		//		transform->Translation.y,
		//		transform->Translation.z));
		//	if (sprite.FlipX == true)
		//	{
		//		SpriteShader->SetInteger("flipx", 1);
		//	}
		//	else
		//	{
		//		SpriteShader->SetInteger("flipx", 0);
		//	}
		//
		//	if (sprite.FlipY == true)
		//	{
		//		SpriteShader->SetInteger("flipy", 1);
		//	}
		//	else
		//	{
		//		SpriteShader->SetInteger("flipy", 0);
		//	}
		//	modelMatrix = glm::rotate(modelMatrix, transform->Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		//	modelMatrix = glm::rotate(modelMatrix, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		//	modelMatrix = glm::scale(modelMatrix, glm::vec3(transform->Scale.x,
		//		transform->Scale.y,
		//		0.0f));
		//
		//
		//	// Update the uniforms in the shader to this particular sprite's data 
		//	SpriteShader->SetMatrix4("model", modelMatrix);
		//	SpriteShader->SetVector4f("spriteColor", sprite.Color);
		//	SpriteShader->SetFloat("CutMinX", (float)spriteSrc->MinX / spriteSrc->PicWidth);
		//	SpriteShader->SetFloat("CutMaxX", (float)spriteSrc->MaxX / spriteSrc->PicWidth);
		//	SpriteShader->SetFloat("CutMinY", (float)spriteSrc->MinY / spriteSrc->PicHeight);
		//	SpriteShader->SetFloat("CutMaxY", (float)spriteSrc->MaxY / spriteSrc->PicHeight);
		//
		//
		//	// Set the active texture
		//	glActiveTexture(GL_TEXTURE0); // Used for 3D???
		//	spriteSrc->getTexture().Bind();
		//	//this->SpriteShader->SetInteger("image", spriteSrc->getTexture().TextureID); // WHAT DO?
		//	DrawArrays(SpriteVAO, 6, GL_TRIANGLES);
		//}


		/**************************************************************************/
		/*!
		@brief Inserts newlines based on specified lineSize
		@param The string to edit
		@param The max character count per line
		@note  To render each character, we extract the corresponding Character
		struct from the Characters map and calculate the quad dimensions
		using the character's metrics.
		*/
		/**************************************************************************/
		//static void StringWrap(std::string& string, unsigned lineSize)
		//{
		//	if (lineSize == 0)
		//		return;
		//	for (unsigned index = lineSize; index < string.length(); index += lineSize)
		//	{
		//		unsigned tempIndex = index;
		//		while (string[tempIndex] != ' ')
		//		{
		//			tempIndex--;
		//			if (tempIndex <= index - lineSize)
		//				return;
		//		}
		//		index = tempIndex;
		//		string.at(index) = '\n';
		//	}
		//}

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
		//void GraphicsGL::DrawSpriteText(Components::SpriteText & st, Components::Camera & camera)
		//{
		//	// Enable alpha blending for opacity.
		//	glEnable(GL_BLEND);
		//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//	// Activate the SpriteText shader
		//	//this->SpriteTextShader->Use();
		//	SpriteTextShader->SetVector4f("textColor", st.getColor());
		//	glActiveTexture(GL_TEXTURE0);
		//	//if (Debug::CheckOpenGLError())
		//	//	DCTrace << "GraphicsGL::DrawSpriteText - Failed to set active texture!\n";
		//	glBindVertexArray(SpriteTextVAO);
		//	//if (Debug::CheckOpenGLError())
		//	//	DCTrace << "GraphicsGL::DrawSpriteText - Failed to bind vertex array!\n";
		//
		//	// Retrieve the Font resource from the content system
    //  auto fontName = st.getFont();
		//	auto font = Daisy->getSystem<Content>()->getFont(fontName);
		//
		//	// (!) This is used to advance cursors
		//	GLfloat x = 0;//static_cast<GLfloat>(st.TransformComponent->Translation.x);
		//	GLfloat y = 0;
		//
		//	auto transform = st.TransformComponent;
		//	glm::mat4 modelMatrix;
		//	modelMatrix = glm::translate(modelMatrix, glm::vec3(transform->Translation.x,
		//		transform->Translation.y,
		//		transform->Translation.z));
		//	modelMatrix = glm::scale(modelMatrix,
		//		glm::vec3(transform->Scale.x / 35, transform->Scale.y / 35, 0.0f));
		//	SpriteTextShader->SetMatrix4("model", modelMatrix);
		//	// Iterate through all the characters
		//	std::string::const_iterator c;
		//	unsigned charCount = 0;
		//	std::string text = st.getText();
		//	StringWrap(text, st.getWordWrap());
		//	for (c = text.begin(); c != text.end(); ++c)
		//	{
		//		if (*c == '\n')
		//		{
		//			x = 0;
		//			y -= 45 * st.getFontSize() / 12;
		//			continue;
		//		}
		//		// Access a character glyph from the characters map
		//		Character ch = font->Characters[*c];
		//
		//		// Calculate the origin position of the quad 
		//		GLfloat xPos = x + ch.Bearing.x;
		//		GLfloat yPos = y - (ch.Size.y - ch.Bearing.y);
		//		// Calculate the quad's size
		//		GLfloat w = ch.Size.x * st.getFontSize() / 12.f;
		//		GLfloat h = ch.Size.y * st.getFontSize() / 12.f;
		//		// Generate a set of 6 vertices to form the 2D quad
		//		GLfloat vertices[6][4] = {
		//			{ xPos    , yPos + h, 0.0, 0.0 },
		//			{ xPos    , yPos    , 0.0, 1.0 },
		//			{ xPos + w, yPos    , 1.0, 1.0 },
		//
		//			{ xPos    , yPos + h, 0.0, 0.0 },
		//			{ xPos + w, yPos    , 1.0, 1.0 },
		//			{ xPos + w, yPos + h, 1.0, 0.0 },
		//		};
		//
		//		// Update glyph texture over quad
		//		glBindTexture(GL_TEXTURE_2D, ch.CharacterTextureID);
		//
		//		// Update content of VBO memory
		//		glBindBuffer(GL_ARRAY_BUFFER, SpriteTextVBO);
		//		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		//		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//		// Update quad
		//		glDrawArrays(GL_TRIANGLES, 0, 6);
		//		// Advance cursors for next glyph (Advance is number of 1/64 pixels)
		//		x += float(ch.Advance) * st.getFontSize() / 12 / 64;
		//
		//	}
		//	// Unbind
		//	glBindVertexArray(0);
		//	glBindTexture(GL_TEXTURE_2D, 0);
		//}

		void GraphicsGL::SetParticleSystemShader(Components::Camera & camera)
		{
			//ParticleSystemShader->Use();
			//ParticleSystemShader->SetInteger("image", 0);
			//SetShaderProjViewUniforms(ParticleSystemShader, camera);
		}


		void GraphicsGL::DrawParticles(Components::SpriteParticleSystem & particles, Components::Camera & camera, double dt)
		{
			//glDepthFunc(GL_LEQUAL);
			//glEnable(GL_BLEND);
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			//auto transform = particles.TransformComponent;
			//
			//glm::mat4 modelMatrix;
			//modelMatrix = glm::translate(modelMatrix, glm::vec3(transform->Translation.x,
			//	transform->Translation.y,
			//	transform->Translation.z));
			//modelMatrix = glm::scale(modelMatrix,
			//	glm::vec3(transform->Scale.x, transform->Scale.y, 0.0f));
			//
			////particles.Update(dt);
			//std::vector<glm::vec2> offset(particles.GetPositionData());
			//std::vector<float> scale(particles.GetScaleData());
			//std::vector<float> rotation(particles.GetRotationData());
			//std::vector<glm::vec4> color(particles.GetColorData());
			//std::vector<glm::mat4> transformData;
			//for (unsigned i = 0; i < particles.GetParticleCount(); ++i)
			//{
			//	glm::mat4 modelMatrix;
			//	modelMatrix = glm::translate(modelMatrix, glm::vec3(
			//		transform->Translation.x + offset[i].x, transform->Translation.y + offset[i].y,
			//		transform->Translation.z));
			//	modelMatrix = glm::rotate(modelMatrix, rotation[i], glm::vec3(0, 0, 1));
			//	modelMatrix = glm::scale(modelMatrix, glm::vec3(
			//		scale[i], scale[i], 0.0f));
			//	transformData.push_back(modelMatrix);
			//}
			//
			//if (particles.Visible)
			//{
			//	glBindBuffer(GL_ARRAY_BUFFER, ParticleColorInstanceVBO);
			//	glBufferSubData(GL_ARRAY_BUFFER, 0,
			//		sizeof(glm::vec4) * particles.GetParticleCount(), color.data());
			//
			//	glBindBuffer(GL_ARRAY_BUFFER, ParticleTransformInstanceVBO);
			//	glBufferSubData(GL_ARRAY_BUFFER, 0,
			//		sizeof(glm::mat4) * particles.GetParticleCount(), transformData.data());
			//	glBindBuffer(GL_ARRAY_BUFFER, 0);
			//
			//	glBindVertexArray(ParticleVAO);
			//	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, GLsizei(offset.size()));
			//	glBindVertexArray(0);
			//}
		}

		/**************************************************************************/
		/*!
		\brief Sets the shader's camera projection matrix and view uniforms.
		\param A pointer to the shader object.
		\param A reference to camera object.
		\note
		*/
    /**************************************************************************/
		void GraphicsGL::SetShaderProjViewUniforms(ShaderPtr shader, Components::Camera& camera) {
      // Set the projection matrix
			shader->SetMatrix4("projection", camera.GetProjectionMatrix());
			// Set the view matrix 
			shader->SetMatrix4("view", camera.GetViewMatrix());
		}

    /**************************************************************************/
    /*!
    \brief Sets the shader's lightning uniforms.
    \param A pointer to the shader object.
    \param A reference to lgiht component    
    */
    /**************************************************************************/
		void GraphicsGL::SetLightUniforms(ShaderPtr shader, Components::Light* light)
		{
			shader->SetInteger("useLight", true);
			
			glm::mat4 modelMatrix;
			Components::Transform* lightTransform = light->Owner()->getComponent<Components::Transform>();
			modelMatrix = glm::translate(modelMatrix, glm::vec3(lightTransform->Translation.x,
				lightTransform->Translation.y,
				lightTransform->Translation.z));
			modelMatrix = glm::rotate(modelMatrix, lightTransform->Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(lightTransform->Scale.x,
				lightTransform->Scale.y, 0.0f));

			std::string var("gLight.");
			std::string member;
			member = var + "Visible";
			shader->SetInteger(member.c_str(), light->getVisible());
			member = var + "VisibilityCulling";
			shader->SetInteger(member.c_str(), light->getVisibilityCulling());
			member = var + "VisibilityEvents";
			shader->SetInteger(member.c_str(), light->getVisibilityEvents());
			member = var + "CastShadows";
			shader->SetInteger(member.c_str(), light->getCastShadows());
			member = var + "LightType";
			shader->SetInteger(member.c_str(), light->getTypeAsInt());
			member = var + "Color";
			shader->SetVector4f(member.c_str(), light->getColor());
			member = var + "Intensity";
			shader->SetFloat(member.c_str(), light->getIntensity());
			member = var + "Range";
			shader->SetFloat(member.c_str(), light->getRange());
			member = var + "Falloff";
			shader->SetFloat(member.c_str(), light->getFalloff());
			member = var + "Direction";
			shader->SetVector3f(member.c_str(), light->getDirectionVector());
			member = var + "InnerAngle";
			shader->SetFloat(member.c_str(), light->getInnerAngle() * 3.141593f / 360.0f);
			member = var + "OuterAngle";
			shader->SetFloat(member.c_str(), light->getOuterAngle() * 3.141593f / 360.0f);
			member = var + "Position";
			shader->SetVector3f(member.c_str(), lightTransform->Translation);
			member = var + "Model";
			shader->SetMatrix4(member.c_str(), modelMatrix);
		}

		/*!************************************************************************\
		@brief  Sets the Sprite's Shaders Uniforms.
		\**************************************************************************/
		void GraphicsGL::SetSpriteShader(Components::Camera& camera, Components::Light* light)
		{
			SpriteShader->Use();
			SpriteShader->SetInteger("image", 0);

			SetLightUniforms(SpriteShader, light);
			SetShaderProjViewUniforms(SpriteShader, camera);
			// Enable alpha blending for opacity.
		}

		void GraphicsGL::SetAllLightUniforms(ShaderPtr shader, const std::vector<Components::Light*>& lightComponents)
		{
			shader->SetInteger("numLights", lightComponents.size());
			SpriteShader->SetInteger("numLights", lightComponents.size());
			std::string var;
			for (unsigned i = 0; i < lightComponents.size(); ++i)
			{
				if (i >= 20)
					break;
				Components::Transform* lightTransform = lightComponents[i]->Owner()->getComponent<Components::Transform>();
				std::stringstream sStream;
				glm::mat4 modelMatrix;
				modelMatrix = glm::translate(modelMatrix, glm::vec3(lightTransform->Translation.x,
					lightTransform->Translation.y,
					lightTransform->Translation.z));
				modelMatrix = glm::rotate(modelMatrix, lightTransform->Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
				//modelMatrix = glm::rotate(modelMatrix, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
				modelMatrix = glm::scale(modelMatrix, glm::vec3(lightTransform->Scale.x,
					lightTransform->Scale.y, 0.0f));
				
				sStream << "Lights[" << i << "].";
				var = sStream.str() + "Visible";
				SpriteShader->SetInteger(var.c_str(), lightComponents[i]->getVisible());
				var = sStream.str() + "VisibilityCulling";
				SpriteShader->SetInteger(var.c_str(), lightComponents[i]->getVisibilityCulling());
				var = sStream.str() + "VisibilityEvents";
				SpriteShader->SetInteger(var.c_str(), lightComponents[i]->getVisibilityEvents());
				var = sStream.str() + "CastShadows";
				SpriteShader->SetInteger(var.c_str(), lightComponents[i]->getCastShadows());
				var = sStream.str() + "LightType";
		    SpriteShader->SetInteger(var.c_str(), lightComponents[i]->getTypeAsInt());
		    //SpriteShader->SetInteger(var.c_str(), lightComponents[i]->getType());
				var = sStream.str() + "Color";
				SpriteShader->SetVector4f(var.c_str(), lightComponents[i]->getColor());
				var = sStream.str() + "Intensity";
				SpriteShader->SetFloat(var.c_str(), lightComponents[i]->getIntensity());
				var = sStream.str() + "Range";
				SpriteShader->SetFloat(var.c_str(), lightComponents[i]->getRange());
				var = sStream.str() + "Falloff";
				SpriteShader->SetFloat(var.c_str(), lightComponents[i]->getFalloff());
				var = sStream.str() + "Direction";
				SpriteShader->SetVector3f(var.c_str(), lightComponents[i]->getDirectionVector());
				var = sStream.str() + "InnerAngle";
				SpriteShader->SetFloat(var.c_str(), lightComponents[i]->getInnerAngle() * 3.141593f / 360.0f);
				var = sStream.str() + "OuterAngle";
				SpriteShader->SetFloat(var.c_str(), lightComponents[i]->getOuterAngle() * 3.141593f / 360.0f);
				var = sStream.str() + "Position";
				SpriteShader->SetVector3f(var.c_str(), lightTransform->Translation);
				var = sStream.str() + "Model";
				SpriteShader->SetMatrix4(var.c_str(), modelMatrix);
			}

		}

    /**************************************************************************/
    /*!
    @brief Sets the SpriteText shaders.
    @param camera A reference to the camera component.
    */
    /**************************************************************************/
		void GraphicsGL::SetSpriteTextShader(Components::Camera& camera)
		{
			SpriteTextShader->Use();
			SetShaderProjViewUniforms(SpriteTextShader, camera);
		}

    /**************************************************************************/
    /*!
    @brief Sets the shadowing shaders.
    @param camera A reference to the camera component.
    @param light A reference to the light component.
    */
    /**************************************************************************/
		void GraphicsGL::SetShadowingShaders(Components::Camera& camera, Components::Light* light)
		{
			ShadowingShader->Use();
			SetLightUniforms(ShadowingShader, light);
			SetShaderProjViewUniforms(ShadowingShader, camera);
		}

		/**************************************************************************/
		/*!
		@brief  Runs the current animation update.
		@todo   Currently hard-coded the ColumnCount.
		*/
		/**************************************************************************/
		//void GraphicsGL::AnimationUpdate(Components::Sprite& sprite, float dt)
		//{
		//	auto spriteSrc = Daisy->getSystem<Content>()->getSpriteSrc(sprite.SpriteSource);
		//	// CHANGE THIS
		//	Daisy->getSystem<Content>()->getSpriteSrc(sprite.SpriteSource)->ColumnCount;
		//	Daisy->getSystem<Content>()->getSpriteSrc(sprite.SpriteSource)->RowCount;
		//	//Animation update
		//	this->SpriteShader->SetInteger("isAnimaitonActivated", 0);
		//	sprite.HaveAnimation = sprite.AnimationActive;
		//	if (sprite.HaveAnimation == true)//Check whether it has animation
		//	{
		//		if (spriteSrc->ColumnCount == 0 || spriteSrc->RowCount == 0)//Check whether the number of frames if 0
		//		{
		//			if (spriteSrc->ColumnCount == 0)
		//			{
		//				DCTrace << "GraphicsGL::DrawSprite - Sprite - Animation - Total Column is 0, but still enabled HaveAnimation" << "\n";
		//			}
		//			else
		//			{
		//				DCTrace << "GraphicsGL::DrawSprite - Sprite - Animation - Total Row is 0, but still enabled HaveAnimation" << "\n";
		//			}
		//		}
		//		else
		//		{
		//			if (sprite.UpdateAnimationSpeed())//Check whether the animation speed is 0
		//			{
		//				if (sprite.CheckAnimationIntialized() == false)
		//				{
		//					this->SpriteShader->SetInteger("isAnimaitonActivated", 1);
		//					this->SpriteShader->SetFloat("columnLength", (float)1 / spriteSrc->ColumnCount);
		//					this->SpriteShader->SetFloat("rowHeight", (float)1 / spriteSrc->RowCount);
		//					this->SpriteShader->SetInteger("currentColumn", sprite.StartColumn);
		//					this->SpriteShader->SetInteger("currentRow", sprite.StartRow);
		//				}
		//				else
		//				{
		//					if (sprite.AnimationActive == true)
		//					{
		//						sprite.IncreaseAnimationCounter(dt);
		//						IsNextFrame(sprite);
		//					}
		//					this->SpriteShader->SetInteger("isAnimaitonActivated", 1);
		//					this->SpriteShader->SetFloat("columnLength", (float)1 / spriteSrc->ColumnCount);
		//					this->SpriteShader->SetFloat("rowHeight", (float)1 / spriteSrc->RowCount);
		//					//DCTrace << (float)1 / sprite.TotalFrames << "\n";
		//					//DCTrace << sprite.currentColumn << "\n";
		//					this->SpriteShader->SetInteger("currentColumn", sprite.CurrentColumn);
		//					//DCTrace << sprite.CurrentColumn << sprite.CurrentRow << "\n";
		//					this->SpriteShader->SetInteger("currentRow", sprite.CurrentRow);
		//				}
		//			}
		//		}
		//	}
		//}

		//int GraphicsGL::IsNextFrame(Components::Sprite& sprite)
		//{
		//	if (sprite.GetAnimationSpeedFPSCounter() >= sprite.GetAnimationSpeedFPS())
		//	{
		//		sprite.ResetSpeedCounter();
		//		sprite.CurrentColumn++;
		//		//Check whether it reaches the next line.
		//		if (sprite.CurrentColumn >= Daisy->getSystem<Content>()->getSpriteSrc(sprite.SpriteSource)->ColumnCount)
		//		{
		//			sprite.CurrentRow++;
		//			sprite.CurrentColumn = 0;
		//			if (sprite.CurrentRow >= Daisy->getSystem<Content>()->getSpriteSrc(sprite.SpriteSource)->RowCount)
		//			{
		//				sprite.CurrentRow = 0;
		//			}
		//		}
		//		//Check If it is go into void frame
		//		if ((Daisy->getSystem<Content>()->getSpriteSrc(sprite.SpriteSource)->TotalFrame != 0) &&
		//			(sprite.CurrentColumn + Daisy->getSystem<Content>()->getSpriteSrc(sprite.SpriteSource)->ColumnCount * sprite.CurrentRow >= Daisy->getSystem<Content>()->getSpriteSrc(sprite.SpriteSource)->TotalFrame))
		//		{
		//			sprite.CurrentColumn = 0;
		//			sprite.CurrentRow = 0;
		//			return 1;
		//		}
		//		//Current frame started from 0
		//		return 1;
		//	}
		//	else
		//	{
		//		return 0;
		//	}
		//}


	}
}
