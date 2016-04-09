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
#pragma once
#include "../ReboundComponent.h"
#include "../../../../Core/Systems/StateMachine/StateMachine.h"
#include "../../ReboundEvents.h"

namespace DCEngine {
  namespace Components {
    class Transform;
    class RigidBody;
    class Sprite;
    class PhysicsSpace;
    class GraphicsSpace;
    class LancerShield;
    class Lancer : public Component
    {

    public:
      GameObject* gameObj;
      Transform* TransformRef;
      RigidBody* RigidBodyRef;
      Sprite* SpriteRef;
      PhysicsSpace* PhysicsSpaceRef;
      CollisionTablePtr CollisionTableRef;
      GraphicsSpace* GraphicsSpaceRef;
      String PlayerName = "Player";
      float IdleRange;        // Past this range, the grunt will be idle, within the range, it will patrol
      float ChargeForce;
      float ShieldVelocityDifferenceThreshold;
      float ShieldActivationSpeed;
      


      DCE_COMPONENT_DECLARE_DEPENDENCIES;

      // Properties
      DCE_DEFINE_PROPERTY(String, PlayerName);
      DCE_DEFINE_PROPERTY(int, startingHealth);
      DCE_DEFINE_PROPERTY(int, maxHealth);
      DCE_DEFINE_PROPERTY(bool, IsInvulnerable);
      DCE_DEFINE_PROPERTY(float, IdleRange);
      DCE_DEFINE_PROPERTY(float, ChargeForce);
      DCE_DEFINE_PROPERTY(float, ShieldVelocityDifferenceThreshold);
      DCE_DEFINE_PROPERTY(float, ShieldActivationSpeed);

      DCE_DEFINE_PROPERTY(String, AttackSound);
      DCE_DEFINE_PROPERTY(String, DeathSound);
      DCE_DEFINE_PROPERTY(String, TakeDamageSound);

      // Methods
      Lancer(Entity& owner) : Component(std::string("Lancer"), owner) {}
      ~Lancer();
      void Initialize();
      void OnCollisionStartedEvent(Events::CollisionStarted* event);
      void OnLogicUpdateEvent(Events::LogicUpdate * event);

#if (DCE_USE_ZILCH_INTERNAL_BINDING)
      ZilchDeclareDerivedType(Lancer, Component);
#endif

    private:
      StateMachine<Lancer> *stateMachine;
      GameObject *player;
      Vec3 startingPosition;
      Vec3 endPosition;
      int health;
      int startingHealth;
      int maxHealth;
      bool IsInvulnerable;
      LancerShield *shield;

      String AttackSound;
      String DeathSound;
      String TakeDamageSound;

      double timer;
      double cooldown;
      bool canPlay;

      bool ModifyHealth(int amount);

      void PlayAttackSound(void);
      void PlayDamagedSound(void);
      void PlayDeathSound(void);
      
      class Global : public IState<Lancer>
      {
      public:
        static Global* Instance();
        void Enter(Lancer *owner);
        void Update(Lancer *owner);
        void Exit(Lancer *owner);

      private:
        Global() {};
      };

      class Idle : public IState<Lancer>
      {
      public:
        static Idle* Instance();
        void Enter(Lancer *owner);
        void Update(Lancer *owner);
        void Exit(Lancer *owner);
      private:
        Idle() {};
      };

      class ChargeLeft : public IState<Lancer>
      {
      public:
        static ChargeLeft* Instance();
        void Enter(Lancer *owner);
        void Update(Lancer *owner);
        void Exit(Lancer *owner);
      private:
        ChargeLeft() {};
      };

      class ChargeRight : public IState<Lancer>
      {
      public:
        static ChargeRight* Instance();
        void Enter(Lancer *owner);
        void Update(Lancer *owner);
        void Exit(Lancer *owner);
      private:
        ChargeRight() {};
      };

      class Die : public IState<Lancer>
      {
      public:
        static Die* Instance();
        void Enter(Lancer *owner);
        void Update(Lancer *owner);
        void Exit(Lancer *owner);
      private:
        Die() {};
      };
    };
  }

}