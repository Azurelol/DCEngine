/******************************************************************************/
/*!
\file   GLTutorial.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/18/2015
\brief  The base OpenGL tutorial component class. For the purposes of learning,
        these tutorials have been written as components to be added to a dummy
        'Doll' object that is loaded into a similarly dummied 'Dollhouse' level.

*/
/******************************************************************************/
#pragma once
#include "..\..\Core\Objects\Component.h"

#include <string>
#include <fstream> // ifstream
#include <iostream> // cout

// GLEW
#define GLEW_STATIC
#include "..\..\Dependencies\include\GLEW\glew.h"
// GLFW
#define GLFW_BUILD_DLL
#include "..\..\Dependencies\include\GLFW\glfw3.h"

#include "..\..\Core\Debug\Debug.h"
#include "..\..\Core\Objects\Resources\GLShader.h"

namespace DCEngine {
  namespace Tutorial {

    class GLTutorial : public Component {

    public:
      GLTutorial() : Component(EnumeratedComponent::Transform, BitfieldComponent::Alive) {
        trace << "GLTutorial::GLTutorial - Constructor \n-An OpenGL tutorial is now being used!- \n";
      }
      ~GLTutorial() {};

      virtual void Initialize() = 0;
      virtual void Update() = 0;
      virtual void Terminate() = 0;


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
        std::string shaderLocation("Core/Systems/Graphics/Tutorials/");
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
          trace << "Failed to load the shader file: " << fileName << "\n";
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
      static void AssertShaderCompilation(GLuint shader, std::string shaderName) {
        GLint success;       // Define an integer to indicate success
        GLchar infoLog[512]; // Container for the error messages (if any)

        // Check if compilation was successful.
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (success) {
          trace << "GLTutorial::AssertShaderCompilation - " << shaderName << " compiled! \n";
        }
        else {
          glGetShaderInfoLog(shader, 512, NULL, infoLog);
          trace << "GLTutorial::AssertShaderCompilation - " << shaderName << " failed to compile! \n"
            << infoLog << "\n";
        }
      }

      /**************************************************************************/
      /*!
      \brief  Checks if the shader program linked successfully.
      \para   The shader.
      \return A string.
      */
      /**************************************************************************/
      static void AssertShaderProgramLinking(GLuint shaderProgram, std::string programName) {
        GLint success;       // Define an integer to indicate success
        GLchar infoLog[512]; // Container for the error messages (if any)

        // Check if compilation was successful.
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

        if (success) {
          trace << "GLTutorial::AssertShaderCompilation - " << programName << " linked! \n";
        }
        else {
          glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
          trace << "GLTutorial::AssertShaderCompilation - " << programName << " failed to link! \n"
            << infoLog << "\n";
        }
      }


    private:




    };

  }


}