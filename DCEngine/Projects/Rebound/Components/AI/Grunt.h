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

      // Properties
      DCE_DEFINE_PROPERTY(String, PlayerName);

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
      float IdleRange;

      class Idle : public IState<Grunt>
      {
        void Enter(Grunt *owner)
        {

        }

        void Update(Grunt *owner)
        {
          //Vec2 distanceToPlayer = player->getComponent<Components::Transform>()->Translation
        }

        void Exit(Grunt *owner)
        {

        }
      };
    };
  }

}