#include "GLCameraTutorial.h"

// For loading images, generating textures
#include "..\..\Dependencies\include\SOIL2\SOIL2.h"

// Access to the engine for key_callback
#include "..\..\Core\Engine\Engine.h"
#include "..\..\Core\Objects\Entities\GameObject.h"

#define APPLY_PROJECTION 1 // If 0, edit the comments in the vertex shader
#define APPLY_TRANSFORM 1
#define DRAW_CUBES 1

// Variables for the key_callback function
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);



namespace DCEngine {
  
  // All components can access the engine.
  extern std::unique_ptr<Engine> Daisy;
  
  namespace Tutorial {

    void GLCameraTutorial::Initialize() {
      trace << "Tutorial::GLCameraTutorial - Initialized. \n";

      // 1. Build and compile the shader program
      shader.reset(new DCEngine::Shader("GLCameraTutorial.vs", "GLTexture.frag"));

      // 2. Set up vertex data, buffers and attrib pointers
      GenerateMesh();

      // 3. Load and create a texture
      GenerateTexture("Tutorials/OpenGL/container.jpg", texture1);
      GenerateTexture("Tutorials/OpenGL/aritmei.png", texture2);

      if (APPLY_PROJECTION)
        GenerateProjection();
      
      // Need access to the engine for input
      using Systems::Window;
      _window = GETSYSTEM(Window)->WindowHandler->GetWindow();      

      //CameraInitialize();
      cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
      cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
      cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

      // Connect to a LogicUpdate
      auto doll = (GameObject*)Owner();
      auto space = doll->GetSpace();
      Connect(space, Events::LogicUpdate, GLCameraTutorial::OnLogicUpdate);
      Connect(Daisy->GetKeyboard(), Events::KeyDown, GLCameraTutorial::OnKeyDown);
    }

    void GLCameraTutorial::OnKeyDown(Event* eventObj) {
      auto event = (Events::KeyDown*)(eventObj);

      trace << "GLCameraTutorial::OnKeyDown\n";

      // We will move in a direction depending on the speed
      if (event->Key == Keys::W)
        cameraPos += cameraSpeed * cameraFront;
      else if (event->Key == Keys::S)
        cameraPos -= cameraSpeed * cameraFront;
      
      if (event->Key == Keys::A)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
      else if (event->Key == Keys::D)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    }

    void GLCameraTutorial::OnLogicUpdate(Event* eventObj) {
      Update();
    }

