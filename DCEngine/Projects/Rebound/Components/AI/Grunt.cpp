/******************************************************************************/
/*!
@file   Grunt.h
@author Jason Jorgenson
@par    email: Jason.Jorgenson\@digipen.edu
@date   1/16/2016
@brief  An AI that patrols on a set route until the player enter's it's area.
        Then it will attack until the player leaves.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Grunt.h"
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
    ZilchDefineType(Grunt, "Grunt", Rebound, builder, type) {
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(Grunt);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, PlayerName);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, IdleRange);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, PatrolDistance);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, IsPatrolRight);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, JumpStrengthX);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, JumpStrengthY);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, JumpPeriod);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, AttackJumpStrengthX);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, AttackJumpStrengthY);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, AttackJumpPeriod);
    }
#endif

    Grunt::~Grunt()
    {
      delete stateMachine;
    }


    void Grunt::Initialize()
    {
      DCTrace << "Grunt Initialize\n";
      gameObj = dynamic_cast<GameObject*>(Owner());
      Connect(SpaceRef, Events::LogicUpdate, Grunt::OnLogicUpdateEvent);
      Connect(gameObj, Events::CollisionStarted, Grunt::OnCollisionStartedEvent);
      Connect(gameObj, Events::DamageEvent, Grunt::OnDeathEvent);

      TransformRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Components::Transform>(); 
      RigidBodyRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Components::RigidBody>();
      SpriteRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Components::Sprite>();
      HealthRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Components::HealthController>();

      stateMachine = new StateMachine<Grunt>(this);
      startingPosition = TransformRef->Translation;
      endPosition = startingPosition;

      if (IsPatrolRight)
      {
        endPosition.x = startingPosition.x + PatrolDistance;
        stateMachine->ChangeState(PatrolRight::Instance());
      }
      else
      {
        endPosition.x = startingPosition.x - PatrolDistance;
        stateMachine->ChangeState(PatrolLeft::Instance());
      }

      stateMachine->SetGlobalState(Global::Instance());

      player = SpaceRef->FindObjectByName(PlayerName);
    }

    void Grunt::OnLogicUpdateEvent(Events::LogicUpdate * event)
    {
      stateMachine->Update();
      dt = event->Dt;
    }

    void Grunt::OnCollisionStartedEvent(Events::CollisionStarted * event)
    {
      if (event->OtherObject->getComponent<BallController>() != NULL)
      {
        HealthRef->ModifyHealth(-1);
      }
    }

    void Grunt::OnDeathEvent(Events::DeathEvent * event)
    {
      stateMachine->ChangeState(Die::Instance());
    };

    // Direction should be 1 (right) or -1 (left). 
    void Grunt::Jump(int direction, float period, float strengthX, float strengthY)
    {
      if (jumpTimer > period)
      {
        RigidBodyRef->setVelocity(RigidBodyRef->getVelocity() + Vec3(strengthX *  direction, strengthY, 0));
        jumpTimer = 0;
      }

      jumpTimer += dt;
    }

#pragma region Global State
    void Grunt::Global::Enter(Grunt *owner){}

    void Grunt::Global::Update(Grunt *owner)
    {
      Vec3 playerPosition = owner->player->getComponent<Components::Transform>()->Translation;
      Vec3 ownerPosition = owner->TransformRef->Translation;
      float distanceFromPlayer = glm::distance(playerPosition, ownerPosition);
      
      if ((distanceFromPlayer > owner->IdleRange) && !owner->stateMachine->isInState(Idle::Instance()))
        owner->stateMachine->ChangeState(Idle::Instance());
    }

    void Grunt::Global::Exit(Grunt *owner){}

    Grunt::Global* Grunt::Global::Instance()
    {
      static Global instance;
      return &instance;
    }
#pragma endregion Global State

#pragma region Idle State
    void Grunt::Idle::Enter(Grunt *owner)
    {
      //DCTrace << "Grunt Idle Enter\n";
    }

    void Grunt::Idle::Update(Grunt *owner)
    {
      Vec3 playerPosition = owner->player->getComponent<Components::Transform>()->Translation;
      Vec3 ownerPosition = owner->TransformRef->Translation;
      float distanceFromPlayer = glm::distance(playerPosition, ownerPosition);
      
      if (distanceFromPlayer < owner->IdleRange)
        owner->stateMachine->RevertToPreviousState();
    }

    void Grunt::Idle::Exit(Grunt *owner)
    {

    }

    Grunt::Idle* Grunt::Idle::Instance()
    {
      static Idle instance;
      return &instance;
    }
#pragma endregion Idle State

#pragma region Right State
    void Grunt::PatrolRight::Enter(Grunt *owner)
    {
      owner->jumpTimer = 0;
      //DCTrace << "Grunt PatrolRight Enter\n";
    }

    void Grunt::PatrolRight::Update(Grunt *owner)
    {
      Vec3 ownerPosition = owner->TransformRef->Translation;
      Vec3 playerPosition = owner->player->getComponent<Transform>()->Translation;
      owner->Jump(1, owner->JumpPeriod, owner->JumpStrengthX, owner->JumpStrengthY);
      
      if (owner->IsPatrolRight)
      {
        if (playerPosition.x > owner->startingPosition.x && playerPosition.x < owner->endPosition.x)
          owner->stateMachine->ChangeState(Attack::Instance());
        else if(ownerPosition.x > owner->endPosition.x)
          owner->stateMachine->ChangeState(PatrolLeft::Instance());
      }
      else
      {
        if (playerPosition.x < owner->startingPosition.x && playerPosition.x > owner->endPosition.x)
          owner->stateMachine->ChangeState(Attack::Instance());
        else if(ownerPosition.x > owner->startingPosition.x)
          owner->stateMachine->ChangeState(PatrolLeft::Instance());
      }        
    }

    void Grunt::PatrolRight::Exit(Grunt *owner)
    {
      owner->RigidBodyRef->setVelocity(Vec3());
    }

    Grunt::PatrolRight* Grunt::PatrolRight::Instance()
    {
      static PatrolRight instance;
      return &instance;
    }
#pragma endregion Right State

#pragma region Left State
    void Grunt::PatrolLeft::Enter(Grunt *owner)
    {
      //DCTrace << "Grunt PatrolLeft Enter\n";
    }

    void Grunt::PatrolLeft::Update(Grunt *owner)
    {
      Vec3 ownerPosition = owner->TransformRef->Translation;
      Vec3 playerPosition = owner->player->getComponent<Transform>()->Translation;
      owner->Jump(-1, owner->JumpPeriod, owner->JumpStrengthX, owner->JumpStrengthY);

      if (owner->IsPatrolRight)
      {
        if (playerPosition.x > owner->startingPosition.x && playerPosition.x < owner->endPosition.x)
          owner->stateMachine->ChangeState(Attack::Instance());
        else if (ownerPosition.x < owner->startingPosition.x)
          owner->stateMachine->ChangeState(PatrolRight::Instance());
      }
      else
      {
        if (playerPosition.x < owner->startingPosition.x && playerPosition.x > owner->endPosition.x)
          owner->stateMachine->ChangeState(Attack::Instance());
        else if (ownerPosition.x < owner->endPosition.x)
          owner->stateMachine->ChangeState(PatrolRight::Instance());
      }
    }

    void Grunt::PatrolLeft::Exit(Grunt *owner)
    {
      owner->RigidBodyRef->setVelocity(Vec3());
    }

    Grunt::PatrolLeft* Grunt::PatrolLeft::Instance()
    {
      static PatrolLeft instance;
      return &instance;
    }
#pragma endregion Left State

#pragma region Attack State
    void Grunt::Attack::Enter(Grunt *owner)
    {
      //DCTrace << "Grunt Attack Enter\n";
    }

    void Grunt::Attack::Update(Grunt *owner)
    {
      Vec3 ownerPosition = owner->TransformRef->Translation;
      Vec3 playerPosition = owner->player->getComponent<Transform>()->Translation;
      Vec3 direction = playerPosition - ownerPosition;
      if (direction.x < 0)
        owner->Jump(-1, owner->AttackJumpPeriod, owner->AttackJumpStrengthX, owner->AttackJumpStrengthY);
      else
        owner->Jump(1, owner->AttackJumpPeriod, owner->AttackJumpStrengthX, owner->AttackJumpStrengthY);

      if (owner->IsPatrolRight)
      {
        if (playerPosition.x < owner->startingPosition.x || playerPosition.x > owner->endPosition.x)
          owner->stateMachine->RevertToPreviousState();
      }
      else
      {
        if (playerPosition.x > owner->startingPosition.x || playerPosition.x < owner->endPosition.x)
          owner->stateMachine->RevertToPreviousState();
      }
    }

    void Grunt::Attack::Exit(Grunt *owner)
    {

    }

    Grunt::Attack* Grunt::Attack::Instance()
    {
      static Attack instance;
      return &instance;
    }
#pragma endregion Attack State

#pragma region Die State
    void Grunt::Die::Enter(Grunt *owner)
    {
      //DCTrace << "Grunt Die Enter\n";

      owner->RigidBodyRef->setVelocity(Vec3(0, 0, 0));

      // Death sound?
    }

    void Grunt::Die::Update(Grunt *owner)
    {
      // Timer for death animation?

      Exit(owner);
    }

    void Grunt::Die::Exit(Grunt *owner)
    {
      // Destroy grunt
      owner->gameObj->Destroy();
    }

    Grunt::Die* Grunt::Die::Instance()
    {
      static Die instance;
      return &instance;
    }
#pragma endregion Die State

  }
}