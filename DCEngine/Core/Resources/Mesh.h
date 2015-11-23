/******************************************************************************/
/*!
\file   Mesh.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/13/2015
\brief  Mesh resource class. Contains hardware vertex buffer and index buffer.
Resource: "http://learnopengl.com/#!Model-Loading/Mesh"
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include "ResourceReference.h"

#include "Texture.h"

namespace DCEngine {

  struct Vertex {
    Vec3 Position;
    Vec3 Normal;
    Vec2 TexCoords;
  };
  using VertexVec = std::vector<Vertex>;
  using IndicesVec = std::vector<GLuint>;


  class Mesh : public Resource {

  public:
    Mesh(VertexVec vertices, IndicesVec indices, TextureVec textures);
      
    /* Mesh Data */
    VertexVec vertices_;
    IndicesVec indices_;
    TextureVec textures_;

    //void Draw(Shader shader);

  private:
    /* Functions */
    void Build(); //!< Sets up the mesh data to be used for rendering.

    /* Update data*/
    GLuint VBO;
    GLuint VAO;
    GLuint EBO;
    

  };


}