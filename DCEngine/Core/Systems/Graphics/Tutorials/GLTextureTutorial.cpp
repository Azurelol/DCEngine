#include "GLTextureTutorial.h"
#include "..\..\..\..\Dependencies\include\SOIL2\SOIL2.h"

namespace Tutorial {
  
  void GLTextureTutorial::Initialize() {
    /* A texture is a 2D (or 1D, 3D) image used to add detail to an object. */

    /* In order to map a texture to an object we need to tell each vertex of the
       object which part of the texture it corresponds to. Each vertex should have
       a texture coordinate associated with what part of the image. Fragment
       interpolation then does the rest.*/

    /* Sampling: Retrieving the texture color using texture coordinates. 
      Texture coordinates start at (0, 0) for the lower left corner of a texture image
      to (1,1) for the upper right corner. */

    /* Loads an image n SOIL2 (Simple Open Image Library 2) */
    image = SOIL_load_image("container.jpg", &width, &height, 0, SOIL_LOAD_RGB);


  }

  void GLTextureTutorial::Update() {
  }

}