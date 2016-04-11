/******************************************************************************/
/*!
\file   GraphicsGL.cpp
\author Christian Sagel, William Mao
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
		GraphicsGL::GraphicsGL(GraphicsConfig& settings) : Settings(settings) {
		}

    /**************************************************************************/
		/*!
		\brief  The constructor for the Engine object.
		*/
		/**************************************************************************/
		void GraphicsGL::Initialize() {

			// GLEW manages function pointers for OpenGL, so we want to initialize
			// it before calling any OpenGL functions. Setting glewExperimental to
			// true uses more modern techniques for managing OpenGL functionality.
			glewExperimental = GL_TRUE;
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_MULTISAMPLE);
			// If OpenGL failed to initialize...
			if (glewInit() != GLEW_OK) {
				DCTrace << "Failed to initialize GLEW \n";
			}
      CompileShaders();
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
      //auto& viewportScale = Settings.ViewportScale;
			glViewport(0, 0, static_cast<GLsizei>(windowDim.x), //* viewportScale.x),
				static_cast<GLsizei>(windowDim.y)); //* viewportScale.y));
		}

		/**************************************************************************/
		/*!
		\brief  Starts the current frame with OpenGL calls.
		*/
		/**************************************************************************/
		void GraphicsGL::StartFrame() {
			glEnable(GL_DEPTH_TEST);
			ViewportUpdate();
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glStencilMask(~0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			glBindFramebuffer(GL_FRAMEBUFFER, FBO);
			GLuint attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(4, attachments);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			glBindFramebuffer(GL_FRAMEBUFFER, multisampleFBO);
			glDrawBuffer(GL_COLOR_ATTACHMENT0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
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

