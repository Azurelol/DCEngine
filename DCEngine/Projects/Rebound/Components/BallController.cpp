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
#include "../../../Core/Engine/SteeringBehaviors.h"


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
      DCE_BINDING_DEFINE_PROPERTY(BallController, ChargeFactor);
      DCE_BINDING_DEFINE_PROPERTY(BallController, FrozenColor);
      DCE_BINDING_DEFINE_PROPERTY(BallController, NormalGravity);
      DCE_BINDING_DEFINE_PROPERTY(BallController, ShotGravity);
	  DCE_BINDING_DEFINE_PROPERTY(BallController, AttractYBoost																																																															);
      DCE_BINDING_DEFINE_PROPERTY(BallController, ChargedColor);
      DCE_BINDING_DEFINE_PROPERTY(BallController, ForcedFreeze);
      DCE_BINDING_DEFINE_PROPERTY(BallController, FreezeEnabled);
      DCE_BINDING_DEFINE_PROPERTY(BallController, MaxAttractSpeed);
	  DCE_BINDING_DEFINE_PROPERTY(BallController, MaxSlamDistance);
      DCE_BINDING_DEFINE_PROPERTY(BallController, MaxAttractForce);
      DCE_BINDING_DEFINE_PROPERTY(BallController, AttractArriveDistance);
      DCE_BINDING_DEFINE_PROPERTY(BallController, MinAttractSpeed);
      DCE_BINDING_DEFINE_METHOD_NO_ARGS(BallController, ParentToPlayer);
      DCE_BINDING_DEFINE_METHOD_NO_ARGS(BallController, FreezeBall);
      DCE_BINDING_DEFINE_METHOD_NO_ARGS(BallController, ToggleForceFreeze);
    DCE_BINDING_DEFINE_METHOD_NO_ARGS(BallController, PseudoFreezeBall);
      DCE_BINDING_DEFINE_METHOD_NO_ARGS(BallController, LockBall);
      DCE_BINDING_DEFINE_METHOD_NO_ARGS(BallController, UnlockBall);

      DCE_BINDING_DEFINE_PROPERTY(BallController, CommandSound);
      DCE_BINDING_DEFINE_PROPERTY(BallController, CollideSound);
      DCE_BINDING_DEFINE_PROPERTY(BallController, FreezeSound);
      //ZilchBindMethod(builder, type, &BallController::FreezeBall, ZilchNoOverload, "FreezeBall", ZilchNoNames);
    }
