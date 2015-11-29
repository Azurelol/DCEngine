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

#define BallControllerTraceOn 0
namespace DCEngine {

	class Transform;
	class RigidBody;
	class Sprite;
	enum class ControlScheme { John, Connor };
	class BallController : public Component {

	public:

		bool Translation = true;
		bool Frozen = false;
		bool CurrentlyFired = false;
		Real MoveSpeed = 0.75;
		ControlScheme ControlScheme = ControlScheme::John;
		Real RotSpeed = 15;
		Transform* TransformRef;
		RigidBody* RigidBodyRef;
		Sprite* SpriteRef;
		Real CurrentCharge = 0;
		Real MaxCharge = 2;
		Real MinCharge = 0.5f;
		Real ChargeFactor = 100 * 1000;
		Boolean Charging = false;
		Boolean FreezeEnabled = false;
		Vec4 FrozenColor = Vec4(1, 0, 1, 1);
		Vec4 NormalColor = Vec4(0.0f, 0.7f, 0.3f, 1.0f);
		Vec4 ChargedColor = Vec4(0.0f, 0.7f, 1.0f, 1.0f);
		GameObject* PlayerRef; // Use a pointer inside, better to use a handle for serialization
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
		void FreezeBall(Vec2 MousePosition);


	};
}

