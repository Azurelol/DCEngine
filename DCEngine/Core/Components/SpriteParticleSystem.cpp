/******************************************************************************/
/*!
@file   SpriteParticleSystem.cpp
@author Chen Shu, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/6/2015
@brief  The SpriteParticleSystem component...
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "SpriteParticleSystem.h"
#include "EngineReference.h"

namespace DCEngine {

  namespace Components
  {
    void SpriteParticleSystem::Serialize(Json::Value & root)
    {
    }
    void SpriteParticleSystem::Deserialize(Json::Value & root)
    {
    }

    /*!************************************************************************\
    @brief  Constructor for the SpriteParticleSystem
    \**************************************************************************/
    SpriteParticleSystem::SpriteParticleSystem(Entity & owner) : Component("SpriteParticleSystem", owner)
    {
    }

    /*!************************************************************************\
    @brief  Initializes the SpriteParticleSystem.
    \**************************************************************************/
    void SpriteParticleSystem::Initialize()
    {
      // Grab a reference to the space's default camera
      CameraComponent = SpaceRef->getComponent<CameraViewport>()->getCamera();

      // Access to owner is automatically given to components through Owner()! Just need
      // to cast them into the type of entity the owner is.
      auto gameObjOwner = dynamic_cast<GameObject*>(Owner());

      // Access to the graphics system is already given
    }
  }

}