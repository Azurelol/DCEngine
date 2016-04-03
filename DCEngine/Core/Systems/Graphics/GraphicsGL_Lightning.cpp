/******************************************************************************/
/*!
\file      GraphicsGL_Lightning.cpp
\author    William Mao
\par       email: c.sagel\@digipen.edu
\date      3/2/2016
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "GraphicsGL.h"
#include "../../Components/Sprite.h"

#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {

		void GraphicsGL::RenderDepths(Components::Camera * camera)
		{
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			glDrawBuffer(GL_NONE);
			for (const auto& drawList : *mDrawList)
				for (const auto& obj : drawList)
					if (dynamic_cast<Components::Sprite*>(obj))
						if (obj->Owner()->getComponent<Components::Transform>()->Translation.z == 0)
						{
							obj->SetUniforms(0, camera, 0);
							obj->Draw();
						}
			glDisable(GL_DEPTH_TEST);
			glDrawBuffer(GL_FRONT);
		}

    void GraphicsGL::RenderShadows(Components::Camera * camera, Components::Light * light)
    {
      SystemMethodTimer timer("RenderShadows", EnumeratedSystem::Graphics);
			glEnable(GL_STENCIL_TEST);
			glClear(GL_STENCIL_BUFFER_BIT);

			glEnable(GL_DEPTH_CLAMP);
			glEnable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
			glDepthFunc(GL_LESS);
			glDepthMask(GL_FALSE);
			glDrawBuffer(GL_NONE);
			glStencilFunc(GL_ALWAYS, 0, 0xff);
			
			if (light->getVisibilityCulling())
				glStencilOp(GL_KEEP, GL_INCR, GL_INCR);
			else
				glStencilOp(GL_KEEP, GL_INCR, GL_KEEP);

			for (const auto& drawList : *mDrawList)
				for (const auto& obj : drawList)
					if (dynamic_cast<Components::Sprite*>(obj))
						if (obj->Owner()->getComponent<Components::Transform>()->Translation.z == 0)
						{
							obj->SetUniforms(ShadowingShader, camera, light);
							obj->Draw();
						}

			// Restore local stuff
			glDepthMask(GL_TRUE);
			glDrawBuffer(GL_FRONT);
			glEnable(GL_CULL_FACE);
			glDisable(GL_DEPTH_CLAMP);
    }

    void GraphicsGL::RenderObjects(Components::Camera * camera, Components::Light * light, ShaderPtr shader)
    {
			for (const auto& drawList : *mDrawList)
			{
				for (const auto& obj : drawList)
				{
					obj->SetUniforms(shader, camera, light);
					obj->Draw();
				}
			}
    }

    void GraphicsGL::RenderBackground(ShaderPtr shader, Components::Camera * camera)
    {
    }

    void GraphicsGL::RenderZ0Scene(Components::Camera * camera, Components::Light * light, ShaderPtr shader)
    {
    }

		void GraphicsGL::PreRender(Components::Camera * camera)
		{
      SystemMethodTimer timer("PreRender", EnumeratedSystem::Graphics);
			glBindFramebuffer(GL_FRAMEBUFFER, FBO);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			glDisable(GL_BLEND);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			
			GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
			glDrawBuffers(3, attachments);

			RenderObjects(camera);
		}

		void GraphicsGL::RenderLights(Components::Light * light)
		{
			LightingShader->Use();
			glDisable(GL_STENCIL_TEST);
			if(light)
			{
				LightingShader->SetInteger("useLight", true);
				glm::mat4 lightMatrix;
				Components::Transform* lightTransform = light->Owner()->getComponent<Components::Transform>();

				lightMatrix = glm::translate(lightMatrix, glm::vec3(
					lightTransform->Translation.x,
					lightTransform->Translation.y,
					lightTransform->Translation.z));
				lightMatrix = glm::rotate(lightMatrix, lightTransform->Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
				lightMatrix = glm::scale(lightMatrix, glm::vec3(lightTransform->Scale.x,
					lightTransform->Scale.y, 1.0f));

				std::string var("gLight.");
				std::string member;
				member = var + "Visible";
				LightingShader->SetInteger(member.c_str(), light->getVisible());
				member = var + "VisibilityCulling";
				LightingShader->SetInteger(member.c_str(), light->getVisibilityCulling());
				member = var + "VisibilityEvents";
				LightingShader->SetInteger(member.c_str(), light->getVisibilityEvents());
				member = var + "CastShadows";
				LightingShader->SetInteger(member.c_str(), light->getVisibilityEvents());
				member = var + "Diffuse";
				LightingShader->SetInteger(member.c_str(), light->getDiffuse());
				member = var + "LightType";
				LightingShader->SetInteger(member.c_str(), light->getTypeAsInt());
				member = var + "Color";
				LightingShader->SetVector4f(member.c_str(), light->getColor());
				member = var + "Intensity";
				LightingShader->SetFloat(member.c_str(), light->getIntensity());
				member = var + "Range";
				LightingShader->SetFloat(member.c_str(), light->getRange());
				member = var + "Falloff";
				LightingShader->SetFloat(member.c_str(), light->getFalloff());
				member = var + "Direction";
				LightingShader->SetVector3f(member.c_str(), light->getDirectionVector());
				member = var + "InnerAngle";
				LightingShader->SetFloat(member.c_str(), light->getInnerAngle() * 3.141593f / 360.0f);
				member = var + "OuterAngle";
				LightingShader->SetFloat(member.c_str(), light->getOuterAngle() * 3.141593f / 360.0f);
				member = var + "Position";
				LightingShader->SetVector3f(member.c_str(), lightTransform->Translation);
				member = var + "Model";
				LightingShader->SetMatrix4(member.c_str(), lightMatrix);

				if (light->getCastShadows())
				{
					glEnable(GL_STENCIL_TEST);
					glStencilFunc(GL_GEQUAL, 0x1, 0xFF);
					glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
				}
			}
			else
				LightingShader->SetInteger("useLight", false);

			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);


			LightingShader->SetInteger("gWorldCoords", 0);
			LightingShader->SetInteger("gWorldNormal", 1);
			LightingShader->SetInteger("gColor", 2);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, PosTexture);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, NormalTexture);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, ColorTexture);

			glDrawBuffer(GL_COLOR_ATTACHMENT3);

			glBegin(GL_TRIANGLE_FAN);
			glVertex4f(-1, -1, 0, 0);
			glVertex4f( 1, -1, 1, 0);
			glVertex4f( 1,  1, 1, 1);
			glVertex4f(-1,  1, 0, 1);
			glEnd();

			glClear(GL_STENCIL_BUFFER_BIT);
			glDisable(GL_STENCIL_TEST);
		}

		void GraphicsGL::RenderScene(float exposure)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDrawBuffer(GL_FRONT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glDisable(GL_BLEND);
			FinalRenderShader->Use();
			FinalRenderShader->SetFloat("Exposure", exposure);

			FinalRenderShader->SetInteger("LightedFrag", 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, FinalColor);

			glBegin(GL_TRIANGLE_FAN);
			glVertex4f(-1, -1, 0, 0);
			glVertex4f(1, -1, 1, 0);
			glVertex4f(1, 1, 1, 1);
			glVertex4f(-1, 1, 0, 1);
			glEnd();
		}

    void GraphicsGL::DrawDebug()
    {
    }

  }
}