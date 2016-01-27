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
#pragma once
#include "../ReboundComponent.h"
#include "../../../../Core/Systems/StateMachine/StateMachine.h"

namespace DCEngine {
  namespace Components {
    class Transform;
    class RigidBody;
    class Sprite;
    class Grunt : public Component 
    {

    public:
      Transform* TransformRef;
      RigidBody* RigidBodyRef;
      Sprite* SpriteRef;
      String PlayerName = "Player";
      float IdleRange;        // Past this range, the grunt will be idle, within the range, it will patrol
      float PatrolDistance;   // The distance from the starting position that the grunt will move before turning around
      bool IsPatrolRight;       // True = Grunt moves right first, false = moves left first

      // Properties
      DCE_DEFINE_PROPERTY(String, PlayerName);
      DCE_DEFINE_PROPERTY(float, IdleRange);
      DCE_DEFINE_PROPERTY(float, PatrolDistance);
      DCE_DEFINE_PROPERTY(bool, IsPatrolRight);

      // Methods
      Grunt(Entity& owner) : Component(std::string("Grunt"), owner) {}
      void Initialize();
      void OnCollisionStartedEvent(Events::CollisionStarted* event);
      void OnCollisionEndedEvent(Events::CollisionEnded* event);
      void OnLogicUpdateEvent(Events::LogicUpdate * event);

#if (DCE_USE_ZILCH_INTERNAL_BINDING)
      ZilchDeclareDerivedType(Grunt, Component);
#endif

    private:
      StateMachine<Grunt> *stateMachine;
      GameObject *player;
      Vec3 startingPosition;

      class Idle : public IState<Grunt>
      {
        void Enter(Grunt *owner);
        void Update(Grunt *owner);
        void Exit(Grunt *owner);
      };

      class PatrolRight : public IState<Grunt>
      {
        void Enter(Grunt *owner);
        void Update(Grunt *owner);
        void Exit(Grunt *owner);
      };

      class PatrolLeft : public IState<Grunt>
      {
        void Enter(Grunt *owner);
        void Update(Grunt *owner);
        void Exit(Grunt *owner);
      };

      class Attack : public IState<Grunt>
      {
        void Enter(Grunt *owner);
        void Update(Grunt *owner);
        void Exit(Grunt *owner);
      };

      class Die : public IState<Grunt>
      {
        void Enter(Grunt *owner);
        void Update(Grunt *owner);
        void Exit(Grunt *owner);
      };
    };
  }

}