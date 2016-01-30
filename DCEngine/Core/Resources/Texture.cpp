/***************************************************************************** /
/*!
\file   Texture.cpp
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   1/29/2015
\brief  The texture class organizes texture data.
\note   "http://learnopengl.com/#!Getting-started/Textures"
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "Texture.h"

#include "../Engine/Engine.h"

namespace DCEngine {

  /**************************************************************************/
  /*!
  @brief Provides the definition of this class to Zilch for reflection.
  */
  /**************************************************************************/
  ZilchDefineType(Texture, "Texture", DCEngineCore, builder, type) {
  }

  /**************************************************************************/
  /*!
  @brief Texture constructor.
  */
  /**************************************************************************/
  Texture::Texture(std::string textureFile) : 
      Resource("Texture", FileSystem::FileNoExtension(textureFile), textureFile)
  {
  }

  /**************************************************************************/
  /*!
  @brief Finds a texture.
  */
  /**************************************************************************/
  TexturePtr Texture::Find(std::string name)
  {
    return Daisy->getSystem<Systems::Content>()->getTexture(name);
  }


}