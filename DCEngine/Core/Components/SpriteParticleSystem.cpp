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
#include "../Debug/DebugGraphics.h"

namespace DCEngine {

  namespace Components
  {
		GLuint SpriteParticleSystem::mTransformInstanceVBO;
		GLuint SpriteParticleSystem::mVAO;
		GLuint SpriteParticleSystem::mColorInstanceVBO;
		ShaderPtr SpriteParticleSystem::mShader;
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
			: Graphical("SpriteParticleSystem", owner), mParticleEmissionTimer(0), Visible(true)
    {
			TransformComponent = dynamic_cast<GameObject*>(Owner())->getComponent<Components::Transform>();
			// Register
			SpaceRef->getComponent<Components::GraphicsSpace>()->RegisterGraphicsComponent(this);
			srand(time(NULL));
    }

		SpriteParticleSystem::~SpriteParticleSystem(void)
		{
			SpaceRef->getComponent<GraphicsSpace>()->RemoveGraphicsComponent(this);
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


		void SpriteParticleSystem::Draw(Camera& camera)
		{
			if (Debug::CheckOpenGLError())
				DCTrace << "GraphicsGL::DrawSpriteText - Failed to set active texture!\n";
			mShader->Use();
			//mShader->SetMatrix4("projection", camera.GetProjectionMatrix());
			//mShader->SetMatrix4("view", camera.GetViewMatrix());

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			auto transform = TransformComponent;

			glm::mat4 modelMatrix;
			modelMatrix = glm::translate(modelMatrix, glm::vec3(transform->Translation.x,
				transform->Translation.y,
				transform->Translation.z));
			modelMatrix = glm::scale(modelMatrix,
				glm::vec3(transform->Scale.x, transform->Scale.y, 0.0f));

			//.Update(dt);
			std::vector<glm::vec2> offset(GetPositionData());
			std::vector<float> scale(GetScaleData());
			std::vector<float> rotation(GetRotationData());
			std::vector<glm::vec4> color(GetColorData());
			std::vector<glm::mat4> transformData;
			for (unsigned i = 0; i < GetParticleCount(); ++i)
			{
				glm::mat4 modelMatrix;
				modelMatrix = glm::translate(modelMatrix, glm::vec3(
					transform->Translation.x + offset[i].x, transform->Translation.y + offset[i].y,
					transform->Translation.z));
				modelMatrix = glm::rotate(modelMatrix, rotation[i], glm::vec3(0, 0, 1));
				modelMatrix = glm::scale(modelMatrix, glm::vec3(
					scale[i], scale[i], 0.0f));
				transformData.push_back(modelMatrix);
			}

			if (Visible)
			{
				glBindBuffer(GL_ARRAY_BUFFER, mColorInstanceVBO);
				glBufferSubData(GL_ARRAY_BUFFER, 0,
					sizeof(glm::vec4) * GetParticleCount(), color.data());
				glBindBuffer(GL_ARRAY_BUFFER, mTransformInstanceVBO);
				glBufferSubData(GL_ARRAY_BUFFER, 0,
					sizeof(glm::mat4) * GetParticleCount(), transformData.data());
				glBindBuffer(GL_ARRAY_BUFFER, 0);

				glBindVertexArray(mVAO);
				glDrawArraysInstanced(GL_TRIANGLES, 0, 6, offset.size());
				glBindVertexArray(0);
			}
			if (Debug::CheckOpenGLError())
				DCTrace << "GraphicsGL::DrawSpriteText - Failed to set active texture!\n";
		}

		SpriteParticleSystem::Particle::Particle(double lifetime,
			const Vec2& position, const Vec2& velocity, const Vec2& acceleration, float scale, float spin, const Vec4& tint,
			ParticleColorAnimator* colorAnimator, LinearParticleAnimator* linearAnimator)
			: mLifetime(lifetime), mLifeleft(lifetime), mPosition(position), mVelocity(velocity), mAcceleration(acceleration),
			mScale(scale),mRotation(0), mRotationRate(spin), mTint(tint),
			mColorAnimator(colorAnimator), mLinearAnimator(linearAnimator)
		{
		}

		void SpriteParticleSystem::Particle::Update(double dt)
		{
			mLifeleft -= dt;
			if (mLifeleft < 0)
				mLifeleft = 0;
			
			mVelocity.x += mAcceleration.x * dt;
			mVelocity.y += mAcceleration.y * dt;

			if (mLinearAnimator)
			{
				mVelocity.x *= (100 - mLinearAnimator->Dampening) / 100;
				mVelocity.y *= (100 - mLinearAnimator->Dampening) / 100;
				mScale += mLinearAnimator->Growth * dt;
				mRotation += mRotationRate * dt + (mLinearAnimator->Torque * dt * dt) / 2;
				mRotationRate += mLinearAnimator->Torque * dt;
			}
			mPosition.x += mVelocity.x * dt - (mAcceleration.x * dt * dt) / 2;
			mPosition.y += mVelocity.y * dt - (mAcceleration.y * dt * dt) / 2;

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

		void SpriteParticleSystem::Update(double dt)
		{
			if (mParticleEmitter)
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
			}
		}

		void SpriteParticleSystem::AddParticle(void)
		{
			unsigned emitCount = 1 + rand() % (mParticleEmitter->EmitVariance + 1);
			for (unsigned i = 0; i < emitCount; ++i)
			{
				double lifetime = mParticleEmitter->Lifetime + mParticleEmitter->LifetimeVariance * (rand() % 100 - 50) / 100;
				Vec2 velocity = Vec2(mParticleEmitter->StartVelocity.x + mParticleEmitter->RandomVelocity.x * (rand() % 100 - 50) / 50,
					mParticleEmitter->StartVelocity.y + mParticleEmitter->RandomVelocity.y * (rand() % 100 - 50) / 50);
				float size = mParticleEmitter->Size + mParticleEmitter->SizeVariance * (rand() % 100 - 50) / 100;
				float spin = mParticleEmitter->Spin + mParticleEmitter->SpinVariance * (rand() % 100 - 50) / 100;
				Vec2 force = Vec2(0, 0);
				if (mLinearAnimator)
				{
					Vec2 force = Vec2(mLinearAnimator->Force.x + mLinearAnimator->RandomForce.x * (rand() % 100 - 50) / 50,
						mLinearAnimator->Force.y + mLinearAnimator->RandomForce.y * (rand() % 100 - 50) / 50);
				}
				mParticleList.push_back(Particle(
					lifetime, Vec2(0, 0), velocity, force, size, spin, Tint, mColorAnimator, mLinearAnimator));
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