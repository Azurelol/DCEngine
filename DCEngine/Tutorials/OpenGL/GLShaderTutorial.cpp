#include "GLShaderTutorial.h"


#define USE_SHADER_CLASS 1
#define UNIFORM_OFFSET 1

namespace DCEngine {
  namespace Tutorial {

    void GLShaderTutorial::Initialize() {

      trace << "GLShaderTutorial::Initialize \n";

#if(!USE_SHADER_CLASS)
      ////////////////////
      // SHADER PROGRAM
      ////////////////////

      // Build and compile our shader program
      //////////////////////////////////////////////////////////////////////////
      // The vertex shader has been written in the file "VertexShader.glsl"
      std::string vertexSource = GetGLSLCode("VertexShader2.glsl");
      const GLchar* vertexShaderSource = (GLchar*)vertexSource.c_str();

      vertexShader = glCreateShader(GL_VERTEX_SHADER);
      glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
      glCompileShader(vertexShader);

      // Check if the shader was compiled successfully
      AssertShaderCompilation(vertexShader, "Vertex shader");

      //////////////////////////////////////////////////////////////////////////
      // The fragment shader has been written in the file "FragmentShader.glsl"
      std::string fragmentSource = GetGLSLCode("FragmentShader_Uniform.glsl");
      const GLchar* fragmentShaderSource = (GLchar*)fragmentSource.c_str();

      fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
      glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
      glCompileShader(fragmentShader);

      // Check if the shader was compiled successfully
      AssertShaderCompilation(fragmentShader, "Fragment shader");

      //////////////////////////////////////////////////////////////////////////
      // Link shaders
      shaderProgram = glCreateProgram();
      glAttachShader(shaderProgram, vertexShader);
      glAttachShader(shaderProgram, fragmentShader);
      glLinkProgram(shaderProgram);

      // Check if the shader program linked successfully
      AssertShaderProgramLinking(shaderProgram, "Shader Program");

      glDeleteShader(vertexShader);
      glDeleteShader(fragmentShader);
#endif

#if(USE_SHADER_CLASS)
      //DCEngine::GLShader Shader = DCEngine::GLShader("VertexShader2.glsl", "FragmentShader_Uniform,glsl");
      shader.reset(new DCEngine::Shader(std::string("GLCamShader"), "VertexShader2.glsl", "FragmentShader_Uniform.glsl"));
      //DCEngine::GLShader
#endif



////////////////////////////////////////////
// VERTEX DATA, BUFFERS, ATTRIBUTE POINTERS
////////////////////////////////////////////

      GLfloat vertices[] = {
        // Positions         // Colors
        -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // Bottom Right
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // Bottom Left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // Top
      };

      glGenVertexArrays(1, &VAO);
      glGenBuffers(1, &VBO);
      glGenBuffers(1, &EBO);

      // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
      glBindVertexArray(VAO);

      // Array Buffer
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

      /* Because we added another vertex attribute and updated the VBO's memory, we have to
         re-configure the vertex attribute pointers. */

         // Position attributes
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
      glEnableVertexAttribArray(0);
      // Color attributes
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
      glEnableVertexAttribArray(1);


      glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

      glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO


    }

    void GLShaderTutorial::Update() {

      // Activate the shader
#if(!USE_SHADER_CLASS)
      glUseProgram(shaderProgram);
#endif    

#if(USE_SHADER_CLASS)
      shader->Use();
#endif

      /* We are using an uniform in the fragment shader. It is currently empty. We have to find the
         index/location of the uniform attribute in the shader, then update its values.
         Instead of passing a isngle color, we will gradually change the color over time. */

         // Retrieve the running time in seconds
      GLfloat timeValue = static_cast<float>(glfwGetTime());
      // Vary the color in the range of 0.0 - 1.0 by using the sin function
      GLfloat greenValue = static_cast<float>((sin(timeValue) / 2) + 0.5);
      // Query for the location, supplying a shader program and the name of the uniform

#if(!USE_SHADER_CLASS)
      GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
#endif    

#if(USE_SHADER_CLASS)
      GLint vertexColorLocation = glGetUniformLocation(shader->Get(), "ourColor");
#endif

#if(UNIFORM_OFFSET)
      GLfloat offset = -0.5f;
      glUniform1f(glGetUniformLocation(shader->Get(), "xOffset"), offset);
#endif


      // Finding an uniform location does not require you to use the shader program,
      // however updating the uniform does.
      glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

      /* (!) Because OpenGL is at its core a C library, it does not have native support for overloading,
        so whenever a function can be called with different types, new functions for each type
        are required. A set of possible postfixes: f, i, ui, 3f (3 floats), fv (float vector/array) */

      glBindVertexArray(VAO);
      glDrawArrays(GL_TRIANGLES, 0, 3);
      glBindVertexArray(0);

      trace << "GLShaderTutorial::Update - Drawing triangle \n";

    }

    void GLShaderTutorial::Terminate() {

      // Properly de-allocate all resources once they've outlived their purpose
      glDeleteVertexArrays(1, &VAO);
      glDeleteBuffers(1, &VBO);
      glDeleteBuffers(1, &EBO);

    }

  }
}