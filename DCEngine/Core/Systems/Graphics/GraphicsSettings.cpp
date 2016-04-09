/*****************************************************************************/
/*!
\file   Graphics.cpp
\author William Mao
\par    email: william.mao\@digipen.edu
\date   9/16/2015
\brief  Graphics system of DCEngine.

@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/

#include "Graphics.h"

namespace DCEngine
{
	namespace Systems
	{
#if(DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDefineType(Graphics, "Graphics", DCEngineCore, builder, type) {
      ZilchBindMethod(builder, type, &Graphics::SetResolution, ZilchNoOverload, "SetResolution", "resolution");
      ZilchBindMethod(builder, type, &Graphics::SetAntiAliasingLevel, ZilchNoOverload, "SetAntiAliasingLevel", "samples");
      ZilchBindMethod(builder, type, &Graphics::ToggleFullscreen, ZilchNoOverload, "ToggleFullscreen", ZilchNoNames);
    }
#endif

		void Graphics::SetResolution(unsigned x, unsigned y)
		{
			Settings.ScreenWidth = x;
			Settings.ScreenHeight = y;
			DispatchSystemEvents::WindowResize(Vec2(x,y));
		}
		void Graphics::SetAntiAliasingLevel(unsigned samples)
		{
			Settings.Samples = samples;
			DispatchSystemEvents::WindowRecreate();
		}
		void Graphics::ToggleFullscreen()
		{
			if (Settings.Fullscreen)
			{
				DispatchSystemEvents::WindowFullScreenDisabled();
				Settings.Fullscreen = false;
			}
			else
			{
				DispatchSystemEvents::WindowFullScreenEnabled();
				Settings.Fullscreen = true;
			}
		}
	}
}