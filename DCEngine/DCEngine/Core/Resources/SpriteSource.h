/*****************************************************************************/
/*!
@file   SpriteSource.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   10/04/2015
@brief  The SpriteSource resource encapsulates an image file amd the texture
        data generated from it.
*/
/******************************************************************************/
#pragma once
#include "ResourceReference.h"

#define USE_SFML_TEXTURE 0

#if (USE_SFML_TEXTURE)
#include <SFML\Graphics\Texture.hpp>
#else
#include "Texture2D.h"
#endif

namespace DCEngine {

  class SpriteSource : public Resource {
  public:

    std::string DefaultTexture = "Square.png";
    float PixelsPerUnit;
    int FrameCount;
    bool Smoothing;
    bool Looping;
    bool Fill;

    SpriteSource(std::string spriteFile);
    void LoadTexture();

    #if (USE_SFML_TEXTURE)
    sf::Texture& getTexture();
    #else
    Texture2D& getTexture();
    #endif
    
    

  private:

    const std::string ImageFileName;

    #if (USE_SFML_TEXTURE)
    std::unique_ptr<sf::Texture> TextureObj;
    #else
    std::unique_ptr<Texture2D> TextureObj;
    #endif
    
    
    

  };

  using SpriteSourcePtr = std::shared_ptr<SpriteSource>;


}