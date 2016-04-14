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

static bool Dead = false;

namespace DCEngine {

	namespace Components
	{
#if(DCE_USE_ZILCH_INTERNAL_BINDING)
		ZilchDefineType(PlayerController, "PlayerController", Rebound, builder, type) {
			DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(PlayerController);
			DCE_BINDING_DEFINE_PROPERTY(PlayerController, MoveSpeed);
			DCE_BINDING_DEFINE_PROPERTY(PlayerController, VelocityXCap);
			DCE_BINDING_DEFINE_PROPERTY(PlayerController, InitialLocation);
			DCE_BINDING_DEFINE_PROPERTY(PlayerController, JumpPower);
			DCE_BINDING_DEFINE_PROPERTY(PlayerController, JumpFrames);
			DCE_BINDING_DEFINE_PROPERTY(PlayerController, Health);
			DCE_BINDING_DEFINE_PROPERTY(PlayerController, AirBrakeScalar);
			DCE_BINDING_DEFINE_PROPERTY(PlayerController, TurnSpeedScalar);
			DCE_BINDING_DEFINE_PROPERTY(PlayerController, DoAutoPlay);
			DCE_BINDING_DEFINE_PROPERTY(PlayerController, StandAnimation);
			DCE_BINDING_DEFINE_PROPERTY(PlayerController, AmountOfJumpPowerAddedToHorizontalJump);
			DCE_BINDING_DEFINE_PROPERTY(PlayerController, AmountOfMaxSpeedRequiredForHorizontalJump);
			DCE_BINDING_DEFINE_PROPERTY(PlayerController, JumpAnimation);
			DCE_BINDING_DEFINE_PROPERTY(PlayerController, RunAnimation);
			DCE_BINDING_DEFINE_PROPERTY(PlayerController, FallAnimation);
			DCE_BINDING_DEFINE_PROPERTY(PlayerController, ThrowAnimation);
			DCE_BINDING_DEFINE_PROPERTY(PlayerController, AutoPlayTimer);
			DCE_BINDING_DEFINE_PROPERTY(PlayerController, LandingParticle);
			DCE_BINDING_DEFINE_PROPERTY(PlayerController, TeleportStartSound);
			DCE_BINDING_DEFINE_PROPERTY(PlayerController, TeleportArriveSound);
			DCE_BINDING_DEFINE_PROPERTY(PlayerController, JumpSound);
			DCE_BINDING_DEFINE_PROPERTY(PlayerController, LandSound);
			DCE_BINDING_DEFINE_PROPERTY(PlayerController, FootstepSound);
			DCE_BINDING_DEFINE_PROPERTY(PlayerController, CollideSound);
      DCE_BINDING_DEFINE_PROPERTY(PlayerController, DamageFromScrapper);
      DCE_BINDING_DEFINE_PROPERTY(PlayerController, DamageFromSentinel);
      DCE_BINDING_DEFINE_PROPERTY(PlayerController, DamageFromLancer);
      DCE_BINDING_DEFINE_PROPERTY(PlayerController, ColorOnDamage);
      DCE_BINDING_DEFINE_PROPERTY(PlayerController, ColorOnDamageFlashDuration);
      DCE_BINDING_DEFINE_PROPERTY(PlayerController, KnockBackForceOnDamageFromScrapperX);
      DCE_BINDING_DEFINE_PROPERTY(PlayerController, KnockBackForceOnDamageFromScrapperY);
      DCE_BINDING_DEFINE_PROPERTY(PlayerController, KnockBackForceOnDamageFromSentinelX);
      DCE_BINDING_DEFINE_PROPERTY(PlayerController, KnockBackForceOnDamageFromSentinelY);
      DCE_BINDING_DEFINE_PROPERTY(PlayerController, KnockBackForceOnDamageFromLancerX);
      DCE_BINDING_DEFINE_PROPERTY(PlayerController, KnockBackForceOnDamageFromLancerY);
      DCE_BINDING_DEFINE_PROPERTY(PlayerController, DamageCooldown);
      DCE_BINDING_DEFINE_RESOURCE_ATTRIBUTE(Archetype);
      DCE_BINDING_DEFINE_PROPERTY(PlayerController, ShieldArchetype);
      DCE_BINDING_PROPERTY_SET_RESOURCE_ATTRIBUTE(propertyShieldArchetype, attributeArchetype);
      DCE_BINDING_DEFINE_PROPERTY(PlayerController, RedHazeArchetype);
      DCE_BINDING_PROPERTY_SET_RESOURCE_ATTRIBUTE(propertyRedHazeArchetype, attributeArchetype);
		}
#endif

