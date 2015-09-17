#include "GLTextureTutorial.h"

// For loading images, generating textures
#include "..\..\Dependencies\include\SOIL2\SOIL2.h"

#define APPLY_TRANSFORM 1

namespace DCEngine {
  namespace Tutorial {

    void GLTextureTutorial::Initialize() {

      
      // 1. Build and compile the shader program
      shader.reset(new DCEngine::Shader("GLTexture.vs", "GLTexture.frag"));

      // 2. Set up vertex data, buffers and attrib pointers
      GenerateMesh();

      // 3. Load and create a texture
      GenerateTexture("Tutorials/OpenGL/container.jpg", texture1);
      GenerateTexture("Tutorials/OpenGL/awesomeface.png", texture2);

      // 4. Apply a transformation to the mesh
      if (APPLY_TRANSFORM)
        ApplyTransformation();

    }

    void GLTextureTutorial::GenerateMesh() {
      // We need to inform OpenGL how to sample the texture data so we'll have to update the 
      // vertex data with the texture coordinates.
      GLfloat vertices[] = {
        // Positions          // Colors           // Texture Coords
        0.5f,  0.5f, 0.0f,    1.0f, 0.0f, 0.0f,    1.0f, 1.0f,   // Top Right
        0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f,   // Bottom Right
        -0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f,   // Bottom Left
        -0.5f,  0.5f, 0.0f,    1.0f, 1.0f, 0.0f,    0.0f, 1.0f    // Top Left 
      };

      // A rectangle made from 2 triangles.
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
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
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
      // Since we have added an extra vertex attribute, we have to notify OpenGL of the new
      // vertex format. We have to adjust the stride parameter of the previous two vertex attribs.
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
      glEnableVertexAttribArray(2);
      // Unbind VAO
      glBindVertexArray(0);

    }

    void GLTextureTutorial::GenerateTexture(const std::string imagePath, GLuint& texture) {
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

    void GLTextureTutorial::ApplyTransformation() {
      // Create a transformation matrix by passing our identity matrix together with a
      // translation vector (the given matrix is then multiplied with a translation matrix
      // and the resulting matrix is returned)
      //trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
      // Multiply our vector by the transformation matrix
      //vec = trans * vec;
      //trace << "Matrix: (" << vec.x << ", " << vec.y << ", " << vec.z << ")\n";

      // Scale and rotate the container object
      // Rotate by 90 degrees CCW
      //transform = glm::rotate(transform, 90.0f, glm::vec3(0.0, 0.0, 1.0));
      //// Scale the container
      //transform = glm::scale(transform, glm::vec3(0.5, 0.5, 0.5));
      //trace << "Matrix: (" << vec.x << ", " << vec.y << ", " << vec.z << ")\n";

      /* (!) To get a transformation matrix to the shaders: GLSL has a mat4 type. We need to
      adapt the vertex shader to accept a mat4 uniform variable and multiply the
      position vector by the matrix uniform. */

      // Create transformations

      // Building the identity matrix each frame that it works
      transform = glm::mat4();

      // This will make the object rotate in the lower right corner
      transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
      transform = glm::rotate(transform, (GLfloat)glfwGetTime() * 50.0f,
        glm::vec3(0.0f, 0.0f, 1.0f));

      // This will make the object spin around the screen (switched the order) See Note #1
      transform = glm::rotate(transform, (GLfloat)glfwGetTime() * 50.0f, glm::vec3(0.0f, 0.0f, 1.0f));
      transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));

      /* Note #1: Why does our container now spin around our screen?:
      == ===================================================
      Remember that matrix multiplication is applied in reverse. This time a translation is thus
      applied first to the container positioning it in the bottom-right corner of the screen.
      After the translation the rotation is applied to the translated container.

      A rotation transformation is also known as a change-of-basis transformation
      for when we dig a bit deeper into linear algebra. Since we're changing the
      basis of the container, the next resulting translations will translate the container
      based on the new basis vectors. Once the vector is slightly rotated, the vertical
      translations would also be slightly translated for example.

      If we would first apply rotations then they'd resolve around the rotation origin (0,0,0), but
      since the container is first translated, its rotation origin is no longer (0,0,0) making it
      looks as if its circling around the origin of the scene. */





      // Get matrix's uniform location 
      GLuint transformLoc = glGetUniformLocation(shader->Get(), "transform");
      // First argument is the uniform location, second is how many matrixes we'd like
      // to send. Third asks us if we want to transpose the matrix, the fourth is the
      // actual matrix data. 
      // (!) GLM stores their matrices not how OpenGL wants them, so we transform them
      // with GLM's built-in 'value_ptr" 
      glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));



    }

    void GLTextureTutorial::Update() {
      /* We have to bind the texture before calling 'glDrawEelements' and it will automatically
         assign the texture fo the fragment shader's sampler

         The location of a texture is known as a 'texture unit'. The default texture unit
         for a texture is 0, which is the default active texture unit.

        The main purpose of texture units is t allow us to use more than 1 texture unit
        in our shaders. By assigning texture units to the samplers we can bind to multiple
        textures at once as long as we activate the corresponding texture unit first.

        OpenGL should have at least a minimum of 16 texture units for you to use.
        hey are defined in order so we could also get GL_TEXTURE8 via GL_TEXTURE0 + 8 for
        example, which is useful when we'd have to loop over several texture units.
         */


         /* Note that we're using glUniform1i to set the location or texture-unit of the uniform
            samplers. By setting them via glUniform1i we make sure each uniform sampler corresponds
            to the proper texture unit. You should get the following result: */

            // First texture
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture1);
      glUniform1i(glGetUniformLocation(shader->Get(), "ourTexture1"), 0);
      // Second texture
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, texture2);
      glUniform1i(glGetUniformLocation(shader->Get(), "ourTexture2"), 1);

      // The container will be twice as small and rotated 90 degrees (tilted to the left)
      if (APPLY_TRANSFORM)
        ApplyTransformation();



      // Activate shader
      shader->Use();

      // Draw container
      glBindVertexArray(VAO);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
      glBindVertexArray(0);
    }

    void GLTextureTutorial::Terminate() {
      // Properly de-allocate all resources once they've outlived their purpose
      glDeleteVertexArrays(1, &VAO);
      glDeleteBuffers(1, &VBO);
      glDeleteBuffers(1, &EBO);

    }

  }
}