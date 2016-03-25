/******************************************************************************/
/*!
@file   PlayerController.cpp
@author Connor Tilley
@par    email: connor.tilley\@digipen.edu
@date   11/23/2015
@brief  Controller for the main player! Handles everything the player does.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "PlayerController.h"
#include "../../CoreComponents.h"

namespace DCEngine {

  namespace Components
  {
    #if(DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDefineType(PlayerController, "PlayerController", Rebound, builder, type) {
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(PlayerController);
      DCE_BINDING_DEFINE_PROPERTY(PlayerController, MoveSpeed);
      DCE_BINDING_DEFINE_PROPERTY(PlayerController, JumpPower);
      DCE_BINDING_DEFINE_PROPERTY(PlayerController, JumpFrames);
      DCE_BINDING_DEFINE_PROPERTY(PlayerController, Health);
      DCE_BINDING_DEFINE_PROPERTY(PlayerController, AirBrakeScalar);
      DCE_BINDING_DEFINE_PROPERTY(PlayerController, TurnSpeedScalar);
      DCE_BINDING_DEFINE_PROPERTY(PlayerController, DoAutoPlay);
      DCE_BINDING_DEFINE_PROPERTY(PlayerController, StandAnimation);
      DCE_BINDING_DEFINE_PROPERTY(PlayerController, JumpAnimation);
	  DCE_BINDING_DEFINE_PROPERTY(PlayerController, RunAnimation);
      DCE_BINDING_DEFINE_PROPERTY(PlayerController, AutoPlayTimer);
    }
    #endif

    void PlayerController::Initialize()
    {
      auto gameObj = dynamic_cast<GameObject*>(Owner());
      Connect(Daisy->getMouse(), Events::MouseDown, PlayerController::OnMouseDownEvent);
      Connect(Daisy->getMouse(), Events::MouseUp, PlayerController::OnMouseUpEvent);
      Connect(Daisy->getKeyboard(), Events::KeyDown, PlayerController::OnKeyDownEvent);
      Connect(gameObj, Events::CollisionStarted, PlayerController::OnCollisionStartedEvent);
      Connect(gameObj, Events::CollisionEnded, PlayerController::OnCollisionEndedEvent);
      Connect(gameObj, Events::CollisionPersisted, PlayerController::OnCollisionPersistedEvent);
      Connect(SpaceRef, Events::LogicUpdate, PlayerController::OnLogicUpdateEvent);
      //Connect(gameObj, Events::DamageEvent, PlayerController::OnDamageEvent);

      TransformRef = dynamic_cast<GameObject*>(Owner())->getComponent<Components::Transform>(); // ew
      RigidBodyRef = dynamic_cast<GameObject*>(Owner())->getComponent<Components::RigidBody>();
      ColliderRef = dynamic_cast<GameObject*>(Owner())->getComponent<Components::BoxCollider>();
      SpriteComponent = dynamic_cast<GameObject*>(Owner())->getComponent<Components::Sprite>();

      // ColliderRef->	 
      auto CollisionTableRef = Daisy->getSystem<Systems::Content>()->getCollisionTable(std::string(this->SpaceRef->getComponent<Components::PhysicsSpace>()->getCollisionTable()));
      //CollisionTableRef->AddGroup("Player");
      auto ColliderRef = dynamic_cast<GameObject*>(Owner())->getComponent<Components::BoxCollider>();
      //ColliderRef->setCollisionGroup("Player");
      //RigidBodyRef->setGravity(false);
    }

    void PlayerController::OnMouseDownEvent(Events::MouseDown * event)
    {

    }
    void PlayerController::OnMouseUpEvent(Events::MouseUp * event)
    {

    }

    void PlayerController::OnKeyDownEvent(Events::KeyDown* event)
    {
      if (event->Key == Keys::Tilde)
      {
        LevelCheatLoaded = false;
      }
      if (LevelCheatLoaded == true)
      {
        return;
      }

      String level;
      switch (event->Key)
      {
      case Keys::I:
        Invincible = !Invincible;
        if (Invincible)
        {
          SpriteComponent->Color = Vec4(1, 1, 0, 1);
        }
        else
        {
          SpriteComponent->Color = Vec4(1, 1, 1, 1);
        }
        break;
      case Keys::U:
        DoAutoPlay = !DoAutoPlay;
        if (!DoAutoPlay)
        {
          SpriteComponent->Color = Vec4(0, 0.5, 0, 1);
        }
        else
        {
          SpriteComponent->Color = Vec4(1, 1, 1, 1);
        }
        break;
      case Keys::O:
        LevelCheatLoaded = true;
        Die();
        break;
      case Keys::P:
        LevelCheatLoaded = true;
        level = "YouWon";
        SpaceRef->LoadLevel(level);
        break;
      case Keys::L:
        LevelCheatLoaded = false;
        break;
      case Keys::Num1:
        LevelCheatLoaded = true;
        level = "Level1";
        SpaceRef->LoadLevel(level);
        break;
      case Keys::Num2:
        LevelCheatLoaded = true;
        level = "Level2";
        SpaceRef->LoadLevel(level);
        break;
      case Keys::Num3:
        LevelCheatLoaded = true;
        level = "Level3";
        SpaceRef->LoadLevel(level);
        break;

      default:
        break;
      }
    }

    void PlayerController::OnCollisionStartedEvent(Events::CollisionStarted * event)
    {
      if (event->OtherObject->getComponent<Components::Transform>()->getTranslation().y + event->OtherObject->getComponent<Components::Transform>()->getScale().y / 2 < TransformRef->getTranslation().y)
      {
        Grounded = true;
        //this->SpaceRef->getComponent<Components::SoundSpace>()->PlayCue("HighThud");
		auto particle = SpaceRef->CreateObject("LandingParticle");
		if (particle)
		{
			particle->getComponent<Components::Transform>()->setTranslation(TransformRef->Translation - Vec3(0, TransformRef->getScale().y / 2, 0));
		}
      }
      if (event->OtherObject->getComponent<Components::LevelManager>())
      {
		  if (event->OtherObject->getComponent<Components::Fade>())
		  {
			  event->OtherObject->getComponent<Components::Fade>()->setFading(true);
		  }
      }
    }

    void PlayerController::OnCollisionEndedEvent(Events::CollisionEnded * event)
    {
      //Grounded = false;
      if (PlayerControllerTraceOn)
      {
        DCTrace << "PlayerController::OnCollisionEndedEvent \n";
      }
    }

    void PlayerController::OnCollisionPersistedEvent(Events::CollisionPersisted * event)
    {
      //DCTrace << "PlayerController::OnCollisionPersistedEvent - \n";
    }


    void PlayerController::OnLogicUpdateEvent(Events::LogicUpdate * event)
    {
	  if (PlayerControllerTraceOn)
	  {
			PrintTranslation();
	  }
	  //Grounded = CheckForGround();
      if (glm::abs(RigidBodyRef->getVelocity().x) > VelocityXCap)
      {
        Vec3 currentVel = RigidBodyRef->getVelocity();
        currentVel.x = VelocityXCap * glm::sign(currentVel.x);
        RigidBodyRef->setVelocity(currentVel);
      }

      //hacking in logic for color changing, use fade later
      if (Dead)
      {
        DeathTimer += event->Dt;
        if (DeathTimer > TimeToDie)
        {
          Dead = false;
          this->SpaceRef->ReloadLevel();
        }
        return;
      }
      if (SpriteComponent->Color == Vec4(1, 0, 0, 1))
      {
        if (FramesOfDamageColorApplied < FramesOfDamageColor)
        {
          FramesOfDamageColorApplied++;
        }
        else
        {
          FramesOfDamageColorApplied = 0;
          SpriteComponent->Color = Vec4(1, 1, 1, 1);
        }

      }


      if (!DoAutoPlay)
      {
        AutoPlayTimer += event->Dt;
        AutoPlay(event);
      }
      //DCTrace << "Grounded =" << Grounded << "\n";
      if (!Grounded)
      {

        SpriteComponent->SpriteSource = JumpAnimation;
		//SpriteComponent->AnimationActive = false;
        //SpriteComponent->HaveAnimation = false;
        //SpriteComponent->AnimationActive = false;
        RigidBodyRef->setVelocity(RigidBodyRef->getVelocity() * Vec3(0.96f, 0.99f, 1));
      }


      if (Daisy->getKeyboard()->KeyIsDown(Keys::W) || Daisy->getKeyboard()->KeyIsDown(Keys::Space))
      {
        if (Grounded)
        {
			auto Sequence = Actions::Sequence(this->Owner()->Actions);
			auto seq = Actions::Sequence(Owner()->Actions);
			Actions::Delay(seq, 0.1f);
			Actions::Call(seq, &PlayerController::Jump, this);
          Jumping = true;
          Grounded = false;
        }
      }
      else
      {
        //SpriteComponent->HaveAnimation = true;
        //SpriteComponent->AnimationActive = true;
        Jumping = false;
        if (RigidBodyRef->getVelocity().y > 0)
        {
          RigidBodyRef->setVelocity(RigidBodyRef->getVelocity() * Vec3(1, AirBrakeScalar, 1));
        }
      }

      if (Daisy->getKeyboard()->KeyIsDown(Keys::A))
      {
        SpriteComponent->FlipX = true;
        MoveLeft();
		auto mat = Daisy->getSystem<Systems::Content>()->getPhysicsMaterial(ColliderRef->getPhysicsMaterial());
		mat->setFriction(GroundFriction);
        if (Grounded)
        {
          SpriteComponent->SpriteSource = RunAnimation;
		  //SpriteComponent->AnimationActive = true;
        }
      }
      else if (Daisy->getKeyboard()->KeyIsDown(Keys::D))
      {
        SpriteComponent->FlipX = false;
        MoveRight();
		auto mat = Daisy->getSystem<Systems::Content>()->getPhysicsMaterial(ColliderRef->getPhysicsMaterial());
		mat->setFriction(GroundFriction);
        if (Grounded)
        {
          SpriteComponent->SpriteSource = RunAnimation;
		  SpriteComponent->AnimationActive = true;
        }
      }
      else
      {
		auto mat = Daisy->getSystem<Systems::Content>()->getPhysicsMaterial(ColliderRef->getPhysicsMaterial());
		mat->setFriction(1.3f);
        if (Grounded)
        {
          SpriteComponent->SpriteSource = StandAnimation;
		  //SpriteComponent->AnimationActive = false;
        }
      }
    }

    //void PlayerController::OnDamageEvent(Events::DamageEvent * event)
    //{
    //}

    void PlayerController::Jump()
    {
      ++JumpFramesApplied;
      RigidBodyRef->setVelocity(RigidBodyRef->getVelocity() + Vec3(0, JumpPower, 0));
      if (JumpFramesApplied >= JumpFrames)
      {
        Jumping = false;
        JumpFramesApplied = 0;
      }
	  if (PlayerControllerTraceOn)
	  {
		  DCTrace << "PlayerController::Jump";
	  }
      //this->SpaceRef->getComponent<Components::SoundSpace>()->PlayCue("FootConcreteBootRun2");
    }

    void PlayerController::TakeDamage(int damage)
    {
      if (Invincible)
      {
        return;
      }
      SpriteComponent->Color = Vec4(1, 0, 0, 1);
      Health -= damage;
      SpaceRef->getComponent<Components::SoundSpace>()->PlayCue("Hit");
      if (PlayerControllerTraceOn)
      {
        DCTrace << "PlayerController::TakeDamage:: Health = " << Health << ".\n";
      }
      //dispatch damage taken event
      if (Health <= 0)
      {
        Die();
      }

    }

    void PlayerController::Die()
    {
      DCTrace << "PlayerController::Die - Reloading level \n";
      SpriteComponent->Color = Vec4(0, 0, 0, 1);
      Dead = true;
      auto cameraRef = SpaceRef->FindObjectByName("Camera");
      SpaceRef->getComponent<Components::SoundSpace>()->PlayCue("Death");
      if (cameraRef)
      {
        cameraRef->getComponent<Components::CameraController>()->DoScreenShake = true;
      }
    }



    void PlayerController::PrintTranslation()
    {
      //DCTrace << Owner()->Name() << "::Transform.Translation(" << TransformRef->Translation.x << ", " << TransformRef->Translation.y << ", " << TransformRef->Translation.z << ")\n";
    }

    void PlayerController::PrintVelocity()
    {
      Vec3 vel = RigidBodyRef->getVelocity();
      //DCTrace << Owner()->Name() << "::RigidBody.Velocity(" << vel.x << ", " << vel.y<< ", " << vel.z << ")\n";
    }

    void PlayerController::AutoPlay(Events::LogicUpdate * event)
    {
      //autoplay is a super hacky copy of enemycontroller's random enemy - fix this later
      if (AutoPlayTimer > 2)
      {
        AutoPlayTimer = 0;
        auto direction = rand() % 11 - 5; //-5 to 5
        RigidBodyRef->setVelocity(RigidBodyRef->getVelocity() + Vec3(direction * 10, 200, 0));
      }

      switch (rand() % 3)
      {

      case 0:
        break;
      case 1:
        break;
      case 2:
        break;
      }
    }
    void PlayerController::MoveLeft()
    {

		//////
      float scalar = 0.0f;

      if (RigidBodyRef->getVelocity().x > 0)
      {
        scalar = TurnSpeedScalar;
      }
      else
      {
        scalar = 1;
      }

      if (RigidBodyRef->getVelocity().length() < 7.0f)
      {
        RigidBodyRef->ApplyLinearVelocity(Vec3(-MoveSpeed * scalar * 2.0f, 0, 0));
      }
      else
      {
        RigidBodyRef->ApplyLinearVelocity(Vec3(-MoveSpeed * scalar, 0, 0));
      }

      if (musicplay)
      {
        //auto objref = this->SpaceRef->FindObjectByName("MusicManager");
       // auto event = new Events::PlayMusic;
       // objref->Dispatch<Events::PlayMusic>(event);
       // delete event;
       // musicplay = false;
      }

      //PrintTranslation();
    }
    void PlayerController::MoveRight()
    {
      float scalar = 0.0f;

      if (RigidBodyRef->getVelocity().x < 0)
      {
        scalar = TurnSpeedScalar;
      }
      else
      {
        scalar = 1;
      }
      //PrintTranslation();
      if (RigidBodyRef->getVelocity().length() < 7.0f)
      {
        RigidBodyRef->ApplyLinearVelocity(Vec3(MoveSpeed * scalar * 2.0f, 0, 0));
      }
      else
      {
        RigidBodyRef->ApplyLinearVelocity(Vec3(MoveSpeed * scalar, 0, 0));
      }

      if (musicplay)
      {
        //auto objref = this->SpaceRef->FindObjectByName("MusicManager");
        //auto event = new Events::PlayMusic;
        //objref->Dispatch<Events::PlayMusic>(event);
        //delete event;
        //musicplay = false;
      }

    }
	Boolean PlayerController::CheckForGround()
	{

		DCEngine::CastFilter filter;
		filter.CollisionGroups.push_back(CollisionGroup("Terrain"));
		filter.Include = true;
		auto physicsSpace = this->SpaceRef->getComponent<Components::PhysicsSpace>();
		DCEngine::Ray ray;
		ray.Direction = Vec3(0, -1, 0);
		ray.Origin = Vec3(TransformRef->Translation) + Vec3(TransformRef->Scale.x / 2.01, -TransformRef->Scale.y / 2, 0);
		auto result = physicsSpace->CastRay(ray);
		if (result.Distance < 0.05)
		{
			return true;
		}
		ray.Origin = Vec3(TransformRef->Translation) + Vec3(0, -TransformRef->Scale.y / 2, 0);
		result = physicsSpace->CastRay(ray, filter);
		if (result.Distance < 0.05)
		{
			return true;
		}
		ray.Origin = Vec3(TransformRef->Translation) + Vec3(-TransformRef->Scale.x / 2.01, -TransformRef->Scale.y / 2, 0);
		result = physicsSpace->CastRay(ray, filter);
		if (result.Distance < 0.05)
		{
			return true;
		}
		return false;
	}
  }



}
