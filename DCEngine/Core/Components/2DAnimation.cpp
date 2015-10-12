#include "Sprite.h"

namespace DCEngine {

	int Sprite::UpdateAnimationSpeed(void)
	{
		if (AnimationSpeed == 0)
		{
			return 0;
		}
		else
		{
			AnimationSpeedFPS = (float)1 / AnimationSpeed;
			return 1;
		}
	}

	void Sprite::IncreaseAnimationCounter(float dt)
	{
		AnimationSpeedFPSCounter += dt;
	}

	int Sprite::IsNextFrame(void)
	{
		if (AnimationSpeedFPSCounter >= AnimationSpeedFPS)
		{
			AnimationSpeedFPSCounter = 0;
			CurrentFrame++;
			if (CurrentFrame >= TotalFrames)
			{
				CurrentFrame = 0;
			}
			//Current frame started from 0
			return 1;
		}
		else
		{
			return 0;
		}
	}

	bool Sprite::CheckAnimationIntialized(void)
	{
		if (AnimationInitialized == false)
		{
			CurrentFrame = StartFrame;
			AnimationInitialized = true;
			return false;
		}
		else
		{
			return true;
		}
	}
}