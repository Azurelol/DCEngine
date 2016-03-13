/******************************************************************************/
/*!
\file   GraphicsGL_Debug.cpp
\author Chen Shu
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
      //this->SpriteShader->SetMatrix4("model", cleanup, true);
      //this->SpriteShader->SetVector4f("color", colorclean, true);
      this->SpriteShader->SetInteger("isTexture", 0, true);
			//glEnable(GL_DEPTH_TEST);
			//glDepthFunc(GL_LESS);
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
    void GraphicsGL::DrawRectangle(const Vec3& pos, Real width, Real height, const Vec4& color, bool fill)
    {
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			CleanBuffer();
			DebugShader->SetVector4f("Color", glm::vec4(color.r, color.g, color.b, color.a), true);
			glm::mat4x4 model;
			model = glm::translate(model, pos);
			model = glm::scale(model, Vec3(width / 2, height / 2, 0));
			DebugShader->SetMatrix4("Model", model, true);

			if (fill)
				glBegin(GL_TRIANGLE_FAN);
			else
				glBegin(GL_LINE_LOOP);

			glVertex2f( 1,  1);
			glVertex2f( 1, -1);
			glVertex2f(-1, -1);
			glVertex2f(-1,  1);

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
    void GraphicsGL::DrawCircle(const Vec3& pos, Real radius, const Vec4& color, bool fill)
    {
      // Do your magic here Chen
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      CleanBuffer();
      DebugShader->SetVector4f("Color", glm::vec4(color.r, color.g, color.b, color.a), true);
			glm::mat4x4 model;
			model = glm::translate(model, pos);
			model = glm::scale(model, Vec3(radius / 2, radius / 2, 0));
			DebugShader->SetMatrix4("Model", model, true);

			if (fill)
				glBegin(GL_TRIANGLE_FAN);
			else
				glBegin(GL_LINE_LOOP);
			
      double M_PI = 3.1415926535;
      static double PointsNumber = 128;
      for (double i = 0; i < 2 * M_PI; i = i + ((2 * M_PI) / PointsNumber))
      {
        auto PositionOrigin = glm::vec4(0, 0, 0, 0.0f);
        //PositionOrigin = cam.GetProjectionMatrix() * cam.GetViewMatrix() * PositionOrigin;
        glm::vec4 Position = glm::vec4(PositionOrigin.x + cos(i), PositionOrigin.y + sin(i), PositionOrigin.z, 0.0);
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
    void GraphicsGL::DrawLineSegment(const Vec3& startPos, const Vec3& endPos, const Vec4& color)
    {
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			CleanBuffer();
			DebugShader->SetVector4f("Color", glm::vec4(color.r, color.g, color.b, color.a), true);
			Vec3 vector = endPos - startPos;
			float magnitude = Math::Normalize(&Math::Vector3(vector.x, vector.y, vector.z));
			glm::mat4x4 model;
			model = glm::translate(model, Vec3(startPos.x, startPos.y, startPos.z));
			model = glm::scale(model, Vec3(magnitude, magnitude, 0));
			DebugShader->SetMatrix4("Model", model, true);

			glVertex2f(0, 0);
			glBegin(GL_LINES);
			glVertex2f(vector.x / magnitude, vector.y / magnitude);
			glEnd();

			CleanBuffer();
    }

  }
}
