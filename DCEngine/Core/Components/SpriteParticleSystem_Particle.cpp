/******************************************************************************/
/*!
@file   SpriteParticleSystem_Particle.cpp
@author William Mao
@par    email: c.sagel\@digipen.edu
@date   1/20/2016
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

    /**************************************************************************/
    /*!
    @brief Updates the particle.
    @param dt The delta time.
    */
    /**************************************************************************/
    void SpriteParticleSystem::Particle::Update(float dt)
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
		if (mScale < 0)
		{
			mScale = 0;
		}
        mRotation += mRotationRate * dt + (mLinearAnimator->Torque * dt * dt) / 2;
        mRotationRate += mLinearAnimator->Torque * dt;
      }
      mPosition.x += mVelocity.x * dt - (mAcceleration.x * dt * dt) / 2;
      mPosition.y += mVelocity.y * dt - (mAcceleration.y * dt * dt) / 2;

      if (mColorAnimator)
      {
        float percentLifeLeft = (mLifeleft / mLifetime) * 100;
        if (percentLifeLeft <= 100 && percentLifeLeft >= 75)
        {
          float t = (percentLifeLeft - 75) / 25;
          mColor.r = mTint.r * (mColorAnimator->Color0.r * t + mColorAnimator->Color1.r * (1 - t));
          mColor.g = mTint.g * (mColorAnimator->Color0.g * t + mColorAnimator->Color1.g * (1 - t));
          mColor.b = mTint.b * (mColorAnimator->Color0.b * t + mColorAnimator->Color1.b * (1 - t));
          mColor.a = mTint.a * (mColorAnimator->Color0.a * t + mColorAnimator->Color1.a * (1 - t));
        }
        else if (percentLifeLeft <= 75 && percentLifeLeft >= 50)
        {
          float t = (percentLifeLeft - 50) / 25;
          mColor.r = mTint.r * mColorAnimator->Color1.r * t + mColorAnimator->Color2.r * (1 - t);
          mColor.g = mTint.g * mColorAnimator->Color1.g * t + mColorAnimator->Color2.g * (1 - t);
          mColor.b = mTint.b * mColorAnimator->Color1.b * t + mColorAnimator->Color2.b * (1 - t);
          mColor.a = mTint.a * mColorAnimator->Color1.a * t + mColorAnimator->Color2.a * (1 - t);
        }
        else if (percentLifeLeft <= 50 && percentLifeLeft >= 25)
        {
          float t = (percentLifeLeft - 25) / 25;
          mColor.r = mTint.r * mColorAnimator->Color2.r * t + mColorAnimator->Color3.r * (1 - t);
          mColor.g = mTint.g * mColorAnimator->Color2.g * t + mColorAnimator->Color3.g * (1 - t);
          mColor.b = mTint.b * mColorAnimator->Color2.b * t + mColorAnimator->Color3.b * (1 - t);
          mColor.a = mTint.a * mColorAnimator->Color2.a * t + mColorAnimator->Color3.a * (1 - t);
        }
        else if (percentLifeLeft <= 25 && percentLifeLeft >= 0)
        {
          float t = percentLifeLeft / 25;
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

    /*===================
          ACCESSORS
    ===================*/
    float SpriteParticleSystem::Particle::GetLifetime(void) const
    {
      return mLifetime;
    }
    float SpriteParticleSystem::Particle::GetLifeleft(void) const
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

  }
}