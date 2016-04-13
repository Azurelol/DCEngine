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
#include "../../ReboundEvents.h"

// AI
#include "../../../../Core/Systems/StateMachine/StateMachine.h"

namespace DCEngine {
  namespace Components {
    class Transform;
    class RigidBody;
    class Sprite;
    class PhysicsSpace;
    class Sentinel : public Component
    {

    public:
      DCE_COMPONENT_DECLARE_DEPENDENCIES;

      // Properties
      String PlayerName = "Player";
      float IdleRange; // Past this range, the grunt will be idle, within the range, it will patrol
      float MoveSpeed;
      ArchetypeHandle HeadArchetype;
      ArchetypeHandle BodyArchetype;
      ArchetypeHandle ShoulderArchetype;
      ArchetypeHandle ShieldArchetype;
      float ShieldRadius;
      float ShieldBashDistance;
      float ShieldBashOutTime;
      float ShieldBashInTime;
      float ShieldBashCooldown;
      float AnimationSpeedHead;
      float AnimationDistanceHead;
      float AnimationSpeedShoulder;
      float AnimationDistanceShoulder;
      Vec4 DamageTakenColor;
      float DamageTakenColorFlashSpeed;
      float BallReflectForce;
	  GameObjectPtr particle;

      DCE_DEFINE_PROPERTY(String, PlayerName);
      DCE_DEFINE_PROPERTY(int, startingHealth);
      DCE_DEFINE_PROPERTY(int, maxHealth);
      DCE_DEFINE_PROPERTY(bool, IsInvulnerable);
      DCE_DEFINE_PROPERTY(float, IdleRange);
      DCE_DEFINE_PROPERTY(float, MoveSpeed);
      DCE_DEFINE_PROPERTY(ArchetypeHandle, ShieldArchetype);
      DCE_DEFINE_PROPERTY(ArchetypeHandle, HeadArchetype);
      DCE_DEFINE_PROPERTY(ArchetypeHandle, ShoulderArchetype);
      DCE_DEFINE_PROPERTY(ArchetypeHandle, BodyArchetype);
      DCE_DEFINE_PROPERTY(float, ShieldRadius);
      DCE_DEFINE_PROPERTY(float, ShieldBashDistance);
      DCE_DEFINE_PROPERTY(float, ShieldBashOutTime);
      DCE_DEFINE_PROPERTY(float, ShieldBashInTime);
      DCE_DEFINE_PROPERTY(float, ShieldBashCooldown);
      DCE_DEFINE_PROPERTY(float, AnimationSpeedHead);
      DCE_DEFINE_PROPERTY(float, AnimationDistanceHead);
      DCE_DEFINE_PROPERTY(float, AnimationSpeedShoulder);
      DCE_DEFINE_PROPERTY(float, AnimationDistanceShoulder);
      DCE_DEFINE_PROPERTY(Vec4, DamageTakenColor);
      DCE_DEFINE_PROPERTY(float, DamageTakenColorFlashSpeed);
      DCE_DEFINE_PROPERTY(float, BallReflectForce);


      // Methods
      ZilchDeclareDerivedType(Sentinel, Component);
      Sentinel(Entity& owner) : Component(std::string("Sentinel"), owner) {}
      ~Sentinel();
      void Initialize();
      void OnCollisionStartedEvent(Events::CollisionStarted* event);
      void OnShieldCollisionStartedEvent(Events::CollisionStarted* event);
      void OnLogicUpdateEvent(Events::LogicUpdate * event);

    private:
      StateMachine<Sentinel> *stateMachine;
      GameObject *player;
      float dt;
      GameObjectPtr shield;
      GameObjectPtr head;
      GameObjectPtr shoulder;
      GameObjectPtr body;
      std::vector<GameObjectPtr> sprites;
      GameObject* gameObj;
      Transform* TransformRef;
      RigidBody* RigidBodyRef;
      Sprite* SpriteRef;
      PhysicsSpace* PhysicsSpaceRef;
      Vec3 shieldLocalTranslation;
      int health;
      int startingHealth;
      int maxHealth;
      bool IsInvulnerable;
      bool isBashing;
      bool canBash;
      float randomPhase;
      void CreateShield();
      void UpdateShield();
      void ShieldBash();
      bool ModifyHealth(int amount);
      void CreateSprites();
      void UpdateSprites(float timePassed);
      void FlipSprites(bool flipx);
      void FlashColor(Vec4 color, float duration);
     
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

      class Attack : public IState<Sentinel>
      {
      public:
        static Attack* Instance();
        void Enter(Sentinel *owner);
        void Update(Sentinel *owner);
        void Exit(Sentinel *owner);
      private:
        Attack() {};
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