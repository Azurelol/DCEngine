#pragma once

#include "Graphics.h"

namespace DCEngine
{
	namespace Systems
	{
		Graphics::DebugObject::DebugObject(const Vec4& color, const Vec3& position)
			: color(color), position(position)
		{

		}
		Graphics::DebugLine::DebugLine(const Vec4& color, const Vec3& startPosition, const Vec3& endPosition)
			: DebugObject(color, startPosition), endPosition(endPosition)
		{

		}
		void Graphics::DebugLine::Draw(void) const
		{
			GraphicsHandler->DrawLineSegment(position, endPosition, color);
		}
		Graphics::DebugRectangle::DebugRectangle(const Vec4& color, const Vec3& position, const Vec2& scale)
			: DebugObject(color, position), scale(scale)
		{

		}
		void Graphics::DebugRectangle::Draw(void) const
		{
			GraphicsHandler->DrawRectangle(position, scale.x, scale.y, color);
		}
		Graphics::DebugCircle::DebugCircle(const Vec4& color, const Vec3& position, float radius)
			: DebugObject(color, position), radius(radius)
		{

		}
		void Graphics::DebugCircle::Draw(void) const
		{
			GraphicsHandler->DrawCircle(position, radius, color);
		}
	}
};