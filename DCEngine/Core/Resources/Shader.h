/******************************************************************************/
/*!
\file   Shader.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/13/2015
\brief  An encapsulated object that reads shaders from disk, compiles, links them,
        checks for errors and other useful functions.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include "ResourceReference.h"
#include "..\Engine\Types.h"

#include <unordered_map>

namespace DCEngine {

  class Shader : public Resource {
  public:
    
    Shader(std::string& shaderName, std::string vertexPath, std::string fragmentPath, std::string geometryPath = "");
    
    bool Load();
    std::string Extension() { return ".Shader"; }
    void Compile();    
    Shader& Use(); 
    Shader& Unbind();
    GLuint Get() { return ShaderProgramID; }
    
    /* Uniform testing */
    bool AddUniform(const std::string& uniformName);
    template <typename T> void UpdateUniforms(const std::string& uniformName, const T& data);    
    void LoadShaderUniform(const GLint location, const GLint &data);
    void LoadShaderUniform(const GLint location, const GLuint &data);
    void LoadShaderUniform(const GLint location, const GLfloat &data);
    void LoadShaderUniform(const GLint location, const glm::vec2 &data);
    void LoadShaderUniform(const GLint location, const glm::vec3 &data);
    void LoadShaderUniform(const GLint location, const glm::vec4 &data);
    void LoadShaderUniform(const GLint location, const glm::mat3x3 &data);
    void LoadShaderUniform(const GLint location, const glm::mat4x4 &data);

    /* (Soon-to-be deprecated) utility functions */
    void    SetFloat(const GLchar *name, GLfloat value, GLboolean useShader = false);
    void    SetInteger(const GLchar *name, GLint value, GLboolean useShader = false);
    void    SetVector2f(const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader = false);
    void    SetVector2f(const GLchar *name, const glm::vec2 &value, GLboolean useShader = false);
    void    SetVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = false);
    void    SetVector3f(const GLchar *name, const glm::vec3 &value, GLboolean useShader = false);
    void    SetVector4f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = false);
    void    SetVector4f(const GLchar *name, const glm::vec4 &value, GLboolean useShader = false);
    void    SetMatrix4(const GLchar *name, const glm::mat4 &matrix, GLboolean useShader = false);

  private:

    std::string VertexPath; //!< The path of the vertex shader file.
    std::string FragmentPath; //!< The path of the fragment shader file.
		std::string GeometryPath;
    std::string vertexCode;  //!< The raw vertex shader code in GLSL
    std::string fragmentCode; //!< The raw fragment shader code in GLSL
		std::string geometryCode;
    GLuint ShaderProgramID; //!< The shader program, tracked by its id
    std::unordered_map<std::string, GLint> ShaderUniformsMap;

    void AssertShaderCompilation(GLuint shader, std::string shaderName);
    void AssertShaderProgramLinking(GLuint shaderProgram);
    

  };

  using ShaderPtr = std::shared_ptr<Shader>;
  
  /**************************************************************************/
  /*!
  \brief Updates the uniform in the shader.
  \param The name of the uniform with which to update.
  \param The data to set into the uniform.
  \note  This function is templatized in order to streamline the variety 
         of uniform functions to be called. So you call this function,
         which will handle deciding which other functions to call.
  */
  /**************************************************************************/
  template<typename T>
  inline void Shader::UpdateUniforms(const std::string & uniformName, const T& data)
  {
    LoadShaderUniform(ShaderUniformsMap[uniformName], data);
  }

}