/******************************************************************************/
/*!
@file   BallController.cpp
@author Connor Tilley
@par    email: connor.tilley\@digipen.edu
@date   11/23/2015
@brief  Controller logic for the ball.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "BallController.h"
#include "../../CoreComponents.h"
#include "../../../Core/Systems/Physics/Interpolation.h"

namespace DCEngine {
  
  namespace Components
  {
#if(DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDefineType(BallController, "BallController", Rebound, builder, type) {
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(BallController);
      DCE_BINDING_DEFINE_PROPERTY(BallController, PlayerName);
      DCE_BINDING_DEFINE_PROPERTY(BallController, MoveSpeed);
      DCE_BINDING_DEFINE_PROPERTY(BallController, RotSpeed);
      DCE_BINDING_DEFINE_PROPERTY(BallController, MinCharge);
      DCE_BINDING_DEFINE_PROPERTY(BallController, MaxCharge);
      DCE_BINDING_DEFINE_PROPERTY(BallController, FrozenColor);
      DCE_BINDING_DEFINE_PROPERTY(BallController, NormalColor);
      DCE_BINDING_DEFINE_PROPERTY(BallController, ChargedColor);
      DCE_BINDING_DEFINE_PROPERTY(BallController, FreezeEnabled);
    }
#endif

    void BallController::Initialize()
    {
      auto gameObj = dynamic_cast<GameObject*>(Owner());
      Connect(Daisy->getMouse(), Events::MouseDown, BallController::OnMouseDownEvent);
      Connect(Daisy->getMouse(), Events::MouseUp, BallController::OnMouseUpEvent);
      Connect(gameObj, Events::CollisionStarted, BallController::OnCollisionStartedEvent);
      Connect(gameObj, Events::CollisionEnded, BallController::OnCollisionEndedEvent);
      Connect(SpaceRef, Events::LogicUpdate, BallController::OnLogicUpdateEvent);
      TransformRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Components::Transform>(); // ew
      RigidBodyRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Components::RigidBody>();
      SpriteRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Components::Sprite>();
      PlayerRef = SpaceRef->FindObjectByName(PlayerName);
      if (BallControllerTraceOn)
      {
        DCTrace << PlayerRef->getComponent<Components::Transform>()->Translation.x;
      }
	  RigidBodyRef->setRestitution(Restitution);
	  RigidBodyRef->setFriction(Friction);
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
      if (event->ButtonPressed == MouseButton::Right)
      {
        if (ControlScheme == ControlScheme::Connor)
        {
          FreezeBall();
        }
      }
      if (event->ButtonPressed == MouseButton::Left)
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
      if (event->ButtonReleased == MouseButton::Left)
      {
        if (!CurrentlyFired)
        {
          auto coords = SpaceRef->getComponent<Components::CameraViewport>()->ScreenToViewport(Vec2(event->Position));
          auto MouseVector = glm::normalize(Vec3(coords.x - PlayerRef->getComponent<Components::Transform>()->Translation.x, coords.y - PlayerRef->getComponent<Components::Transform>()->Translation.y, 0));
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
            DCTrace << "BallController::OnMouseUpEvent - PlayerPos =" << PlayerRef->getComponent<Components::Transform>()->Translation.x << ", " << PlayerRef->getComponent<Components::Transform>()->Translation.y << ").\n";
            DCTrace << "BallController::OnMouseUpEvent - Vector of ball shot = (" << MouseVector.x << ", " << MouseVector.y << ", " << MouseVector.z << ").\n";
            DCTrace << "BallController::OnMouseUpEvent - released at screen position: " << event->Position.x << " y: " << event->Position.y << "\n";
          }

        }
      }


    }

    void BallController::OnCollisionStartedEvent(Events::CollisionStarted * event)
    {
      RigidBodyRef->setGravityRatio(1.0f);
      if (event->OtherObject->getComponent<Components::PlayerController>())
      {
        CurrentlyFired = false;
        SpriteRef->Color = NormalColor;
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
        FreezeBall();
      }
		if (ControlScheme == ControlScheme::Connor && Daisy->getMouse()->MouseDown(MouseButton::Left))
		{
			AttractBall();
		}
		if (ControlScheme == ControlScheme::John && Daisy->getMouse()->MouseDown(MouseButton::Right))
		{
			AttractBall();
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
      PrintVelocity();
    }

    void BallController::ChangeColor()
    {
      if (Frozen)
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
      DCTrace << Owner()->Name() << "::RigidBody.Velocity(" << vel.x << ", " << vel.y << ", " << vel.z << ")\n";
    }

    void BallController::AttractBall()
    {
		if (BallControllerTraceOn)
		{
			DCTrace << "BallController::AttractBall :: Now attracting!";
		}
      if (Frozen)
      {
        Frozen = false;
        RigidBodyRef->setDynamicState(DynamicStateType::Dynamic);
		SpriteRef->Color = NormalColor;
      }
      Vec3 CenteringVector = glm::normalize(PlayerRef->getComponent<Components::Transform>()->Translation - TransformRef->Translation);
      if (CenteringVector.y > 0)
      {
        CenteringVector.y *= AttractYBoost;
      }
	  if (Locked)
	  {
		  PlayerRef->getComponent<Components::RigidBody>()->AddForce(-CenteringVector * AttractPower);
	  }
	  else
	  {
		  RigidBodyRef->AddForce(CenteringVector * AttractPower);
	  }
    }

    void BallController::FreezeBall()
    {
      if (Frozen || Locked || !FreezeEnabled)
      {
        return;
      }

      Frozen = true;
	  SpriteRef->Color = FrozenColor;
      if (CurrentlyFired)
      {
        RigidBodyRef->setVelocity(Vec3(0,0,0));
        RigidBodyRef->setDynamicState(DynamicStateType::Static);
      }
      else
      {
		  //deprecated, leaving in place in case of control scheme change
        //auto coords = SpaceRef->getComponent<Components::CameraViewport>()->ScreenToViewport(Vec2(mousePosition));
        //Interpolate(TransformRef->getTranslation(), Vec3(coords.x, coords.y, 0), 1.0f);
      }
    }

  }
}

