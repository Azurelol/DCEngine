/******************************************************************************/
/*!
\file   GraphicsGL.cpp
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   7/30/2015
\brief  The

@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "GraphicsGL.h"

#include "../../Debug/DebugGraphics.h"
// (!) Should these be included? Perhaps only the data needed should be passed in.
#include "../../Components/EngineReference.h"
#include "../../Components/Sprite.h"
#include "../../Components/SpriteText.h"
#include "../../Components/Transform.h"
#include "../../Components/CameraViewport.h"
#include "../../Components/Camera.h"

namespace DCEngine {
	namespace Systems {

		/**************************************************************************/
		/*!
		\brief  The constructor for the Engine object.
		*/
		/**************************************************************************/
		GraphicsGL::GraphicsGL() {
      ViewportRatio.x = 1;
      ViewportRatio.y = 1;
		}

		/**************************************************************************/
		/*!
		\brief  The constructor for the Engine object.
		*/
		/**************************************************************************/
		void GraphicsGL::Initialize() {

			// Set a pointer to the Window system

			// GLEW manages function pointers for OpenGL, so we want to initialize
			// it before calling any OpenGL functions. Setting glewExperimental to
			// true uses more modern techniques for managing OpenGL functionality.
			glewExperimental = GL_TRUE;
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			// If OpenGL failed to initialize...
			if (glewInit() != GLEW_OK) {
				DCTrace << "Failed to initialize GLEW \n";
			}

			DCTrace << "\n[GraphicsGL::Initialize] - Compiling shaders \n";

			// Grab a reference to the Sprite shader
			SpriteShader = Daisy->getSystem<Content>()->getShader("SpriteShader");
			SpriteShader->Compile();
			Components::Sprite::mShader = SpriteShader;
			ConfigureSpriteVAO();
			// Construct the SpriteText shader
			SpriteTextShader = Daisy->getSystem<Content>()->getShader(std::string("SpriteTextShader"));
			SpriteTextShader->Compile();
			Components::SpriteText::mShader = SpriteTextShader;
			ConfigureSpriteTextVAO();
			// Construct the ParticleSystem shader
			ParticleSystemShader = Daisy->getSystem<Content>()->getShader(std::string("ParticleShader"));
			ParticleSystemShader->Compile();
			Components::SpriteParticleSystem::mShader = ParticleSystemShader;
			ConfigureParticleBuffers();
			DCTrace << "[GraphicsGL::Initialize] - Finished compiling shaders \n";
		}

		/**************************************************************************/
		/*!
		@brief  Update function for the graphics system. Renders objects on screen.
		@todo   OPTIMIZATION: Possibly do not need to call it twice?
		*/
		/**************************************************************************/
		void GraphicsGL::ViewportUpdate() {
			// Tells OpenGL the current size of the rendering window
			auto windowDim = Daisy->getSystem<Window>()->getWindowDimensions();
      auto& viewportScale = Daisy->getSystem<Graphics>()->ViewportScale;
			glViewport(0, 0, static_cast<GLsizei>(windowDim.x * viewportScale.x),
                       static_cast<GLsizei>(windowDim.y * viewportScale.y));
		}

    /**************************************************************************/
    /*!
    @brief  Scales the viewport down to a percentage of its max value.
    @param  ratio The percentage to which to scale the viewport
    */
    /**************************************************************************/
    void GraphicsGL::ResizeViewport(glm::vec2 ratio)
    {
      ViewportRatio = ratio;
    }

		/**************************************************************************/
		/*!
		\brief  Starts the current frame with OpenGL calls.
		*/
		/**************************************************************************/
		void GraphicsGL::StartFrame() {
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
			ViewportUpdate();
			glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
			//glClearColor(ClearColor.r, ClearColor.g, ClearColor.b, ClearColor.a);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		/**************************************************************************/
		/*!
		\brief  Ends the current frame.
		*/
		/**************************************************************************/
		void GraphicsGL::EndFrame() {


		}    

    /**************************************************************************/
    /*!
    \brief  Terminates the system.
    */
    /**************************************************************************/
    void GraphicsGL::Terminate() {

    }

	}
}

