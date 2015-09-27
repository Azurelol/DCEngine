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

      // Construct the sprite shader
      SpriteShader.reset(new Shader(std::string("SpriteShader"), "SpriteShader.vs", "SpriteShader.frag"));
      // Configure the sprite shader VAO
      ConfigureSpriteVAO();
      
      // TESTING
      
      if (DRAW_TEST) {
        SimpleShader.reset(new Shader(std::string("SimpleShader"), "SimpleShader.vs", "SimpleShader.frag"));
        SetUpTest();
      }
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
      //glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
      glClearColor(ClearColor.x, ClearColor.y, ClearColor.z, ClearColor.w);
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
    void GraphicsGL::DrawRectangle(Real3 pos, Real width, Real height, Real4 color, Camera& cam)
    {
      
    }
    /**************************************************************************/
    /*!
    \brief Draws a circle on screen.
    \param The position of the center of the circle.
    \param The radius of the circle.
    \param The color of the circle.
    */
    /**************************************************************************/
    void GraphicsGL::DrawCircle(Real3 pos, Real radius, Real4 color, Camera& cam)
    {
      // Do your magic here Chen
      trace << "Drawing a circle\n";
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
    void GraphicsGL::DrawLineSegment(Real3 startPos, Real3 endPos, Real length, Real4 color, Camera& cam)
    {

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


    /**************************************************************************/
    /*!
    \brief Sets the shader's camera projection matrix once before drawing
           everything.            
    \param A pointer to camera object.
    \note
    */
    /**************************************************************************/
    void GraphicsGL::SetShaderProjectionUniform(Camera& camera) {

      auto camTrans = camera.Owner()->getComponent<Transform>();

      // (???) Sets the "image" uniform to 0
      SpriteShader->SetInteger("image", 0);
      // Set the projection matrix      
      
     // glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 1.0f, 10.0f);
      glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(800),
        static_cast<GLfloat>(600), 0.0f, -1.0f, 1.0f);

      this->SpriteShader->SetMatrix4("projection", camera.GetProjectionMatrix());
      //this->SpriteShader->SetMatrix4("projection", projection);
      auto projM = camera.GetProjectionMatrix();
      //glm::mat4 view = glm::lookAt(
      //  glm::vec3(1.2f, 1.2f, 1.2f),
      //  glm::vec3(0.0f, 0.0f, 0.0f),
      //  glm::vec3(0.0f, 0.0f, 1.0f)
      //  );

      // Set the view matrix
      this->SpriteShader->SetMatrix4("view", camera.GetViewMatrix());
      //this->SpriteShader->SetMatrix4("view", view);
      auto viewM = camera.GetViewMatrix();

      //trace << "lol";

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
      modelMatrix = glm::rotate(modelMatrix, 0.0f, Real3(0.0f, 0.0f, 1.0f));
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

      //this->SpriteShader->SetInteger("image", sprite->getSpriteSource()->getTexture().TextureID);
      // Bind the vertex array
      glBindVertexArray(this->SpriteVAO);
      // Draw the array
      glDrawArrays(GL_TRIANGLES, 0, 6);
      // Unbind the vertex array
      glBindVertexArray(0);

      if (DRAW_TEST) {
        DrawTest();
      }
    }


    void GraphicsGL::SetUpTest()
    {
      /* 1. CONFIGURE VAO */
      GLfloat vertices[]{
        // Position,  Texture
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
      };

      /*
      Next, we simply send the vertices to the GPU and configure the vertex attributes,
      which in this case is a single vertex attribute.
      */
      glGenVertexArrays(1, &testVAO);
      glGenBuffers(1, &testVBO);

      glBindBuffer(GL_ARRAY_BUFFER, testVBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

      glBindVertexArray(testVAO);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
      glBindVertexArray(0);
    }

    void GraphicsGL::DrawTest()
    {        
      // Send matrixes to the shader

      // Draw our first triangle
      this->SimpleShader->Use();
      glBindVertexArray(testVAO);
      glDrawArrays(GL_TRIANGLES, 0, 6);
      // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
      glBindVertexArray(0);
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

