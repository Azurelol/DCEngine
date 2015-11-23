

#include "BallController.h"
#include "../../CoreComponents.h"
#include "../../../Core/Systems/Physics/Interpolation.h"

namespace DCEngine {

	void BallController::Initialize()
	{
		auto gameObj = dynamic_cast<GameObject*>(Owner());
		Connect(Daisy->getMouse(), Events::MouseDown, BallController::OnMouseDownEvent);
		Connect(Daisy->getMouse(), Events::MouseUp, BallController::OnMouseUpEvent);
		Connect(gameObj, Events::CollisionStarted, BallController::OnCollisionStartedEvent);
		Connect(gameObj, Events::CollisionEnded, BallController::OnCollisionEndedEvent);
		Connect(SpaceRef, Events::LogicUpdate, BallController::OnLogicUpdateEvent);
		TransformRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Transform>(); // ew
		RigidBodyRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<RigidBody>();
		SpriteRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Sprite>();
		PlayerRef = SpaceRef->FindObjectByName("Mariah");
		if (BallControllerTraceOn)
		{
			DCTrace << PlayerRef->getComponent<Transform>()->Translation.x;
		}
	}

	void BallController::Serialize(Json::Value & root)
	{
	}

	void BallController::Deserialize(Json::Value & root)
	{
	}

	void BallController::OnMouseDownEvent(Events::MouseDown * event) 
	{

    // Call the CameraViewport component of the space with this screen position.. ?


    //DCTrace << "BallController::OnMouseDownEvent - Mouse Pos (Pixels): " 
    //      << event->Position.x << " y: " << event->Position.y << "\n";
    //DCTrace << "BallController::OnMouseDownEvent - Mouse Pos (World): "
    //  << coords.x << " y: " << coords.y << "\n";
		if(event->ButtonPressed == MouseButton::Right)
		{
			if (ControlScheme == ControlScheme::Connor)
			{
				FreezeBall(event->Position);
			}
		}
		if(event->ButtonPressed == MouseButton::Left)
		{
			//if (ControlScheme == ControlScheme::Connor)
			//{
			//	Frozen = false;
			//	RigidBodyRef->setDynamicState(DynamicStateType::Dynamic);
		//	}
			

			if (CurrentlyFired)
			{

			}
			else
			{
				Charging = true;
			}
		}
	}

	void BallController::OnMouseUpEvent(Events::MouseUp * event)
	{ 
		if(event->ButtonReleased == MouseButton::Left)
		{
			if (!CurrentlyFired)
			{
				auto coords = SpaceRef->getComponent<CameraViewport>()->ScreenToViewport(Vec2(event->Position));
				auto MouseVector = glm::normalize(Vec3(coords.x - PlayerRef->getComponent<Transform>()->Translation.x, coords.y - PlayerRef->getComponent<Transform>()->Translation.y, 0));
				if (CurrentCharge < MinCharge)
				{
					CurrentCharge = MinCharge;
				}
			
				RigidBodyRef->AddForce(MouseVector * ChargeFactor * CurrentCharge);

				Charging = false;
				CurrentCharge = 0;
				CurrentlyFired = true;
				SpriteRef->Color = Vec4(1, 0, 0, 1);
				RigidBodyRef->setGravityRatio(0.1);

				if (BallControllerTraceOn)
				{
					DCTrace << "BallController::OnMouseUpEvent - WorldCoords =" << coords.x << ", " << coords.y << ").\n";
					DCTrace << "BallController::OnMouseUpEvent - PlayerPos =" << PlayerRef->getComponent<Transform>()->Translation.x << ", " << PlayerRef->getComponent<Transform>()->Translation.y << ").\n";
					DCTrace << "BallController::OnMouseUpEvent - Vector of ball shot = (" << MouseVector.x << ", " << MouseVector.y << ", " << MouseVector.z << ").\n";
					DCTrace << "BallController::OnMouseUpEvent - released at screen position: " << event->Position.x << " y: " << event->Position.y << "\n";
				}
			
			}
		}
       

	}

