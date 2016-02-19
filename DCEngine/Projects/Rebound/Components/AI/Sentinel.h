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
#pragma once
#include "../ReboundComponent.h"
#include "../../../../Core/Systems/StateMachine/StateMachine.h"
#include "../../ReboundEvents.h"

namespace DCEngine {
  namespace Components {
    class Transform;
    class RigidBody;
    class Sprite;
    class HealthController;
    class Sentinel : public Component
    {

    public:
      GameObject* gameObj;
      Transform* TransformRef;
      RigidBody* RigidBodyRef;
      Sprite* SpriteRef;
      HealthController* HealthRef;
      String PlayerName = "Player";
      float IdleRange;        // Past this range, the grunt will be idle, within the range, it will patrol
      String shieldArchetypeName = "Sentinel Shield";

      // Dependancies
      DCE_COMPONENT_DECLARE_DEPENDENCIES;

      // Properties
      DCE_DEFINE_PROPERTY(String, PlayerName);
      DCE_DEFINE_PROPERTY(float, IdleRange);
      DCE_DEFINE_PROPERTY(String, shieldArchetypeName);


      // Methods
      Sentinel(Entity& owner) : Component(std::string("Sentinel"), owner) {}
      ~Sentinel();
      void Initialize();
      void OnCollisionStartedEvent(Events::CollisionStarted* event);
      void OnLogicUpdateEvent(Events::LogicUpdate * event);
      void OnDeathEvent(Events::DeathEvent * event);
      void CreateShield();

#if (DCE_USE_ZILCH_INTERNAL_BINDING)
      ZilchDeclareDerivedType(Sentinel, Component);
#endif

    private:
      StateMachine<Sentinel> *stateMachine;
      GameObject *player;
      float dt;
      GameObjectPtr shield;
     
      class Global : public IState<Sentinel>
      {
      public:
        static Global* Instance();
        void Enter(Sentinel *owner);
        void Update(Sentinel *owner);
        void Exit(Sentinel *owner);

      private:
        Global() {};
      };

      class Idle : public IState<Sentinel>
      {
      public:
        static Idle* Instance();
        void Enter(Sentinel *owner);
        void Update(Sentinel *owner);
        void Exit(Sentinel *owner);
      private:
        Idle() {};
      };

      

      class Die : public IState<Sentinel>
      {
      public:
        static Die* Instance();
        void Enter(Sentinel *owner);
        void Update(Sentinel *owner);
        void Exit(Sentinel *owner);
      private:
        Die() {};
      };
    };
  }

}