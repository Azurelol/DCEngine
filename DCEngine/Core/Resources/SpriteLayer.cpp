/*****************************************************************************/
/*!
@file      SpriteLayer.cpp
@author    Christian Sagel
@par       email: c.sagel\@digipen.edu
@date      1/08/2016
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "SpriteLayer.h"

#include "../../Core/Engine/Engine.h"

namespace DCEngine {

  /*===================*
  *     SpriteLayer    *
  *===================*/
  ZilchDefineType(SpriteLayer, "SpriteLayer", DCEngineCore, builder, type) {
  }

  /**************************************************************************/
  /*!
  @brief  SpriteLayer constructor.
  */
  /**************************************************************************/
  SpriteLayer::SpriteLayer(std::string spriteLayerFile) : Resource("SpriteLayer", FileSystem::FileNoExtension(spriteLayerFile),
                                                                                  spriteLayerFile)
  {
  }

  /**************************************************************************/
  /*!
  @brief  Finds a Spritelayer
  @param  The name of the SpriteLayer.
  @return A pointer to the specified SpriteLayer resource.
  */
  /**************************************************************************/
  SpriteLayerPtr SpriteLayer::Find(std::string name)
  {
    return Daisy->getSystem<Systems::Content>()->getSpriteLayer(name);
  }




  /*============================*
  *     SpriteLayerOrder        *
  *============================*/
  ZilchDefineType(SpriteLayerOrder, "SpriteLayerOrder", DCEngineCore, builder, type) {
  }

  /**************************************************************************/
  /*!
  @brief  SpriteLayerOrder constructor.
  */
  /**************************************************************************/
  SpriteLayerOrder::SpriteLayerOrder(std::string spriteLayerOrderFile) : Resource("SpriteLayerOrder",
                                     FileSystem::FileNoExtension(spriteLayerOrderFile), spriteLayerOrderFile)
  {
  }

  /**************************************************************************/
  /*!
  @brief  SpriteLayerOrder constructor.
  @param  The name of the SpriteLayerOrder.
  @return A pointer to the specified SpriteLayerOrder resource.
  */
  /**************************************************************************/
  SpriteLayerOrderPtr SpriteLayerOrder::Find(std::string name)
  {
    return Daisy->getSystem<Systems::Content>()->getSpriteLayerOrder(name);
  }

}