    void GLCameraTutorial::GenerateMesh() {
      std::vector<GLfloat> vertices;

      // Vertices for a 3D cube
      if (DRAW_CUBES) {
        GLfloat verticesTemp[] = {
          -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
          0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
          0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
          0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
          -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
          -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

          -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
          0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
          0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
          0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
          -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
          -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

          -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
          -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
          -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
          -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
          -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
          -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

          0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
          0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
          0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
          0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
          0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
          0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

          -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
          0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
          0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
          0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
          -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
          -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

          -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
          0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
          0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
          0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
          -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
          -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
        };
        vertices.insert(vertices.end(), &verticesTemp[0],
          &verticesTemp[sizeof(verticesTemp) / sizeof(GLfloat)]);

        // Cube positions
        glm::vec3 cubePositionsTemp[]{
          glm::vec3(0.0f,  0.0f,  0.0f),
          glm::vec3(2.0f,  5.0f, -15.0f),
          glm::vec3(-1.5f, -2.2f, -2.5f),
          glm::vec3(-3.8f, -2.0f, -12.3f),
          glm::vec3(2.4f, -0.4f, -3.5f),
          glm::vec3(-1.7f,  3.0f, -7.5f),
          glm::vec3(1.3f, -2.0f, -2.5f),
          glm::vec3(1.5f,  2.0f, -2.5f),
          glm::vec3(1.5f,  0.2f, -1.5f),
          glm::vec3(-1.3f,  1.0f, -1.5f)
        };
        cubePositions.insert(cubePositions.end(), &cubePositionsTemp[0],
          &cubePositionsTemp[sizeof(cubePositionsTemp) / sizeof(glm::vec3)]);
      }
      // Vertices for a 2D cube
      else {
        GLfloat verticesTemp[] = {
          // Positions          // Colors           // Texture Coords
          0.5f,  0.5f, 0.0f,    1.0f, 0.0f, 0.0f,    1.0f, 1.0f,   // Top Right
          0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f,   // Bottom Right
          -0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f,   // Bottom Left
          -0.5f,  0.5f, 0.0f,    1.0f, 1.0f, 0.0f,    0.0f, 1.0f    // Top Left 
        };
        vertices.insert(vertices.end(), &verticesTemp[0],
          &verticesTemp[sizeof(verticesTemp) / sizeof(GLfloat)]);
      }

      GLuint indices[]{
        0, 1, 3, // First Triangle
        1, 2, 3  // Second Triangle
      };

      glGenVertexArrays(1, &VAO);
      glGenBuffers(1, &VBO);
      glGenBuffers(1, &EBO);

      // Bind the VAO
      glBindVertexArray(VAO);
      // Copy the vertices array into a buffer
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
      glBufferData(GL_ARRAY_BUFFER, vertices.capacity() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

      if (DRAW_CUBES) {
        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        // TexCoord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
      }
      else {
        // Copy the indeces array into an element buffer    
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        // Color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        // TexCoord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
      }

      // Unbind VAO
      glBindVertexArray(0);

    }

    void GLCameraTutorial::GenerateTexture(const std::string imagePath, GLuint& texture) {
      /* Loads an image in SOIL2 (Simple Open Image Library 2). We need the image's
      width and height for generating textures later on. The 4th argument
      specifies the number of channels the image has (left at 0). The fifth argument
      specifies howw it should load the image (we only want the RGB values) */
      //unsigned char* lol = SOIL_load_image("container.jpg", &width, &height, 0, SOIL_LOAD_RGB);
      image = SOIL_load_image(imagePath.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

      if (image == '\0') {
        trace << "Unable to load " << imagePath << "\n";
        return;
      }

      trace << "Loaded " << imagePath << "\n";

      /* The first argument specifies the number of textures we want to generate,
      the second where to store them in a GLuint array. (In our case just a single GLuint) */
      glGenTextures(1, &texture);

      // Like other objects we need to bind it so any subsequent texture commands will configure
      // the currently bound texture.
      glBindTexture(GL_TEXTURE_2D, texture);

      // (!) Now that the texture is bound, we can start generating a texture using the previously
      // loaded image data.

      // Set the texture wrapping parameters
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      // Set the texture filtering parameters
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      /* glTextImage2D(
      + The first argument specifies the texture target: setting this to GL_TEXTURE_2D means
      this operation will generate a texture on the currently bund texture object at the
      same target (so any textures bound to targets GL_TEXTURE_1D or GL_TEXTURE_3D will not be affected)
      + The second argument specifies the mipmap level for which we want to create a texture
      for if you want to set it manually. We'll leave it at base level (0).
      + The third argument tells OpenGL in what kidn of format we want to store the texture.
      + The 4th and 5th argument sets the width and height of the resulting texture.
      + The 6th argument should always be 0 (Legacy??)
      + The 7th and and 8th arguments specify the format and datatype of the source image.
      We loaded the image with RGB values and stored them as chars (bytes).
      + The 9th argument is the actual image data. */
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

      /* Once glTexImage2D is called, the currently bound texture object now has the texture image
      attached to it. If we want to use mimaps we have to specify all the different iamges
      manually or we could call 'glGenerateMipmap' after generating the texture. This will
      automatically generate all the required mipmaps. */
      glGenerateMipmap(GL_TEXTURE_2D);

      // It is good practice free the image memory and unbind the texture object
      SOIL_free_image_data(image);
      glBindTexture(GL_TEXTURE_2D, 0);
    }

    void GLCameraTutorial::GenerateProjection() {
      /* To start drawing in 3D, we'll first create a model matrix, which consists of translations,
         scaling and/or rotations we'd like to apply to transform all object's vertices to the
         global world space. */
         // Transform the plane by rotating it on the x-axis (so it looks like it's laying on the floor)
         // By multiplying the vetex coordinates with this model matrix, we're transforming the
         // vertex coordinates to world coordinates.
         //glm::mat4 model;
      model = glm::rotate(model, -55.0f, glm::vec3(1.0f, 0.0f, 0.0f));

      /* We need to create a view matrix. We want to move slightly backwards in the scene
         so the object becomes visible. With a view matrix, we move the entire scene inversed
         to where we want the amera to move. Because we want to move backwards and since OpenGL
         is a right-handed system we have to move in the positive z-axis. We do this by
         translating the scene towards the negative z-axis. */

         //glm::mat4 view;
         // Note that we're translating the scene in the reverse direction of where we want to move
      view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); // -3 in the z axis
      view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); // -3 in the z axis


      // Define the projection matrix. We want to use perspective projection
      //glm::mat4 projection;
      /* 'glmperspective' creates a large frustrum that defines the visible space
         - Fov: How large the viewspace is. For a realistic view, usually set at 45.0f.
         - Aspect ratio: Calculated by dividing the viewport's width by its height.
         - Near plane of the frustrum
         - Far plane of the frustrum
         (!) All vertices between the near and far plane and inside the frustrum will be
         rendered.

      */
      projection = glm::perspective(45.0f, screenWidth / screenHeight, 0.1f, 100.0f);

    }


    void GLCameraTutorial::UpdateViewMatrix() {
      double speed = 0.1;
      //view = glm::translate(view, glm::vec3(0.1f * speed, 0.0f * speed, 0.1f * speed)); // -3 in the z axis
      view = glm::rotate(view, -10.0f * static_cast<float>(speed), glm::vec3(1.0f, 0.0f, 0.0f));
    }

