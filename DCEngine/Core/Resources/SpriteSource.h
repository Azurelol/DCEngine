#pragma once
#include "ResourceReference.h"
#include "Texture2D.h"
#include <SOIL2\SOIL2.h>

namespace DCEngine {

  class SpriteSource : public Resource {
  public:

    float PixelsPerUnit;
    int FrameCount;
    bool Smoothing;
    bool Looping;
    bool Fill;

    SpriteSource(std::string imageFile) : Resource("SpriteSource"), ImageFileName(imageFile) {
      TextureObj.reset(new Texture2D(imageFile));
    }

    void LoadTexture(GLboolean alpha) {
      if (alpha) {
        TextureObj->InternalFormat = GL_RGBA;
        TextureObj->ImageFormat = GL_RGBA;
      }

      // Load image
      std::string spritePath = "Core/Resources/Sprites/" + ImageFileName;
      int width, height;
      unsigned char* image = SOIL_load_image(spritePath.c_str(),
        &width, &height, 
        0, TextureObj->ImageFormat == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);

      // If the image file failed to load, throw an exception
      if (image == NULL) {
      }

      // Generate texture
      TextureObj->Generate(width, height, image);
      // Free image data
      SOIL_free_image_data(image);
    }

    Texture2D& getTexture() {
      return *(TextureObj.get()); 
    } //!< Gets the reference to the used texture/

    
  private:

    const std::string ImageFileName;
    std::unique_ptr<Texture2D> TextureObj;
  };


}