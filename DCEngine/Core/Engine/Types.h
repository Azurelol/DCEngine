/*****************************************************************************/
/*!
@file   Types.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   10/02/2015
@brief  Saves the types used in the Daisy Chain engine.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once

#include <string>
#include <memory>
#include <vector>
#include <typeinfo>
#include <algorithm>
#include <cassert>
#include <map>

// GLM
#include <GLM\glm.hpp>
#include <GLM\matrix.hpp>
#include <GLM\gtc\type_ptr.hpp>
#include <GLM\gtc\matrix_transform.hpp>
// GLEW
#define GLEW_STATIC
#include <GLEW\glew.h>
// Custom
#include "../Types/Containers.h"
#include "../Types/Enumerations.h"

namespace DCEngine {

  // Built-in types
  using String = std::string;
  using Boolean = bool;
  using Integer = int;
  using Integer2 = glm::i64vec2;
  using Integer3 = glm::i64vec3;
  using Integer4 = glm::i64vec4;
  using Real = float;
  using Vec2 = glm::vec2;
  using Vec3 = glm::vec3;
  using Vec4 = glm::vec4;  
  using Mat3 = glm::mat3;
  using Mat4 = glm::mat4;

  // Common Containers
  using StringVec = std::vector<std::string>;  
  // Handles
  using CollisionGroupHandle = std::string;

  struct OpenGLStateData {
    GLint lastProgram;
    GLint lastTexture;
    GLint lastArrayBuffer;
    GLint lastElementArrayBuffer;
    GLint lastVertexArray;
    GLint lastBlendSrc;
    GLint lastBlendDst;
    GLint lastBlendEquationRGB;
    GLint lastBlendEquationAlpha;
    GLboolean lastEnableBlend;
    GLboolean lastEnableCullFace;
    GLboolean lastEnableDepthTest;
    GLboolean lastEnableScissorTest;
  };


  struct TransformData {
    Vec3 Translation;
    Vec3 Rotation;
    Vec3 Scale;
  };
  namespace Components {
    class Transform;
  }
  using TransformDataPair = std::pair<Components::Transform*, TransformData>;
  using TransformDataPairVec = std::vector<TransformDataPair>;


}
