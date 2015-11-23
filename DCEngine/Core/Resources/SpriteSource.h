/*****************************************************************************/
/*!
@file   SpriteSource.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   10/04/2015
@brief  The SpriteSource resource encapsulates an image file amd the texture
        data generated from it.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
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

    //std::string DefaultTexture = "Square.png";
    float PixelsPerUnit = 0.0f;
    //Animation
    int ColumnCount;
    int RowCount;
    //Texture cut
    int MinX = 0;
    int MaxX = 0;
    int MinY = 0;
    int MaxY = 0;
    //PicWidth and PicHeight already initialized. Do not change it.
    int PicWidth;
    int PicHeight;
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