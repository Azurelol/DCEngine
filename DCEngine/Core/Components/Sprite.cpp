/******************************************************************************/
/*!
@file   Sprite.cpp
@author Chen Shu, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/11/2015
@brief  The Sprite component allows the graphical representation of this object
in the world space through the drawing of sprites.

@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/

#include "Sprite.h"
#include "EngineReference.h"
#include <GLM\gtc\matrix_transform.hpp>
#include "../Engine/Engine.h"
#include "../Systems/Content/Content.h"
#include "../Systems/Graphics/GraphicsGL.h"
#include "../Debug/DebugGraphics.h"

namespace DCEngine {
  namespace Components
  {
		ShaderPtr Sprite::mShader;
		GLuint Sprite::mVAO;
    DCE_COMPONENT_DEFINE_DEPENDENCIES(Sprite, "Transform");
    
    /**************************************************************************/
    /*!
    @brief Provides the definition of this class to Zilch.
    @note This can only go in the translational unit (.cpp)
    */
    /**************************************************************************/
    #if(DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDefineType(Sprite, "Sprite", DCEngineCore, builder, type) {
      // Constructor / Destructor
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(Sprite);
      // Properties
      DCE_BINDING_PROPERTY_DEFINE_UNSIGNED;
      DCE_BINDING_DEFINE_RESOURCE_ATTRIBUTE(SpriteSource);
      DCE_BINDING_DEFINE_PROPERTY(Sprite, Visible);

      DCE_BINDING_DEFINE_PROPERTY(Sprite, Color);
      DCE_BINDING_PROPERTY_DEFINE_RANGE(Color, 0, 1);      
      DCE_BINDING_PROPERTY_SET_ATTRIBUTE(propertyColor, attributeRangeColor);

      DCE_BINDING_DEFINE_PROPERTY(Sprite, SpriteSource);
      DCE_BINDING_PROPERTY_SET_RESOURCE_ATTRIBUTE(propertySpriteSource, attributeSpriteSource);
      DCE_BINDING_DEFINE_PROPERTY(Sprite, FlipX);
      DCE_BINDING_DEFINE_PROPERTY(Sprite, FlipY);
      DCE_BINDING_DEFINE_PROPERTY(Sprite, AnimationActive);
      DCE_BINDING_DEFINE_PROPERTY(Sprite, AnimationSpeed);
			DCE_BINDING_DEFINE_PROPERTY(Sprite, DrawLayer);
      DCE_BINDING_PROPERTY_DEFINE_RANGE(DrawLayer, 0, 5);
      DCE_BINDING_PROPERTY_SET_ATTRIBUTE(propertyDrawLayer, attributeRangeDrawLayer);
      DCE_BINDING_PROPERTY_SET_UNSIGNED(propertyDrawLayer);
    }
    #endif

    /**************************************************************************/
    /*!
    \fn DCEngine::Sprite::Sprite(Entity& owner)

    \brief Default constructor for the Sprite component.

    \param owner
      The owner of sprite.

    \return
      None.
    */
    /**************************************************************************/
    Sprite::Sprite(Entity& owner) : Graphical(std::string("Sprite"), owner), DrawLayer(0) 
                   {
      // Register this component to the GraphicsSpace so that it can be drawn
      // by the graphics system.

      if (this->HasDependencies())
        Register();
    }

    /**************************************************************************/
    /*!
    \fn DCEngine::Sprite::~Sprite()

    \brief Sprite Destructor.

    \return
      None.
    */
    /**************************************************************************/
    Sprite::~Sprite()
    {
      // Deregister this component from the GraphicsSpace
			SpaceRef->getComponent<GraphicsSpace>()->RemoveGraphicsComponent(this);
    }

    /**************************************************************************/
    /*!
    \fn void DCEngine::Sprite::Initialize()

    \brief Initializes the Sprite component, registering it into the graphics
           space for drawing and initializing with a default SpriteSource.

    \return
      None.
    */
    /**************************************************************************/
    void Sprite::Initialize() {
      if (DCE_TRACE_COMPONENT_INITIALIZE)
        DCTrace << Owner()->Name() << "::" << ObjectName << "::Initialize\n";
    }

    void Sprite::Register()
    {
      // Store the reference to this owner's Transform component
      TransformComponent = dynamic_cast<GameObject*>(Owner())->getComponent<Components::Transform>();
      // Register
      SpaceRef->getComponent<Components::GraphicsSpace>()->RegisterGraphicsComponent(this);
    }

    /**************************************************************************/
    /*!
    \fn void DCEngine::Sprite::SetColorUsing255(Vec3 newColor)

    \brief Sets the color using 255 as the max instead of 1. Preserves opacity.

    \param newColor
    The color in range 0.0f - 1.0f.

    \return
      None.
    */
    /**************************************************************************/
    void Sprite::SetColorUsing255(Vec3 newColor)
    {
      DCTrace << "Color passed into 255 function = " << newColor.x << " " << newColor.y << " " << newColor.z << ".\n";
      Color = Vec4(newColor.x / 255.0f, newColor.y / 255.0f, newColor.z / 255.0f, Color.w);
      DCTrace << "Color set using 255 function = " << Color.x << " " << Color.y << " " << Color.z << " " << Color.w << ".\n";
    }

		void Sprite::Update(float dt)
		{
			auto spriteSrc = Daisy->getSystem<Systems::Content>()->getSpriteSrc(SpriteSource);
			//Animation update
			if (UpdateAnimationSpeed())//Check whether the animation speed is 0
			{
				if (AnimationActive == true)
				{
					AnimationSpeedFPSCounter += dt;
					if (AnimationSpeedFPSCounter >= AnimationSpeedFPS)
					{
						AnimationSpeedFPSCounter = 0;
						CurrentColumn++;
						//Check whether it reaches the next line.
						if (CurrentColumn + spriteSrc->ColumnCount * CurrentRow >= spriteSrc->TotalFrame)
						{
							if (spriteSrc->Looping)
							{
								CurrentColumn = 0;
								CurrentRow = 0;
							}
							else
								AnimationActive = false;
						}
						else if (CurrentColumn >= spriteSrc->ColumnCount)
						{
							CurrentRow++;
							CurrentColumn = 0;
							if (CurrentRow >= spriteSrc->RowCount)
								CurrentRow = 0;
						}
					}
				}
			}
		}

		void Sprite::SetUniforms(ShaderPtr shader, Camera* camera, Light* light)
		{
			if (!shader)
			{
				silhouette = false;
				shader = mShader;
			}
			else silhouette = true;
			shader->Use();

			shader->SetInteger("isTexture", 1);
			//set matrix uniforms
			auto transform = TransformComponent;
			glm::mat4 modelMatrix;
			// Matrices
			modelMatrix = glm::translate(modelMatrix, glm::vec3(transform->Translation.x,
				transform->Translation.y,
				transform->Translation.z));
			modelMatrix = glm::rotate(modelMatrix, transform->Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(transform->Scale.x,
				transform->Scale.y, 1.0f));
			shader->SetMatrix4("model", modelMatrix);

			glm::mat4 rotationMatrix;
			rotationMatrix = glm::rotate(rotationMatrix, transform->Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
			shader->SetMatrix4("rotation", rotationMatrix);

			auto spriteSrc = Daisy->getSystem<Systems::Content>()->getSpriteSrc(SpriteSource);
			if (FlipX == true)
				shader->SetInteger("flipx", 1);
			else
				shader->SetInteger("flipx", 0);

			if (FlipY == true)
				shader->SetInteger("flipy", 1);
			else
				shader->SetInteger("flipy", 0);
			shader->SetVector4f("spriteColor", Color);
			shader->SetInteger("image", 0);
			glActiveTexture(GL_TEXTURE0); // Used for 3D???
			spriteSrc->getTexture().Bind();
			shader->SetInteger("totalColumns", spriteSrc->ColumnCount);
			shader->SetInteger("totalRows", spriteSrc->RowCount);
			if (AnimationActive == true)//Check whether it has animation
			{
				if (CheckAnimationIntialized() == false)
				{
					shader->SetInteger("currentColumn", StartColumn);
					shader->SetInteger("currentRow", StartRow);
				}
				else
				{
					shader->SetInteger("currentColumn", CurrentColumn);
					shader->SetInteger("currentRow", CurrentRow);
				}
			}
			else
			{
				shader->SetInteger("currentColumn", 0);
				shader->SetInteger("currentRow", 0);
			}
			// Set the projection matrix
			shader->SetMatrix4("projection", camera->GetProjectionMatrix());
			// Set the view matrix 
			shader->SetMatrix4("view", camera->GetViewMatrix());
		}

		void Sprite::Draw(void)
		{
      // Skip drawing if visible is false...
      if (!Visible)
        return;

			glBindVertexArray(mVAO);
			if(silhouette)
				glDrawArrays(GL_LINE_LOOP, 0, 4);
			else
				glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			glBindVertexArray(0);
		}

		void Sprite::SetShader()
		{
			mShader->Use();
		}

		//unsigned Sprite::GetDrawLayer(void)
		//{
		//	return DrawLayer;
		//}

    /**************************************************************************/
    /*!
    \fn void DCEngine::Sprite::UpdateSprite()

    \brief Updates the sprite on every update.

    \return
      None.
    */
    /**************************************************************************/
    void Sprite::UpdateSprite()
    {
     
    }

  }
}
