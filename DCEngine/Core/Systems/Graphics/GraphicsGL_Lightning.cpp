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

		void GraphicsGL::PreRender(Components::Camera * camera)
		{
      SystemMethodTimer timer("PreRender", EnumeratedSystem::Graphics);
			glBindFramebuffer(GL_FRAMEBUFFER, multisampleFBO);

			GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
			glDrawBuffers(3, attachments);

			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			RenderObjects(camera);

			//transfer multisample fbo to regular fbo
			///transfer color buffers
			for (unsigned i = 0; i < 3; ++i)
			{
				glBindFramebuffer(GL_READ_FRAMEBUFFER, multisampleFBO);
				glReadBuffer(GL_COLOR_ATTACHMENT0 + i);
				glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
				glDrawBuffer(GL_COLOR_ATTACHMENT0 + i);
				glBlitFramebuffer(
					0, 0, Settings.ScreenWidth, Settings.ScreenHeight,
					0, 0, Settings.ScreenWidth, Settings.ScreenHeight,
					GL_COLOR_BUFFER_BIT, GL_NEAREST);
			}
			///transfer depth buffer
			glBindFramebuffer(GL_READ_FRAMEBUFFER, multisampleFBO);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
			glBlitFramebuffer(
				0, 0, Settings.ScreenWidth, Settings.ScreenHeight,
				0, 0, Settings.ScreenWidth, Settings.ScreenHeight,
				GL_DEPTH_BUFFER_BIT, GL_NEAREST);
			///return control flow to regular fbo
			glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		}

		void GraphicsGL::RenderShadows(Components::Camera * camera, Components::Light * light)
		{
			SystemMethodTimer timer("RenderShadows", EnumeratedSystem::Graphics);
			glBindFramebuffer(GL_FRAMEBUFFER, FBO);
			glEnable(GL_STENCIL_TEST);
			glClear(GL_STENCIL_BUFFER_BIT);

			glEnable(GL_DEPTH_CLAMP);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			glDepthMask(GL_FALSE);
			glDisable(GL_CULL_FACE);
			//glEnable(GL_BLEND);
			//glBlendFunc(GL_ONE, GL_ONE);
			//glDrawBuffer(GL_COLOR_ATTACHMENT2);
			glDrawBuffer(GL_NONE);
			glStencilFunc(GL_ALWAYS, 0, 0xff);

			for (const auto& drawList : *mDrawList)
			{
				for (const auto& obj : drawList)
				{
					Components::Sprite* sprite = dynamic_cast<Components::Sprite*>(obj);
					if (sprite)
					{
						if (sprite->getNOSHADOW())
							continue;
						if (light->getVisibilityCulling())
						{
							if (sprite->getCullVisibility())
								glStencilOp(GL_KEEP, GL_INCR, GL_INCR);
							else
								glStencilOp(GL_KEEP, GL_INCR, GL_KEEP);
						}
						else
							glStencilOp(GL_KEEP, GL_INCR, GL_KEEP);

						if (sprite->getForceLightCulling())
							glStencilOp(GL_KEEP, GL_INCR, GL_INCR);

						Components::Transform* objTfm = obj->Owner()->getComponent<Components::Transform>();
						if (objTfm->Translation.z == 0)
						{
							Components::Transform* lTfm = light->Owner()->getComponent<Components::Transform>();
							Vec3 vector = Vec3(objTfm->Translation - lTfm->Translation);
							float lengthSquared = vector.x * vector.x + vector.y * vector.y + vector.z * vector.z;
							float objRadiusSquared = objTfm->Scale.x * objTfm->Scale.x + objTfm->Scale.y * objTfm->Scale.y;
							if (lengthSquared < objRadiusSquared + 2 * light->getRange() * (objTfm->Scale.x + objTfm->Scale.y)
								+ light->getRange() * light->getRange())
							{
								obj->SetUniforms(ShadowingShader, camera, light);
								obj->Draw();
							}
						}
					}
				}
			}

			// Restore local stuff
			glDepthMask(GL_TRUE);
			//glDrawBuffer(GL_BACK);
			glEnable(GL_CULL_FACE);
			glDisable(GL_DEPTH_CLAMP);
		}

		void GraphicsGL::RenderLights(Components::Light * light)
		{
			SystemMethodTimer timer("RenderLights", EnumeratedSystem::Graphics);
			glBindFramebuffer(GL_FRAMEBUFFER, FBO);
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
				member = var + "CullLight";
				LightingShader->SetFloat(member.c_str(), light->getCullLight());
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
			glVertex4f(-1,-1, 0, 0);
			glVertex4f( 1,-1, 1, 0);
			glVertex4f( 1, 1, 1, 1);
			glVertex4f(-1, 1, 0, 1);
			glEnd();

			glClear(GL_STENCIL_BUFFER_BIT);
			glDisable(GL_STENCIL_TEST);
		}

		void GraphicsGL::RenderScene(float exposure, bool lit)
		{
			SystemMethodTimer timer("RenderScene", EnumeratedSystem::Graphics);
			//glBindFramebuffer(GL_READ_FRAMEBUFFER, multisampleFBO);
			//glReadBuffer(GL_COLOR_ATTACHMENT0);
			//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
			//glDrawBuffer(GL_COLOR_ATTACHMENT3);
			//glBlitFramebuffer(
			//	0, 0, Settings.ScreenWidth, Settings.ScreenHeight,
			//	0, 0, Settings.ScreenWidth, Settings.ScreenHeight,
			//	GL_COLOR_BUFFER_BIT, GL_NEAREST);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDrawBuffer(GL_BACK);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			FinalRenderShader->Use();
			FinalRenderShader->SetInteger("useLight", lit);
			FinalRenderShader->SetFloat("Exposure", exposure);
			FinalRenderShader->SetInteger("LightedFrag", 0);
      FinalRenderShader->SetVector4f("ClearColor", Vec4(1,1,1,1));
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, FinalColor);

			glBegin(GL_TRIANGLE_FAN);
			glVertex4f(-1,-1, 0, 0);
			glVertex4f( 1,-1, 1, 0);
			glVertex4f( 1, 1, 1, 1);
			glVertex4f(-1, 1, 0, 1);
			glEnd();
		}

		void GraphicsGL::FinalRender()
		{

		}

    void GraphicsGL::DrawDebug()
    {
    }

		void GraphicsGL::ClearFrameBufferObjects()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, FBO);
			GLuint attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
				GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(4, attachments);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			glBindFramebuffer(GL_FRAMEBUFFER, multisampleFBO);
			glDrawBuffers(3, attachments);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDrawBuffer(GL_BACK);
		}

  }
}