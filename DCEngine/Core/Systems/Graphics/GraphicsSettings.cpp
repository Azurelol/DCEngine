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
		void Graphics::SetResolution(const glm::uvec2& resolution)
		{
			Settings.ScreenWidth = resolution.x;
			Settings.ScreenHeight = resolution.y;
			DispatchSystemEvents::WindowResize(resolution);
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