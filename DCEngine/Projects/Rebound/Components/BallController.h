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
			Real MaxCharge = 2.0f;
			Real MinCharge = 0.5f;
			Real ChargeFactor = 30.0f * 1000.0f;
			Real Restitution = 0.1f; //set from 0 to 1
			Real Friction = 0.1f; //set from 0 to 1
			Real AttractPower = 400.0f;
			Real AttractYBoost = 3.0f;
		    Real MaxAttractSpeed = 100.0f;
            Real MaxAttractForce = 100.0f;
            Real AttractArriveDistance = 200.0f;
			Real SlamPower = ChargeFactor * MaxCharge;
			Real MinimumLightRange = 0.2f;
			Real MaximumLightRange = 2.0f;

			Boolean Charging = false;
			Boolean FreezeEnabled = true;
			Vec4 FrozenColor = Vec4(1, 0, 1, 1);
			Vec4 NormalColor = Vec4(0.0f, 0.7f, 0.3f, 1.0f);
			Vec4 ChargedColor = Vec4(0.0f, 0.7f, 1.0f, 1.0f);
			GameObject* PlayerRef; // Use a pointer inside, better to use a handle for serialization
			GameObject* gameObj;
			String PlayerName = "Player";

			DCE_DEFINE_PROPERTY(String, PlayerName);
			//DCE_DEFINE_PROPERTY(Boolean, Translation);
			DCE_DEFINE_PROPERTY(Real, MoveSpeed);
			DCE_DEFINE_PROPERTY(Real, RotSpeed);
			DCE_DEFINE_PROPERTY(Real, CurrentCharge);
			DCE_DEFINE_PROPERTY(Real, MaxCharge);
			DCE_DEFINE_PROPERTY(Real, MinCharge);
			DCE_DEFINE_PROPERTY(Vec4, FrozenColor);
			DCE_DEFINE_PROPERTY(Vec4, NormalColor);
			DCE_DEFINE_PROPERTY(Vec4, ChargedColor);
			DCE_DEFINE_PROPERTY(Boolean, FreezeEnabled);
			DCE_DEFINE_PROPERTY(Boolean, Charging);
      DCE_DEFINE_PROPERTY(Real, MaxAttractSpeed);
      DCE_DEFINE_PROPERTY(Real, MaxAttractForce);
      DCE_DEFINE_PROPERTY(Real, AttractArriveDistance);


			BallController(Entity& owner) : Component(std::string("BallController"), owner) {}
			void Initialize();
			virtual void Serialize(Json::Value& root);
			virtual void Deserialize(Json::Value& root);
			void OnMouseDownEvent(Events::MouseDown* event);
			void OnMouseUpEvent(Events::MouseUp* event);
			void OnCollisionStartedEvent(Events::CollisionStarted* event);
			void OnCollisionEndedEvent(Events::CollisionEnded* event);
			void BallController::OnLogicUpdateEvent(Events::LogicUpdate * event);
			void ChangeColor();

#if (DCE_USE_ZILCH_INTERNAL_BINDING)
			ZilchDeclareDerivedType(BallController, Component);
#endif

		private:
			void PrintTranslation();
			void PrintVelocity();
			void AttractBall();
			void FreezeBall();
		};
	}

}

