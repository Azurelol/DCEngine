#pragma once
#include "ResourceReference.h"
#include "Texture2D.h"
#include <SOIL2\SOIL2.h>
//#include <SFML\Graphics.hpp>

namespace DCEngine {

  class SpriteSource : public Resource {
  public:

    std::string DefaultTexture = "Square.png";
    float PixelsPerUnit;
    int FrameCount;
    bool Smoothing;
    bool Looping;
    bool Fill;

    /**************************************************************************/
    /*!
    \brief Default constructor for the SpriteSource resource.
    */
    /**************************************************************************/
    SpriteSource() : Resource("SpriteSource") {
      TextureObj.reset(new Texture2D(DefaultTexture));
    }
   
    /**************************************************************************/
    /*!
    \brief Constructor for the SpriteSource resource.
    \param The name of the image (texture) file.
    */
    /**************************************************************************/
    SpriteSource(std::string imageFile) : Resource("SpriteSource"), ImageFileName(imageFile) {
      TextureObj.reset(new Texture2D(imageFile));
    }

    /**************************************************************************/
    /*!
    \brief Loads a texture! 
    \param The name of the image (texture) file.
    */
    /**************************************************************************/
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
        trace << "SpriteSource::LoadTexture - Failed to load image file!\n";
      }

      // Generate texture
      TextureObj->Generate(width, height, image);
      // Free image data
      SOIL_free_image_data(image);
    }

    /**************************************************************************/
    /*!
    \brief  Returns the underlying texture resource to be used by the graphics
            system.
    \return A reference to the Texture2D object.
    */
    /**************************************************************************/
    Texture2D& getTexture() {
      return *(TextureObj.get()); 
    } //!< Gets the reference to the used texture/

    
  private:

    const std::string ImageFileName;
    std::unique_ptr<Texture2D> TextureObj;

  };

  using SpriteSourcePtr = std::shared_ptr<SpriteSource>;


}