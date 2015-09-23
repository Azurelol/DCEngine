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
      SimpleShader.reset(new Shader(std::string("SimpleShader"), "SimpleShader.vs", "SimpleShader.frag"));
      SetUpTest();
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
      glGenVertexArrays(1, &this->SpriteVAO);
      glGenBuffers(1, &SpriteVBO);

      glBindBuffer(GL_ARRAY_BUFFER, SpriteVBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
      
      glBindVertexArray(this->SpriteVAO);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
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
      SpriteShader->SetMatrix4("projection", camera.GetProjectionMatrix());
      // Set the view matrix
      SpriteShader->SetMatrix4("view", camera.GetViewMatrix());

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

      // Set the sprite shader as the current shader
      this->SpriteShader->Use();
      
      // The data to be used to draw the sprite:      
      auto transform = gameObj.getComponent<Transform>();
      auto sprite = gameObj.getComponent<Sprite>();

      // Create the matrix of the transform
      glm::mat4 objectToWorld;
      // Translate
      objectToWorld = glm::translate(objectToWorld, transform->Translation);
      // Translate
      objectToWorld = glm::translate(objectToWorld, glm::vec3(0.5f * transform->Scale.x,
                                              0.5f * transform->Scale.y,
                                              0.0f));
      // Rotate
      objectToWorld = glm::rotate(objectToWorld, (GLfloat)0, transform->Rotation);
      // Translate back
      objectToWorld = glm::translate(objectToWorld, glm::vec3(-0.5f * transform->Scale.x,
                                              -0.5f * transform->Scale.y,
                                              0.0f));
      // Scale
      objectToWorld = glm::scale(objectToWorld, glm::vec3(transform->Scale.x,
                                          transform->Scale.y,
                                          0.0f));

      // Update the uniforms in the shader to this particular sprite's data
      this->SpriteShader->SetMatrix4("model", objectToWorld);
      // Pass the color into the shader's uniform
      this->SpriteShader->SetVector3f("spriteColor", sprite->Color);
      // Set the active texture
      glActiveTexture(GL_TEXTURE0);
      // Bind the texture
      sprite->getSpriteSource()->getTexture().Bind();
      // Bind the vertex array
      glBindVertexArray(this->SpriteVAO);
      // Draw the array
      glDrawArrays(GL_TRIANGLES, 0, 6);
      // Unbind the vertex array
      glBindVertexArray(0);
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

