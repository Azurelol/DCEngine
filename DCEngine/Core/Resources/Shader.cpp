/******************************************************************************/
/*!
@file   Shader.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   8/13/2015
@brief  An encapsulated object that reads shaders from disk, compiles, links them,
        checks for errors and other useful functions.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved
@todo   Should shaders be a resource?
*/
/******************************************************************************/

#include "Shader.h"
#include "../Debug/DebugGraphics.h"

#include <fstream>
#include <sstream>

namespace DCEngine {

  Shader::Shader(std::string& shaderName, std::string vertexPath, std::string fragmentPath) 
                 : Resource("Shader", shaderName, "None"), VertexPath(vertexPath), FragmentPath(fragmentPath) {
    if (TRACE_CONSTRUCTOR)
      DCTrace << "\n" << ObjectName << "::Shader - Constructor \n";


    // Loads the shaders
    Load();
    // Compiles the shader
    //Compile();
  }

  /**************************************************************************/
  /*!
  \brief   Loads the shaders from their source files, then stores them.
  \param   The path of the vertex shader.
  \param   The path of the fragment shader.
  */
  /**************************************************************************/
  bool Shader::Load() {
    //if (TRACE_ON)
    //  DCTrace << ObjectName << "::Load - Vertex: " << VertexPath << " , Fragment: " << FragmentPath << "\n";
    std::ifstream vertexShaderFile;
    std::ifstream fragmentShaderFile;

    // Ensures ifstream objects can throw exceptions
    vertexShaderFile.exceptions(std::ifstream::badbit);
    fragmentShaderFile.exceptions(std::ifstream::badbit);

    try {
      //std::string shaderLocation("Core/Resources/Shaders/");
      //vertexShaderFile.open(shaderLocation + VertexPath);
      //fragmentShaderFile.open(shaderLocation + FragmentPath);
      vertexShaderFile.open(VertexPath);
      fragmentShaderFile.open(FragmentPath);
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
        return false;
        DCTrace << ObjectName << "Shader::LoadShaders failed! \n";
    }
    return true;
  }

  /**************************************************************************/
  /*!
  \brief   Creates the shaders from their sources, then links them.
  */
  /**************************************************************************/
  void Shader::Compile() {
    if (TRACE_ON)
      DCTrace << ObjectName << "::Compile \n";
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
    this->ShaderProgramID = glCreateProgram();
    glAttachShader(this->ShaderProgramID, vertex);
    glAttachShader(this->ShaderProgramID, fragment);
    glLinkProgram(this->ShaderProgramID);
    AssertShaderProgramLinking(this->ShaderProgramID);
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
      DCTrace << ObjectName << "::Use \n";

    glUseProgram(this->ShaderProgramID);
    return *this;
  }

  Shader & Shader::Unbind()
  {
    glUseProgram(0);
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
      DCTrace << ObjectName << "::AssertShaderCompilation - " << shaderName << " compiled! \n";
    }
    else {
      glGetShaderInfoLog(shader, 512, NULL, infoLog);
      DCTrace << ObjectName << "::AssertShaderCompilation - " << shaderName << " failed to compile! \n"
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
      DCTrace << ObjectName << "::AssertShaderCompilation - Shader program linked! \n";
    }
    else {
      glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
      DCTrace << ObjectName << "::AssertShaderCompilation - Shader program failed to link! \n"
        << infoLog << "\n";
    }

    //DCTrace << "\n";
  }

  /**************************************************************************/
  /*!
  \brief Searches for and adds the uniform to the shader's map. This map
         is used later by the template UpdateUniforms function.
  \param The name of the uniform
  \param A bool to decide whether to start using the shader program.
  */
  /**************************************************************************/
  bool Shader::AddUniform(const std::string & uniformName)
  {
    // Searches for the target uniform in the shader
    ShaderUniformsMap[uniformName] = glGetUniformLocation(ShaderProgramID, uniformName.c_str());

    if (ShaderUniformsMap[uniformName] == -1) {
      DCTrace << ObjectName << "::AddUniform - Failed to find the uniform: " << uniformName << "\n";
      return false;
    }

    return true;
  }

  /**************************************************************************/
  /*!
  \brief Utility functions to send shader uniform data to the GPU.
  \param The location of the handle of the shader.
  \param The uniform data.
  */
  /**************************************************************************/
  void Shader::LoadShaderUniform(const GLint location, const GLint & data)
  {
    glUniform1i(location, data);
  }

  void Shader::LoadShaderUniform(const GLint location, const GLuint & data)
  {
    glUniform1ui(location, data);
  }

  void Shader::LoadShaderUniform(const GLint location, const GLfloat & data)
  {
    glUniform1f(location, data);
  }

  void Shader::LoadShaderUniform(const GLint location, const glm::vec2 & data)
  {
    // glm::value_ptr helps data transfer over to the GPU.
    glUniform2fv(location, 1, glm::value_ptr(data));
  }

  void Shader::LoadShaderUniform(const GLint location, const glm::vec3 & data)
  {
    glUniform3fv(location, 1, glm::value_ptr(data));
  }

  void Shader::LoadShaderUniform(const GLint location, const glm::vec4 & data)
  {
    glUniform4fv(location, 1, glm::value_ptr(data));
  }

  void Shader::LoadShaderUniform(const GLint location, const glm::mat3x3 & data)
  {
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(data));
  }

  void Shader::LoadShaderUniform(const GLint location, const glm::mat4x4 & data)
  {
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(data));
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
    glUniform1f(glGetUniformLocation(this->ShaderProgramID, name), value);
  }
  void Shader::SetInteger(const GLchar *name, GLint value, GLboolean useShader) {
    if (useShader)
      this->Use();
    glUniform1i(glGetUniformLocation(this->ShaderProgramID, name), value);
  }
  void Shader::SetVector2f(const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader) {
    if (useShader)
      this->Use();
    glUniform2f(glGetUniformLocation(this->ShaderProgramID, name), x, y);
  }
  void Shader::SetVector2f(const GLchar *name, const glm::vec2 &value, GLboolean useShader) {
    if (useShader)
      this->Use();
    glUniform2f(glGetUniformLocation(this->ShaderProgramID, name), value.x, value.y);
  }
  void Shader::SetVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader) {
    if (useShader)
      this->Use();
    glUniform3f(glGetUniformLocation(this->ShaderProgramID, name), x, y, z);
  }
  void Shader::SetVector3f(const GLchar *name, const glm::vec3 &value, GLboolean useShader) {
    if (useShader)
      this->Use();
    glUniform3f(glGetUniformLocation(this->ShaderProgramID, name), value.x, value.y, value.z);
  }
  void Shader::SetVector4f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader) {
    if (useShader)
      this->Use();
    glUniform4f(glGetUniformLocation(this->ShaderProgramID, name), x, y, z, w);
  }
  void Shader::SetVector4f(const GLchar *name, const glm::vec4 &value, GLboolean useShader) {
    if (useShader)
      this->Use();
    glUniform4f(glGetUniformLocation(this->ShaderProgramID, name), value.x, value.y, value.z, value.w);
  }
  void Shader::SetMatrix4(const GLchar *name, const glm::mat4 &matrix, GLboolean useShader) {
    if (useShader)
      this->Use();
    glUniformMatrix4fv(glGetUniformLocation(this->ShaderProgramID, name), 1, GL_FALSE, glm::value_ptr(matrix));
  }


}