		void PlayerController::Initialize()
		{
			gameObj = dynamic_cast<GameObject*>(Owner());
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
			BallRef = SpaceRef->FindObjectByName("Ball");
			SpriteComponent = dynamic_cast<GameObject*>(Owner())->getComponent<Components::Sprite>();
			InitialVelocityXCap = VelocityXCap;
			// ColliderRef->	 
			auto CollisionTableRef = Daisy->getSystem<Systems::Content>()->getCollisionTable(std::string(this->SpaceRef->getComponent<Components::PhysicsSpace>()->getCollisionTable()));
			//CollisionTableRef->AddGroup("Player");
			auto ColliderRef = dynamic_cast<GameObject*>(Owner())->getComponent<Components::BoxCollider>();
			//ColliderRef->setCollisionGroup("Player");
			//RigidBodyRef->setGravity(false);

			//SpaceRef->getComponent<Components::SoundSpace>()->PlayCue("Dogma");
			InitialLocation = TransformRef->getTranslation();
			InitialHealth = Health;
			maxHealth = Health;
			redHazeAlphaValue = 0;
			CreateShield();

			if (Dead)
			{
			TeleportIn();
			}

			Dead = false;
		}

    void PlayerController::FlashColor(Vec4 color, float duration)
    {
      if (Dead == true)
        return;

      shield->getComponent<Sprite>()->Color = shieldColor;
      ActionSetPtr seq = Actions::Sequence(Owner()->Actions);
      Actions::Property(seq, shield->getComponent<Sprite>()->Color.w, 0, duration, Ease::Linear);
      
      float width = Daisy->getSystem<Systems::Graphics>()->GetScreenWidth();
      float height = Daisy->getSystem<Systems::Graphics>()->GetScreenHeight();
     // redHaze->getComponent<Transform>()->Scale = Vec3(width / 38, height / 38, 0);
      redHaze->getComponent<Sprite>()->Color = redHazeColor;
      
      
      
      if (Health <= maxHealth / 2)
      {
        redHazeAlphaValue =  1 - (Health / maxHealth);
      }
      else
      {
        redHazeAlphaValue = 0.0f;
      }
      
      ActionSetPtr seq2 = Actions::Sequence(Owner()->Actions);
      Actions::Property(seq2, redHaze->getComponent<Sprite>()->Color.w, redHazeAlphaValue, duration, Ease::Linear);

			
    }

    void PlayerController::CreateShield()
    {
      shield = SpaceRef->CreateObject(ShieldArchetype);
      shield->AttachTo(gameObj);
      shield->getComponent<Transform>()->SetLocalTranslation(Vec3(0, 0, 0));
      shieldColor = shield->getComponent<Sprite>()->Color;
      shield->getComponent<Sprite>()->Color.w = 0;

      camera = SpaceRef->FindObjectByName("Camera");

      redHaze = SpaceRef->CreateObject(RedHazeArchetype);
      redHaze->AttachTo(camera);
      redHaze->getComponent<Transform>()->setLocalTranslation(Vec3(0, 0, 1));
      float width = Daisy->getSystem<Systems::Graphics>()->GetScreenWidth();
      float height = Daisy->getSystem<Systems::Graphics>()->GetScreenHeight();
      redHaze->getComponent<Transform>()->Scale = Vec3(width / 38, height / 38, 0);
      redHazeColor = redHaze->getComponent<Sprite>()->Color;
      redHaze->getComponent<Sprite>()->Color.w = 0;

      //DCTrace << "width: " << width << " height: " << height << "\n";
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
			case Keys::L:
				LevelCheatLoaded = false;
				break;
			case Keys::P:
				LevelCheatLoaded = true;
				level = "Victory";
				SpaceRef->LoadLevel(level);
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
			case Keys::Num4:
				LevelCheatLoaded = true;
				level = "Level4";
				SpaceRef->LoadLevel(level);
				break;
			case Keys::Num5:
				LevelCheatLoaded = true;
				level = "Level5";
				SpaceRef->LoadLevel(level);
				break;
			case Keys::Num6:
				LevelCheatLoaded = true;
				level = "Level6";
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
				//Grounded = true;
				// play landing sound.
				SpaceRef->getComponent<Components::SoundSpace>()->PlayCue(LandSound);
				if (event->OtherObject->getComponent<Components::BoxCollider>() && event->OtherObject->getComponent<Components::BoxCollider>()->getCollisionGroup() == "Terrain")
				{
					auto particle = SpaceRef->CreateObject(LandingParticle);
					if (particle)
					{
						particle->getComponent<Components::Transform>()->setTranslation(TransformRef->Translation - Vec3(0, TransformRef->getScale().y / 2, 0));
					}
				}
				if (event->OtherObject->getComponent<Components::HazardArea>())
				{
					auto particle = SpaceRef->CreateObject("AcidSplashParticle");
					if (particle)
					{
						particle->getComponent<Components::Transform>()->setTranslation(TransformRef->Translation - Vec3(0, TransformRef->getScale().y / 2, 0));
					}
				}
			}
			if (event->OtherObject->getComponent<Components::LevelManager>())
			{
				if (event->OtherObject->getComponent<Components::Fade>())
				{
					event->OtherObject->getComponent<Components::Fade>()->setFading(true);
				}
			}

