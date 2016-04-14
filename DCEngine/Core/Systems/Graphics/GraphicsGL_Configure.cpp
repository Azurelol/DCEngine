/******************************************************************************/
/*!
\file   GraphicsGL_Configure.cpp
\author Christian Sagel, Chen Shu
\par    email: c.sagel\@digipen.edu
\date   7/30/2015
\brief  Configuration of VAOs.

@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "GraphicsGL.h"

#include "../../Components/Sprite.h"
#include "../../Components/SpriteText.h"
#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {

		void GraphicsGL::ConfigureFBO()
		{
			glGenFramebuffers(1, &FBO);
			glBindFramebuffer(GL_FRAMEBUFFER, FBO);

			// - Position color buffer
			glGenTextures(1, &PosTexture);
			glBindTexture(GL_TEXTURE_2D, PosTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Settings.ScreenWidth, Settings.ScreenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, PosTexture, 0);

			// - Normal color buffer
			glGenTextures(1, &NormalTexture);
			glBindTexture(GL_TEXTURE_2D, NormalTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Settings.ScreenWidth, Settings.ScreenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER,  GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, NormalTexture, 0);

			// - Color + Specular color buffer
			glGenTextures(1, &ColorTexture);
			glBindTexture(GL_TEXTURE_2D, ColorTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Settings.ScreenWidth, Settings.ScreenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, ColorTexture, 0);

			glGenTextures(1, &FinalColor);
			glBindTexture(GL_TEXTURE_2D, FinalColor);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Settings.ScreenWidth, Settings.ScreenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, FinalColor, 0);
			
			GLuint rboDepth;
			glGenRenderbuffers(1, &rboDepth);
			glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Settings.ScreenWidth, Settings.ScreenHeight);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
			// - Finally check if framebuffer is complete
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "Framebuffer not complete!" << std::endl;

			glGenFramebuffers(1, &multisampleFBO);
			glBindFramebuffer(GL_FRAMEBUFFER, multisampleFBO);

			GLuint msColorTexture, msPositionTexture, msNormalTexture;

			// - Color + Specular color buffer
			glGenTextures(1, &msColorTexture);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, msColorTexture);
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, Settings.Samples, GL_RGBA16F,
				Settings.ScreenWidth, Settings.ScreenHeight, GL_TRUE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE,
				msColorTexture, 0);

			glGenTextures(1, &msPositionTexture);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, msPositionTexture);
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, Settings.Samples, GL_RGBA16F,
				Settings.ScreenWidth, Settings.ScreenHeight, GL_TRUE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D_MULTISAMPLE,
				msPositionTexture, 0);

			glGenTextures(1, &msNormalTexture);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, msNormalTexture);
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, Settings.Samples, GL_RGBA16F,
				Settings.ScreenWidth, Settings.ScreenHeight, GL_TRUE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D_MULTISAMPLE,
				msNormalTexture, 0);

			GLuint msRboDepth;
			glGenRenderbuffers(1, &msRboDepth);
			glBindRenderbuffer(GL_RENDERBUFFER, msRboDepth);
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, Settings.Samples, GL_DEPTH24_STENCIL8,
				Settings.ScreenWidth, Settings.ScreenHeight);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
				msRboDepth);
			// - Finally check if framebuffer is complete
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "Framebuffer not complete!" << std::endl;
		}

		void GraphicsGL::FreeFBO()
		{
			glDeleteFramebuffers(1, &multisampleFBO);
			glDeleteFramebuffers(1, &FBO);
		}


    /**************************************************************************/
    /*!
    \brief Compiles the engine's shaders.
    */
    /**************************************************************************/
    void GraphicsGL::CompileShaders()
    {
      DCTrace << "\n[GraphicsGL::CompileShaders] - Compiling shaders \n";
      // Grab a reference to the Sprite shader
      SpriteShader = Daisy->getSystem<Content>()->getShader("SpriteShader");
      SpriteShader->Compile();
      Components::Sprite::mShader = SpriteShader;
      ConfigureSpriteVAO();
      // Construct the SpriteText shader
      SpriteTextShader = Daisy->getSystem<Content>()->getShader("SpriteTextShader");
      SpriteTextShader->Compile();
      Components::SpriteText::mShader = SpriteTextShader;
      ConfigureSpriteTextVAO();
      // Construct the ParticleSystem shader
      ParticleSystemShader = Daisy->getSystem<Content>()->getShader("ParticleShader");
      ParticleSystemShader->Compile();
      Components::SpriteParticleSystem::mShader = ParticleSystemShader;
      ConfigureParticleBuffers();
      // Construct the Shadowing shader
      ShadowingShader = Daisy->getSystem<Content>()->getShader("ShadowingShader");
      ShadowingShader->Compile();
			// Construct the Debug shader
			DebugShader = Daisy->getSystem<Content>()->getShader("DebugShader");
			DebugShader->Compile();
			// Construct the Final Render Shader
			FinalRenderShader = Daisy->getSystem<Content>()->getShader("FinalRenderShader");
			FinalRenderShader->Compile();
			// Construct the Lighting shader
			LightingShader = Daisy->getSystem<Content>()->getShader("LightingShader");
			LightingShader->Compile();
			ConfigureFBO();
      DCTrace << "[GraphicsGL::CompileShaders] - Finished compiling shaders \n";
    }

    /**************************************************************************/
    /*!
    \brief Configures the QuadVAO, used for drawing sprites.
    \note  Because all sprites share the same vertex data, we only have to
    define a single VAO.
    */
    /**************************************************************************/
    void GraphicsGL::ConfigureSpriteVAO() {

      /*
      We first define a set of vertices with (0,0) coordinate being the top-left
      corner of the quad. This means that when translation or scaling transformations
      are applied onto the quad, they're transformmed from the top-left position
      of the quad.
      This is commonly accepted in 2D graphics/GUI systems where elements' positions
      are correspond to the top-left corner of the elements.
      */
      GLfloat vertices[]{
        // Position,      Texture
        -.5f, -.5f,      0.0f, 0.0f,
         .5f, -.5f,      1.0f, 0.0f,
         .5f,  .5f,      1.0f, 1.0f,
        -.5f,  .5f,      0.0f, 1.0f,
      };

      /*
      Next, we simply send the vertices to the GPU and configure the vertex attributes,
      which in this case is a single vertex attribute.
      */
			//if (SpriteVAO == 0)
			//{
				glGenVertexArrays(1, &SpriteVAO);
				Components::Sprite::mVAO = SpriteVAO;

				glGenBuffers(1, &SpriteVBO);

				glBindBuffer(GL_ARRAY_BUFFER, SpriteVBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
				glBindVertexArray(SpriteVAO);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(0);
			//}
    }

    /**************************************************************************/
    /*!
    @brief Configures the VAO used for SpriteText.
    @note  We reserve enough memory when initiating the VBO so that we can
    later update the VBO's memory when rendering characters.
    THe 2D quad requries 6 vertices of 4 floats each so we reserve
    6 * 4 floats of memory.
    Because we will be updating the content of the VBO's memory quite often,
    we allocate with GL_DYNAMIC_DRAW.
    */
    /**************************************************************************/
    void GraphicsGL::ConfigureSpriteTextVAO()
    {
      glGenVertexArrays(1, &SpriteTextVAO);
      glGenBuffers(1, &SpriteTextVBO);
			Components::SpriteText::mVAO = SpriteTextVAO;
			Components::SpriteText::mVBO = SpriteTextVBO;
      glBindVertexArray(SpriteTextVAO);
      glBindBuffer(GL_ARRAY_BUFFER, SpriteTextVBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindVertexArray(0);
    }

		void GraphicsGL::ConfigureParticleBuffers()
		{
			GLfloat vertices[]{
				// Position,      Texture
				-.5f, -.5f,      0.0f, 0.0f,
				 .5f, -.5f,      1.0f, 0.0f,
				 .5f,  .5f,      1.0f, 1.0f,
				-.5f,  .5f,      0.0f, 1.0f,
			};

			const unsigned MAX_PARTICLES = 10000;

			/*
			Next, we simply send the vertices to the GPU and configure the vertex attributes,
			which in this case is a single vertex attribute.
			*/
			glGenVertexArrays(1, &ParticleVAO);
			glGenBuffers(1, &ParticleVBO);
			glGenBuffers(1, &ParticleColorInstanceVBO);
			glGenBuffers(1, &ParticleTransformInstanceVBO);
			Components::SpriteParticleSystem::mVAO = ParticleVAO;
			Components::SpriteParticleSystem::mColorInstanceVBO = ParticleColorInstanceVBO;
			Components::SpriteParticleSystem::mTransformInstanceVBO = ParticleTransformInstanceVBO;


			glBindBuffer(GL_ARRAY_BUFFER, ParticleVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
			glBindVertexArray(ParticleVAO);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);

			glBindBuffer(GL_ARRAY_BUFFER, ParticleColorInstanceVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * MAX_PARTICLES, NULL, GL_STREAM_DRAW);

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (GLvoid*)0);
			glVertexAttribDivisor(1, 1);

			glBindBuffer(GL_ARRAY_BUFFER, ParticleTransformInstanceVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * MAX_PARTICLES, NULL, GL_STREAM_DRAW);

			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 4, (GLvoid*)0);
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 4, (GLvoid*)(sizeof(glm::vec4)));
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 4, (GLvoid*)(sizeof(glm::vec4) * 2));
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 4, (GLvoid*)(sizeof(glm::vec4) * 3));
			
			glVertexAttribDivisor(2, 1);
			glVertexAttribDivisor(3, 1);
			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

    /**************************************************************************/
    /*!
    \brief  Backs up the current OpenGL state.
    */
    /**************************************************************************/
    void GraphicsGL::BackupState()
    {
      glGetIntegerv(GL_CURRENT_PROGRAM, &GLState.lastProgram);
      glGetIntegerv(GL_TEXTURE_BINDING_2D, &GLState.lastTexture);
      glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &GLState.lastArrayBuffer);
      glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &GLState.lastElementArrayBuffer);
      glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &GLState.lastVertexArray);
      glGetIntegerv(GL_BLEND_SRC, &GLState.lastBlendSrc);
      glGetIntegerv(GL_BLEND_DST, &GLState.lastBlendDst);
      glGetIntegerv(GL_BLEND_EQUATION_RGB, &GLState.lastBlendEquationRGB);
      glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &GLState.lastBlendEquationAlpha);
      GLState.lastEnableBlend = glIsEnabled(GL_BLEND);
      GLState.lastEnableCullFace = glIsEnabled(GL_CULL_FACE);
      GLState.lastEnableDepthTest = glIsEnabled(GL_DEPTH_TEST);
      GLState.lastEnableScissorTest = glIsEnabled(GL_SCISSOR_TEST);
    }

    void GraphicsGL::RestoreState()
    {
      //glUseProgram(GLState.lastProgram);
      //glBindTexture(GL_TEXTURE_2D, GLState.lastTexture);
      //glBindBuffer(GL_ARRAY_BUFFER, GLState.lastArrayBuffer);
      //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GLState.lastElementArrayBuffer);
      //glBindVertexArray(GLState.lastVertexArray);
      //glBlendEquationSeparate(GLState.lastBlendEquationRGB, GLState.lastBlendEquationAlpha);
      //glBlendFunc(GLState.lastBlendSrc, GLState.lastBlendDst);
      //if (GLState.lastEnableBlend) glEnable(GL_BLEND); else glDisable(GL_BLEND);
      //if (GLState.lastEnableCullFace) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
      //if (GLState.lastEnableDepthTest) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
			GLState.lastEnableScissorTest = glIsEnabled(GL_SCISSOR_TEST);
      if (GLState.lastEnableScissorTest)
				glEnable(GL_SCISSOR_TEST);
			else
				glDisable(GL_SCISSOR_TEST);
    }


  }
}