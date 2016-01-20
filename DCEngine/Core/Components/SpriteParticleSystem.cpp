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
			mColorAnimator = Owner()->getComponent<Components::ParticleColorAnimator>();
			mLinearAnimator = Owner()->getComponent<Components::LinearParticleAnimator>();
    }

		SpriteParticleSystem::Particle::Particle(double lifetime,
			const Vec2& position, const Vec2& velocity, const Vec2& acceleration, float scale, float spin, const Vec4& tint,
			ParticleColorAnimator* colorAnimator, LinearParticleAnimator* linearAnimator)
			: mLifetime(lifetime), mLifeleft(lifetime), mPosition(position), mVelocity(velocity), mAcceleration(acceleration),
			mScale(scale),mRotation(0), mRotationRate(spin), mTint(tint),
			mColorAnimator(colorAnimator), mLinearAnimator(linearAnimator)
		{
		}

		//SpriteParticleSystem::Particle& SpriteParticleSystem::Particle::operator=(const Particle& rhs)
		//{
		//	mLifeleft = rhs.mLifetime;
		//	mPosition = rhs.mPosition;
		//	mScale = rhs.mScale;
		//	mRotation = rhs.mRotation;
		//	mColor = rhs.mColor;
		//	return *this;
		//}
		void SpriteParticleSystem::Particle::Update(double dt)
		{
			mLifeleft -= dt;
			if (mLifeleft < 0)
				mLifeleft = 0;
			
			mVelocity.x += mAcceleration.x * dt;
			mVelocity.y += mAcceleration.y * dt;

			mVelocity.x *= (100 - mLinearAnimator->Dampening) / 100;
			mVelocity.y *= (100 - mLinearAnimator->Dampening) / 100;

			mPosition.x += mVelocity.x * dt - (mAcceleration.x * dt * dt) / 2;
			mPosition.y += mVelocity.y * dt - (mAcceleration.y * dt * dt) / 2;

			mScale += mLinearAnimator->Growth * dt;
			mRotation += mRotationRate * dt + (mLinearAnimator->Torque * dt * dt) / 2;
			mRotationRate += mLinearAnimator->Torque * dt;

			if (mColorAnimator)
			{
				double percentLifeLeft = (mLifeleft / mLifetime) * 100;
				if (percentLifeLeft <= 100 && percentLifeLeft >= 75)
				{
					double t = (percentLifeLeft - 75) / 25;
					mColor.r = mTint.r * (mColorAnimator->Color0.r * t + mColorAnimator->Color1.r * (1 - t));
					mColor.g = mTint.g * (mColorAnimator->Color0.g * t + mColorAnimator->Color1.g * (1 - t));
					mColor.b = mTint.b * (mColorAnimator->Color0.b * t + mColorAnimator->Color1.b * (1 - t));
					mColor.a = mTint.a * (mColorAnimator->Color0.a * t + mColorAnimator->Color1.a * (1 - t));
				}
				else if (percentLifeLeft <= 75 && percentLifeLeft >= 50)
				{
					double t = (percentLifeLeft - 50) / 25;
					mColor.r = mTint.r * mColorAnimator->Color1.r * t + mColorAnimator->Color2.r * (1 - t);
					mColor.g = mTint.g * mColorAnimator->Color1.g * t + mColorAnimator->Color2.g * (1 - t);
					mColor.b = mTint.b * mColorAnimator->Color1.b * t + mColorAnimator->Color2.b * (1 - t);
					mColor.a = mTint.a * mColorAnimator->Color1.a * t + mColorAnimator->Color2.a * (1 - t);
				}
				else if (percentLifeLeft <= 50 && percentLifeLeft >= 25)
				{
					double t = (percentLifeLeft - 25) / 25;
					mColor.r = mTint.r * mColorAnimator->Color2.r * t + mColorAnimator->Color3.r * (1 - t);
					mColor.g = mTint.g * mColorAnimator->Color2.g * t + mColorAnimator->Color3.g * (1 - t);
					mColor.b = mTint.b * mColorAnimator->Color2.b * t + mColorAnimator->Color3.b * (1 - t);
					mColor.a = mTint.a * mColorAnimator->Color2.a * t + mColorAnimator->Color3.a * (1 - t);
				}
				else if (percentLifeLeft <= 25 && percentLifeLeft >= 0)
				{
					double t = percentLifeLeft / 25;
					mColor.r = mTint.r * mColorAnimator->Color3.r * t + mColorAnimator->Color4.r * (1 - t);
					mColor.g = mTint.g * mColorAnimator->Color3.g * t + mColorAnimator->Color4.g * (1 - t);
					mColor.b = mTint.b * mColorAnimator->Color3.b * t + mColorAnimator->Color4.b * (1 - t);
					mColor.a = mTint.a * mColorAnimator->Color3.a * t + mColorAnimator->Color4.a * (1 - t);
				}
			}
			else
			{
				mColor = mTint;
			}
		}
		double SpriteParticleSystem::Particle::GetLifetime(void) const
		{
			return mLifetime;
		}
		double SpriteParticleSystem::Particle::GetLifeleft(void) const
		{
			return mLifeleft;
		}
		Vec2 SpriteParticleSystem::Particle::GetPosition(void) const
		{
			return mPosition;
		}
		float SpriteParticleSystem::Particle::GetScale(void) const
		{
			return mScale;
		}
		float SpriteParticleSystem::Particle::GetRotation(void) const
		{
			return mRotation;
		}
		Vec4 SpriteParticleSystem::Particle::GetColor(void) const
		{
			return mColor;
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
				[](const Particle& p) { return p.GetLifeleft() <= 0; }), mParticleList.end());
			std::sort(mParticleList.begin(), mParticleList.end(),
				[](const Particle& p1, const Particle& p2) { return p1.GetLifeleft() > p2.GetLifeleft(); });
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
					Vec2(mLinearAnimator->Force.x + mLinearAnimator->RandomForce.x * (rand() % 100 - 50) / 50,
						mLinearAnimator->Force.y + mLinearAnimator->RandomForce.y * (rand() % 100 - 50) / 50),
					mParticleEmitter->Size + mParticleEmitter->SizeVariance * (rand() % 100 - 50) / 100,
					mParticleEmitter->Spin + mParticleEmitter->SpinVariance * (rand() % 100 - 50) / 100,
					Tint, mColorAnimator, mLinearAnimator));
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
		std::vector<Vec4> SpriteParticleSystem::GetColorData(void)
		{
			std::vector<Vec4> color;
			for (auto&& particle : mParticleList)
			{
				color.push_back(particle.GetColor());
			}
			return color;
		}
		unsigned SpriteParticleSystem::GetParticleCount(void)
		{
			return mParticleList.size();
		}
  }
}