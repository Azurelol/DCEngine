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
      SpriteShader.reset(new Shader("SpriteShader.vs", "SpriteShader.frag"));
      // Configure the sprite shader VAO
      ConfigureSpriteVAO();
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
      GLuint VBO;
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
      glGenBuffers(1, &VBO);

      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
      
      glBindVertexArray(this->SpriteVAO);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
      glBindVertexArray(0);
    }


    /**************************************************************************/
    /*!
    \brief Sets the sprite shader once before drawing every sprite.
    \param A pointer to the graphics space.
    \note
    */
    /**************************************************************************/
    void GraphicsGL::SetSpriteShader(GraphicsSpace* gfxSpace) {
      GLfloat Near = -1.0f;
      GLfloat Far = 1.0f;

      auto cameraViewportRef = gfxSpace->Owner()->getComponent<CameraViewport>();
      auto camera = cameraViewportRef->getCamera();

      glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->screenwidth_),
        static_cast<GLfloat>(this->screenheight_),
        0.0f, Near, Far);
      SpriteShader->SetInteger("image", 0);
      SpriteShader->SetMatrix4("projection", projection);
                                    
    }

    /**************************************************************************/
    /*!
    \brief Draws a sprite on screen.
    \param A reference to the GameObject with the sprite component.
    \note  
    */
    /**************************************************************************/
    void GraphicsGL::DrawSprite(GameObject & gameObj) {
      //trace << "GraphicsGL::DrawSprite - Drawing " << gameObj.Name() << "\n";
      // Set the sprite shader as the current shader
      this->SpriteShader->Use();
      
      // The data to be used to draw the sprite:
      //auto transform = (Transform*)gameObj.getComponentByName("Transform");
      //auto sprite = (Sprite*)gameObj.getComponentByName("Transform");
      
      auto transform = gameObj.getComponent<Transform>();
      auto sprite = gameObj.getComponent<Sprite>();
      //trace << transform2->LocalScale << "\n";
      
      // Get references of the GameObject's sprite and transform components

      // Create the model matrix
      glm::mat4 model;
      // Translate
      model = glm::translate(model, transform->Translation);
      // Translate
      model = glm::translate(model, glm::vec3(0.5f * transform->Scale.x,
                                              0.5f * transform->Scale.y,
                                              0.0f));
      // Rotate
      model = glm::rotate(model, (GLfloat)0, transform->Rotation);
      // Translate back
      model = glm::translate(model, glm::vec3(-0.5f * transform->Scale.x,
                                              -0.5f * transform->Scale.y,
                                              0.0f));
      // Scale
      model = glm::scale(model, glm::vec3(transform->Scale.x,
                                          transform->Scale.y,
                                          0.0f));

      // Pass the model matrix into the shader
      this->SpriteShader->SetMatrix4("model", model);
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


    /**************************************************************************/
    /*!
    \brief  Terminates the system.
    */
    /**************************************************************************/
    void GraphicsGL::Terminate() {

    }



  }


}

