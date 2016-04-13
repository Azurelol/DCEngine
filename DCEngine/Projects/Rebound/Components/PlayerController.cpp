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
			DCE_BINDING_DEFINE_PROPERTY(PlayerController, VelocityXCap);
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
				DeathTimer += event->Dt;
				if (DeathTimer > TimeToDie)
				{
					Dead = false;
					this->SpaceRef->ReloadLevel();
					// Play teleport in sound.
					SpaceRef->getComponent<Components::SoundSpace>()->PlayCue(TeleportArriveSound);
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

		void PlayerController::TakeDamage(int damage)
		{
			if (Invincible)
			{
				return;
			}
			SpriteComponent->Color = Vec4(1, 0, 0, 1);
			Health -= damage;

			// Play hurt sound.
			SpaceRef->getComponent<Components::SoundSpace>()->PlayCue(CollideSound);

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
				//cameraRef->getComponent<Components::CameraControllerZilch>()->DoScreenShake = true;
				//dispatch event
			}
			// play teleport start.
			SpaceRef->getComponent<Components::SoundSpace>()->PlayCue(TeleportStartSound);
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
