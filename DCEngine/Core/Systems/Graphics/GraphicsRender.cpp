/******************************************************************************/
/*!
@file   GraphicsRender.cpp
@author William Mao
@par    email: c.sagel\@digipen.edu
@date   11/07/2015
@brief  This file contains the main rendering functions used by the
        Graphics system.
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Graphics.h"
#include "../../Engine/Profiler.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    @brief Updates the objects in the main draw list.
    @param dt The delta time.
    */
    /**************************************************************************/
    void Graphics::UpdateObjects(float dt)
    {      
      SystemMethodTimer timer("UpdateObjects", EnumeratedSystem::Graphics);
      for (auto&& drawList : mDrawList)
        for (auto&& obj : drawList)
          obj->Update(dt);
    }

    /**************************************************************************/
    /*!
    @brief Renders all shadows on the scene.
    @param camera A pointer to the camera.
    @param light A pointer to the light which to render shadows from.
    */
    /**************************************************************************/
    void Graphics::RenderShadows(Components::Camera* camera, Components::Light* light)
    {
      SystemMethodTimer timer("RenderShadows", EnumeratedSystem::Graphics);
      glEnable(GL_DEPTH_CLAMP);
      glDisable(GL_CULL_FACE);

      glStencilFunc(GL_ALWAYS, 0, 0xff);
      glEnable(GL_DEPTH_TEST);
      if (light->getVisibilityCulling())
        glStencilOp(GL_KEEP, GL_INCR, GL_INCR);
      else
        glStencilOp(GL_KEEP, GL_INCR, GL_KEEP);

      for (const auto& drawList : mDrawList)
      {
        for (const auto& obj : drawList)
        {
          if (obj->Owner()->getComponent<Components::Sprite>())
          {
            Components::Transform* transform = obj->Owner()->getComponent<Components::Transform>();
            if (transform->Translation.z == 0)
            {
              glDepthMask(GL_FALSE);
              obj->SetUniforms(GraphicsHandler->ShadowingShader, camera, light);
              obj->Draw();
            }
          }
        }
      }
      // Restore local stuff
      glDisable(GL_DEPTH_CLAMP);
      glEnable(GL_CULL_FACE);
    }

    /**************************************************************************/
    /*!
    @brief Renders the scene.
    @param camera A pointer to the camera.
    @param light A pointer to the light which to render shadows from.
    @param shader A pointer to the shader that will be used.
    */
    /**************************************************************************/
    void Graphics::RenderScene(Components::Camera* camera, Components::Light* light, ShaderPtr shader)
    {
      SystemMethodTimer timer("RenderScene", EnumeratedSystem::Graphics);
      for (const auto& drawList : mDrawList)
      {
        for (const auto& obj : drawList)
        {
          obj->SetUniforms(shader, camera, light);
          obj->Draw();
        }
      }
    }

    /**************************************************************************/
    /*!
    @brief Renders the background.
    @param shader A pointer to the shader that will be used.
    @param camera A pointer to the camera.
    */
    /**************************************************************************/
    void Graphics::RenderBackground(ShaderPtr shader, Components::Camera * camera)
    {
      SystemMethodTimer timer("RenderBackground", EnumeratedSystem::Graphics);
      for (auto&& drawList : mDrawList)
      {
        for (const auto& obj : drawList)
        {
          Components::Transform* transform = obj->Owner()->getComponent<Components::Transform>();
          if (transform->Translation.z != 0)
          {
            obj->SetUniforms(shader, camera, 0);
            obj->Draw();
          }
        }
      }
    }

    /**************************************************************************/
    /*!
    @brief Renders the scene at Z = 0.
    @param camera A pointer to the camera.
    @param light A pointer to the light which to render shadows from.
    @param shader A pointer to the shader that will be used.
    */
    /**************************************************************************/
    void Graphics::RenderZ0Scene(Components::Camera * camera, Components::Light* light, ShaderPtr shader)
    {
      SystemMethodTimer timer("RenderZ0Scene", EnumeratedSystem::Graphics);
      for (const auto& drawList : mDrawList)
      {
        for (const auto& obj : drawList)
        {
          Components::Transform* transform = obj->Owner()->getComponent<Components::Transform>();
          if (transform->Translation.z == 0)
          {
            obj->SetUniforms(shader, camera, light);
            obj->Draw();
          }
        }
      }
    }

    /**************************************************************************/
    /*!
    @brief Draws all debug objects.
    */
    /**************************************************************************/
    void Graphics::DrawDebug(Components::Camera* camera)
    {
      SystemMethodTimer timer("DrawDebug", EnumeratedSystem::Graphics);
      GraphicsHandler->DebugShader->Use();
			GraphicsHandler->DebugShader->SetMatrix4("Proj", camera->GetProjectionMatrix());
			GraphicsHandler->DebugShader->SetMatrix4("View", camera->GetViewMatrix());
			for (const auto& debugObj : mDebugRectangleList)
			{
				debugObj.Draw();
			}
      for (const auto& debugObj : mDebugLineList)
      {
        debugObj.Draw();
      }
      
      for (const auto& debugObj : mDebugCircleList)
      {
        debugObj.Draw();
      }
      mDebugLineList.clear();
      mDebugRectangleList.clear();
      mDebugCircleList.clear();
    }

  }
}