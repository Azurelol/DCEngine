/******************************************************************************/
/*!
@file   BallController.h
@author Connor Tilley
@par    email: connor.tilley\@digipen.edu
@date   11/23/2015
@brief Header for BallController
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ReboundComponent.h"
#include "../../../Core/Components/BoxCollider.h"

#define BallControllerTraceOn 0
namespace DCEngine {
  namespace Components
  {
    class Transform;
    class RigidBody;
    class Sprite;
    class Light;
    class CircleCollider;
    enum class ControlScheme { John, Connor };
    class BallController : public Component {

    public:

      bool Translation = true;
      bool Frozen = false;
      bool Locked = false;
      Boolean Powering = false;
      bool CurrentlyFired = false;
      bool CollidingWithPlayer = false;
      Real MoveSpeed = 0.75f;
      ControlScheme ControlScheme = ControlScheme::John;
      Real RotSpeed = 15.0f;
      Transform* TransformRef;
      RigidBody* RigidBodyRef;
      Sprite* SpriteRef;
      Light* LightRef;
      CollisionTablePtr CollisionTableRef;
      //CircleCollider* ColliderRef;
      Real CurrentCharge = 0.0f;
      Real MaxCharge = 1.0f;
      Real MinCharge = 0.5f;
      Real ChargeFactor = 20.0f * 1000.0f;
      Real Restitution = 0.1f; //set from 0 to 1
      Real Friction = 0.1f; //set from 0 to 1
      Real AttractPower = 450.0f;
      Real AttractYBoost = 3.0f;
      Real MaxAttractSpeed = 12.50f;
      Real MinAttractSpeed = 6.0f;
      Real MaxAttractForce = 12.50f;
      Real AttractArriveDistance = 200.0f;
      Real SlamPower = ChargeFactor * MaxCharge;
      Real MinimumLightRange = 0.2f;
      Real MaximumLightRange = 2.0f;
      Real ShotGravity = 0.1f;
      Real NormalGravity = 0.25f;
      Boolean Charging = false;
      Boolean ForcedFreeze = false;
      Boolean FreezeEnabled = true;
      Vec4 FrozenColor = Vec4(1, 0, 1, 1);
      Vec4 NormalColor = Vec4(0.0f, 0.7f, 0.3f, 1.0f);
      Vec4 ChargedColor = Vec4(0.0f, 0.7f, 1.0f, 1.0f);
      GameObject* PlayerRef; // Use a pointer inside, better to use a handle for serialization
      GameObject* gameObj;
      GameObject* TrailRef;
      String PlayerName = "Player";

      DCE_DEFINE_PROPERTY(String, PlayerName);
      //DCE_DEFINE_PROPERTY(Boolean, Translation);
      DCE_DEFINE_PROPERTY(Real, MoveSpeed);
      DCE_DEFINE_PROPERTY(Real, RotSpeed);
      DCE_DEFINE_PROPERTY(Real, CurrentCharge);
      DCE_DEFINE_PROPERTY(Real, ChargeFactor);
      DCE_DEFINE_PROPERTY(Real, MaxCharge);
      DCE_DEFINE_PROPERTY(Real, MinCharge);
      DCE_DEFINE_PROPERTY(Real, NormalGravity);
      DCE_DEFINE_PROPERTY(Real, ShotGravity);
      DCE_DEFINE_PROPERTY(Vec4, FrozenColor);
      DCE_DEFINE_PROPERTY(Vec4, NormalColor);
      DCE_DEFINE_PROPERTY(Vec4, ChargedColor);
      DCE_DEFINE_PROPERTY(Boolean, ForcedFreeze);
      DCE_DEFINE_PROPERTY(Boolean, FreezeEnabled);
      DCE_DEFINE_PROPERTY(Boolean, Charging);
      DCE_DEFINE_PROPERTY(Real, MaxAttractSpeed);
      DCE_DEFINE_PROPERTY(Real, MinAttractSpeed);
      DCE_DEFINE_PROPERTY(Real, MaxAttractForce);
      DCE_DEFINE_PROPERTY(Real, AttractArriveDistance);

      DCE_DEFINE_PROPERTY(String, FreezeSound);
      DCE_DEFINE_PROPERTY(String, CommandSound);
      DCE_DEFINE_PROPERTY(String, CollideSound);


      BallController(Entity& owner) : Component(std::string("BallController"), owner) {}
      void Initialize();
      void OnMouseDownEvent(Events::MouseDown* event);
      void OnMouseUpEvent(Events::MouseUp* event);
      void OnCollisionStartedEvent(Events::CollisionStarted* event);
      void OnCollisionEndedEvent(Events::CollisionEnded* event);
      void BallController::OnLogicUpdateEvent(Events::LogicUpdate * event);
      void ChangeColor();
      void ParentToPlayer();
      void AttractBall();
      void FreezeBall();
      void ToggleForceFreeze();
	    void PseudoFreezeBall();
      void LockBall();
      void UnlockBall();

#if (DCE_USE_ZILCH_INTERNAL_BINDING)
      ZilchDeclareDerivedType(BallController, Component);
#endif

    private:
      void PrintTranslation();
      void PrintVelocity();

      void SoundFreeze(void);
      void SoundCommand(void);
      void SoundCollide(void);

      String FreezeSound  = "BallFreeze";
      String CommandSound = "BallCommand";
      String CollideSound = "BallCollide";
    };
  }

}

