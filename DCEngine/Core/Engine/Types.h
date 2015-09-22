#pragma once

#include <string>
#include <memory>
#include <typeinfo>
#include <algorithm>

#include <GLM\glm.hpp>
#include <GLM\matrix.hpp>
#include <GLM\gtc\type_ptr.hpp>
#include <GLM\gtc\matrix_transform.hpp>
#define GLEW_STATIC
#include <GLEW\glew.h>

namespace DCEngine {
  
  // Reference: http://codereview.stackexchange.com/questions/48344/yet-another-any-class-implementation-named-some
  
  enum class Keys {
    Escape,
    Space ,
    Return,
    Up,
    Down,
    Left,
    Right,
    W,
    A,
    S,
    D,
  };

  // Built-in types
  using Real = float;
  using Real2 = glm::vec2;
  using Real3 = glm::vec3;
  using Real4 = glm::vec4;

  using Mat3 = glm::mat3;
  using Mat4 = glm::mat4;

}