      if (event->OtherObject->getComponent<Grunt>() != nullptr)
      {
        if (TakeDamage(DamageFromScrapper))
        {
          FlashColor(ColorOnDamage, ColorOnDamageFlashDuration);
          RigidBodyRef->ApplyForce(Vec3(event->Normal.x * KnockBackForceOnDamageFromScrapperX, KnockBackForceOnDamageFromScrapperY, 0));
        }
      }
      if (event->OtherObject->getComponent<BoxCollider>()->getCollisionGroup() == "SentinelShield")
      {
        if (TakeDamage(DamageFromSentinel))
        {
          FlashColor(ColorOnDamage, ColorOnDamageFlashDuration);
          RigidBodyRef->ApplyForce(Vec3(event->Normal.x * KnockBackForceOnDamageFromSentinelX, KnockBackForceOnDamageFromSentinelY, 0));
        }
      }
      if (event->OtherObject->getComponent<Lancer>() != nullptr)
      {
        if (TakeDamage(DamageFromLancer))
        {
          FlashColor(ColorOnDamage, ColorOnDamageFlashDuration);
          RigidBodyRef->ApplyForce(Vec3(event->Normal.x * KnockBackForceOnDamageFromLancerX, KnockBackForceOnDamageFromLancerY, 0));
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
		  float width = Daisy->getSystem<Systems::Graphics>()->GetScreenWidth();
		  float height = Daisy->getSystem<Systems::Graphics>()->GetScreenHeight();
		  //DCTrace << "Player velocity: " << RigidBodyRef->getVelocity().x << ", " << RigidBodyRef->getVelocity().y << "\n";
		  Daisy->getSystem<Systems::Graphics>()->ScreenSpaceRectangle(redHaze->getComponent<Sprite>()->Color,
			Daisy->getSystem<Systems::Content>()->getSpriteSrc(redHaze->getComponent<Sprite>()->SpriteSource));
		 // DCTrace << "RedHazeAlpha " << redHaze->getComponent<Sprite>()->Color.w << "\n";
		  if (TransformRef->getTranslation().y < -100)
		  {
			  Die();
		  }

			bool animationChanged = false;
			if (PlayerControllerTraceOn)
			{
				PrintTranslation();
			}
			Grounded = CheckForGround();
			if (glm::abs(RigidBodyRef->getVelocity().x) > VelocityXCap)
			{
				Vec3 currentVel = RigidBodyRef->getVelocity();
				currentVel.x = VelocityXCap * glm::sign(currentVel.x);
				RigidBodyRef->setVelocity(currentVel);
			}

			//hacking in logic for color changing, use fade later
			if (Dead)
			{
				//DeathTimer += event->Dt;
				//if (DeathTimer > TimeToDie)
				//{
				//	Dead = false;
				//	this->SpaceRef->ReloadLevel();
				//	// Play teleport in sound.
				//	SpaceRef->getComponent<Components::SoundSpace>()->PlayCue(TeleportArriveSound);
				//}
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
					// Actions::Delay(seq, 0.1f);
					Actions::Call(seq, &PlayerController::Jump, this);
					Jumping = true;
					Grounded = false;
					SpriteComponent->SpriteSource = JumpAnimation;
					SpriteComponent->AnimationActive = true;
					SpriteComponent->ResetAnimationIndex();
				}
			}
			else
			{
				//SpriteComponent->HaveAnimation = true;
				//SpriteComponent->AnimationActive = true;
				Jumping = false;
				if (RigidBodyRef->getVelocity().y > 0 && BallRef && BallRef->getComponent<Components::BallController>()->Locked == false)
				{
					RigidBodyRef->setVelocity(RigidBodyRef->getVelocity() * Vec3(1, AirBrakeScalar, 1));
				}
			}
			if (RigidBodyRef->getVelocity().y < 0 && SpriteComponent->SpriteSource == JumpAnimation)
			{
				SpriteComponent->SpriteSource = FallAnimation;
				SpriteComponent->AnimationActive = false;
				SpriteComponent->ResetAnimationIndex();
			}
			if (Daisy->getKeyboard()->KeyIsDown(Keys::A))
			{
				SpriteComponent->FlipX = true;
				MoveLeft();
				auto mat = Daisy->getSystem<Systems::Content>()->getPhysicsMaterial(ColliderRef->getPhysicsMaterial());
				mat->setFriction(GroundFriction);
				if (Grounded)
				{
					if (SpriteComponent->SpriteSource != RunAnimation)
						SpriteComponent->ResetAnimationIndex();
					SpriteComponent->SpriteSource = RunAnimation;
					SpriteComponent->AnimationActive = true;
					SoundFootstep();
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
					if(SpriteComponent->SpriteSource != RunAnimation)
						SpriteComponent->ResetAnimationIndex();
					SpriteComponent->SpriteSource = RunAnimation;
					SpriteComponent->AnimationActive = true;
					
					SoundFootstep();
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
			if (FramesOfThrowAnimation > 0 && SpriteComponent->SpriteSource != ThrowAnimation)
			{
				--FramesOfThrowAnimation;
				SpriteComponent->SpriteSource = ThrowAnimation;
			}
		}

		//void PlayerController::OnDamageEvent(Events::DamageEvent * event)
		//{
		//}

		void PlayerController::Jump()
		{
			++JumpFramesApplied;
			if (RigidBodyRef->getVelocity().x > VelocityXCap * AmountOfMaxSpeedRequiredForHorizontalJump)
			{
				VelocityXCap = HorizontalJumpingVelocityXCap;
				RigidBodyRef->setVelocity(RigidBodyRef->getVelocity() + Vec3(AmountOfJumpPowerAddedToHorizontalJump, JumpPower, 0));
			}
			else if (RigidBodyRef->getVelocity().x < -VelocityXCap * AmountOfMaxSpeedRequiredForHorizontalJump)
			{
				VelocityXCap = HorizontalJumpingVelocityXCap;
				RigidBodyRef->setVelocity(RigidBodyRef->getVelocity() + Vec3(-AmountOfJumpPowerAddedToHorizontalJump, JumpPower, 0));
			}
			else
			{
				RigidBodyRef->setVelocity(RigidBodyRef->getVelocity() + Vec3(0, JumpPower, 0));
			}
			
			if (JumpFramesApplied >= JumpFrames)
			{
				Jumping = false;
				JumpFramesApplied = 0;
			}
			if (PlayerControllerTraceOn)
			{
				DCTrace << "PlayerController::Jump";
			}
			// play jump sound
			//SpaceRef->getComponent<Components::SoundSpace>()->PlayCue(JumpSound);
			SpaceRef->getComponent<Components::SoundSpace>()->PlayCue(JumpSound);
			Grounded = false;
		}

		bool PlayerController::TakeDamage(int damage)
		{
      int startingHealth = Health;

			if (Invincible || Health <= 0)
			{
				return false;
			}
			//SpriteComponent->Color = Vec4(1, 0, 0, 1);

		  if (IsDamageable)
		  {


			Health -= damage;

				  // Play hurt sound.
				  SpaceRef->getComponent<Components::SoundSpace>()->PlayCue(CollideSound);

			IsDamageable = false;
			ActionSetPtr seq = Actions::Sequence(Owner()->Actions);
			Actions::Property(seq, IsDamageable, true, DamageCooldown, Ease::Linear);
		  }


			if (PlayerControllerTraceOn)
			{
				DCTrace << "PlayerController::TakeDamage:: Health = " << Health << ".\n";
			}
			//dispatch damage taken event
			if (Health <= 0)
			{
				Die();
			}
			else
			{
				// play take damage sound.
			}

      if (Health == startingHealth)
        return false;
      else
        return true;

		}

    void PlayerController::TeleportIn()
    {
      teleportInParticle = SpaceRef->CreateObject("ReverseTeleportationParticle");
      teleportInParticle->getComponent<Transform>()->Translation = TransformRef->Translation;
      SpaceRef->getComponent<Components::SoundSpace>()->PlayCue(TeleportArriveSound);
      ActionSetPtr seq = Actions::Sequence(Owner()->Actions);
      Actions::Delay(seq, 2);
      Actions::Call(seq, &PlayerController::DestroyTeleportParticle, this);
    }

    void PlayerController::DestroyTeleportParticle()
    {
		if (teleportOutParticle != nullptr)
		{
			teleportInParticle->Destroy();
		}
    }

	void PlayerController::WarpToCheckpoint()
	{
		TransformRef->Translation = InitialLocation;
	}

	void PlayerController::RestoreHealth()
	{
		Health = InitialHealth;
		redHazeAlphaValue = 0;
		ActionSetPtr seq2 = Actions::Sequence(Owner()->Actions);
		Actions::Property(seq2, redHaze->getComponent<Sprite>()->Color.w, redHazeAlphaValue, 1, Ease::Linear);
	}

	void PlayerController::DestroyTeleportOutParticle()
	{
		if (teleportOutParticle != nullptr)
		{
			teleportOutParticle->Destroy();
		}
	}

		void PlayerController::Die()
		{
			DCTrace << "PlayerController::Die - Reloading level \n";



			SpaceRef->getComponent<Components::SoundSpace>()->PlayCue("Death");

			// play teleport start.

		  //Dead = true;
		  teleportOutParticle = SpaceRef->CreateObject("TeleportationParticle");
		  teleportOutParticle->AttachTo(gameObj);
		  teleportOutParticle->getComponent<Transform>()->setLocalTranslation(Vec3(0, 0, 0));
				SpaceRef->getComponent<Components::SoundSpace>()->PlayCue(TeleportStartSound);
		  ActionSetPtr seq = Actions::Sequence(Owner()->Actions);
		  Actions::Delay(seq, 1);
		  Actions::Call(seq, &PlayerController::DestroyTeleportOutParticle, this);
		  Actions::Call(seq, &PlayerController::WarpToCheckpoint, this);
		  Actions::Call(seq, &PlayerController::RestoreHealth, this);
		}

    void PlayerController::PlayTeleportArriveSound()
    {
      SpaceRef->getComponent<Components::SoundSpace>()->PlayCue(TeleportArriveSound);
    }

    void PlayerController::ReloadLevel()
    {
      SpaceRef->ReloadLevel();
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
			if (glm::abs(RigidBodyRef->getVelocity().y) > 4)
			{
				return false;
			}
			DCEngine::CastFilter filter;
			filter.CollisionGroups.push_back(CollisionGroup::Find("Terrain"));
			filter.CollisionGroups.push_back(CollisionGroup::Find("Ball"));
			filter.Include = true;
			auto physicsSpace = this->SpaceRef->getComponent<Components::PhysicsSpace>();
			DCEngine::Ray ray;
			ray.Direction = Vec3(0, -1, 0);
			ray.Origin = TransformRef->Translation + Vec3(TransformRef->Scale.x * ColliderRef->getSize().x / 2.1, -TransformRef->Scale.y / 2.01, 0);
			auto result = physicsSpace->CastRay(ray, filter);
			//DCTrace << "raydist1 = " << result.Distance << "\n";
			auto graphicsSpace = this->SpaceRef->getComponent<Components::GraphicsSpace>();
			graphicsSpace->DrawLineSegment(ray.Origin, ray.Origin + Vec3(0, -1, 0), Vec4(1, 0, 0, 1));
			if (result.Distance < 0.07)
			{
				VelocityXCap = InitialVelocityXCap;
				return true;
			}
			ray.Origin = TransformRef->Translation + Vec3(0, -TransformRef->Scale.y / 2.01, 0);
			result = physicsSpace->CastRay(ray, filter);
			//DCTrace << "raydist2 = " << result.Distance << "\n";
			graphicsSpace->DrawLineSegment(ray.Origin, ray.Origin + Vec3(0, -1, 0), Vec4(1, 0, 0, 1));
			if (result.Distance < 0.07)
			{
				VelocityXCap = InitialVelocityXCap;
				return true;
			}
			ray.Origin = TransformRef->Translation + Vec3(-TransformRef->Scale.x * ColliderRef->getSize().x / 2.1, -TransformRef->Scale.y / 2.01, 0);
			result = physicsSpace->CastRay(ray, filter);
			//DCTrace << "raydist3 = " << result.Distance << "\n";
			graphicsSpace->DrawLineSegment(ray.Origin, ray.Origin + Vec3(0, -1, 0), Vec4(1, 0, 0, 1));
			if (result.Distance < 0.07)
			{
				VelocityXCap = InitialVelocityXCap;
				return true;
			}
			return false;
		}

		void PlayerController::SoundFootstep(void)
		{
			//SpaceRef->getComponent<Components::SoundSpace>()->PlayCue(FootstepSound);
		}
	}
}
