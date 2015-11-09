#include "Sprite.h"
#include "EngineReference.h"

namespace DCEngine {

  /**************************************************************************/
  /*!
  \brief Default constructor for the Sprite component.
  */
  /**************************************************************************/
  Sprite::Sprite(Entity& owner) : Component(std::string("Sprite"), owner) {
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
    // Subscribe this component to the graphics space
    SpaceRef->getComponent<GraphicsSpace>()->AddSprite(*this);

  }

  /**************************************************************************/
  /*!
  \brief Serialize/Deserialize functions.
  */
  /**************************************************************************/
  void Sprite::Serialize(Json::Value & root) {
  }

  void Sprite::Deserialize(Json::Value & root) {
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
}
