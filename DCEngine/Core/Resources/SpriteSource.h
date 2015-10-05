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

    SpriteSource(std::string spriteFile);
    void LoadTexture();
    Texture2D& getTexture();

  private:

    const std::string ImageFileName;
    std::unique_ptr<Texture2D> TextureObj;

  };

  using SpriteSourcePtr = std::shared_ptr<SpriteSource>;


}