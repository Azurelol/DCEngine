/*****************************************************************************/
/*!
@file      SpriteLayer.h
@author    Christian Sagel
@par       email: c.sagel\@digipen.edu
@date      1/08/2016
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ResourceReference.h"

#include <set>

namespace DCEngine {

  class SpriteLayer;
  using SpriteLayerPtr = std::shared_ptr<SpriteLayer>;
  using SpriteLayerHandle = std::string;
  /**************************************************************************/
  /*!
  @class  The SpriteLayer class is a resource that represents a specific
          layer.
  */
  /**************************************************************************/
  class SpriteLayer : public Resource {
  public:
    ZilchDeclareDerivedType(SpriteLayer, Resource);
    SpriteLayer(std::string name);
    ~SpriteLayer() {}
    static std::string Extension() { return ".SpriteLayer"; }
    static SpriteLayerPtr Find(std::string);

  };

  using SpriteLayerOrderHandle = std::string;
  class SpriteLayerOrder;
  using SpriteLayerOrderPtr = std::shared_ptr<SpriteLayerOrder>;
  /**************************************************************************/
  /*!
  @class  The SpriteLayerOrder class is a resource that specifies the order
          at which sprites will be drawn through the spritelayers it has
          added.
  */
  /**************************************************************************/
  class SpriteLayerOrder : public Resource {
  public:
    using SpriteLayerList = std::list<SpriteLayerHandle>;
    SpriteLayerList List;
    DCE_DEFINE_PROPERTY(SpriteLayerList, List);
    void Add(SpriteLayerHandle spriteLayer);
    void Remove(SpriteLayerHandle spriteLayer);
    void Move(SpriteLayerHandle spriteLayer, Direction direction);
    bool Has(SpriteLayerHandle spriteLayer);

    ZilchDeclareDerivedType(SpriteLayerOrder, Resource);
    SpriteLayerOrder(std::string name);
    ~SpriteLayerOrder() {}
    static std::string Extension() { return ".SpriteLayerOrder"; }
    static std::string Default() { return "DefaultSpriteLayerOrder"; }
    static SpriteLayerOrderPtr Find(std::string);

  };

}