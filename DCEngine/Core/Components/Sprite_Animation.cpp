/******************************************************************************/
/*!
@file   Sprite_Animation.cpp
@author Chen Shu, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/23/2015
@brief
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Sprite.h"

namespace DCEngine {
  namespace Components
  {

		int Sprite::UpdateAnimationSpeed(void)
		{
			if (AnimationSpeed == 0)
				return 0;
			else
			{
				AnimationSpeedFPS = (float)1 / AnimationSpeed;
				return 1;
			}
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
        return true;
    }
		void Sprite::ResetAnimationIndex(void)
		{
			CurrentColumn = 0;
			CurrentRow = 0;
		}
  }
}