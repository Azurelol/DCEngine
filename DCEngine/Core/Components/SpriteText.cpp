/******************************************************************************/
/*!
@file  SpriteText.cpp
@author Chen Shu, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/23/2015
@brief
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/

#include "SpriteText.h"
#include "Transform.h"
#include "EngineReference.h"

namespace DCEngine {
  

  void SpriteText::Initialize()
  {
    // Store the reference to this owner's Transform component
    TransformComponent = dynamic_cast<GameObject*>(Owner())->getComponent<Transform>();
    // Subscribe this component to the graphics space
    SpaceRef->getComponent<GraphicsSpace>()->AddSpriteText(*this);
  }

  void SpriteText::Serialize(Json::Value & root)
  {
  }

  void SpriteText::Deserialize(Json::Value & root)
  {
  }

}