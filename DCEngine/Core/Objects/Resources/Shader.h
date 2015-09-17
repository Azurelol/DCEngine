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
#include "..\Resource.h"

// GLEW
#define GLEW_STATIC
#include "..\..\..\Dependencies\include\GLEW\glew.h"
//#include "..\..\Dependencies\include\GLEW\glew.h"
// GLFW
#define GLFW_BUILD_DLL
#include "..\..\..\Dependencies\include\GLFW\glfw3.h"
//#include "..\..\Dependencies\include\GLFW\glfw3.h"

#include <string>
#include <memory>

#include "..\..\Debug\Debug.h" // trace

namespace DCEngine {

  class Shader : public Resource {
  public:
    Shader(std::string vertexPath, std::string fragmentPath);
    
    void Load(std::string vertexPath, std::string fragmentPath);
    void Compile();    
    void Use();

    GLuint Get() {
      
      return _shaderProgram;
    }


  private:
    void AssertShaderCompilation(GLuint shader, std::string shaderName);
    void AssertShaderProgramLinking(GLuint shaderProgram);
    
    std::string vertexCode;  //!< The raw vertex shader code in GLSL
    std::string fragmentCode;
        
    GLuint _shaderProgram; //!< The shader program, tracked by its id


  };

  using ShaderPtr = std::shared_ptr<Shader>;
  

}