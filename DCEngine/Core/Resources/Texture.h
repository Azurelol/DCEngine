/******************************************************************************/
/*!
\file   Texture.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/1/2015
\brief  The texture class organizes texture data.
\note   "http://learnopengl.com/#!Getting-started/Textures"
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include "ResourceReference.h"

namespace DCEngine {

  class Texture : public Resource {
  public:
    GLuint id;
    std::string type;

  };

  using TextureVec = std::vector<Texture>;


}