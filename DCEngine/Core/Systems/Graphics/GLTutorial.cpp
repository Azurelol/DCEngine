#include "GLTutorial.h"

#define GLEW_STATIC
#include "..\..\..\Dependencies\include\GLEW\glew.h"

namespace Tutorial {

  /************************/
  /*    Initialize()
  /************************/
  void Initialize() {
    TriangleInitialize();

  }

  /************************/
  /*    Update()
  /************************/
  void Update() {
    TriangleUpdate();
  }  

  /**************************************************************************/
  /**************************************************************************/
  /*                            Drawing a Triangle
  /**************************************************************************/
  /*    Initialize()
  /**********************/
  void TriangleInitialize() {

    // Because we want to render a single triangle
    GLfloat vertices[]{
      -0.5f, -0.5f, 0.0f,
      0.5f,  -0.5f, 0.0f,
      0.0f,  0.5f,  0.0f
    };
  }

  /************************/
  /*    Update()
  /************************/
  void TriangleUpdate() {

  }

}