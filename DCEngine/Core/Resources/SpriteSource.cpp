/******************************************************************************/
/*!
@file   SpriteSource.cpp
@author Chen Shu, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/23/2015
@brief
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "SpriteSource.h"
#include "../../Core/Engine/Engine.h"

namespace DCEngine {
  
  /**************************************************************************/
  /*!
  @brief Provides the definition of this class to Zilch for reflection.
  */
  /**************************************************************************/
  #if(DCE_USE_ZILCH_INTERNAL_BINDING)
  ZilchDefineType(SpriteSource, "SpriteSource", DCEngineCore, builder, type) {
    DCE_BINDING_DEFINE_RESOURCE_ATTRIBUTE(Image);
    DCE_BINDING_DEFINE_PROPERTY(SpriteSource, FrameRate);
    DCE_BINDING_DEFINE_PROPERTY(SpriteSource, PixelsPerUnit);
    DCE_BINDING_DEFINE_PROPERTY(SpriteSource, ColumnCount);
    DCE_BINDING_DEFINE_PROPERTY(SpriteSource, RowCount);
    DCE_BINDING_DEFINE_PROPERTY(SpriteSource, Smoothing);
    DCE_BINDING_DEFINE_PROPERTY(SpriteSource, Looping);
    DCE_BINDING_DEFINE_PROPERTY(SpriteSource, Fill);
    DCE_BINDING_DEFINE_PROPERTY(SpriteSource, TotalFrame);
    // Image
    DCE_BINDING_DEFINE_PROPERTY(SpriteSource, AssetPath);
    DCE_BINDING_PROPERTY_SET_RESOURCE_ATTRIBUTE(propertyAssetPath, attributeImage);
    ZilchBindMethod(builder, type, &SpriteSource::Find, ZilchNoOverload, "Find", ZilchNoNames);
  }
  #endif

  /**************************************************************************/
  /*!
  @brief Constructor for the SpriteSource resource.
  @param The name of the image (texture) file.
  */
  /**************************************************************************/
  SpriteSource::SpriteSource(std::string spriteFile) : 
                             Resource("SpriteSource", FileSystem::FileNoExtension(spriteFile), spriteFile) {

    #if (USE_SFML_TEXTURE)
    TextureObj.reset(new sf::Texture());
    #else
    TextureObj.reset(new Texture2D(spriteFile));
    #endif
    
  }

  /**************************************************************************/
  /*!
  @brief Loads an image from file into memory.
  @return True if the image was successfully loaded, false otherwise.
  */
  /**************************************************************************/
  bool SpriteSource::LoadImageFromFile()
  {
    // ??
    bool alpha = true;
    if (alpha) {
      TextureObj->InternalFormat = GL_RGBA;
      TextureObj->ImageFormat = GL_RGBA;
    }

    // Load image
    Image = new sf::Image;
    Image->loadFromFile(AssetPath);
    Image->flipVertically();

    // If the image file failed to load, throw an exception
    if (Image->getPixelsPtr() == NULL) {
      DCTrace << Name() << "SpriteSource::LoadImageFromFile - Failed to load image file!\n";
      return false;
    }

    DCTrace << Name() << "SpriteSource::LoadImageFromFile - Image was successfully loaded!\n";
    return true;
    
  }

  /**************************************************************************/
  /*!
  @brief Generates a texture for OpenGL from an image file.
  @return True if the texture was successfully generated, false otherwise.
  */
  /**************************************************************************/
  bool SpriteSource::GenerateTexture()
  {
    // Generate the texture using the image
    TextureObj->Generate(Image->getSize().x, Image->getSize().y, *Image);
    MaxX = Image->getSize().x;
    MaxY = Image->getSize().y;
    PicHeight = Image->getSize().y;
    PicWidth =  Image->getSize().x;
    // Delete the image now that we are done with it
    delete Image;

    return true;
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
    DCTrace << "SpriteSource::LoadTexture - " << ImageFileName <<  " was successfully loaded!\n";
    #else if
    // (?) Change later?
    bool alpha = true;
    if (alpha) {
      TextureObj->InternalFormat = GL_RGBA;
      TextureObj->ImageFormat = GL_RGBA;
    }

    // Load image
    sf::Image image;
    image.loadFromFile(AssetPath);
    image.flipVertically();

    // If the image file failed to load, throw an exception
    if (image.getPixelsPtr() == NULL) {
      DCTrace << "SpriteSource::LoadTexture - Failed to load image file!\n";
      // Exception here
    }
    else {
      DCTrace << "SpriteSource::LoadTexture - Image was successfully loaded!\n";
    }

    // Generate texture
    TextureObj->Generate(image.getSize().x, image.getSize().y, image);
    MaxX = image.getSize().x;
    MaxY = image.getSize().y;
    PicHeight = image.getSize().y;
    PicWidth = image.getSize().x;
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

  /**************************************************************************/
  /*!
  @brief  Returns the SpriteSource resource to be used by the graphics system.
  @return A reference to the SpriteSource object.
  */
  /**************************************************************************/
  SpriteSourceRawPtr SpriteSource::Find(std::string name)
  {
    return Daisy->getSystem<Systems::Content>()->getSpriteSrc(name).get();
  }





}