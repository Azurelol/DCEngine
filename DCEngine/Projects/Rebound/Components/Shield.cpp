/*****************************************************************************/
/*!
@file   Shield.cpp
@author Jason Jorgenson
@par    email: j.jorgenson\@digipen.edu
@date   3-1-16
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Shield.h"
#include "../../CoreComponents.h"

namespace DCEngine {
  namespace Components {

    /**************************************************************************/
    /*!
    @brief Provides the definition of this class to Zilch.
    @note This can only go in the translational unit (.cpp)
    */
    /**************************************************************************/
#if(DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDefineType(Shield, "Shield", Rebound, builder, type) {
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(Shield);
      DCE_BINDING_DEFINE_PROPERTY(Shield, Timer);
    }
#endif


    void Shield::Initialize()
    {
      auto gameObj = dynamic_cast<GameObject*>(Owner());
      TransformRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Components::Transform>(); // ew
      SpriteRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Components::Sprite>();
      BoxColliderRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Components::BoxCollider>();
      Connect(SpaceRef, Events::LogicUpdate, Shield::OnLogicUpdateEvent);

      CollisionTable = Daisy->getSystem<Systems::Content>()->getCollisionTable(std::string(this->SpaceRef->getComponent<Components::PhysicsSpace>()->getCollisionTable()));
      CollisionTable->AddGroup("Shield");
      CollisionTable->SetResolve("Shield", "Ball", CollisionFlag::SkipDetecting);
    }

    void Shield::Deactivate()
    {
      BoxColliderRef->Ghost = true;
      SpriteRef->Visible = false;
    }


    void Shield::OnCollisionStartedEvent(Events::CollisionStarted * event)
    {

    }



    void Shield::OnLogicUpdateEvent(Events::LogicUpdate * event)
    {

    }


  }
}