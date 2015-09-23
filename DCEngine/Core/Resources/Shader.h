/******************************************************************************/
/*!
\file   GLShader.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/13/2015
\brief  An encapsulated object that reads shaders from disk, compiles, links them,
        checks for errors and other useful functions.
*/
/******************************************************************************/
#pragma once
#include "ResourceReference.h"
#include "..\Engine\Types.h"

namespace DCEngine {

  class Shader : public Resource {
  public:
    GLuint ShaderID;
    
    Shader(std::string& shaderName, std::string vertexPath, std::string fragmentPath);
    
    void Load(std::string vertexPath, std::string fragmentPath);
    void Compile();    
    Shader& Use();
    GLuint Get() { return _shaderProgram; }

    /* Utility functions */
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
    void AssertShaderCompilation(GLuint shader, std::string shaderName);
    void AssertShaderProgramLinking(GLuint shaderProgram);
    
    std::string vertexCode;  //!< The raw vertex shader code in GLSL
    std::string fragmentCode;        
    GLuint _shaderProgram; //!< The shader program, tracked by its id


  };

  using ShaderPtr = std::shared_ptr<Shader>;
  

}