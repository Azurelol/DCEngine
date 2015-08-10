#pragma once

#include <string>
#include <fstream>
#include <iostream> // cout

#define GLEW_STATIC
#include "..\..\..\..\Dependencies\include\GLEW\glew.h"

namespace Tutorial {

  class GLTutorial {

  public:
    //GLTutorial() {};
    virtual void Initialize() = 0;
    virtual void Update() = 0;


    /**************************************************************************/
    /*!
    \brief  Loads a GLSL shader from a file and returns it as a string.
    \note   This function call can be made used as follows:
            "string source = getGLSLCode("vertexshader.txt");
            if (source == "") exit(1);
            const GLchar* vertexShaderSource = (GLchar*)source.c_str();"
    \para   The filename of the shader.
    \return A string.
    */
    /**************************************************************************/
    static std::string GetGLSLCode(std::string fileName) {
      
      // We have to get the location of the folder, then append the filename
      std::string shaderFileName(fileName);
      std::string shaderLocation("/Core/Systems/Graphics/Tutorials/");
      shaderLocation += shaderFileName;
      // Load the file
      std::ifstream shaderFile(shaderLocation.c_str(), std::ios::in);
      // This string will contain the shader code
      std::string shaderCode;

      if (shaderFile) {
        std::string line;
        while (std::getline(shaderFile, line)) {
          shaderCode = shaderCode + line + "\n";
        }
        shaderFile.close();
      }
      // FAILURE
      else {
        std::cout << "Failed to load the shader file: " << fileName << std::endl;
        exit(1);
        //return "";
      }
      return shaderCode;
    }

    /**************************************************************************/
    /*!
    \brief  Checks if the shader was compiled successfully.
    \para   The shader.
    \return A string.
    */
    /**************************************************************************/
    static void AssertShaderCompilation(GLuint shader) {
      // Define an integer to indicate success
      GLint success;
      // Container for the error messages (if any)
      GLchar infoLog[512];
      // Check if compilation was successful.
      glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

      if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED \n"
          << infoLog << std::endl;
      }


    }

  private:




  };

}