    void GLCameraTutorial::ApplyTexture() {
      // First texture
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture1);
      glUniform1i(glGetUniformLocation(shader->Get(), "ourTexture1"), 0);
      // Second texture
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, texture2);
      glUniform1i(glGetUniformLocation(shader->Get(), "ourTexture2"), 1);
    }


    void GLCameraTutorial::ApplyProjection() {
      // Send matrices to the shader. Done each render iteration.
      GLint modelLoc = glGetUniformLocation(shader->Get(), "model");
      GLint viewLoc = glGetUniformLocation(shader->Get(), "view");
      GLint projectionLoc = glGetUniformLocation(shader->Get(), "projection");

      //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

      glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
      glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

      if (DRAW_CUBES) {
        for (GLuint i = 0; i < 10; ++i) {
          glm::mat4 model;
          model = glm::translate(model, cubePositions[i]);
          GLfloat angle = 20.0f * i;
          model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
          glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

          glDrawArrays(GL_TRIANGLES, 0, 36);


        }
      }
      else {
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
      }
    }

    void GLCameraTutorial::ApplyTransformation(GLuint box) {
      // Building the identity matrix each frame that it works
      transform = glm::mat4();

      // This will make the object rotate in the lower right corner
      transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
      transform = glm::rotate(transform, (GLfloat)glfwGetTime() * 50.0f,
        glm::vec3(0.0f, 0.0f, 1.0f));

      // This will make the object spin around the screen (switched the order) See Note #1
      transform = glm::rotate(transform, (GLfloat)glfwGetTime() * 50.0f, glm::vec3(0.0f, 0.0f, 1.0f));
      transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));

      // Get matrix's uniform location 
      GLuint transformLoc = glGetUniformLocation(shader->Get(), "transform");
      glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

    }
    
    void GLCameraTutorial::CameraInitialize() {
      /* The view matrix transforms all of the world coordinates into view coordinates
         that are relative to the camera's position and direction.

         To define a camera we need:
         - Its position in world space
         - The direction it's looking at
         - A vector pointing to the right
         - A vector pointing upwards from the camera.

         We are actually going to create a coordinate system with 3 perpendicular unit axes
         with the camera's position as the origin. This p rocess is known as the
         Gram-Schmidth process in linear algebra. */

      

         // 1. Camera position
      cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
      cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
      // 2. Camera direction: By switching the substraction order we get a vector pointing 
      // towards the camera's +y-axis. (It should be probably named cameraReverseDireection??)
      cameraDirection = glm::normalize(cameraPos - cameraTarget);
      // 3. Right axis: A right vector represents the positive x-axis of the camera space.
      glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
      // Since the result of a cross product is a vector perpendicular to both vectors, 
      // we get a vector that points in the +x-axis. (If we switched the vetors, we'd get
      // a vector pointing in -x-axis.
      glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
      // 4. Up axis
      glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

      /* Using these camera vectors we can now create a LookAt matrix.
         If you define a coordinate space using 3 perpendicular you can create a matrix
         with those 3 axes plus a translation vector and you can transform any vector
         to that coordinate space by multiplying with this matrix.
         This is what the LookAt matrix does. */

      glm::mat4 view;
      // GLM already does this for us. We specify a camera position, target position
      // and a vector that represents the up vector in world space.
      view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));

      



    }

    void GLCameraTutorial::CameraUpdate() {
      /* Using trigonometry, create an x and z coordinate each frame that represent
       a point on a circle and use these for the camera position. By re-calculating
        the x and y coordinate we're traversing all the points in a circle and thus
        the camera rotates around the scene. */
      //GLfloat radius = 10.0f;
      //GLfloat camX = sin(glfwGetTime()) * radius;
      //GLfloat camZ = cos(glfwGetTime()) * radius;
      //view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0),
      //  glm::vec3(0.0, 1.0, 0.0));
      
      view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }

    void GLCameraTutorial::Update() {
      ApplyTexture();

      // Activate shader
      shader->Use();

      // Draw container
      glBindVertexArray(VAO);

      //UpdateViewMatrix();
      CameraUpdate();

      // Create transformations
      if (APPLY_PROJECTION) {
        ApplyProjection();
      }

      //if (APPLY_TRANSFORM) {
      //  ApplyTransformation();
      //}


      // When not drawing a cube
      if (!DRAW_CUBES)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

      glBindVertexArray(0);
    }

    void GLCameraTutorial::Terminate() {
      // Properly de-allocate all resources once they've outlived their purpose
      glDeleteVertexArrays(1, &VAO);
      glDeleteBuffers(1, &VBO);
      glDeleteBuffers(1, &EBO);
    }



  } // Tutorial
} // DCEngine