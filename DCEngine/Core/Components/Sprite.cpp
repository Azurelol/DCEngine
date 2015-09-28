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
    if (TRACE_INITIALIZE)
      trace << Owner()->Name() << "::" << _name << "::Initialize\n";
    
    // Store the reference to this owner's Transform component
    TransformComponent = dynamic_cast<GameObject*>(Owner())->getComponent<Transform>();

    // Register this component, and thus its GameObject to the GraphicsSpace
    auto registerEvent = new Events::SpriteRegistration();
    registerEvent->SpriteObj = (GameObject*)Owner(); // SHOULD THIS BE CASTED?
    space_->Dispatch<Events::SpriteRegistration>(registerEvent);

    // Load the default SpriteSource
    SpriteSourceObj.reset(new SpriteSource());
    SpriteSourceObj->LoadTexture(true);
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
  \brief Changes the SpriteSource.
  */
  /**************************************************************************/
  void Sprite::setSpriteSource(std::string imageFile) {
    SpriteSourceObj.reset(new SpriteSource(imageFile));
    SpriteSourceObj->LoadTexture(true);
  }
  
  /**************************************************************************/
  /*!
  \brief Gets a pointer to the SpriteSource resource.
  \param A pointer to the SpriteSource resource used by this Sprite component.
  */
  /**************************************************************************/
  SpriteSource * Sprite::getSpriteSource() {
    return SpriteSourceObj.get();
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
  \brief Updates the sprite on every update.
  */
  /**************************************************************************/
  void Sprite::UpdateSprite()
  {
	  UpdateFlip();
  }
}
