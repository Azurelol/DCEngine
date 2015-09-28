/******************************************************************************/
/*!
\file   GraphicsGL.cpp
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   7/30/2015
\brief  The 
*/
/******************************************************************************/
#include "GraphicsGL.h"


// (!) Should these be included? Perhaps only the data needed should be passed in.
#include "../../Components/Sprite.h"
#include "../../Components/Transform.h"
#include "../../Components/GraphicsSpace.h"
#include "../../Components/CameraViewport.h"
#include "../../Components/Camera.h"

#define DRAW_TEST 0

namespace DCEngine {
  namespace Systems {

    void GraphicsGL::BufferCleaner()
    {
   	  glm::mat4 cleanup;
	    this->SpriteShader->SetMatrix4("model", cleanup);	
	}

    /**************************************************************************/
    /*!
    \brief  The constructor for the Engine object.
    */
    /**************************************************************************/
    GraphicsGL::GraphicsGL() {      

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

      // If OpenGL failed to initialize...
      if (glewInit() != GLEW_OK) {
        trace << "Failed to initialize GLEW \n";
      }

      // Construct the Sprite shader
      SpriteShader.reset(new Shader(std::string("SpriteShader"), "SpriteShader.vs", "SpriteShader.frag"));
      // Configure the Sprite shader VAO
      ConfigureSpriteVAO();

      // Construct the SpriteText shader
      SpriteTextShader.reset(new Shader(std::string("SpriteTextShader"), "SpriteTextShader.vs", "SpriteTextShader.frag"));
      // Configure the SpriteText shader VAO
      ConfigureSpriteTextVAO();


    }

    /**************************************************************************/
    /*!
    \brief  Update function for the graphics system. Renders objects on screen.
    */
    /**************************************************************************/
    void GraphicsGL::Update(float dt) {

      // Tells OpenGL the current size of the rendering window
      glViewport(0, 0, screenwidth_, screenheight_);
    }

