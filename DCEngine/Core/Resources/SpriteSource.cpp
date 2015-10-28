#include "SpriteSource.h"

namespace DCEngine {

  /**************************************************************************/
  /*!
  @brief Constructor for the SpriteSource resource.
  @param The name of the image (texture) file.
  */
  /**************************************************************************/
  SpriteSource::SpriteSource(std::string spriteFile) : Resource(FileSystem::FileExtractWithoutExtension(spriteFile)), 
                                                       ImageFileName(spriteFile) {
    
    #if (USE_SFML_TEXTURE)
    TextureObj.reset(new sf::Texture());
    #else
    TextureObj.reset(new Texture2D(spriteFile));
    #endif
    
  }

  /**************************************************************************/
  /*!
  @brief Loads a texture!
  @param The name of the image (texture) file.
  @todo  Replace the bool alpha.
  */
  /**************************************************************************/
  void SpriteSource::LoadTexture() {
    
    #if (USE_SFML_TEXTURE)
    TextureObj->loadFromFile(ImageFileName);
    trace << "SpriteSource::LoadTexture - " << ImageFileName <<  " was successfully loaded!\n";
    #else if
    // (?) Change later?
    bool alpha = true;
    if (alpha) {
      TextureObj->InternalFormat = GL_RGBA;
      TextureObj->ImageFormat = GL_RGBA;
    }

    // Load image
    sf::Image image;
    image.loadFromFile(ImageFileName);
    image.flipVertically();

    // If the image file failed to load, throw an exception
    if (image.getPixelsPtr() == NULL) {
      trace << "SpriteSource::LoadTexture - Failed to load image file!\n";
      // Exception here
    }
    else {
      trace << "SpriteSource::LoadTexture - Image was successfully loaded!\n";
    }

    // Generate texture
    TextureObj->Generate(image.getSize().x, image.getSize().y, image);
    // Free image data
    //SOIL_free_image_data(image);
    #endif
  }

  /**************************************************************************/
  /*!
  @brief  Returns the texture resource to be used by the graphics system.
  @return A reference to the texture object.
  */
  /**************************************************************************/
  #if (USE_SFML_TEXTURE)
  sf::Texture & SpriteSource::getTexture()
  {
    return *TextureObj.get();
  }
  #else
  Texture2D& SpriteSource::getTexture() {
    return *(TextureObj.get());
  }
  #endif
  





}