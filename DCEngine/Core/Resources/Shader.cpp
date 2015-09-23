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

  Shader::Shader(std::string& shaderName, std::string vertexPath, std::string fragmentPath) : Resource(shaderName) {
    trace << "\n" << _name << "::Shader - Constructor \n";
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
      trace << _name << "::Load - Vertex: " << vertexPath << " , Fragment: " << fragmentPath << "\n";
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
    }
    catch (std::ifstream::failure e) {
      if (TRACE_ON)
        trace << _name << "Shader::LoadShaders failed! \n";
    }    
  }

  /**************************************************************************/
  /*!
  \brief   Creates the shaders from their sources, then links them.
  */
  /**************************************************************************/
  void Shader::Compile() {
    if (TRACE_ON)
      trace << _name << "::Compile \n";
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
  Shader& Shader::Use() {
    if (TRACE_ON && TRACE_UPDATE)
      trace << _name << "::Use \n";

    glUseProgram(this->_shaderProgram);
    return *this;
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
      trace << _name << "::AssertShaderCompilation - " << shaderName << " compiled! \n";
    }
    else {
      glGetShaderInfoLog(shader, 512, NULL, infoLog);
      trace << _name << "::AssertShaderCompilation - " << shaderName << " failed to compile! \n"
        << infoLog << "\n";
    }
  }

  /**************************************************************************/
  /*!
  \brief  Checks if the shader program linked successfully.
  \param  The shader program.
  \param  A bool to decide whether to fire off the shader program.
  */
  /**************************************************************************/
  void Shader::AssertShaderProgramLinking(GLuint shaderProgram) {
    GLint success;       // Define an integer to indicate success
    GLchar infoLog[512]; // Container for the error messages (if any)

                         // Check if compilation was successful.
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (success) {
      trace << _name << "::AssertShaderCompilation - Shader program linked! \n";
    }
    else {
      glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
      trace << _name << "::AssertShaderCompilation - Shader program failed to link! \n"
        << infoLog << "\n";
    }

    trace << "\n";
  }

  /**************************************************************************/
  /*!
  \brief Utility functions to quickly set unfirom values.
  \param The name of the uniform
  \param The value(s) to set.
  \param A bool to decide whether to start using the shader program.
  */
  /**************************************************************************/
  void Shader::SetFloat(const GLchar * name, GLfloat value, GLboolean useShader) {
    if (useShader)
      this->Use();
    glUniform1f(glGetUniformLocation(this->ShaderID, name), value);
  }
  void Shader::SetInteger(const GLchar *name, GLint value, GLboolean useShader) {
    if (useShader)
      this->Use();
    glUniform1i(glGetUniformLocation(this->ShaderID, name), value);
  }
  void Shader::SetVector2f(const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader) {
    if (useShader)
      this->Use();
    glUniform2f(glGetUniformLocation(this->ShaderID, name), x, y);
  }
  void Shader::SetVector2f(const GLchar *name, const glm::vec2 &value, GLboolean useShader) {
    if (useShader)
      this->Use();
    glUniform2f(glGetUniformLocation(this->ShaderID, name), value.x, value.y);
  }
  void Shader::SetVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader) {
    if (useShader)
      this->Use();
    glUniform3f(glGetUniformLocation(this->ShaderID, name), x, y, z);
  }
  void Shader::SetVector3f(const GLchar *name, const glm::vec3 &value, GLboolean useShader) {
    if (useShader)
      this->Use();
    glUniform3f(glGetUniformLocation(this->ShaderID, name), value.x, value.y, value.z);
  }
  void Shader::SetVector4f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader) {
    if (useShader)
      this->Use();
    glUniform4f(glGetUniformLocation(this->ShaderID, name), x, y, z, w);
  }
  void Shader::SetVector4f(const GLchar *name, const glm::vec4 &value, GLboolean useShader) {
    if (useShader)
      this->Use();
    glUniform4f(glGetUniformLocation(this->ShaderID, name), value.x, value.y, value.z, value.w);
  }
  void Shader::SetMatrix4(const GLchar *name, const glm::mat4 &matrix, GLboolean useShader) {
    if (useShader)
      this->Use();
    glUniformMatrix4fv(glGetUniformLocation(this->ShaderID, name), 1, GL_FALSE, glm::value_ptr(matrix));
  }


}