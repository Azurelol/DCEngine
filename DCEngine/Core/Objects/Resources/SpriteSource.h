#pragma once
#include "../Resource.h"
#include "Texture2D.h"

namespace DCEngine {

  class SpriteSource : public Resource {
  public:
    Texture2D& getTexture(); //!< Gets the reference to the used texture/
    float PixelsPerUnit;
    int FrameCount;
    bool Smoothing;
    bool Looping;
    bool Fill;
    
  private:
    std::unique_ptr<Texture2D> TextureObj;
  };


}