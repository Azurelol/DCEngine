#include "Sprite.h"
#include "EngineReference.h"

namespace DCEngine {
  void Sprite::Initialize() {
    if (TRACE_INITIALIZE)
      trace << Owner()->Name() << "::" << _name << "::Initialize\n";

    // Register this component, and thus its GameObject to the GraphicsSpace
    auto registerEvent = new Events::SpriteRegistration();
    registerEvent->SpriteObj = (GameObject*)Owner(); // SHOULD THIS BE CASTED?
    space_->Dispatch<Events::SpriteRegistration>(registerEvent);
  }

  void Sprite::setSpriteSource(std::string imageFile) {
    SpriteSourceObj.reset(new SpriteSource(imageFile));
    SpriteSourceObj->LoadTexture(true);
  }

  SpriteSource * Sprite::getSpriteSource() {
    return SpriteSourceObj.get();
  }

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

  void Sprite::UpdateSprite()
  {
	  UpdateFlip();
  }
}