	void BallController::OnCollisionStartedEvent(Events::CollisionStarted * event)
	{
		RigidBodyRef->setGravityRatio(1.0f);
		if (event->OtherObject->getComponent<PlayerController>())
		{
			CurrentlyFired = false;
			SpriteRef->Color = Vec4(0, 1, 0, 1);
		}
	}

	void BallController::OnCollisionEndedEvent(Events::CollisionEnded * event)
	{
	}

	void BallController::OnLogicUpdateEvent(Events::LogicUpdate * event)
	{
		if (Daisy->getKeyboard()->KeyIsDown(Keys::F))
		{
			if (BallControllerTraceOn)
			{
				DCTrace << "BallController::OnLogicUpdate :: F key pressed";
			}
			FreezeBall(Vec2(0, 0));
		}
		if (CurrentlyFired)
		{
			if(ControlScheme == ControlScheme::Connor && Daisy->getMouse()->MouseDown(MouseButton::Left))
			{
				AttractBall();
			}
			if (ControlScheme == ControlScheme::John && Daisy->getMouse()->MouseDown(MouseButton::Right))
			{
				AttractBall();
			}
		}
		if (Charging)
		{
			CurrentCharge += event->Dt;
			//DCTrace << CurrentCharge << "\n";
			if (CurrentCharge > MaxCharge)
			{
				CurrentCharge = MaxCharge;
			}
		}
		//PrintVelocity();
	}

	void BallController::ChangeColor()
	{
		if(Frozen)
		{
			SpriteRef->Color = FrozenColor;
		}
		else
		{
			auto amountCharged = CurrentCharge / MaxCharge;
			auto newColor = NormalColor * (1 - amountCharged);
			newColor += ChargedColor * amountCharged;
			SpriteRef->Color = newColor;
		}
	}



	void BallController::PrintTranslation()
	{
		DCTrace << Owner()->Name() << "::Transform.Translation(" << TransformRef->Translation.x
			<< ", " << TransformRef->Translation.y
			<< ", " << TransformRef->Translation.z << ")\n";
	}

	void BallController::PrintVelocity()
	{
		Vec3 vel = RigidBodyRef->getVelocity();
		DCTrace << Owner()->Name() << "::RigidBody.Velocity(" << vel.x << ", " << vel.y<< ", " << vel.z << ")\n";
	}

	void BallController::AttractBall()
	{
		if (Frozen)
		{
			Frozen = false;
			RigidBodyRef->setDynamicState(DynamicStateType::Dynamic);
		}
		Vec3 CenteringVector = glm::normalize(PlayerRef->getComponent<Transform>()->Translation - TransformRef->Translation);
		if (CenteringVector.y > 0)
		{
			CenteringVector.y *= 5;
		}
		RigidBodyRef->AddForce(CenteringVector * 200.0f);
	}

	void BallController::FreezeBall(Vec2 mousePosition = Vec2(0,0))
	{
		if (Frozen)
		{
			return;
		}

		Frozen = true;
		if (CurrentlyFired)
		{
			RigidBodyRef->setVelocity(Vec3());
			RigidBodyRef->setDynamicState(DynamicStateType::Static);
		}
		else
		{
			//auto coords = SpaceRef->getComponent<CameraViewport>()->ScreenToViewport(Vec2(mousePosition));
			//Interpolate(TransformRef->getTranslation(), Vec3(coords.x, coords.y, 0), 1.0f);
		}
	}

	/**************************************************************************/
	/*!
	@brief Provides the definition of this class to Zilch.
	@note This can only go in the translational unit (.cpp)
	*/
	/**************************************************************************/
#if(DCE_USE_ZILCH_INTERNAL_BINDING)
	ZilchDefineType(BallController, "BallController", DCEngineCore, builder, type) {
		// Constructor / Destructor
		ZilchBindConstructor(builder, type, BallController, "owner", Entity&);
		ZilchBindDestructor(builder, type, BallController);
		// Properties
		ZilchBindProperty(builder, type, &Transform::getTranslation, &Transform::setTranslation, "Translation");
		ZilchBindProperty(builder, type, &Transform::getRotation, &Transform::setRotation, "Rotation");
		ZilchBindProperty(builder, type, &Transform::getScale, &Transform::setScale, "Scale");
	}
#endif

}

