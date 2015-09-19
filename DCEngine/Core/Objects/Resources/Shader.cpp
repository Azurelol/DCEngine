/******************************************************************************/
/*!
\file   GLShader.cpp
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/13/2015
\brief  An encapsulated object that reads shaders from disk, compiles, links them,
checks for errors and other useful functions.
*/
/******************************************************************************/
#include "Shader.h"

#include <fstream>
#include <sstream>

namespace DCEngine {

  Shader::Shader(std::string vertexPath, std::string fragmentPath) : Resource("Shader") {
    trace << "Shader::Shader() \n";
    // Loads the shaders
    Load(vertexPath, fragmentPath);
    // Compiles the shader
    Compile();
  }

  /**************************************************************************/
  /*!
  \brief   Loads the shaders from their source files, then stores them.
  \param   The path of the vertex shader.
  \param   The path of the fragment shader.
  */
  /**************************************************************************/
  void Shader::Load(std::string vertexPath, std::string fragmentPath) {
    if (TRACE_ON)
      trace << "Shader::Load - Vertex: " << vertexPath << " , Fragment: " << fragmentPath << "\n";
    std::ifstream vertexShaderFile;
    std::ifstream fragmentShaderFile;

    // Ensures ifstream objects can throw exceptions
    vertexShaderFile.exceptions(std::ifstream::badbit);
    fragmentShaderFile.exceptions(std::ifstream::badbit);

    try {
      // Open the files ( YES I KNOW THIS WAY IS DUMB PLZ HELP )
      std::string shaderLocation("Core/Resources/Shaders/");
      vertexShaderFile.open(shaderLocation + vertexPath);
      fragmentShaderFile.open(shaderLocation + fragmentPath);
      // Use C++ filestreams to read the content from the file
      std::stringstream vertexShaderStream, fragmentShaderStream;
      vertexShaderStream << vertexShaderFile.rdbuf();
      fragmentShaderStream << fragmentShaderFile.rdbuf();
      // Close the files
      vertexShaderFile.close();
      fragmentShaderFile.close();
      // Store the shader code within the object   
      vertexCode = vertexShaderStream.str();
      fragmentCode = fragmentShaderStream.str();  
      trace << "lol\n";
    }
    catch (std::ifstream::failure e) {
      if (TRACE_ON)
        trace << "Shader::LoadShaders failed! \n";
    }    
  }

  /**************************************************************************/
  /*!
  \brief   Creates the shaders from their sources, then links them.
  */
  /**************************************************************************/
  void Shader::Compile() {
    if (TRACE_ON)
      trace << "Shader::Compile \n";
    GLuint vertex, fragment;   
    
    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* vShaderCode = vertexCode.c_str();
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    AssertShaderCompilation(vertex, "Vertex Shader");
    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar* fShaderCode = fragmentCode.c_str();
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    AssertShaderCompilation(fragment, "Fragment Shader");
    // Link shader program
    this->_shaderProgram = glCreateProgram();
    glAttachShader(this->_shaderProgram, vertex);
    glAttachShader(this->_shaderProgram, fragment);
    glLinkProgram(this->_shaderProgram);
    AssertShaderProgramLinking(this->_shaderProgram);
    // Delete the shaders as they're now linked into the program and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);

  }

  /**************************************************************************/
  /*!
  \brief   Activates the shader program.
  */
  /**************************************************************************/
  void Shader::Use() {
    if (TRACE_ON && TRACE_UPDATE)
      trace << "Shader::Use \n";

    glUseProgram(this->_shaderProgram);
  }

  /**************************************************************************/
  /*!
  \brief  Checks if the shader was compiled successfully.
  \param   The shader.
  \param   The name of the shader.
  */
  /**************************************************************************/
  void Shader::AssertShaderCompilation(GLuint shader, std::string shaderName) {
    GLint success;       // Define an integer to indicate success
    GLchar infoLog[512]; // Container for the error messages (if any)

                         // Check if compilation was successful.
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success) {
      trace << "Shader::AssertShaderCompilation - " << shaderName << " compiled! \n";
    }
    else {
      glGetShaderInfoLog(shader, 512, NULL, infoLog);
      trace << "Shader::AssertShaderCompilation - " << shaderName << " failed to compile! \n"
        << infoLog << "\n";
    }
  }

  /**************************************************************************/
  /*!
  \brief  Checks if the shader program linked successfully.
  \param   The shader program.
  */
  /**************************************************************************/
  void Shader::AssertShaderProgramLinking(GLuint shaderProgram) {
    GLint success;       // Define an integer to indicate success
    GLchar infoLog[512]; // Container for the error messages (if any)

                         // Check if compilation was successful.
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (success) {
      trace << "Shader::AssertShaderCompilation - Shader program linked! \n";
    }
    else {
      glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
      trace << "Shader::AssertShaderCompilation - Shader program failed to link! \n"
        << infoLog << "\n";
    }
  }


}