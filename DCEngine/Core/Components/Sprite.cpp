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
    //
    type->HandleManager = ZilchManagerId(Zilch::PointerManager);
    // Constructor / Destructor
    ZilchBindConstructor(builder, type, Sprite, "owner", Entity&);
    ZilchBindDestructor(builder, type, Sprite);
    // Properties
    ZilchBindProperty(builder, type, &Sprite::getVisible, &Sprite::setVisible, "Visible");
    ZilchBindProperty(builder, type, &Sprite::getColor, &Sprite::setColor, "Color");
    ZilchBindProperty(builder, type, &Sprite::getSpriteSource, &Sprite::setSpriteSource, "SpriteSource");
    ZilchBindProperty(builder, type, &Sprite::getFlipX, &Sprite::setFlipX, "FlipX");
    ZilchBindProperty(builder, type, &Sprite::getFlipY, &Sprite::setFlipY, "FlipY");
    ZilchBindProperty(builder, type, &Sprite::getAnimationActive, &Sprite::setAnimationActive, "AnimationActive");
    ZilchBindProperty(builder, type, &Sprite::getAnimationSpeed, &Sprite::setAnimationSpeed, "AnimationSpeed");
  }
  #endif


  /**************************************************************************/
  /*!
  \brief Default constructor for the Sprite component.
  */
  /**************************************************************************/
  Sprite::Sprite(Entity& owner) : Component(std::string("Sprite"), owner) {

  }

  /**************************************************************************/
  /*!
  \brief Sprite Destructor.
  */
  /**************************************************************************/
  Sprite::~Sprite()
  {
    // Deregister this component from the GraphicsSpace
    SpaceRef->getComponent<GraphicsSpace>()->RemoveSprite(*this);
  }

  /**************************************************************************/
  /*!
  \brief Initializes the Sprite component, registering it into the graphics
         space for drawing and initializing with a default SpriteSource.
  */
  /**************************************************************************/
  void Sprite::Initialize() {
    if (TRACE_COMPONENT_INITIALIZE)
      DCTrace << Owner()->Name() << "::" << ObjectName << "::Initialize\n";
    
    // Store the reference to this owner's Transform component
    TransformComponent = dynamic_cast<GameObject*>(Owner())->getComponent<Transform>();

    // Register this component to the GraphicsSpace so that it can be drawn
    // by the graphics system.
    SpaceRef->getComponent<GraphicsSpace>()->AddSprite(*this);

  }
    
  /**************************************************************************/
  /*!
  \brief Updates the flip??
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
  \brief Sets the color using 255 as the max instead of 1. Preserves opacity.
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
  \brief Updates the sprite on every update.
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
  bool Sprite::getVisible() const
  {
    return this->Visible;
  }
  void Sprite::setVisible(bool visible)
  {
    this->Visible = visible;
  }

  Vec4 Sprite::getColor() const
  {
    return this->Color;
  }
  void Sprite::setColor(Vec4 color)
  {
    this->Color = color;
  }
  void Sprite::addColor(Vec4 color)
  {
	 this->Color += color;
  }

  String Sprite::getSpriteSource() const
  {
    return this->SpriteSource;
  }
  void Sprite::setSpriteSource(String spriteSource)
  {
    this->SpriteSource = spriteSource;
  }

  bool Sprite::getFlipX() const
  {
    return this->FlipX;
  }
  void Sprite::setFlipX(bool flip)
  {
    this->FlipX = this;
  }

  bool Sprite::getFlipY() const
  {
    return this->FlipY;
  }
  void Sprite::setFlipY(bool flip)
  {
    this->FlipY = flip;
  }

  bool Sprite::getAnimationActive() const
  {
    return this->AnimationActive;
  }
  void Sprite::setAnimationActive(bool active)
  {
    this->AnimationActive = active;
  }

  Real Sprite::getAnimationSpeed() const
  {
    return this->AnimationSpeed;
  }
  void Sprite::setAnimationSpeed(Real speed)
  {
    this->AnimationSpeed = speed;
  }
}
