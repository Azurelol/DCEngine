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
      DCE_BINDING_DEFINE_PROPERTY(Grunt, Epsilon);
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
      if (IsPatrolRight)
      {
        endPosition = startingPosition + PatrolDistance;
        stateMachine->ChangeState(new PatrolRight());
      }
      else
      {
        endPosition = startingPosition - PatrolDistance;
        stateMachine->ChangeState(new PatrolLeft());
      }

      stateMachine->SetGlobalState(new Global());

      player = SpaceRef->FindObjectByName(PlayerName);
      startingPosition = TransformRef->Translation;
    }

    void Grunt::OnLogicUpdateEvent(Events::LogicUpdate * event)
    {
      stateMachine->Update();
    }

    void Grunt::OnCollisionStartedEvent(Events::CollisionStarted * event)
    {
    }

    void Grunt::OnCollisionEndedEvent(Events::CollisionEnded * event)
    {
    }

#pragma region Global State
    void Grunt::Global::Enter(Grunt *owner)
    {

    }

    void Grunt::Global::Update(Grunt *owner)
    {
      DCTrace << "Grunt Global Update\n";
      Vec3 playerPosition = owner->player->getComponent<Components::Transform>()->Translation;
      Vec3 ownerPosition = owner->TransformRef->Translation;
      float distanceFromPlayer = glm::distance(playerPosition, ownerPosition);
      
      if (distanceFromPlayer > owner->IdleRange && !owner->stateMachine->isInState<Idle>())
        owner->stateMachine->ChangeState(new Idle());
    }

    void Grunt::Global::Exit(Grunt *owner)
    {

    }
#pragma endregion Global State

#pragma region Idle State
    void Grunt::Idle::Enter(Grunt *owner)
    {
      DCTrace << "Grunt Idle Enter\n";
    }

    void Grunt::Idle::Update(Grunt *owner)
    {
      DCTrace << "Grunt Idle Update\n";
      Vec3 playerPosition = owner->player->getComponent<Components::Transform>()->Translation;
      Vec3 ownerPosition = owner->TransformRef->Translation;
      float distanceFromPlayer = glm::distance(playerPosition, ownerPosition);
      
      if (distanceFromPlayer < owner->IdleRange)
        owner->stateMachine->RevertToPreviousState();
    }

    void Grunt::Idle::Exit(Grunt *owner)
    {

    }
#pragma endregion Idle State

#pragma region Right State
    void Grunt::PatrolRight::Enter(Grunt *owner)
    {
      DCTrace << "Grunt PatrolRight Enter\n";
      owner->RigidBodyRef->setVelocity(Vec3(owner->MoveSpeed, 0, 0));
    }

    void Grunt::PatrolRight::Update(Grunt *owner)
    {
      DCTrace << "Grunt PatrolRight Update\n";
      Vec3 ownerPosition = owner->TransformRef->Translation;
      float distanceFromEnd;

      if (owner->IsPatrolRight)
        distanceFromEnd = glm::distance(ownerPosition, owner->endPosition);
      else
        distanceFromEnd = glm::distance(ownerPosition, owner->startingPosition);
      
      if (distanceFromEnd < owner->Epsilon)
        owner->stateMachine->ChangeState(new PatrolLeft());
    }

    void Grunt::PatrolRight::Exit(Grunt *owner)
    {
      owner->RigidBodyRef->setVelocity(Vec3());
    }
#pragma endregion Right State

#pragma region Left State
    void Grunt::PatrolLeft::Enter(Grunt *owner)
    {
      DCTrace << "Grunt PatrolLeft Enter\n";
      owner->RigidBodyRef->setVelocity(Vec3(owner->MoveSpeed, 0, 0));
    }

    void Grunt::PatrolLeft::Update(Grunt *owner)
    {
      DCTrace << "Grunt PatrolLeft Update\n";
      Vec3 ownerPosition = owner->TransformRef->Translation;
      float distanceFromEnd;

      if (owner->IsPatrolRight)
        distanceFromEnd = glm::distance(ownerPosition, owner->startingPosition);
      else
        distanceFromEnd = glm::distance(ownerPosition, owner->endPosition);

      if (distanceFromEnd < owner->Epsilon)
        owner->stateMachine->ChangeState(new PatrolRight());
    }

    void Grunt::PatrolLeft::Exit(Grunt *owner)
    {
      owner->RigidBodyRef->setVelocity(Vec3());
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
#pragma endregion Die State

  }
}