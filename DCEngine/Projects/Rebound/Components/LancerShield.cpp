/*****************************************************************************/
/*!
@file   LancerShield.cpp
@author Jason Jorgenson
@par    email: j.jorgenson\@digipen.edu
@date   3/25/2016
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "LancerShield.h"
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
    ZilchDefineType(LancerShield, "LancerShield", Rebound, builder, type) {
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(LancerShield);
      DCE_BINDING_DEFINE_PROPERTY(LancerShield, ShieldReflectionForce);
    }
    DCE_COMPONENT_DEFINE_DEPENDENCIES(LancerShield, "Transform", "Sprite", "BoxCollider", "RigidBody");
#endif


    void LancerShield::Initialize()
    {
      auto gameObj = dynamic_cast<GameObject*>(Owner());
      parent = dynamic_cast<GameObjectPtr>(gameObj->Parent());
      TransformRef = dynamic_cast<GameObject*>(Owner())->getComponent<Components::Transform>(); // ew
      SpriteRef = dynamic_cast<GameObject*>(Owner())->getComponent<Components::Sprite>();
      CollisionTableRef = Daisy->getSystem<Systems::Content>()->getCollisionTable(std::string(this->SpaceRef->getComponent<Components::PhysicsSpace>()->getCollisionTable()));
      CollisionTableRef->SetResolve("LancerShield", "Player", CollisionFlag::Resolve);
      CollisionTableRef->SetResolve("LancerShield", "Ball", CollisionFlag::Resolve);
      CollisionTableRef->SetResolve("LancerShield", "Terrain", CollisionFlag::SkipDetecting);
      CollisionTableRef->SetResolve("LancerShield", "Enemy", CollisionFlag::SkipDetecting);

      Connect(SpaceRef, Events::LogicUpdate, LancerShield::OnLogicUpdateEvent);
      Connect(gameObj, Events::CollisionStarted, LancerShield::OnCollisionStartedEvent);

      //gameObj->getComponent<RigidBody>()->setDynamicState(DynamicStateType::Static);
    }

    void LancerShield::OnCollisionStartedEvent(Events::CollisionStarted * event)
    {
      if (isActive &&
          event->OtherObject->Name() == "Player" ||
          event->OtherObject->Name() == "Ball")
      {
        Vec3 parentVelocity = parent->getComponent<Components::RigidBody>()->getVelocity();
        Vec3 parentPosition = parent->getComponent<Components::Transform>()->getTranslation();
        Vec3 otherVelocity = event->OtherObject->getComponent<Components::RigidBody>()->getVelocity();
        Vec3 otherPosition = event->OtherObject->getComponent<Components::Transform>()->getTranslation();
        float velocityDifferenceThreshold = parent->getComponent<Components::Lancer>()->ShieldVelocityDifferenceThreshold;

        //if (parentVelocity.x - otherVelocity.x > velocityDifferenceThreshold)
        //{
          //event->OtherObject->getComponent<Components::RigidBody>()->setVelocity(-otherVelocity);

          event->OtherObject->getComponent<Components::RigidBody>()->setVelocity(Vec3(0,0,0));
          event->OtherObject->getComponent<Components::RigidBody>()->ApplyForce(glm::normalize(otherPosition - parentPosition) * ShieldReflectionForce);
        //}
      }
    }

    void LancerShield::OnLogicUpdateEvent(Events::LogicUpdate * event)
    {

    }


  }
}