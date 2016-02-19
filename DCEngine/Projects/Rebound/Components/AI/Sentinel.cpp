/******************************************************************************/
/*!
@file   Sentinel.h
@author Jason Jorgenson
@par    email: Jason.Jorgenson\@digipen.edu
@date   2/18/2016
@brief  An AI with a shield that blocks head-on attacks
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Sentinel.h"
#include "../../../CoreComponents.h"

namespace DCEngine {
  namespace Components {

    /**************************************************************************/
    /*!
    @brief Provides the definition of this class to Zilch.
    @note This can only go in the translational unit (.cpp)
    */
    /**************************************************************************/
#if(DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDefineType(Sentinel, "Sentinel", Rebound, builder, type) {
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(Sentinel);
      DCE_BINDING_DEFINE_PROPERTY(Sentinel, PlayerName);
      DCE_BINDING_DEFINE_PROPERTY(Sentinel, IdleRange);
      DCE_BINDING_DEFINE_PROPERTY(Sentinel, MoveSpeed);

    }

    DCE_COMPONENT_DEFINE_DEPENDENCIES(Sentinel, "Transform", "RigidBody", "Sprite", "HealthController");
#endif

    Sentinel::~Sentinel()
    {
      delete stateMachine;
    }


    void Sentinel::Initialize()
    {
      DCTrace << "Sentinel Initialize\n";
      gameObj = dynamic_cast<GameObject*>(Owner());
      Connect(SpaceRef, Events::LogicUpdate, Sentinel::OnLogicUpdateEvent);
      Connect(gameObj, Events::CollisionStarted, Sentinel::OnCollisionStartedEvent);
      Connect(gameObj, Events::DeathEvent, Sentinel::OnDeathEvent);

      TransformRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Components::Transform>();
      RigidBodyRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Components::RigidBody>();
      SpriteRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Components::Sprite>();
      HealthRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Components::HealthController>();

      stateMachine = new StateMachine<Sentinel>(this);

      stateMachine->SetGlobalState(Global::Instance());
      stateMachine->SetCurrentState(Idle::Instance());

      player = SpaceRef->FindObjectByName(PlayerName);

      CreateShield();
    }

    void Sentinel::OnLogicUpdateEvent(Events::LogicUpdate * event)
    {
      stateMachine->Update();
      dt = event->Dt;
    }

    void Sentinel::OnCollisionStartedEvent(Events::CollisionStarted * event)
    {
      if (event->OtherObject->getComponent<BallController>() != NULL)
      {
        HealthRef->ModifyHealth(-1);
      }
    }

    void Sentinel::OnDeathEvent(Events::DeathEvent * event)
    {
      stateMachine->ChangeState(Die::Instance());
    };

    void Sentinel::CreateShield()
    {
      //shield = SpaceRef->CreateObject(shieldArchetype);
      //shield->AttachTo(gameObj);
      //shield->getComponent<Transform>()->setTranslation(Vec3(0, 0, 0));
    }


#pragma region Global State
    void Sentinel::Global::Enter(Sentinel *owner) {}

    void Sentinel::Global::Update(Sentinel *owner)
    {
      Vec3 playerPosition = owner->player->getComponent<Components::Transform>()->Translation;
      Vec3 ownerPosition = owner->TransformRef->Translation;
      float distanceFromPlayer = glm::distance(playerPosition, ownerPosition);

      if ((distanceFromPlayer > owner->IdleRange) && !owner->stateMachine->isInState(Idle::Instance()))
        owner->stateMachine->ChangeState(Idle::Instance());

    }

    void Sentinel::Global::Exit(Sentinel *owner) {}

    Sentinel::Global* Sentinel::Global::Instance()
    {
      static Global instance;
      return &instance;
    }
#pragma endregion Global State

#pragma region Idle State
    void Sentinel::Idle::Enter(Sentinel *owner)
    {
      //DCTrace << "Sentinel Idle Enter\n";
    }

    void Sentinel::Idle::Update(Sentinel *owner)
    {
      Vec3 playerPosition = owner->player->getComponent<Components::Transform>()->Translation;
      Vec3 ownerPosition = owner->TransformRef->Translation;
      float distanceFromPlayer = glm::distance(playerPosition, ownerPosition);

      if (distanceFromPlayer < owner->IdleRange)
        owner->stateMachine->ChangeState(Attack::Instance());
    }

    void Sentinel::Idle::Exit(Sentinel *owner)
    {

    }

    Sentinel::Idle* Sentinel::Idle::Instance()
    {
      static Idle instance;
      return &instance;
    }
#pragma endregion Idle State

#pragma region Attack State
    void Sentinel::Attack::Enter(Sentinel *owner)
    {

    }

    void Sentinel::Attack::Update(Sentinel *owner)
    {
      Vec3 ownerPosition = owner->TransformRef->Translation;
      Vec3 playerPosition = owner->player->getComponent<Transform>()->Translation;
      Vec3 direction = playerPosition - ownerPosition;
      if (direction.x < 0)
        owner->RigidBodyRef->setVelocity(Vec3(-owner->MoveSpeed, owner->RigidBodyRef->getVelocity().y, 0));
      else
        owner->RigidBodyRef->setVelocity(Vec3(owner->MoveSpeed, owner->RigidBodyRef->getVelocity().y, 0));

      float distanceFromPlayer = glm::distance(playerPosition, ownerPosition);
      if (distanceFromPlayer > owner->IdleRange)
        owner->stateMachine->ChangeState(Idle::Instance());
    }

    void Sentinel::Attack::Exit(Sentinel *owner)
    {
      owner->RigidBodyRef->setVelocity(Vec3(0, 0, 0));
    }

    Sentinel::Attack* Sentinel::Attack::Instance()
    {
      static Attack instance;
      return &instance;
    }

#pragma endregion Attack State

#pragma region Die State
    void Sentinel::Die::Enter(Sentinel *owner)
    {
      //DCTrace << "Sentinel Die Enter\n";

      owner->RigidBodyRef->setVelocity(Vec3(0, 0, 0));

      // Death sound?
    }

    void Sentinel::Die::Update(Sentinel *owner)
    {
      // Timer for death animation?

      Exit(owner);
    }

    void Sentinel::Die::Exit(Sentinel *owner)
    {
      // Destroy grunt
      owner->gameObj->Destroy();
    }

    Sentinel::Die* Sentinel::Die::Instance()
    {
      static Die instance;
      return &instance;
    }
#pragma endregion Die State

  }
}