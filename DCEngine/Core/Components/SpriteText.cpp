/******************************************************************************/
/*!
@file   SpriteText.cpp
@author Chen Shu, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/23/2015
@brief
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "SpriteText.h"
#include "Transform.h"
#include "EngineReference.h"
#include "../Debug/DebugGraphics.h"

namespace DCEngine {
  namespace Components {
		ShaderPtr SpriteText::mShader;
		GLuint SpriteText::mVAO, SpriteText::mVBO;

    /**************************************************************************/
    /*!
    @brief Provides the definition of this class to Zilch.
    @note This can only go in the translational unit (.cpp)
    */
    /**************************************************************************/
    #if(DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDefineType(SpriteText, "SpriteText", DCEngineCore, builder, type) {
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(SpriteText);
      DCE_BINDING_DEFINE_RESOURCE_ATTRIBUTE(Font);
      DCE_BINDING_PROPERTY_DEFINE_UNSIGNED;
      DCE_BINDING_INTERNAL_COMPONENT_SET_ATTRIBUTE_RESOURCE;
      // Properties
      DCE_BINDING_DEFINE_PROPERTY(SpriteText, Visible);
      
      DCE_BINDING_DEFINE_PROPERTY(SpriteText, Color);
      DCE_BINDING_PROPERTY_DEFINE_RANGE(Color, 0, 1);
      DCE_BINDING_PROPERTY_SET_ATTRIBUTE(propertyColor, attributeRangeColor);

      DCE_BINDING_DEFINE_PROPERTY(SpriteText, Font);
      DCE_BINDING_PROPERTY_SET_RESOURCE_ATTRIBUTE(propertyFont, attributeFont);
      DCE_BINDING_DEFINE_PROPERTY(SpriteText, FontSize);
      DCE_BINDING_PROPERTY_SET_UNSIGNED(propertyFontSize);
      DCE_BINDING_DEFINE_PROPERTY(SpriteText, Text);
      DCE_BINDING_DEFINE_PROPERTY(SpriteText, PixelsPerUnit);
      DCE_BINDING_DEFINE_PROPERTY(SpriteText, Smoothing);
			DCE_BINDING_DEFINE_PROPERTY(SpriteText, WordWrap);
    }
    #endif

    /**************************************************************************/
    /*!
    @brief SpriteText constructor.
    */
    /**************************************************************************/
    SpriteText::SpriteText(Entity & owner) : Graphical(std::string("SpriteText"), owner)
    {
      Register();
    }

    /**************************************************************************/
    /*!
    @brief SpriteText destructor. Deregisters the SpriteText.
    */
    /**************************************************************************/
    SpriteText::~SpriteText()
    {
      // Deregister this component from the GraphicsSpace
			SpaceRef->getComponent<GraphicsSpace>()->RemoveGraphicsComponent(this);
    }

    /**************************************************************************/
    /*!
    @brief Registers the SpriteText to the graphics system.
    */
    /**************************************************************************/
    void SpriteText::Register()
    {
      // Store the reference to this owner's Transform component
      TransformComponent = dynamic_cast<GameObject*>(Owner())->getComponent<Components::Transform>();
      // Subscribe this component to the graphics space
			SpaceRef->getComponent<Components::GraphicsSpace>()->RegisterGraphicsComponent(this);
    }

    void SpriteText::Initialize()
    {
    }

		void SpriteText::Update(float dt)
		{
		}

		void SpriteText::SetUniforms(ShaderPtr shader, Camera* camera, Light* light)
		{
			if (!shader)
				shader = mShader;
			shader->Use();
			shader->SetVector4f("textColor", getColor(), true);
			auto fontName = getFont();
			auto font = Daisy->getSystem<Systems::Content>()->getFont(fontName);
			shader->SetInteger("text", 0);

			auto transform = TransformComponent;
			glm::mat4 modelMatrix;
			modelMatrix = glm::translate(modelMatrix, glm::vec3(transform->Translation.x,
				transform->Translation.y,
				transform->Translation.z));
			modelMatrix = glm::scale(modelMatrix,
				glm::vec3(transform->Scale.x / 35, transform->Scale.y / 35, 0.0f));
			shader->SetMatrix4("model", modelMatrix);
			shader->SetMatrix4("projection", camera->GetProjectionMatrix());
			shader->SetMatrix4("view", camera->GetProjectionMatrix());
		}

		static void StringWrap(std::string& string, unsigned lineSize)
		{
			if (lineSize == 0)
				return;
			for (unsigned index = lineSize; index < string.length(); index += lineSize)
			{
				unsigned tempIndex = index;
				while (string[tempIndex] != ' ')
				{
					tempIndex--;
					if (tempIndex <= index - lineSize)
						return;
				}
				index = tempIndex;
				string.at(index) = '\n';
			}
		}

		void SpriteText::Draw(void)
		{
			// Enable alpha blending for opacity.
			glBindVertexArray(mVAO);

			// Retrieve the Font resource from the content system
      auto fontName = getFont();
      auto font = Daisy->getSystem<Systems::Content>()->getFont(fontName);

			// (!) This is used to advance cursors
			GLfloat x = 0;//static_cast<GLfloat>(TransformComponent->Translation.x);
			GLfloat y = 0;
			std::string::const_iterator c;
			unsigned charCount = 0;
			std::string text = getText();
			StringWrap(text, getWordWrap());
			for (c = text.begin(); c != text.end(); ++c)
			{
				if (*c == '\n')
				{
					x = 0;
					y -= 45 * FontSize / 12;
					continue;
				}
				// Access a character glyph from the characters map
				Character ch = font->Characters[*c];

				// Calculate the origin position of the quad 
				GLfloat xPos = x + ch.Bearing.x;
				GLfloat yPos = y - (ch.Size.y - ch.Bearing.y);
				// Calculate the quad's size
				GLfloat w = ch.Size.x * FontSize / 12.0f;
				GLfloat h = ch.Size.y * FontSize / 12.0f;
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
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, ch.CharacterTextureID);

				// Update content of VBO memory
				glBindBuffer(GL_ARRAY_BUFFER, mVBO);
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				// Update quad
				glDrawArrays(GL_TRIANGLES, 0, 6);
				// Advance cursors for next glyph (Advance is number of 1/64 pixels)
				x += float(ch.Advance) * FontSize / 12 / 64;
			}
			// Unbind
			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
  }

}