/******************************************************************************/
/*!
\file   GraphicsGL_Debug.cpp
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   7/30/2015
\brief  The implementation of the DebugDraw functions.

@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "GraphicsGL.h"
#include "../../Debug/DebugGraphics.h"

namespace DCEngine {
  namespace Systems {

    void GraphicsGL::CleanBuffer()
    {
      glm::mat4 cleanup;
      glm::vec4 colorclean;
      this->SpriteShader->SetMatrix4("model", cleanup, true);
      this->SpriteShader->SetVector4f("color", colorclean, true);
      this->SpriteShader->SetInteger("isTexture", 0, true);
    }

    /**************************************************************************/
    /*!
    \brief  Configures the DebugDraw Line VAO.
    */
    /**************************************************************************/
    void GraphicsGL::ConfigureLineVAO()
    {
    }

    /**************************************************************************/
    /*!
    \brief  Configures the DebugDraw Circle VAO.
    */
    /**************************************************************************/
    void GraphicsGL::ConfigureCircleVAO()
    {
    }

    /**************************************************************************/
    /*!
    \brief  Configures the DebugDraw Rectangle VAO.
    */
    /**************************************************************************/
    void GraphicsGL::ConfigureRectangleVAO()
    {
    }

    /**************************************************************************/
    /*!
    \brief  Sets the DebugDraw projection and view uniforms from the camera
    */
    /**************************************************************************/
    void GraphicsGL::SetDebugDrawShaderProjViewUniform(Components::Camera & camera)
    {

    }

    /**************************************************************************/
    /*!
    \brief  Takes a DrawDebug object and draws depending on.
    \param  A reference to the drawdebug object.
    \para   A reference to the space's camera.
    */
    /**************************************************************************/
    void GraphicsGL::DrawDebug(DebugDrawObject & debugDrawObj, Components::Camera& cam)
    {
      // The received object is a DrawLine
      //if (std::type_index(typeid(debugDrawObj)) == typeid(DrawLine)) {
      //  
      //}

    }

    void GraphicsGL::DrawCircle(DrawCircleObj & obj)
    {
    }

    void GraphicsGL::DrawRectangle(DrawRectObj & obj)
    {
    }

    void GraphicsGL::DrawLineSegment(DrawLineObj & obj)
    {
    }

    /**************************************************************************/
    /*!
    \brief  Draws a rectangle on screen.
    \param  The position of the center of the rectangle.
    \param  The length of the rectangle.
    \param  The height of the rectangle.
    \param  The color of the rectangle.
    */
    /**************************************************************************/
    void GraphicsGL::DrawRectangle(Vec3& pos, Real& width, Real& height, Vec4& color, Components::Camera& cam, bool fill)
    {
			CleanBuffer();
			this->SpriteShader->SetVector4f("spriteColor", glm::vec4(color.r, color.g, color.b, 1.0), true);
			//auto CameraMatrix = cam.GetProjectionMatrix() * cam.GetViewMatrix();
			glBegin(GL_LINE_LOOP);
			auto PositionOrigin = glm::vec4(pos.x, pos.y, pos.z, 0.0);
			auto Position1 = PositionOrigin + glm::vec4(width / 2.0f , height / 2.0f, 0.0f, 0.0f);
			auto Position2 = PositionOrigin + glm::vec4(width / 2.0f , height / -2.0f, 0.0f, 0.0f);
			auto Position3 = PositionOrigin + glm::vec4(width / -2.0f , height / -2.0f, 0.0f, 0.0f);
			auto Position4 = PositionOrigin + glm::vec4(width / -2.0f , height / 2.0f, 0.0f, 0.0f);
			
			glVertex3f(Position1.x, Position1.y, Position1.z);
			glVertex3f(Position2.x, Position2.y, Position2.z);
			glVertex3f(Position3.x, Position3.y, Position3.z);
			glVertex3f(Position4.x, Position4.y, Position4.z);
			
			glEnd();
			CleanBuffer();
    }
    /**************************************************************************/
    /*!
    \brief Draws a circle on screen.
    \param The position of the center of the circle.
    \param The radius of the circle.
    \param The color of the circle.
    */
    /**************************************************************************/
    void GraphicsGL::DrawCircle(Vec3& pos, Real& radius, Vec4& color, Components::Camera& cam, bool fill)
    {
      // Do your magic here Chen
      CleanBuffer();
      this->SpriteShader->SetVector4f("spriteColor", glm::vec4(color.r, color.g, color.b, 1.0), true);
      glBegin(GL_LINE_LOOP);
			
      double M_PI = 3.1415926535;
      static double PointsNumber = 128;
      for (double i = 0; i < 2 * M_PI; i = i + ((2 * M_PI) / PointsNumber))
      {
        auto PositionOrigin = glm::vec4(pos.x, pos.y, pos.z, 0.0f);
        //PositionOrigin = cam.GetProjectionMatrix() * cam.GetViewMatrix() * PositionOrigin;
        glm::vec4 Position = glm::vec4(PositionOrigin.x + radius * cos(i), PositionOrigin.y + radius * sin(i), PositionOrigin.z, 0.0);
        glVertex3f(Position.x, Position.y, Position.z);
      }
			
      glEnd();
			CleanBuffer();
    }

    /**************************************************************************/
    /*!
    \brief  Draws a line segment on screen.
    \param  The starting position of the line segment.
    \param  The ending position of the line segment.
    \param  The length of the line segment.
    \param  The color of the line segment.
    */
    /**************************************************************************/
    void GraphicsGL::DrawLineSegment(Vec3& startPos, Vec3& endPos, Vec4& color, Components::Camera& cam)
    {
			CleanBuffer();
			this->SpriteShader->SetVector4f("spriteColor", glm::vec4(color.r, color.g, color.b, 1.0), true);
			glBegin(GL_LINES);
			auto Position1 = glm::vec4(startPos.x, startPos.y, startPos.z, 0.0);
			auto Position2 = glm::vec4(endPos.x, endPos.y, endPos.z, 0.0);
			glVertex3f(Position1.x, Position1.y, Position1.z);
			glVertex3f(Position2.x, Position2.y, Position2.z);
			glEnd();
			CleanBuffer();
    }




  }
}
