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
		//DCTrace << std::round(1 / dt) << "\n";
		AnimationSpeedFPSCounter += dt;
	}

	bool Sprite::CheckAnimationIntialized(void)
	{
		if (AnimationInitialized == false)
		{
			CurrentColumn = StartColumn;
			CurrentRow = StartRow;
			AnimationInitialized = true;
			return false;
		}
		else
		{
			return true;
		}
	}

	float Sprite::GetAnimationSpeedFPSCounter(void)
	{
		return AnimationSpeedFPSCounter;
	}
	float Sprite::GetAnimationSpeedFPS(void)
	{
		return AnimationSpeedFPS;
	}

	void Sprite::ResetSpeedCounter(void)
	{
		AnimationSpeedFPSCounter = 0;
	}
}