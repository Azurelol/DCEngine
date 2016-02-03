/***************************************************************************** /
/*!
\file   Texture.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   1/29/2015
\brief  The texture class organizes texture data.
\note   "http://learnopengl.com/#!Getting-started/Textures"
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include "ResourceReference.h"

namespace DCEngine {
    
  class Texture;
  using TexturePtr = std::shared_ptr<Texture>;

  class Texture : public Resource {
  public:

    ZilchDeclareDerivedType(Texture, Resource);
    static TexturePtr Find(std::string);
    Texture(std::string textureFile);
    static std::string Extension() { return ".Texture"; }

  };

  using TextureVec = std::vector<TexturePtr>;
  using TextureMap = std::map<std::string, TexturePtr>;
  


}