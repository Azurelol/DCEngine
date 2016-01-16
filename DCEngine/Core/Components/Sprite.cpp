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

namespace DCEngine {
  namespace Components
  {
    /**************************************************************************/
    /*!
    @brief Provides the definition of this class to Zilch.
    @note This can only go in the translational unit (.cpp)
    */
    /**************************************************************************/
    #if(DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDefineType(Sprite, "Sprite", DCEngineCore, builder, type) {
      DCE_BINDING_DEFINE_ATTRIBUTE(SpriteSource);
      // Constructor / Destructor
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(Sprite);
      // Properties
      ZilchBindProperty(builder, type, &Sprite::getVisible, &Sprite::setVisible, "Visible");
      ZilchBindProperty(builder, type, &Sprite::getColor, &Sprite::setColor, "Color");
      auto spriteSource = ZilchBindProperty(builder, type, &Sprite::getSpriteSource, &Sprite::setSpriteSource, "SpriteSource");
      spriteSource->Attributes.push_back(attributeSpriteSource);
      ZilchBindProperty(builder, type, &Sprite::getFlipX, &Sprite::setFlipX, "FlipX");
      ZilchBindProperty(builder, type, &Sprite::getFlipY, &Sprite::setFlipY, "FlipY");
      ZilchBindProperty(builder, type, &Sprite::getAnimationActive, &Sprite::setAnimationActive, "AnimationActive");
      ZilchBindProperty(builder, type, &Sprite::getAnimationSpeed, &Sprite::setAnimationSpeed, "AnimationSpeed");
			DCE_BINDING_DEFINE_PROPERTY(Sprite, DrawLayer);
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
    Sprite::Sprite(Entity& owner) : Component(std::string("Sprite"), owner), DrawLayer(0) {
      // Register this component to the GraphicsSpace so that it can be drawn
      // by the graphics system.
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
      SpaceRef->getComponent<GraphicsSpace>()->RemoveSprite(*this);
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
      SpaceRef->getComponent<Components::GraphicsSpace>()->AddSprite(*this);
    }

    /**************************************************************************/
    /*!
    \fn void DCEngine::Sprite::UpdateFlip()

    \brief Updates the flip??

    \return
      None.

    @todo   Delete this function. This function is not used anymore.
    */

    /**************************************************************************/
    void Sprite::UpdateFlip()
    {
      Visible = true;
      Color.x = 1;
      Color.y = 1;
      Color.z = 1;
      BlendMode = BlendModeType::Alpha;
      FlipX = false;
      FlipY = false;
      XFlipped = false;
      YFlipped = false;

      for (int i = 0; i < 4; i++)//Initialize the flip matrix
      {
        for (int j = 0; j < 4; j++) {
          if (i == j) {
            FlipMatrix[i][j] = 1;
          }
        }
      }

      if (FlipX == true)
      {
        if (XFlipped == false)
        {
          FlipMatrix[0][0] = -1;
          XFlipped = true;
        }
      }
      else
      {
        if (XFlipped == true)
        {
          FlipMatrix[0][0] = 1;
          XFlipped = false;
        }
      }

      if (FlipY == true)
      {
        if (YFlipped == false)
        {
          FlipMatrix[1][1] = 1;
          YFlipped = true;
        }
      }
      else
      {
        if (YFlipped == true)
        {
          FlipMatrix[1][1] = -1;
          YFlipped = false;
        }
      }
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
      UpdateFlip();
    }

  }
}
