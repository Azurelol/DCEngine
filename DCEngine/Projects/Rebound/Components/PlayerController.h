/******************************************************************************/
/*!
@file   PlayerController.h
@author Connor Tilley
@par    email: connor.tilley\@digipen.edu
@date   11/23/2015
@brief  Header for PlayerController
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ReboundComponent.h" 


#define PlayerControllerTraceOn 0 // Use a static bool inside the class

namespace DCEngine {
  namespace Components {
    class Sprite;
    class Transform;
    class RigidBody;
    class BoxCollider;
    class PlayerController : public Component {

    public:
      bool Grounded = true;
      bool Jumping = false;
      bool DoAutoPlay = true; //this is a lie, false is true and true is false. i am sorry, i will fix this.
      bool Invincible = false;
      bool LevelCheatLoaded = true;
      bool musicplay = true;
      
      Real AutoPlayTimer = 2.0f;
      Real DeathTimer = 0.0f;
      Real TimeToDie = 0.7f;
      Real FramesOfDamageColor = 10.0f;
      Real FramesOfDamageColorApplied = 0.0f;
      Real MoveSpeed = 0.35f;
      Real JumpPower = 8.0f;
      Real JumpFrames = 1.0f;
      Real JumpFramesApplied = 0.0f;
      Real Health = 10.0f;
	  Real InitialHealth = Health;
      Real AirBrakeScalar = 0.94f; //multiplies your current y velocity by this when you release jump while jumping upwards
      Real TurnSpeedScalar = 5.0f; //how much faster you accelerate when attemping to accelerate opposite your current velocity
      Real InitialVelocityXCap;
      Real VelocityXCap = 30.0f;
      Real HorizontalJumpingVelocityXCap = VelocityXCap * 1.33f;
      Real GroundFriction = 0.2f;
      Real FramesOfThrowAnimation = 0.0f;
      Real AmountOfMaxSpeedRequiredForHorizontalJump = 0.8f;
      Real AmountOfJumpPowerAddedToHorizontalJump = 0.5f;
      String StandAnimation = "Char_Asha_Idle_Spritesheet";
      String JumpAnimation = "Char_Asha_Jump_Spritesheet";
      String RunAnimation = "Char_Asha_Run_Spritesheet";
      String FallAnimation = "Char_Asha_Fall_Spritesheet";
      String ThrowAnimation = "Char_Asha_Throw_Spritesheet";
      String LandingParticle = "LandingParticle";
      String TeleportStartSound = "Teleport";
      String TeleportArriveSound = "TeleportArrive";
      String FootstepSound = "Step";
      String JumpSound = "Jump";
      String LandSound = "Land";
      String CollideSound = "Collide";
      int DamageFromScrapper;
      int DamageFromSentinel;
      int DamageFromLancer;
      Vec4 ColorOnDamage;
	  Vec3 InitialLocation;
      float ColorOnDamageFlashDuration;
      float KnockBackForceOnDamageFromScrapperX;
      float KnockBackForceOnDamageFromScrapperY;
      float KnockBackForceOnDamageFromSentinelX;
      float KnockBackForceOnDamageFromSentinelY;
      float KnockBackForceOnDamageFromLancerX;
      float KnockBackForceOnDamageFromLancerY;
      float DamageCooldown;
      ArchetypeHandle ShieldArchetype;
      ArchetypeHandle RedHazeArchetype;

      Transform* TransformRef;
      RigidBody* RigidBodyRef;
      BoxCollider* ColliderRef;
      GameObject* BallRef;

      DCE_DEFINE_PROPERTY(Real, MoveSpeed);
      DCE_DEFINE_PROPERTY(Real, VelocityXCap);
      DCE_DEFINE_PROPERTY(Real, JumpPower);
      DCE_DEFINE_PROPERTY(Real, JumpFrames);
      DCE_DEFINE_PROPERTY(Real, Health);
      DCE_DEFINE_PROPERTY(Real, AirBrakeScalar);
      DCE_DEFINE_PROPERTY(Real, TurnSpeedScalar);
      DCE_DEFINE_PROPERTY(Real, AutoPlayTimer);
      DCE_DEFINE_PROPERTY(Real, AmountOfMaxSpeedRequiredForHorizontalJump);
      DCE_DEFINE_PROPERTY(Real, AmountOfJumpPowerAddedToHorizontalJump);
      DCE_DEFINE_PROPERTY(Boolean, DoAutoPlay);
      DCE_DEFINE_PROPERTY(String, StandAnimation);
      DCE_DEFINE_PROPERTY(String, JumpAnimation);
      DCE_DEFINE_PROPERTY(String, RunAnimation);
      DCE_DEFINE_PROPERTY(String, FallAnimation);
      DCE_DEFINE_PROPERTY(String, ThrowAnimation);
      DCE_DEFINE_PROPERTY(String, LandingParticle);
      DCE_DEFINE_PROPERTY(String, TeleportStartSound);
      DCE_DEFINE_PROPERTY(String, TeleportArriveSound);
      DCE_DEFINE_PROPERTY(String, JumpSound);
      DCE_DEFINE_PROPERTY(String, LandSound);
      DCE_DEFINE_PROPERTY(String, FootstepSound);
      DCE_DEFINE_PROPERTY(String, CollideSound);
      DCE_DEFINE_PROPERTY(int, DamageFromScrapper);
      DCE_DEFINE_PROPERTY(int, DamageFromSentinel);
      DCE_DEFINE_PROPERTY(int, DamageFromLancer);
      DCE_DEFINE_PROPERTY(Vec4, ColorOnDamage);
      DCE_DEFINE_PROPERTY(float, ColorOnDamageFlashDuration);
      DCE_DEFINE_PROPERTY(float, KnockBackForceOnDamageFromScrapperX);
      DCE_DEFINE_PROPERTY(float, KnockBackForceOnDamageFromScrapperY);
      DCE_DEFINE_PROPERTY(float, KnockBackForceOnDamageFromSentinelX);
      DCE_DEFINE_PROPERTY(float, KnockBackForceOnDamageFromSentinelY);
      DCE_DEFINE_PROPERTY(float, KnockBackForceOnDamageFromLancerX);
      DCE_DEFINE_PROPERTY(float, KnockBackForceOnDamageFromLancerY);
      DCE_DEFINE_PROPERTY(float, DamageCooldown);
      DCE_DEFINE_PROPERTY(ArchetypeHandle, ShieldArchetype);
      DCE_DEFINE_PROPERTY(ArchetypeHandle, RedHazeArchetype);


      PlayerController(Entity& owner) : Component(std::string("PlayerController"), owner) {}
      void Initialize();
      void OnMouseDownEvent(Events::MouseDown* event);
      void OnMouseUpEvent(Events::MouseUp* event);
      void OnKeyDownEvent(Events::KeyDown* event);
      void OnCollisionStartedEvent(Events::CollisionStarted* event);
      void OnCollisionEndedEvent(Events::CollisionEnded* event);
      void OnCollisionPersistedEvent(Events::CollisionPersisted* event);
      void OnLogicUpdateEvent(Events::LogicUpdate * event);
      //void OnDamageEvent(Events::DamageEvent event);
      void Jump();
      bool TakeDamage(int damage);
      void Die();
      Boolean CheckForGround();
      #if (DCE_USE_ZILCH_INTERNAL_BINDING)
      ZilchDeclareDerivedType(PlayerController, Component);
      #endif

    private:
      GameObject* gameObj;
      GameObjectPtr camera;
      Sprite* SpriteComponent;
      void PrintTranslation();
      void PrintVelocity();
      void AutoPlay(Events::LogicUpdate * event);
      void MoveLeft();
      void MoveRight();
      Boolean FootstepSoundEnabled = false;
      void SoundFootstep(void);
      void FlashColor(Vec4 color, float duration);
      bool IsDamageable = true;
      GameObjectPtr shield;
      Vec4 shieldColor;
      GameObjectPtr redHaze;
      void CreateShield();
      Vec4 redHazeColor;
      float redHazeAlphaValue;
      int maxHealth;
      void PlayTeleportArriveSound();
      void ReloadLevel();
      void TeleportIn();
      void DestroyTeleportParticle();
	  void WarpToCheckpoint();
	  void RestoreHealth();
	  void DestroyTeleportOutParticle();
      GameObjectPtr teleportInParticle;
	  GameObjectPtr teleportOutParticle;
    };
  }


}