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
      DCE_BINDING_DEFINE_PROPERTY(Grunt, PlayerName);
    }
#endif


    void Grunt::Initialize()
    {
      auto gameObj = dynamic_cast<GameObject*>(Owner());
      TransformRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Components::Transform>(); 
      SpriteRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Components::Sprite>();
    
      *stateMachine = StateMachine<Grunt>(this);
      if (IsPatrolRight)
        stateMachine->ChangeState(new PatrolRight());
      else
        stateMachine->ChangeState(new PatrolLeft());

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

    static float distanceSq(Vec3 a, Vec3 b)
    {
      return  (a.x - b.x) + (a.y - b.y) + (a.z - b.z);
    }

#pragma region Idle State
    void Grunt::Idle::Enter(Grunt *owner)
    {

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
#pragma endregion Idle State

#pragma region Patrol1 State
    void Grunt::PatrolRight::Enter(Grunt *owner)
    {

    }

    void Grunt::PatrolRight::Update(Grunt *owner)
    {

    }

    void Grunt::PatrolRight::Exit(Grunt *owner)
    {

    }
#pragma endregion Patrol1 State

#pragma region Patrol2 State
    void Grunt::PatrolLeft::Enter(Grunt *owner)
    {

    }

    void Grunt::PatrolLeft::Update(Grunt *owner)
    {

    }

    void Grunt::PatrolLeft::Exit(Grunt *owner)
    {

    }
#pragma endregion Patrol2 State

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