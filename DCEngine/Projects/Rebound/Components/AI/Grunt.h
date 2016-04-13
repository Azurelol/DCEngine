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
#include "../../ReboundEvents.h"
#include <random>

namespace DCEngine {
  namespace Components {
    class Transform;
    class RigidBody;
    class Sprite;
    class Grunt : public Component 
    {

    public:
      GameObject* gameObj;
      Transform* TransformRef;
      RigidBody* RigidBodyRef;
      Sprite* SpriteRef;
      String PlayerName = "Player";
      ArchetypeHandle HeadArchetype;
      ArchetypeHandle BodyArchetype;
      ArchetypeHandle SawArchetype;
      float IdleRange;        // Past this range, the grunt will be idle, within the range, it will patrol
      float PatrolDistance;   // The distance from the starting position that the grunt will move before turning around
      bool IsPatrolRight;       // True = Grunt moves right first, false = moves left first
      bool IsAggressive;
      float JumpStrengthX;
      float JumpStrengthY;
      float JumpPeriod;
      float randomPhase;
      float AttackJumpStrengthX;
      float AttackJumpStrengthY;
      float AttackJumpPeriod;
      float AnimationSpeedHead;
      float AnimationDistanceHead;
      float AnimationSpeedSaw;
      float AnimationDistanceSaw;
      //bool IsDebugColorActive;
      //Vec4 IdleColor = Vec4(0, 1, 0, 1);
      //Vec4 PatrolColor = Vec4(0, 0, 1, 1);
      Vec4 AttackColor;
      Vec4 DamageTakenColor;
      float DamageTakenColorFlashSpeed;
      float BallReflectForce;
      

      DCE_COMPONENT_DECLARE_DEPENDENCIES;

      // Properties
      DCE_DEFINE_PROPERTY(String, PlayerName);
      DCE_DEFINE_PROPERTY(int, startingHealth);
      DCE_DEFINE_PROPERTY(int, maxHealth);
      DCE_DEFINE_PROPERTY(bool, IsInvulnerable);
      DCE_DEFINE_PROPERTY(ArchetypeHandle, HeadArchetype);
      DCE_DEFINE_PROPERTY(ArchetypeHandle, BodyArchetype);
      DCE_DEFINE_PROPERTY(ArchetypeHandle, SawArchetype);
      DCE_DEFINE_PROPERTY(float, IdleRange);
      DCE_DEFINE_PROPERTY(float, PatrolDistance);
      DCE_DEFINE_PROPERTY(bool, IsPatrolRight);
      DCE_DEFINE_PROPERTY(bool, IsAggressive);
      DCE_DEFINE_PROPERTY(float, JumpStrengthX);
      DCE_DEFINE_PROPERTY(float, JumpStrengthY);
      DCE_DEFINE_PROPERTY(float, JumpPeriod);
      DCE_DEFINE_PROPERTY(float, AttackJumpStrengthX);
      DCE_DEFINE_PROPERTY(float, AttackJumpStrengthY);
      DCE_DEFINE_PROPERTY(float, AttackJumpPeriod);
      DCE_DEFINE_PROPERTY(float, AnimationSpeedHead);
      DCE_DEFINE_PROPERTY(float, AnimationDistanceHead);
      DCE_DEFINE_PROPERTY(float, AnimationSpeedSaw);
      DCE_DEFINE_PROPERTY(float, AnimationDistanceSaw);
      //DCE_DEFINE_PROPERTY(Vec4, IdleColor);
      //DCE_DEFINE_PROPERTY(Vec4, PatrolColor);
      DCE_DEFINE_PROPERTY(Vec4, AttackColor);
      DCE_DEFINE_PROPERTY(Vec4, DamageTakenColor);
      //DCE_DEFINE_PROPERTY(bool, IsDebugColorActive);
      DCE_DEFINE_PROPERTY(float, DamageTakenColorFlashSpeed);
      DCE_DEFINE_PROPERTY(float, BallReflectForce);

      DCE_DEFINE_PROPERTY(String, AttackSound);
      DCE_DEFINE_PROPERTY(String, DeathSound);
      DCE_DEFINE_PROPERTY(String, TakeDamageSound);

      // Methods
      Grunt(Entity& owner) : Component(std::string("Grunt"), owner) {}
      ~Grunt();
      void Initialize();
      void OnCollisionStartedEvent(Events::CollisionStarted* event);
      void OnLogicUpdateEvent(Events::LogicUpdate * event);

#if (DCE_USE_ZILCH_INTERNAL_BINDING)
      ZilchDeclareDerivedType(Grunt, Component);
#endif

    private:
      StateMachine<Grunt> *stateMachine;
      GameObject *player;
      GameObjectPtr head;
      GameObjectPtr body;
      GameObjectPtr saw;
      std::vector<GameObjectPtr> sprites;
      int health;
      int startingHealth;
      int maxHealth;
      bool IsInvulnerable;
      Vec3 startingPosition;
      Vec3 endPosition;
      float jumpTimer = 0;
      float dt;
      Vec4 defaultColor;
      String AttackSound;
      String DeathSound;
      String TakeDamageSound;

      double timer;
      double cooldown;
      bool canPlay;

      bool ModifyHealth(int amount);
      void CreateSprites();
      void UpdateSprites(float timePassed);
      void FlipSprites(bool flipx);
      void ChangeStateRight();
      void ChangeStateLeft();
      void ChangeSpritesColor(Vec4 newColor);
      void FlashColor(Vec4 color, float duration);

      void Jump(int direction, float period, float strengthX, float strengthY);

      void PlayAttackSound(void);
      void PlayDamagedSound(void);
      void PlayDeathSound(void);

      class Global : public IState<Grunt>
      {
      public:
        static Global* Instance();
        void Enter(Grunt *owner);
        void Update(Grunt *owner);
        void Exit(Grunt *owner);

      private:
        Global() {};
      };

      class Idle : public IState<Grunt>
      {
      public:
        static Idle* Instance();
        void Enter(Grunt *owner);
        void Update(Grunt *owner);
        void Exit(Grunt *owner);
      private:
        Idle() {};
      };

      class PatrolRight : public IState<Grunt>
      {
      public:
        static PatrolRight* Instance();
        void Enter(Grunt *owner);
        void Update(Grunt *owner);
        void Exit(Grunt *owner);
      private:
        PatrolRight() {};
      };

      class PatrolLeft : public IState<Grunt>
      {
      public:
        static PatrolLeft* Instance();
        void Enter(Grunt *owner);
        void Update(Grunt *owner);
        void Exit(Grunt *owner);
      private:
        PatrolLeft() {};
      };

      class Attack : public IState<Grunt>
      {
      public:
        static Attack* Instance();
        void Enter(Grunt *owner);
        void Update(Grunt *owner);
        void Exit(Grunt *owner);
      private:
        Attack() {};
      };

      class Die : public IState<Grunt>
      {
      public:
        static Die* Instance();
        void Enter(Grunt *owner);
        void Update(Grunt *owner);
        void Exit(Grunt *owner);
      private:
        Die() {};
      };
    };
  }

}