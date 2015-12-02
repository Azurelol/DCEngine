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

  /**************************************************************************/
  /*!
  @brief Provides the definition of this class to Zilch.
  @note This can only go in the translational unit (.cpp)
  */
  /**************************************************************************/
  #if(DCE_USE_ZILCH_INTERNAL_BINDING)
  ZilchDefineType(Sprite, "Sprite", DCEngineCore, builder, type) {
    DCE_BINDING_DEFINE_ATTRIBUTE(SpriteSource);
    DCE_BINDING_INTERNAL_COMPONENT_SET_HANDLE_TYPE;
    //type->HandleManager = ZilchManagerId(Zilch::PointerManager);
    // Constructor / Destructor
    ZilchBindConstructor(builder, type, Sprite, "owner", Entity&);
    ZilchBindDestructor(builder, type, Sprite);
    // Properties
    ZilchBindProperty(builder, type, &Sprite::getVisible, &Sprite::setVisible, "Visible");
    ZilchBindProperty(builder, type, &Sprite::getColor, &Sprite::setColor, "Color");
    ZilchBindProperty(builder, type, &Sprite::getSpriteSource, &Sprite::setSpriteSource, "SpriteSource")->Attributes.push_back(attributeSpriteSource);
    ZilchBindProperty(builder, type, &Sprite::getFlipX, &Sprite::setFlipX, "FlipX");
    ZilchBindProperty(builder, type, &Sprite::getFlipY, &Sprite::setFlipY, "FlipY");
    ZilchBindProperty(builder, type, &Sprite::getAnimationActive, &Sprite::setAnimationActive, "AnimationActive");
    ZilchBindProperty(builder, type, &Sprite::getAnimationSpeed, &Sprite::setAnimationSpeed, "AnimationSpeed");
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
  Sprite::Sprite(Entity& owner) : Component(std::string("Sprite"), owner) {
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
    if (TRACE_COMPONENT_INITIALIZE)
      DCTrace << Owner()->Name() << "::" << ObjectName << "::Initialize\n";


  }

  void Sprite::Register()
  {
    // Store the reference to this owner's Transform component
    TransformComponent = dynamic_cast<GameObject*>(Owner())->getComponent<Transform>();
    // Register
    SpaceRef->getComponent<GraphicsSpace>()->AddSprite(*this);
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

  /**************************************************************************/
  /**************************************************************************!
                                     PROPERTIES

  /**************************************************************************/
  /**************************************************************************/
  /*!
  \fn bool DCEngine::Sprite::getVisible() const

  \brief Get the visible value in Sprite component.

  \return
    Whether this object is visible.
  */
  /**************************************************************************/
  //bool Sprite::getVisible() const
  //{
  //  return this->Visible;
  //}

  /**************************************************************************/
  /*!
  \fn void DCEngine::Sprite::setVisible(bool visible)

  \brief Set the visible value in Sprite component.

  \param visible
    The input boolean.

  \return
    None.
  */
  /**************************************************************************/
  //void Sprite::setVisible(bool visible)
  //{
  //  this->Visible = visible;
  //}

  /**************************************************************************/
  /*!
  \fn Vec4 DCEngine::Sprite::getColor() const

  \brief Get the color data in Sprite component.

  \return
    The color data stored in Vec4.
  */
  /**************************************************************************/
  Vec4 Sprite::getColor() const
  {
    return this->Color;
  }

  /**************************************************************************/
  /*!
  \fn void DCEngine::Sprite::setColor(Vec4 color)

  \brief Set the color value in Sprite component.

  \param color
    The input color value.

  \return
    None.
  */
  /**************************************************************************/
  void Sprite::setColor(Vec4 color)
  {
    this->Color = color;
  }

  /**************************************************************************/
  /*!
  \fn void DCEngine::Sprite::addColor(Vec4 color)

  \brief 
    Add specified color value to the color of owner

  \param color
    The input color value.

  \return
    None.
  */
  /**************************************************************************/
  void Sprite::addColor(Vec4 color)
  {
	 this->Color += color;
  }

  /**************************************************************************/
  /*!
  \fn String DCEngine::Sprite::getSpriteSource() const

  \brief
    Get the spritesource name of this owner.

  \return
    A sting contains the name of spritesource.
  */
  /**************************************************************************/
  String Sprite::getSpriteSource() const
  {
    return this->SpriteSource;
  }

  /**************************************************************************/
  /*!
  \fn void DCEngine::Sprite::setSpriteSource(String spriteSource)

  \brief
    Set the spritesource.

  \param spriteSource
    The name of sprite source.

  \return
    None.
  */
  /**************************************************************************/
  void Sprite::setSpriteSource(String spriteSource)
  {
    this->SpriteSource = spriteSource;
  }

  /**************************************************************************/
  /*!
  \fn bool DCEngine::Sprite::getFlipX() const

  \brief
    Get the flip value of owwner.

  \return
    Whether owner is flipped on local X axis.
  */
  /**************************************************************************/
  bool Sprite::getFlipX() const
  {
    return this->FlipX;
  }

  /**************************************************************************/
  /*!
  \fn void DCEngine::Sprite::setFlipX(bool flip)

  \brief
    Set the flip value of owwner.

  \param flip
	The input value.

  \return
    None.
  */
  /**************************************************************************/
  void Sprite::setFlipX(bool flip)
  {
    this->FlipX = flip;
  }

  /**************************************************************************/
  /*!
  \fn bool DCEngine::Sprite::getFlipY() const

  \brief
    Set the flipY value of owner.

  \return
    Whether owner is flipped on local Y axis..
  */
  /**************************************************************************/
  bool Sprite::getFlipY() const
  {
    return this->FlipY;
  }

  /**************************************************************************/
  /*!
  \fn void DCEngine::Sprite::setFlipY(bool flip)

  \brief
    Set the flipY value of owner.

  \param flip
    The input value.

  \return
    None.
  */
  /**************************************************************************/
  void Sprite::setFlipY(bool flip)
  {
    this->FlipY = flip;
  }

  /**************************************************************************/
  /*!
  \fn bool DCEngine::Sprite::getAnimationActive() const

  \brief
    Get the aniamtion active variable in sprite.

  \return
    Whether the animation is active now.
  */
  /**************************************************************************/
  bool Sprite::getAnimationActive() const
  {
    return this->AnimationActive;
  }

  /**************************************************************************/
  /*!
  \fn void DCEngine::Sprite::setAnimationActive(bool active)

  \brief
    Set the aniamtion active variable in sprite.

  \param active
    The input value.

  \return
    None.
  */
  /**************************************************************************/
  void Sprite::setAnimationActive(bool active)
  {
    this->AnimationActive = active;
  }

  /**************************************************************************/
  /*!
  \fn Real DCEngine::Sprite::getAnimationSpeed() const

  \brief
    Get the animation speed of owwner.

  \return
    The speed of animation.
  */
  /**************************************************************************/
  Real Sprite::getAnimationSpeed() const
  {
    return this->AnimationSpeed;
  }

  /**************************************************************************/
  /*!
  \fn void DCEngine::Sprite::setAnimationSpeed(Real speed)

  \brief
    Set the animation speed of owwner.

  \param speed
    The input speed.

  \return
    None.
  */
  /**************************************************************************/
  void Sprite::setAnimationSpeed(Real speed)
  {
    this->AnimationSpeed = speed;
  }
}
