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

#include "..\..\Engine\Engine.h"
#include "Graphics.h"

namespace DCEngine
{
	namespace Systems
	{

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
				auto fsevent = new Events::WindowFullScreenToggle();
				Daisy->Dispatch<Events::WindowFullScreenToggle>(fsevent);
				delete fsevent;
				Settings.Fullscreen = false;
			}
			else
			{
				auto fsevent = new Events::WindowFullScreenToggle();
				Daisy->Dispatch<Events::WindowFullScreenToggle>(fsevent);
				delete fsevent;
				Settings.Fullscreen = true;
			}
		}
		unsigned Graphics::GetScreenWidth() const
		{
			return Settings.ScreenWidth;
		}
		unsigned Graphics::GetScreenHeight() const
		{
			return Settings.ScreenHeight;
		}
	}
}