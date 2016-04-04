/******************************************************************************/
/*!
@file   Lancer.h
@author Jason Jorgenson
@par    email: Jason.Jorgenson\@digipen.edu
@date   2/18/2016
@brief  An AI that charges its target
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Lancer.h"
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
    ZilchDefineType(Lancer, "Lancer", Rebound, builder, type) {
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(Lancer);
      DCE_BINDING_DEFINE_PROPERTY(Lancer, PlayerName);
      DCE_BINDING_DEFINE_PROPERTY(Lancer, startingHealth);
      DCE_BINDING_DEFINE_PROPERTY(Lancer, maxHealth);
      DCE_BINDING_DEFINE_PROPERTY(Lancer, IsInvulnerable);
      DCE_BINDING_DEFINE_PROPERTY(Lancer, IdleRange);
      DCE_BINDING_DEFINE_PROPERTY(Lancer, ChargeForce);
      DCE_BINDING_DEFINE_PROPERTY(Lancer, ShieldVelocityDifferenceThreshold);
      DCE_BINDING_DEFINE_PROPERTY(Lancer, ShieldActivationSpeed);
    }

    // Dependancies
    DCE_COMPONENT_DEFINE_DEPENDENCIES(Lancer, "Transform", "RigidBody", "Sprite");
#endif

    Lancer::~Lancer()
    {
      delete stateMachine;
    }


    void Lancer::Initialize()
    {
      DCTrace << "Lancer Initialize\n";
      gameObj = dynamic_cast<GameObject*>(Owner());
      Connect(SpaceRef, Events::LogicUpdate, Lancer::OnLogicUpdateEvent);
      Connect(gameObj, Events::CollisionStarted, Lancer::OnCollisionStartedEvent);

      TransformRef = dynamic_cast<GameObject*>(Owner())->getComponent<Components::Transform>();
      RigidBodyRef = dynamic_cast<GameObject*>(Owner())->getComponent<Components::RigidBody>();
      SpriteRef = dynamic_cast<GameObject*>(Owner())->getComponent<Components::Sprite>();
      PhysicsSpaceRef = SpaceRef->getComponent<Components::PhysicsSpace>();
      GraphicsSpaceRef = SpaceRef->getComponent<Components::GraphicsSpace>();

      stateMachine = new StateMachine<Lancer>(this);
      startingPosition = TransformRef->Translation;
      endPosition = startingPosition;

      stateMachine->SetGlobalState(Global::Instance());

      player = SpaceRef->FindObjectByName(PlayerName);

      shield = gameObj->Children().front()->getComponent<LancerShield>();
    }

    void Lancer::OnLogicUpdateEvent(Events::LogicUpdate * event)
    {
      stateMachine->Update();
    }

    void Lancer::OnCollisionStartedEvent(Events::CollisionStarted * event)
    {
      if (event->OtherObject->getComponent<BallController>() != NULL)
      {
        ModifyHealth(-1);
      }
    }

    bool Lancer::ModifyHealth(int amount)
    {
      int oldHealth = health;

      if (!IsInvulnerable)
      {
        health += amount;

        if (health > maxHealth)
          health = maxHealth;
        if (health < 0)
          health = 0;
      }

      if (health == 0)
      {
        stateMachine->ChangeState(Die::Instance());
      }

      if (oldHealth == health)
        return false;
      else
        return true;
    }




#pragma region Global State
    void Lancer::Global::Enter(Lancer *owner) {}

    void Lancer::Global::Update(Lancer *owner)
    {
      Vec3 playerPosition = owner->player->getComponent<Components::Transform>()->Translation;
      Vec3 ownerPosition = owner->TransformRef->Translation;
      Vec3 direction = playerPosition - ownerPosition;
      float distanceFromPlayer = glm::distance(playerPosition, ownerPosition);

      if ((distanceFromPlayer > owner->IdleRange) && !owner->stateMachine->isInState(Idle::Instance()))
        owner->stateMachine->ChangeState(Idle::Instance());
      else
      {
        Ray leftRay;
        leftRay.Direction = Vec3(-1, 0, 0);
        leftRay.Origin = owner->TransformRef->Translation;
        CastFilter filter;
        filter.CollisionGroups.push_back(CollisionGroup::Find("Player"));
        filter.CollisionGroups.push_back(CollisionGroup::Find("Terrain"));
        filter.Include = true;
        CastResult castLeft = owner->PhysicsSpaceRef->CastRay(leftRay, filter);

        if (castLeft.ObjectHit == owner->player)
        {
          owner->stateMachine->ChangeState(ChargeLeft::Instance());
        }
       
        Ray rightRay;
        rightRay.Direction = Vec3(1, 0, 0);
        rightRay.Origin = owner->TransformRef->Translation;
        CastResult castRight = owner->PhysicsSpaceRef->CastRay(rightRay, filter);

        if (castRight.ObjectHit == owner->player)
        {
          owner->stateMachine->ChangeState(ChargeRight::Instance());
        }
      }

      if (abs(owner->RigidBodyRef->getVelocity().x) > owner->ShieldActivationSpeed)
      {
        owner->shield->isActive = true;
        owner->shield->SpriteRef->Visible = true;
      }
      else
      {
        owner->shield->isActive = false;
        owner->shield->SpriteRef->Visible = false;
      }
    }

    void Lancer::Global::Exit(Lancer *owner) {}

    Lancer::Global* Lancer::Global::Instance()
    {
      static Global instance;
      return &instance;
    }
#pragma endregion Global State

#pragma region Idle State
    void Lancer::Idle::Enter(Lancer *owner)
    {
      //DCTrace << "Lancer Idle Enter\n";
    }

    void Lancer::Idle::Update(Lancer *owner)
    {
      Vec3 playerPosition = owner->player->getComponent<Components::Transform>()->Translation;
      Vec3 ownerPosition = owner->TransformRef->Translation;
      float distanceFromPlayer = glm::distance(playerPosition, ownerPosition);

      if (distanceFromPlayer < owner->IdleRange)
        owner->stateMachine->RevertToPreviousState();
    }

    void Lancer::Idle::Exit(Lancer *owner)
    {

    }

    Lancer::Idle* Lancer::Idle::Instance()
    {
      static Idle instance;
      return &instance;
    }
#pragma endregion Idle State

#pragma region ChargeLeft State
    void Lancer::ChargeLeft::Enter(Lancer *owner)
    {
      owner->SpriteRef->FlipX = false;
    }

    void Lancer::ChargeLeft::Update(Lancer *owner)
    {
      owner->RigidBodyRef->setAcceleration(Vec3(-owner->ChargeForce, 0, 0));
    }

    void Lancer::ChargeLeft::Exit(Lancer *owner)
    {

    }

    Lancer::ChargeLeft* Lancer::ChargeLeft::Instance()
    {
      static ChargeLeft instance;
      return &instance;
    }
#pragma endregion ChargeLeft State

#pragma region ChargeRight State
    void Lancer::ChargeRight::Enter(Lancer *owner)
    {
      owner->SpriteRef->FlipX = true;
    }

    void Lancer::ChargeRight::Update(Lancer *owner)
    {
      owner->RigidBodyRef->setAcceleration(Vec3(owner->ChargeForce, 0, 0));
    }

    void Lancer::ChargeRight::Exit(Lancer *owner)
    {

    }

    Lancer::ChargeRight* Lancer::ChargeRight::Instance()
    {
      static ChargeRight instance;
      return &instance;
    }
#pragma endregion ChargeRight State

#pragma region Die State
    void Lancer::Die::Enter(Lancer *owner)
    {
      //DCTrace << "Lancer Die Enter\n";

      owner->RigidBodyRef->setVelocity(Vec3(0, 0, 0));

      // Death sound?
    }

    void Lancer::Die::Update(Lancer *owner)
    {
      // Timer for death animation?

      Exit(owner);
    }

    void Lancer::Die::Exit(Lancer *owner)
    {
      // Destroy Lancer
      owner->gameObj->Destroy();
    }

    Lancer::Die* Lancer::Die::Instance()
    {
      static Die instance;
      return &instance;
    }
#pragma endregion Die State

  }
}