    /**************************************************************************/
    /*!
    \brief  Starts the current frame with OpenGL calls.
    */
    /**************************************************************************/
    void GraphicsGL::StartFrame() {
      glEnable(GL_DEPTH_TEST);
      glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
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
    void GraphicsGL::SetDebugDrawShaderProjViewUniform(Camera & camera)
    {

    }

    /**************************************************************************/
    /*!
    \brief  Takes a DrawDebug object and draws depending on.
    \param  A reference to the drawdebug object.
    \para   A reference to the space's camera.
    */
    /**************************************************************************/
    void GraphicsGL::DrawDebug(DebugDrawObject & debugDrawObj, Camera& cam)
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
    void GraphicsGL::DrawRectangle(Real3& pos, Real& width, Real& height, Real4& color, Camera& cam)
    {
		this->SpriteShader->SetVector4f("spriteColor", glm::vec4(color.r, color.g, color.b, 1.0));
		BufferCleaner();
		glBegin(GL_LINE_LOOP);

		//auto CameraMatrix = cam.GetProjectionMatrix() * cam.GetViewMatrix();
		auto PositionOrigin = glm::vec4(pos.x, pos.y, pos.z, 0.0);
		auto Position1 = PositionOrigin + glm::vec4(width / 2.0f, height / 2.0f, 0.0f, 0.0f);
		auto Position2 = PositionOrigin + glm::vec4(width / 2.0f, height / -2.0f, 0.0f, 0.0f);
		auto Position3 = PositionOrigin + glm::vec4(width / -2.0f, height / -2.0f, 0.0f, 0.0f);
		auto Position4 = PositionOrigin + glm::vec4(width / -2.0f, height / 2.0f, 0.0f, 0.0f);

		glVertex3f(Position1.x, Position1.y, Position1.z);
		glVertex3f(Position2.x, Position2.y, Position2.z);
		glVertex3f(Position3.x, Position3.y, Position3.z);
		glVertex3f(Position4.x, Position4.y, Position4.z);

		glEnd();
    }
    /**************************************************************************/
    /*!
    \brief Draws a circle on screen.
    \param The position of the center of the circle.
    \param The radius of the circle.
    \param The color of the circle.
    */
    /**************************************************************************/
    void GraphicsGL::DrawCircle(Real3& pos, Real& radius, Real4& color, Camera& cam)
    {
		// Do your magic here Chen
		//trace << "Drawing a circle\n";
		BufferCleaner();
    this->SpriteShader->SetVector4f("spriteColor", glm::vec4(color.r, color.g, color.b, 1.0));
		glBegin(GL_LINE_LOOP);

		double M_PI = 3.1415926535;
		static double PointsNumber = 128;
		for (double i = 0; i < 2 * M_PI; i = i + ((2 * M_PI) / PointsNumber))
		{
			auto PositionOrigin = glm::vec4(pos.x, pos.y, pos.z, 0.0f);
			//PositionOrigin = cam.GetProjectionMatrix() * cam.GetViewMatrix() * PositionOrigin;
			glm::vec4 Position = glm::vec4(PositionOrigin.x + radius * cos(i), PositionOrigin.y + radius * sin(i), PositionOrigin.z, 0.0);
			//trace << Position.x << " ," << Position.y << " ," << (camera.GetProjectionMatrix() * camera.GetViewMatrix())[3][2] << " ," << (camera.GetProjectionMatrix() * camera.GetViewMatrix())[3][3] << "\n";
			glVertex3f(Position.x, Position.y, Position.z);
		}

		glEnd();
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
    void GraphicsGL::DrawLineSegment(Real3& startPos, Real3& endPos, Real& length, Real4& color, Camera& cam)
    {
		this->SpriteShader->SetVector4f("spriteColor", glm::vec4(color.r, color.g, color.b, 1.0));
		BufferCleaner();
		glBegin(GL_LINE_LOOP);

		auto Position1 = glm::vec4(startPos.x, startPos.y, startPos.z, 0.0);
		auto Position2 = glm::vec4(endPos.x, endPos.y, endPos.z, 0.0);
		glVertex3f(Position1.x, Position1.y, Position1.z);
		glVertex3f(Position2.x, Position2.y, Position2.z);

		glEnd();
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
      GLuint VBO;
      GLfloat vertices[]{
        // Position,      Texture
        -1.0f, 1.0f,     0.0f, 1.0f,
        1.0f, 1.0f,      1.0f, 0.0f,
        1.0f, -1.0f,     0.0f, 0.0f,

        1.0f, -1.0f,      0.0f, 1.0f,
        -1.0f, -1.0f,     1.0f, 1.0f,
        -1.0f, 1.0f,      1.0f, 0.0f
      };

      /*
        Next, we simply send the vertices to the GPU and configure the vertex attributes,
        which in this case is a single vertex attribute.
      */
      glGenVertexArrays(1, &this->SpriteVAO);
      glGenBuffers(1, &VBO);

      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);      
      glBindVertexArray(this->SpriteVAO);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindVertexArray(0);
    }

    void GraphicsGL::ConfigureSpriteTextVAO()
    {
    }

    /**************************************************************************/
    /*!
    \brief Sets the shader's camera projection matrix and view uniforms.
    \param A (smart) pointer to the shader object.
    \param A reference to camera object.
    \note 
    */
    /**************************************************************************/
    void GraphicsGL::SetShaderProjViewUniforms(ShaderPtr shader, Camera& camera) {

      auto camTrans = camera.Owner()->getComponent<Transform>();

      // (???) Sets the "image" uniform to 0
      //SpriteShader->SetInteger("image", 0);
      // Set the projection matrix
      shader->SetMatrix4("projection", camera.GetProjectionMatrix());
      // Set the view matrix
      shader->SetMatrix4("view", camera.GetViewMatrix());
    }

    void GraphicsGL::SetSpriteShader(Camera& camera)
    {
      SetShaderProjViewUniforms(this->SpriteShader, camera);
    }

    void GraphicsGL::SetSpriteTextShader(Camera& camera)
    {
      SetShaderProjViewUniforms(this->SpriteTextShader, camera);
    }

    /**************************************************************************/
    /*!
    \brief Draws a sprite on screen.
    \param A reference to the GameObject with the sprite component.
    \note  
    */
    /**************************************************************************/
    void GraphicsGL::DrawSprite(GameObject& gameObj, Camera& camera) {
      //trace << "GraphicsGL::DrawSprite - Drawing " << gameObj.Name() << "\n";
      this->SpriteShader->Use();

      // Enable alpha blending for opacity.
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE);
      
      // Set the sprite shader as the current shader
      // The data to be used to draw the sprite:      
      auto transform = gameObj.getComponent<Transform>();
      auto sprite = gameObj.getComponent<Sprite>();

      // Create the matrix of the transform
      glm::mat4 modelMatrix;
      modelMatrix = glm::translate(modelMatrix, glm::vec3(transform->Translation.x,
                                                          transform->Translation.y, 
                                                          0.0f));
      //modelMatrix = glm::rotate(modelMatrix, transform->Rotation.y * 3.141592f / 180.0f, transform->Rotation);
      modelMatrix = glm::rotate(modelMatrix, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
      //objectToWorld = glm::translate(objectToWorld, glm::vec3(-0.5f * transform->Scale.x,
      //                                        -0.5f * transform->Scale.y,
      //                                        0.0f));
      modelMatrix = glm::scale(modelMatrix, glm::vec3(transform->Scale.x,
                                          transform->Scale.y,
                                          0.0f));
      
      glm::mat4x4 idMat;
      
      // Update the uniforms in the shader to this particular sprite's data 
      //UniformMatrix4fv(glGetUniformLocation(this->SpriteShader->Get(), "model"), 1, GL_FALSE, glm::value_ptr(idMat));
      this->SpriteShader->SetMatrix4("model", modelMatrix);
      this->SpriteShader->SetVector4f("spriteColor", sprite->Color);

      // Set the active texture
      glActiveTexture(GL_TEXTURE0); // Used for 3D
      sprite->getSpriteSource()->getTexture().Bind();

      this->SpriteShader->SetInteger("image", sprite->getSpriteSource()->getTexture().TextureID);
      // Bind the vertex array
      glBindVertexArray(this->SpriteVAO);
      // Draw the array
      glDrawArrays(GL_TRIANGLES, 0, 6);
      // Unbind the vertex array
      glBindVertexArray(0);

    }

    /**************************************************************************/
    /*!
    \brief Draws a SpriteText on screen.
    \param A reference to the SpriteText component.
    \param A reference to the space's viewport camera.
    \note
    */
    /**************************************************************************/
    void GraphicsGL::DrawSpriteText(SpriteText & st, Camera & camera)
    {
      
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

