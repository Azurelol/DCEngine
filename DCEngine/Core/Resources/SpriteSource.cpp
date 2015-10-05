#include "SpriteSource.h"

namespace DCEngine {

  /**************************************************************************/
  /*!
  @brief Constructor for the SpriteSource resource.
  @param The name of the image (texture) file.
  */
  /**************************************************************************/
  SpriteSource::SpriteSource(std::string spriteFile) : Resource("SpriteSource"), ImageFileName(spriteFile) {
    TextureObj.reset(new Texture2D(spriteFile));
    LoadTexture();
  }

  /**************************************************************************/
  /*!
  @brief Loads a texture!
  @param The name of the image (texture) file.
  @todo  Replace the bool alpha.
  */
  /**************************************************************************/
  void SpriteSource::LoadTexture() {
    
    // (?) Change later?
    bool alpha = true;    
    if (alpha) {
      TextureObj->InternalFormat = GL_RGBA;
      TextureObj->ImageFormat = GL_RGBA;
    }

    // Load image
    std::string spritePath;
    spritePath.append("Core/Resources/Sprites/");
    spritePath.append(ImageFileName);
    int width, height;
    unsigned char* image = SOIL_load_image(spritePath.c_str(),
      &width, &height,
      0, TextureObj->ImageFormat == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);

    // If the image file failed to load, throw an exception
    if (image == NULL) {
      trace << "SpriteSource::LoadTexture - Failed to load image file!\n";
      // Exception here
    }
    else {
      trace << "SpriteSource::LoadTexture - Image was successfully loaded!\n";
    }

    // Generate texture
    TextureObj->Generate(width, height, image);
    // Free image data
    SOIL_free_image_data(image);

 
  }

  /**************************************************************************/
  /*!
  @brief  Returns the underlying texture resource to be used by the graphics
           system.
  @return A reference to the Texture2D object.
  */
  /**************************************************************************/
  Texture2D& SpriteSource::getTexture() {
    return *(TextureObj.get());
  }

}