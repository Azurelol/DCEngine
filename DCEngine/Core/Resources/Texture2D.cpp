#include "Texture2D.h"
#include <SFML\Graphics\Image.hpp>

namespace DCEngine {

  Texture2D::Texture2D(std::string& imageFile) : Resource(imageFile), 
                       Width(0), Height(0), InternalFormat(GL_RGB), ImageFormat(GL_RGB),
                       Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), FilterMin(GL_LINEAR),
                       FilterMax(GL_LINEAR)
  {
   
  }

  void Texture2D::Generate(GLuint width, GLuint height, sf::Image& data) {
  //void Texture2D::Generate(GLuint width, GLuint height, unsigned char * data) {
    this->Width = width;
    this->Height = height;
    GLenum error;
   
    glGenTextures(1, &this->TextureID);
    error = glGetError(); 
    if (error)
      DCTrace << "Texture2D::Generate - Failed to generate texture ID!\n";

    // Bind texture
    glBindTexture(GL_TEXTURE_2D, this->TextureID);
    error = glGetError();
    if (error)
      DCTrace << "Texture2D::Generate - Failed to bind the texture!\n";

    // Generate texture
    glTexImage2D(GL_TEXTURE_2D, 0, this->InternalFormat, width, height, 0,
                 this->ImageFormat, GL_UNSIGNED_BYTE, data.getPixelsPtr());
    error = glGetError();
    if (error)
      DCTrace << "Texture2D::Generate - Failed to generate texture!\n";

    // Set texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->FilterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->FilterMax);
    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);

    DCTrace << "Texture2D::Generate - Successfully generated texture!\n";
  }

  void Texture2D::Bind() const {
    glBindTexture(GL_TEXTURE_2D, this->TextureID);
  }

}