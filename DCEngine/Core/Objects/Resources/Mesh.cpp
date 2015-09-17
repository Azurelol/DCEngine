#include "Mesh.h"

namespace DCEngine {

  Mesh::Mesh(VertexVec vertices, IndicesVec indices, TextureVec textures) 
    : Resource("Shader"), vertices_(vertices), indices_(indices), textures_(textures) {}

  void Mesh::Build() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    /* Because structs in C++ have sequential memory layout. If we were
      to represent a struct as an array of data it would only contain the
      struct's variables in sequential order which directly translates to a
      float. (byte) array that we want for an array buffer. */
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex),
                  &vertices_[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, EBO);
    glBufferData(GL_ARRAY_BUFFER, indices_.size() * sizeof(GLuint),
                  &indices_[0], GL_STATIC_DRAW);

    // Vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
                         (GLvoid*)0);
    // Vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
                          (GLvoid*)offsetof(Vertex, Normal));
    // Vertex Texture Coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
      (GLvoid*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
  }

}