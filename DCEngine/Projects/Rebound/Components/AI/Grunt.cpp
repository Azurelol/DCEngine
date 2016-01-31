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
      DCE_BINDING_DEFINE_PROPERTY(Grunt, MoveSpeed);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, JumpStrengthX);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, JumpStrengthY);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, JumpFrequency);
    }
#endif

    Grunt::~Grunt()
    {
      delete stateMachine;
    }


    void Grunt::Initialize()
    {
      DCTrace << "Grunt Initialize\n";
      auto gameObj = dynamic_cast<GameObject*>(Owner());
      Connect(SpaceRef, Events::LogicUpdate, Grunt::OnLogicUpdateEvent);

      TransformRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Components::Transform>(); 
      RigidBodyRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Components::RigidBody>();
      SpriteRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Components::Sprite>();
    
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
    }

    void Grunt::OnCollisionEndedEvent(Events::CollisionEnded * event)
    {
    }

    // Direction should be 1 (right) or -1 (left). 
    void Grunt::Jump(int direction)
    {
      if (jumpTimer > JumpFrequency)
      {
        RigidBodyRef->setVelocity(RigidBodyRef->getVelocity() + Vec3(JumpStrengthX *  direction, JumpStrengthY, 0));
        jumpTimer = 0;
      }

      jumpTimer += dt;
    }

#pragma region Global State
    void Grunt::Global::Enter(Grunt *owner)
    {

    }

    void Grunt::Global::Update(Grunt *owner)
    {
      Vec3 playerPosition = owner->player->getComponent<Components::Transform>()->Translation;
      Vec3 ownerPosition = owner->TransformRef->Translation;
      float distanceFromPlayer = glm::distance(playerPosition, ownerPosition);
      
      if ((distanceFromPlayer > owner->IdleRange) && !owner->stateMachine->isInState(Idle::Instance()))
        owner->stateMachine->ChangeState(Idle::Instance());
    }

    void Grunt::Global::Exit(Grunt *owner)
    {

    }

    Grunt::Global* Grunt::Global::Instance()
    {
      static Global instance;
      return &instance;
    }
#pragma endregion Global State

#pragma region Idle State
    void Grunt::Idle::Enter(Grunt *owner)
    {
      DCTrace << "Grunt Idle Enter\n";
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
      DCTrace << "Grunt PatrolRight Enter\n";
      DCTrace << "StartPosition: " << owner->startingPosition.x << ", " << owner->startingPosition.y << ", " << owner->startingPosition.z << "\n";
      DCTrace << "EndPosition: " << owner->endPosition.x << ", " << owner->endPosition.y << ", " << owner->endPosition.z << "\n";
    }

    void Grunt::PatrolRight::Update(Grunt *owner)
    {
      Vec3 ownerPosition = owner->TransformRef->Translation;
      owner->Jump(1);
      
      if (owner->IsPatrolRight)
      {
        if(ownerPosition.x > owner->endPosition.x)
          owner->stateMachine->ChangeState(PatrolLeft::Instance());
      }
      else
      {
        if(ownerPosition.x > owner->startingPosition.x)
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
      DCTrace << "Grunt PatrolLeft Enter\n";
      DCTrace << "StartPosition: " << owner->startingPosition.x << ", " << owner->startingPosition.y << ", " << owner->startingPosition.z << "\n";
      DCTrace << "EndPosition: " << owner->endPosition.x << ", " << owner->endPosition.y << ", " << owner->endPosition.z << "\n";
    }

    void Grunt::PatrolLeft::Update(Grunt *owner)
    {
      Vec3 ownerPosition = owner->TransformRef->Translation;
      owner->Jump(-1);

      if (owner->IsPatrolRight)
      {
        if (ownerPosition.x < owner->startingPosition.x)
          owner->stateMachine->ChangeState(PatrolRight::Instance());
      }
      else
      {
        if (ownerPosition.x < owner->endPosition.x)
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

    }

    void Grunt::Attack::Update(Grunt *owner)
    {

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

    }

    void Grunt::Die::Update(Grunt *owner)
    {

    }

    void Grunt::Die::Exit(Grunt *owner)
    {

    }

    Grunt::Die* Grunt::Die::Instance()
    {
      static Die instance;
      return &instance;
    }
#pragma endregion Die State

  }
}