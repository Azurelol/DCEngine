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
      bool Dead = false;
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
      Real AirBrakeScalar = 0.95f; //multiplies your current y velocity by this when you release jump while jumping upwards
      Real TurnSpeedScalar = 5.0f; //how much faster you accelerate when attemping to accelerate opposite your current velocity
      Real VelocityXCap = 30.0f;
      Real GroundFriction = 0.2f;
      String StandAnimation = "Char_Asha_Idle_Spritesheet";
      String JumpAnimation = "Char_Asha_Jump_Spritesheet";
      String RunAnimation = "Char_Asha_Run_Spritesheet";


      Transform* TransformRef;
      RigidBody* RigidBodyRef;
	  BoxCollider* ColliderRef;

      DCE_DEFINE_PROPERTY(Real, MoveSpeed);
      DCE_DEFINE_PROPERTY(Real, JumpPower);
      DCE_DEFINE_PROPERTY(Real, JumpFrames);
      DCE_DEFINE_PROPERTY(Real, Health);
      DCE_DEFINE_PROPERTY(Real, AirBrakeScalar);
      DCE_DEFINE_PROPERTY(Real, TurnSpeedScalar);
      DCE_DEFINE_PROPERTY(Real, AutoPlayTimer);
      DCE_DEFINE_PROPERTY(Boolean, DoAutoPlay);
      DCE_DEFINE_PROPERTY(String, StandAnimation);
      DCE_DEFINE_PROPERTY(String, JumpAnimation);
      DCE_DEFINE_PROPERTY(String, RunAnimation);


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
      void TakeDamage(int damage);
      void Die();
	  Boolean CheckForGround();
#if (DCE_USE_ZILCH_INTERNAL_BINDING)
      ZilchDeclareDerivedType(PlayerController, Component);
#endif

    private:
      Sprite* SpriteComponent;
      void PrintTranslation();
      void PrintVelocity();
      void AutoPlay(Events::LogicUpdate * event);
      void MoveLeft();
      void MoveRight();
      std::string FootstepSound;
      Boolean FootstepSoundEnabled = false;
    };
  }


}