#endif

    void BallController::Initialize()
    {
      gameObj = dynamic_cast<GameObject*>(Owner());
      Connect(Daisy->getMouse(), Events::MouseDown, BallController::OnMouseDownEvent);
      Connect(Daisy->getMouse(), Events::MouseUp, BallController::OnMouseUpEvent);
      Connect(gameObj, Events::CollisionStarted, BallController::OnCollisionStartedEvent);
	  Connect(gameObj, Events::CollisionPersisted, BallController::OnCollisionPersistedEvent);
      Connect(gameObj, Events::CollisionEnded, BallController::OnCollisionEndedEvent);
      Connect(SpaceRef, Events::LogicUpdate, BallController::OnLogicUpdateEvent);
      TransformRef = dynamic_cast<GameObject*>(Owner())->getComponent<Components::Transform>(); // ew
      RigidBodyRef = dynamic_cast<GameObject*>(Owner())->getComponent<Components::RigidBody>();
      SpriteRef = dynamic_cast<GameObject*>(Owner())->getComponent<Components::Sprite>();
	  ParticleRef = dynamic_cast<GameObject*>(Owner())->getComponent<Components::SpriteParticleSystem>();
      LightRef = dynamic_cast<GameObject*>(Owner())->getComponent<Components::Light>();
      //ColliderRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Components::CircleCollider>();
      CollisionTableRef = Daisy->getSystem<Systems::Content>()->getCollisionTable(std::string(this->SpaceRef->getComponent<Components::PhysicsSpace>()->getCollisionTable()));
      CollisionTableRef->AddGroup("Ball");
      CollisionTableRef->AddGroup("Player");
      CollisionTableRef->AddGroup("Shield");
      ColliderRef = dynamic_cast<GameObject*>(Owner())->getComponent<Components::BoxCollider>();
      ColliderRef->setCollisionGroup("Ball");
      CollisionTableRef->SetResolve("Ball", "Shield", CollisionFlag::SkipResolution);
      PlayerRef = SpaceRef->FindObjectByName(PlayerName);
      TrailRef = SpaceRef->CreateObject("TestParticle");
      TrailRef->AttachTo(gameObj);
      TrailRef->getComponent<Components::Transform>()->setTranslation(TransformRef->Translation);
	  NormalColor = SpriteRef->Color;
	  NormalParticleColor = ParticleRef->getTint();


    Frozen = false;
    Powering = false;
    RigidBodyRef->setDynamicState(DynamicStateType::Dynamic);
    SpriteRef->Color = NormalColor;
    ParentToPlayer();

      if (BallControllerTraceOn)
      {
        DCTrace << "Player X is " << PlayerRef->getComponent<Components::Transform>()->Translation.x;
      }
      RigidBodyRef->setRestitution(Restitution);
      RigidBodyRef->setFriction(Friction);
      //CollisionTableRef->SetResolve("Ball", "Player", CollisionFlag::SkipResolution);

      ChargeInstance = nullptr;
    }

    void BallController::OnMouseDownEvent(Events::MouseDown * event)
    {

      if (ForcedFreeze == true)
      {
        return;
      }

      // Call the CameraViewport component of the space with this screen position.. ?


      //DCTrace << "BallController::OnMouseDownEvent - Mouse Pos (Pixels): " 
      //      << event->Position.x << " y: " << event->Position.y << "\n";
      //DCTrace << "BallController::OnMouseDownEvent - Mouse Pos (World): "
      //  << coords.x << " y: " << coords.y << "\n";
      if (event->ButtonPressed == MouseButton::Right)
      {

        IsAttracting = true;

        SoundCommand();

        if (ControlScheme == ControlScheme::Connor)
        {
          FreezeBall();
        }
        if (ControlScheme == ControlScheme::John)
        {
          CollisionTableRef->SetResolve("Ball", "Player", CollisionFlag::SkipResolution);
        }
      }
      if (event->ButtonPressed == MouseButton::Left)
      {
        //if (ControlScheme == ControlScheme::Connor)
        //{
        //	Frozen = false;
        //	RigidBodyRef->setDynamicState(DynamicStateType::Dynamic);
        //	}
        //SoundCommand();


        if (CurrentlyFired)
        {
        }
        else
        {
          ChargeInstance = SpaceRef->getComponent<Components::SoundSpace>()->PlayCue("BallCharge");
          Charging = true;
        }
      }
    }

    void BallController::OnMouseUpEvent(Events::MouseUp * event)
    {

      if (ForcedFreeze == true)
      {
        return;
      }
      if (event->ButtonReleased == MouseButton::Left)
      {
        if (gameObj->Parent() != nullptr)
        {
          int x = 10;
          gameObj->Detach();
          int i = 9;
        }
        auto coords = SpaceRef->getComponent<Components::CameraViewport>()->ScreenToViewport(Vec2(event->Position));
        auto MouseVector = glm::normalize(Vec3(coords.x - PlayerRef->getComponent<Components::Transform>()->Translation.x, coords.y - PlayerRef->getComponent<Components::Transform>()->Translation.y, 0));
		if (CurrentlyFired && glm::distance(TransformRef->getTranslation(), PlayerRef->getComponent<Components::Transform>()->getTranslation()) < MaxSlamDistance)
        {
          // DCTrace << "BallController::OnMouseUpEvent - Slam\n";
          RigidBodyRef->setVelocity(Vec3(0, 0, 0));
          RigidBodyRef->ApplyForce(MouseVector * SlamPower);
        }
        else if (CurrentlyFired)
        {
          return;
        }
        else
        {
          if (CurrentCharge < MinCharge)
          {
            CurrentCharge = MinCharge;
          }

          RigidBodyRef->ApplyForce(MouseVector * ChargeFactor * CurrentCharge);
		  PlayerRef->getComponent<Components::PlayerController>()->FramesOfThrowAnimation = 10;
		  ColliderRef->Ghost = false;
		  ColliderRef->SendsEvents = true;
		  SpriteRef->Visible = true;
          Charging = false;
          CurrentCharge = 0;
          CurrentlyFired = true;
          RigidBodyRef->setGravityRatio(ShotGravity);
          
          if (ChargeInstance != nullptr)
          {
            ChargeInstance->Stop();
          }

          if (BallControllerTraceOn)
          {
            DCTrace << "BallController::OnMouseUpEvent - WorldCoords =" << coords.x << ", " << coords.y << ").\n";
            DCTrace << "BallController::OnMouseUpEvent - PlayerPos =" << PlayerRef->getComponent<Components::Transform>()->Translation.x << ", " << PlayerRef->getComponent<Components::Transform>()->Translation.y << ").\n";
            DCTrace << "BallController::OnMouseUpEvent - Vector of ball shot = (" << MouseVector.x << ", " << MouseVector.y << ", " << MouseVector.z << ").\n";
            DCTrace << "BallController::OnMouseUpEvent - released at screen position: " << event->Position.x << " y: " << event->Position.y << "\n";
          }

        }
      }
      if (event->ButtonReleased == MouseButton::Right && CollidingWithPlayer == false && Locked == false)
      {
        CollisionTableRef->SetResolve("Ball", "Player", CollisionFlag::Resolve);
      }


    }

    void BallController::OnCollisionStartedEvent(Events::CollisionStarted * event)
    {
      Boolean hitPlayer = false;
      Boolean hitTerrain = false;

      RigidBodyRef->setGravityRatio(NormalGravity);
      if (event->OtherObject->getComponent<Components::PlayerController>())
	  {
        hitPlayer = true;
        CollidingWithPlayer = true;
        if (CollisionTableRef->GetResolve("Ball", "Player") == CollisionFlag::SkipResolution && (gameObj->Parent() == nullptr || gameObj->Parent()->getComponent<Components::LockField>()))
        {
			if(gameObj->Parent() && gameObj->Parent()->getComponent<Components::LockField>() != nullptr)
			{
				gameObj->Parent()->getComponent<Components::LockField>()->UnlockBall(gameObj);
			}
          ParentToPlayer();
        }
      }
      std::string terrain = std::string("Terrain");
      if(event->OtherObject->getComponent<Components::BoxCollider>() && event->OtherObject->getComponent<Components::BoxCollider>()->getCollisionGroup() == terrain)  //this is a bad check for terrain, fix later
      {
        hitTerrain = true;
        auto particle = SpaceRef->CreateObject("BounceParticle");
        if (particle)
        {
          particle->getComponent<Components::Transform>()->setTranslation(TransformRef->Translation + RigidBodyRef->getVelocity() / 50.0f); //bad way to get collision point
        }
      }

      std::string DialogTrigger = std::string("DialogTrigger");
      // If collides with any type of enemy
      if (event->OtherObject->Name() != DialogTrigger && (hitPlayer == false) && (hitTerrain == false) )
      {
        SoundCollide();
      }
      
    }

    void BallController::OnCollisionEndedEvent(Events::CollisionEnded * event)
    {
      if (event->OtherObject->getComponent<Components::PlayerController>())
      {
        CollidingWithPlayer = false;
        if (CollisionTableRef->GetResolve("Ball", "Player") == CollisionFlag::SkipResolution)
        {
          CollisionTableRef->SetResolve("Ball", "Player", CollisionFlag::Resolve);
          //gameObj->Detach();
        }
      }
    }

	void BallController::OnCollisionPersistedEvent(Events::CollisionPersisted * event)
	{
	}

    void BallController::OnLogicUpdateEvent(Events::LogicUpdate * event)
    {
	  ChangeParticle();
      //DCTrace << "BallController::Init- trail is at" << TrailRef->getComponent<Components::Transform>()->getTranslation().x << ", " << TrailRef->getComponent<Components::Transform>()->getTranslation().y << "\n";
      //DCTrace << "BallController::Init- ball is at" << TransformRef->getTranslation().x << ", " << TransformRef->getTranslation().y << "\n";
      if (gameObj->Parent() != nullptr)
      {
		  if (gameObj->Parent()->getComponent<Components::PlayerController>() != nullptr)
		  {
			  RigidBodyRef->setDynamicState(DynamicStateType::Dynamic);
			  CollisionTableRef->SetResolve("Ball", "Player", CollisionFlag::SkipResolution);
			  ColliderRef->SendsEvents = false;
		  }
		RigidBodyRef->setVelocity(Vec3(0, 0, 0));
        TransformRef->setTranslation(gameObj->Parent()->getComponent<Components::Transform>()->Translation);
      }
      //DCTrace << (CollisionTableRef->GetResolve("Ball", "Player");
      if (Daisy->getKeyboard()->KeyIsDown(Keys::F))
      {
        if (ForcedFreeze == true)
        {
          return;
        }
        if (BallControllerTraceOn)
        {
          DCTrace << "BallController::OnLogicUpdate :: F key pressed";
        }
        FreezeBall();
      }			
      if (Daisy->getKeyboard()->KeyIsDown(Keys::E))
      {
        if (ForcedFreeze == true)
        {
          return;
        }
        ParentToPlayer();
      }
      if (ControlScheme == ControlScheme::Connor && Daisy->getMouse()->MouseDown(MouseButton::Left))
      {
        if (ForcedFreeze == true)
        {
          return;
        }
        AttractBall();
      }
      if (ControlScheme == ControlScheme::John && Daisy->getMouse()->MouseDown(MouseButton::Right))
      {
        if (ForcedFreeze == true)
        {
          return;
        }
        //IsAttracting = true;
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
      //PrintVelocity();
      if (LightRef != nullptr)
      {
        LightRef->setRange(((MaximumLightRange - MinimumLightRange) * CurrentCharge / MaxCharge) + MinimumLightRange);
      }
      if (BallControllerTraceOn)
      {
        PrintTranslation();
      }
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

	void BallController::ChangeParticle()
	{
		if (Frozen)
		{
			ParticleRef->setTint(FrozenParticleColor);
		}
		else
		{
			ParticleRef->setTint(NormalParticleColor);
		}
	}



    void BallController::PrintTranslation()
    {
      //DCTrace << Owner()->Name() << "::Transform.Translation(" << TransformRef->Translation.x
      //	<< ", " << TransformRef->Translation.y
      //	<< ", " << TransformRef->Translation.z << ")\n";
    }

    void BallController::PrintVelocity()
    {
      Vec3 vel = RigidBodyRef->getVelocity();
      //DCTrace << Owner()->Name() << "::RigidBody.Velocity(" << vel.x << ", " << vel.y << ", " << vel.z << ")\n";
    }

    void BallController::AttractBall()
    {
      if (!IsAttracting)
        return;

      if (gameObj->Parent() != nullptr)
        return;
      if (BallControllerTraceOn)
      {
        DCTrace << "BallController::AttractBall :: Now attracting!";
      }
      if (Frozen || Powering)
      {
        Frozen = false;
        Powering = false;
        RigidBodyRef->setDynamicState(DynamicStateType::Dynamic);
        SpriteRef->Color = NormalColor;
      }
      CollisionTableRef->SetResolve("Ball", "Player", CollisionFlag::SkipResolution);

      Vec3 CenteringVector = PlayerRef->getComponent<Components::Transform>()->Translation - TransformRef->Translation;
      if (CenteringVector != Vec3(0, 0, 0))
      {
        CenteringVector = glm::normalize(CenteringVector);
      }

      if (Locked)
      {
        if (CenteringVector.y < 0)
        {
          CenteringVector.y *= AttractYBoost;
        }
        PlayerRef->getComponent<Components::RigidBody>()->ApplyForce(-CenteringVector * AttractPower);
      }
	  else if (gameObj->Parent() != nullptr)
	  {
		  return;
	  }
      else
      {
        // JJ- Using steering behaviors for more natural looking movement
        //if (CenteringVector.y > 0)
        //{
        //	CenteringVector.y *= AttractYBoost;
        //}
        //RigidBodyRef->AddForce(CenteringVector * AttractPower);
        Vec3 seekForce = SteeringBehaviors::GetSeekVelocity(TransformRef->Translation, PlayerRef->getComponent<Components::Transform>()->Translation, RigidBodyRef->getVelocity(), MaxAttractSpeed, MaxAttractForce);
        //Vec3 arriveForce = SteeringBehaviors::GetArriveVelocity(TransformRef->Translation, PlayerRef->getComponent<Components::Transform>()->Translation, RigidBodyRef->getVelocity(), MaxAttractSpeed, MaxAttractForce, AttractArriveDistance, MinAttractSpeed);
        RigidBodyRef->ApplyLinearVelocity(seekForce);
      }
    }

    void BallController::FreezeBall()
    {
      if (gameObj->Parent() != nullptr)
        return;
      if (Frozen || Locked || !FreezeEnabled)
      {
        return;
      }

      Frozen = true;
      SpriteRef->Color = FrozenColor;
      SoundFreeze();
      if (CurrentlyFired)
      {
        RigidBodyRef->setVelocity(Vec3(0, 0, 0));
        RigidBodyRef->setDynamicState(DynamicStateType::Static);
        SoundFreeze();
      }
      else
      {
        //deprecated, leaving in place in case of control scheme change
        //auto coords = SpaceRef->getComponent<Components::CameraViewport>()->ScreenToViewport(Vec2(mousePosition));
        //Interpolate(TransformRef->getTranslation(), Vec3(coords.x, coords.y, 0), 1.0f);
      }
    }

    void BallController::ToggleForceFreeze()
    {
      if (ForcedFreeze == false)
      {
        UnlockBall();
        CollidingWithPlayer = true;
        ParentToPlayer();
       // FreezeBall();
        ForcedFreeze = true;
      }
      else
      {
        ForcedFreeze = false;
      }
    }

    void BallController::PseudoFreezeBall()
    {
    if (Frozen || Locked || !FreezeEnabled)
    {
      return;
    }

    Frozen = true;
    SpriteRef->Color = FrozenColor;
    SoundFreeze();
    if (CurrentlyFired)
    {
      RigidBodyRef->setVelocity(Vec3(0, 0, 0));
      RigidBodyRef->setGravityRatio(0.0);
      SoundFreeze();
    }
    else
    {
      //deprecated, leaving in place in case of control scheme change
      //auto coords = SpaceRef->getComponent<Components::CameraViewport>()->ScreenToViewport(Vec2(mousePosition));
      //Interpolate(TransformRef->getTranslation(), Vec3(coords.x, coords.y, 0), 1.0f);
    }
  }

    void BallController::ParentToPlayer()
    {
      if (gameObj->Parent() != nullptr)
      {
        return;
      }

      SoundCommand();
	  Locked = false;
	  Frozen = false;
      CollisionTableRef->SetResolve("Ball", "Player", CollisionFlag::SkipResolution);
      auto particle = SpaceRef->CreateObject("BallExplosionParticle");
      if (particle)
      {
        particle->getComponent<Components::Transform>()->setTranslation(TransformRef->Translation);
      }
      CurrentlyFired = false;
      //SpriteRef->Color = NormalColor;
      RigidBodyRef->setVelocity(Vec3(0, 0, 0));
	  ColliderRef->Ghost = true;
	  ColliderRef->SendsEvents = false;
	  //SpriteRef->Visible = false;
      RigidBodyRef->setDynamicState(DynamicStateType::Dynamic);
      TransformRef->setTranslation(PlayerRef->getComponent<Components::Transform>()->Translation);
      gameObj->AttachTo(PlayerRef);
    }

    void BallController::UnlockBall()
    {
      Locked = false;
    }

    void BallController::LockBall()
    {
      Locked = true;
    }

    
    void BallController::SoundFreeze(void)
    {
      SpaceRef->getComponent<Components::SoundSpace>()->PlayCue(FreezeSound);
    }
    
    void BallController::SoundCommand(void)
    {
      SpaceRef->getComponent<Components::SoundSpace>()->PlayCue(CommandSound);
    }

    void BallController::SoundCollide(void)
    {
      SpaceRef->getComponent<Components::SoundSpace>()->PlayCue(CollideSound);
    }

  }
}

