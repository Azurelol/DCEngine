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
      ArchetypeHandle HeadArchetype;
      ArchetypeHandle ShoulderArchetype;
      ArchetypeHandle BodyArchetype;
      ArchetypeHandle SpearArchetype;
      float IdleRange;        // Past this range, the grunt will be idle, within the range, it will patrol
      float ChargeForce;
      float ShieldVelocityDifferenceThreshold;
      float ShieldActivationSpeed;
      float AnimationSpeedHead;
      float AnimationDistanceHead;
      float AnimationSpeedShoulder;
      float AnimationDistanceShoulder;
      float AnimationSpeedSpear;
      float AnimationDistanceSpear;


      DCE_COMPONENT_DECLARE_DEPENDENCIES;

      // Properties
      DCE_DEFINE_PROPERTY(String, PlayerName);
      DCE_DEFINE_PROPERTY(int, startingHealth);
      DCE_DEFINE_PROPERTY(int, maxHealth);
      DCE_DEFINE_PROPERTY(bool, IsInvulnerable);
      DCE_DEFINE_PROPERTY(ArchetypeHandle, HeadArchetype);
      DCE_DEFINE_PROPERTY(ArchetypeHandle, ShoulderArchetype);
      DCE_DEFINE_PROPERTY(ArchetypeHandle, BodyArchetype);
      DCE_DEFINE_PROPERTY(ArchetypeHandle, SpearArchetype);
      DCE_DEFINE_PROPERTY(float, IdleRange);
      DCE_DEFINE_PROPERTY(float, ChargeForce);
      DCE_DEFINE_PROPERTY(float, ShieldVelocityDifferenceThreshold);
      DCE_DEFINE_PROPERTY(float, ShieldActivationSpeed);
      DCE_DEFINE_PROPERTY(float, AnimationSpeedHead);
      DCE_DEFINE_PROPERTY(float, AnimationDistanceHead);
      DCE_DEFINE_PROPERTY(float, AnimationSpeedShoulder);
      DCE_DEFINE_PROPERTY(float, AnimationDistanceShoulder);
      DCE_DEFINE_PROPERTY(float, AnimationSpeedSpear);
      DCE_DEFINE_PROPERTY(float, AnimationDistanceSpear);

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
      GameObjectPtr head;
      GameObjectPtr shoulder;
      GameObjectPtr body;
      GameObjectPtr spear;
      std::vector<GameObjectPtr> sprites;
      Vec3 startingPosition;
      Vec3 endPosition;
      int health;
      int startingHealth;
      int maxHealth;
      bool IsInvulnerable;
      LancerShield *shield;
      float randomPhase;
      bool ModifyHealth(int amount);
      void CreateSprites();
      void UpdateSprites(float timePassed);
      void FlipSprites(bool flipx);

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