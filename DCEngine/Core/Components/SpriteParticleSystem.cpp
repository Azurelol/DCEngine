/******************************************************************************/
/*!
@file   SpriteParticleSystem.cpp
@author Chen Shu, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/6/2015
@brief  The SpriteParticleSystem component...
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "SpriteParticleSystem.h"
#include "EngineReference.h"

namespace DCEngine {

  namespace Components
  {
    /**************************************************************************/
    /*!
    @brief Provides the definition of this class to Zilch.
    @note This can only go in the translational unit (.cpp)
    */
    /**************************************************************************/
    #if(DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDefineType(SpriteParticleSystem, "SpriteParticleSystem", DCEngineCore, builder, type) {      
      // Constructor / Destructor
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(SpriteParticleSystem);
      // Properties
      DCE_BINDING_DEFINE_PROPERTY(SpriteParticleSystem, Visible);
      DCE_BINDING_DEFINE_PROPERTY(SpriteParticleSystem, Tint);

      DCE_BINDING_DEFINE_RESOURCE_ATTRIBUTE(SpriteSource);
      DCE_BINDING_DEFINE_PROPERTY(SpriteParticleSystem, Texture);
      DCE_BINDING_PROPERTY_SET_ATTRIBUTE(propertyTexture, attributeSpriteSource);

      DCE_BINDING_DEFINE_PROPERTY(SpriteParticleSystem, VelocityScale);
      DCE_BINDING_DEFINE_PROPERTY(SpriteParticleSystem, LengthScale);
      DCE_BINDING_DEFINE_PROPERTY(SpriteParticleSystem, SystemSize);
    }
    #endif

    /*!************************************************************************\
    @brief  Constructor for the SpriteParticleSystem
    \**************************************************************************/
    SpriteParticleSystem::SpriteParticleSystem(Entity & owner)
			: Component("SpriteParticleSystem", owner), mParticleEmissionTimer(0)
    {
			TransformComponent = dynamic_cast<GameObject*>(Owner())->getComponent<Components::Transform>();
			// Register
			SpaceRef->getComponent<Components::GraphicsSpace>()->AddParticleSystem(*this);
			srand(time(NULL));
    }

    /*!************************************************************************\
    @brief  Initializes the SpriteParticleSystem.
    \**************************************************************************/
    void SpriteParticleSystem::Initialize()
    {
      // Grab a reference to the space's default camera
      CameraComponent = SpaceRef->getComponent<CameraViewport>()->getCamera();

      // Access to owner is automatically given to components through Owner()! Just need
      // to cast them into the type of entity the owner is.
      auto gameObjOwner = dynamic_cast<GameObject*>(Owner());

      // Access to the graphics system is already given
			mParticleEmitter = Owner()->getComponent<Components::ParticleEmitter>();
			
    }

		SpriteParticleSystem::Particle::Particle(double lifetime,
			const Vec2& position, const Vec2& velocity, float scale, float spin)
			: mLifetime(lifetime), mPosition(position), mVelocity(velocity), mScale(scale),
			mRotation(0), mRotationRate(spin)
		{
		}
		void SpriteParticleSystem::Particle::Update(double dt)
		{
			mLifetime -= dt;
			mPosition.x += mVelocity.x * dt;
			mPosition.y += mVelocity.y * dt;
			mRotation += mRotationRate * dt;
		}
		double SpriteParticleSystem::Particle::GetLifetime(void)
		{
			return mLifetime;
		}
		Vec2 SpriteParticleSystem::Particle::GetPosition(void)
		{
			return mPosition;
		}
		float SpriteParticleSystem::Particle::GetScale(void)
		{
			return mScale;
		}
		float SpriteParticleSystem::Particle::GetRotation(void)
		{
			return mRotation;
		}

		void SpriteParticleSystem::UpdateParticles(double dt)
		{
			if (mActiveFlag)
			{
				if (mParticleEmitter->Active == false)
				{
					mEmitCounter = 0;
					mParticleEmissionTimer == 0;
					mActiveFlag = false;
				}
				mParticleEmissionTimer -= dt;

				if (mParticleEmitter->EmitRate > 0)
				{
					if (mParticleEmissionTimer <= 0)
					{
						mParticleEmissionTimer += 1.f / float(mParticleEmitter->EmitRate);
						if (mParticleEmitter->EmitCount > 0)
						{
							if (mParticleEmitter->EmitCount > mEmitCounter)
							{
								AddParticle();
								++mEmitCounter;
							}
						}
						else
							AddParticle();
					}
				}
			}
			else
			{
				if (mParticleEmitter->Active)
				{
					mActiveFlag = true;
				}
			}
			for (auto&& particle : mParticleList)
			{
				particle.Update(dt);
			}
			mParticleList.erase(std::remove_if(mParticleList.begin(), mParticleList.end(),
				[](Particle& p) { return p.GetLifetime() <= 0; }), mParticleList.end());
		}
		void SpriteParticleSystem::AddParticle(void)
		{
			unsigned emitCount = 1 + rand() % (mParticleEmitter->EmitVariance + 1);
			for (unsigned i = 0; i < emitCount; ++i)
			{
				mParticleList.push_back(Particle(
					mParticleEmitter->Lifetime + mParticleEmitter->LifetimeVariance * (rand() % 100 - 50) / 100, Vec2(0, 0), Vec2(
						mParticleEmitter->StartVelocity.x + mParticleEmitter->RandomVelocity.x * (rand() % 100 - 50) / 50,
						mParticleEmitter->StartVelocity.y + mParticleEmitter->RandomVelocity.y * (rand() % 100 - 50) / 50),
					mParticleEmitter->Size + mParticleEmitter->SizeVariance * (rand() % 100 - 50) / 100,
					mParticleEmitter->Spin + mParticleEmitter->SpinVariance * (rand() % 100 - 50) / 100));
			}
		}
		std::vector<Vec2> SpriteParticleSystem::GetPositionData(void)
		{
			std::vector<Vec2> positions;
			for (auto&& particle : mParticleList)
			{
				positions.push_back(particle.GetPosition());
			}
			return positions;
		}
		std::vector<float> SpriteParticleSystem::GetScaleData(void)
		{
			std::vector<float> scale;
			for (auto&& particle : mParticleList)
			{
				scale.push_back(particle.GetScale());
			}
			return scale;
		}
		std::vector<float> SpriteParticleSystem::GetRotationData(void)
		{
			std::vector<float> rotation;
			for (auto&& particle : mParticleList)
			{
				rotation.push_back(particle.GetRotation());
			}
			return rotation;
		}
		unsigned SpriteParticleSystem::GetParticleCount(void)
		{
			return mParticleList.size();
		}
  }
}