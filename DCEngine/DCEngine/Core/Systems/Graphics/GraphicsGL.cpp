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

#include "../../Components/EngineReference.h"
#include "../../Components/Sprite.h"
#include "../../Components/SpriteText.h"
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
      glm::vec4 colorclean;
	    this->SpriteShader->SetMatrix4("model", cleanup);	
      this->SpriteShader->SetVector4f("color", colorclean);
      this->SpriteShader->SetInteger("image", 0);
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
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      // If OpenGL failed to initialize...
      if (glewInit() != GLEW_OK) {
        trace << "Failed to initialize GLEW \n";
      }

      trace << "\n[GraphicsGL::Initialize] - Compiling shaders \n";

      // Construct the Sprite shader
      SpriteShader = Daisy->getSystem<Content>()->getShader(std::string( "SpriteShader"));
      SpriteShader->Compile();
      // Configure the Sprite shader VAO
      ConfigureSpriteVAO();
      // Construct the SpriteText shader
      SpriteTextShader = Daisy->getSystem<Content>()->getShader(std::string("SpriteTextShader"));
      SpriteTextShader->Compile();
      // Configure the SpriteText shader VAO
      ConfigureSpriteTextVAO();

      trace << "[GraphicsGL::Initialize] - Finished compiling shaders \n";
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
        0.0f, 1.0f,      0.0f, 1.0f,
        1.0f, 0.0f,      1.0f, 0.0f,
        0.0f, 0.0f,      0.0f, 0.0f,

         0.0f, 1.0f,      0.0f, 1.0f,
         1.0f, 1.0f,      1.0f, 1.0f,
         1.0f, 0.0f,      1.0f, 0.0f
      };

      //GLfloat vertices[]{
      //  // Position,      Texture
      //  -1.0f, 1.0f,     0.0f, 1.0f,
      //  1.0f, 1.0f,      1.0f, 0.0f,
      //  1.0f, -1.0f,     0.0f, 0.0f,

      //  1.0f, -1.0f,      0.0f, 1.0f,
      //  -1.0f, -1.0f,     1.0f, 1.0f,
      //  -1.0f, 1.0f,      1.0f, 0.0f
      //};

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
      glBindVertexArray(SpriteTextVAO);
      glBindBuffer(GL_ARRAY_BUFFER, SpriteTextVBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindVertexArray(0);
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
      shader->SetInteger("image", 0);
      // Set the projection matrix
      shader->SetMatrix4("projection", camera.GetProjectionMatrix());
      // Set the view matrix
      shader->SetMatrix4("view", camera.GetViewMatrix());
    }

    void GraphicsGL::SetSpriteShader(Camera& camera)
    {
      SetShaderProjViewUniforms(this->SpriteShader, camera);
      // Enable alpha blending for opacity.

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
    void GraphicsGL::DrawSprite(Sprite& sprite, Camera& camera) {
      //trace << "GraphicsGL::DrawSprite - Drawing " << gameObj.Name() << "\n";
      //glEnable(GL_CULL_FACE);
      //glEnable(GL_BLEND);
      //glEnable(GL_TEXTURE_2D);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      this->SpriteShader->Use();
      
      // Retrieve the 'SpriteSource' resource from the content system
      auto spriteSrc = Daisy->getSystem<Content>()->getSpriteSrc(sprite.SpriteSource);
            
      // We use transform data for drawing:
      auto transform = sprite.TransformComponent;

      // Create the matrix of the transform
      GLfloat verticesOffset = 0.5f;
      glm::mat4 modelMatrix;


/*      modelMatrix = glm::translate(modelMatrix, glm::vec3(transform->Translation.x,
                                                          transform->Translation.y,
                                                          0.0f));
      modelMatrix = glm::translate(modelMatrix, glm::vec3(verticesOffset * sprite.TransformComponent->Scale.x,
                                                          verticesOffset * sprite.TransformComponent->Scale.y,
                                                          0.0f));
      modelMatrix = glm::rotate(modelMatrix, transform->Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
      modelMatrix = glm::rotate(modelMatrix, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
      modelMatrix = glm::translate(modelMatrix, glm::vec3(-verticesOffset * sprite.TransformComponent->Scale.x,
                                                          -verticesOffset * sprite.TransformComponent->Scale.y,
                                                          0.0f));
      modelMatrix = glm::scale(modelMatrix, glm::vec3(transform->Scale.x * 1.5, transform->Scale.y * 1.5, 0.0f))*/;

      modelMatrix = glm::translate(modelMatrix, glm::vec3(transform->Translation.x,
                                                          transform->Translation.y, 
                                                          0.0f));
      modelMatrix = glm::rotate(modelMatrix, transform->Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
      modelMatrix = glm::rotate(modelMatrix, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
      modelMatrix = glm::scale(modelMatrix, glm::vec3(transform->Scale.x,
                                          transform->Scale.y,
                                          0.0f));
      
      
      // Update the uniforms in the shader to this particular sprite's data 
      this->SpriteShader->SetMatrix4("model", modelMatrix);
      this->SpriteShader->SetVector4f("spriteColor", sprite.Color);

      //if (&spriteSrc->getTexture() == NULL) {
      //  this->SpriteShader->SetInteger("IsTexture", 0);
      //}
      //else {
      //  this->SpriteShader->SetInteger("IsTexture", 1);
      //  glActiveTexture(GL_TEXTURE0); // Used for 3D
      //  spriteSrc->getTexture().Bind();
      //}

      // Set the active texture
      glActiveTexture(GL_TEXTURE0); // Used for 3D
      spriteSrc->getTexture().Bind();

      //this->SpriteShader->SetInteger("image", spriteSrc->getTexture().TextureID); // WHAT DO?
      // Bind the vertex array
      glBindVertexArray(this->SpriteVAO);
      // Draw the array
      glDrawArrays(GL_TRIANGLES, 0, 6);
      // Unbind the vertex array
      glBindVertexArray(0);

    }

    /**************************************************************************/
    /*!
    @brief Draws a SpriteText on screen.
    @param A reference to the SpriteText component.
    @param A reference to the space's viewport camera.
    @note  To render each character, we extract the corresponding Character
           struct from the Characters map and calculate the quad dimensions
           using the character's metrics. 
    */
    /**************************************************************************/
    void GraphicsGL::DrawSpriteText(SpriteText & st, Camera & camera)
    {
      // Enable alpha blending for opacity.
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      // Activate the SpriteText shader
      SpriteTextShader->Use();
      SpriteTextShader->SetVector4f("textColor", st.Color);
      glActiveTexture(GL_TEXTURE0);
      glBindVertexArray(SpriteTextVAO);

      // Retrieve the Font resource from the content system
      auto font = Daisy->getSystem<Content>()->getFont(st.Font);
      
      // (!) This is used to advance cursoes
      GLfloat x = static_cast<GLfloat>(st.TransformComponent->Translation.x);

      // Iterate through all the characters
      for (auto c : st.Text) {
        // Access a character glyph from the characters map
        Character ch = font->Characters[c];

        // Calculate the origin position of the quad 
        GLfloat xPos = x + ch.Bearing.x * st.FontSize;
        GLfloat yPos = st.TransformComponent->Translation.y - (ch.Size.y - ch.Bearing.y) * st.FontSize;
        // Calculate the quad's size
        GLfloat w = ch.Size.x * static_cast<GLfloat>(st.FontSize);
        GLfloat h = ch.Size.y * static_cast<GLfloat>(st.FontSize);
        // Generate a set of 6 vertices to form the 2D quad
        GLfloat vertices[6][4] = {
          { xPos    , yPos + h, 0.0, 0.0 },
          { xPos    , yPos    , 0.0, 1.0 },
          { xPos + w, yPos    , 1.0, 1.0 },

          { xPos    , yPos + h, 0.0, 0.0 },
          { xPos + w, yPos    , 1.0, 1.0 },
          { xPos + w, yPos + h, 1.0, 1.0 },
        };

        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.CharacterTextureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, SpriteTextVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Advance cursors for next glyph (Advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * st.FontSize;
      }

      // Unbind
      glBindVertexArray(0);
      glBindTexture(GL_TEXTURE_2D, 0);      
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

