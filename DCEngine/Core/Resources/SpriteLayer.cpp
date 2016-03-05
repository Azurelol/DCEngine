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
    //DCE_BINDING_DEFINE_PROPERTY(SpriteLayerOrder, List);
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
  @brief  Adds a SpriteLayer to this SpriteLayerOrder.
  @param  spriteLayer The handle to this SpriteLayer
  */
  /**************************************************************************/
  void SpriteLayerOrder::Add(SpriteLayerHandle spriteLayer)
  {
    // ERROR: The SpriteLayer is already in the list    
    if (this->Has(spriteLayer)) {
      DCTrace << Name() << "::SpriteLayerOrder::Add: '" << spriteLayer << "' already present! \n";
      return;
    }

    // Add the item to the list
    List.Add(spriteLayer.c_str());
    DCTrace << Name() << "::SpriteLayerOrder::Add: '" << spriteLayer << "' has been added! \n";

  }

  /**************************************************************************/
  /*!
  @brief  Removes the specified sprite layer.
  @param  spriteLayer The name of the Spritelayer.
  */
  /**************************************************************************/
  void SpriteLayerOrder::Remove(SpriteLayerHandle spriteLayer)
  {
    DCTrace << Name() << "SpriteLayerOrder::Remove: '" << spriteLayer << "' \n";
    //List.Remove(spriteLayer);
  }

  /**************************************************************************/
  /*!
  @brief  Moves a SpriteLayer up or down the list.
  */
  /**************************************************************************/
  void SpriteLayerOrder::Move(SpriteLayerHandle spriteLayer, Direction direction)
  {
    // Find the position of the SpriteLayer
    auto spriteLayerPos = std::find(List.begin(), List.end(), spriteLayer);
    if (direction == Direction::Up) {
      // If it is already at the front
      if (spriteLayerPos == List.begin())
        return;
      // Look for the object before the current one
      auto previousSpriteLayer = spriteLayerPos;
      std::advance(previousSpriteLayer, -1);
      // If there's an object before the current one
      std::iter_swap(spriteLayerPos, previousSpriteLayer);
      DCTrace << Name() << "SpriteLayerOrder::Move: Moved '" << spriteLayer << "' towards the back! \n";
    }
    else if (direction == Direction::Down) {
      // Check if it will go outside of bounds
      if (spriteLayerPos == List.end())
        return;
      // Look for the object after this one
      auto nextSpriteLayer = spriteLayerPos;
      std::advance(nextSpriteLayer, 1);
      // If there's an object after the current one
      std::iter_swap(nextSpriteLayer, spriteLayerPos);
      DCTrace << Name() << "SpriteLayerOrder::Move: Moved '" << spriteLayer << "' towards the front! \n";
    }
  }

  /**************************************************************************/
  /*!
  @brief  Checks whether the SpriteLayerOrder has the layer present.
  @param  spriteLayer The name of the SpriteLayer.
  @return Whether it's present or not.
  */
  /**************************************************************************/
  bool SpriteLayerOrder::Has(SpriteLayerHandle spriteLayer)
  {
    return std::find(List.begin(), List.end(), spriteLayer) != List.end();
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