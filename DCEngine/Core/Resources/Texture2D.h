/******************************************************************************/
/*!
\file   Texture2D.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   9/16/2015
\brief  The texture class stores and configures a texture in OpenGL.
\note   "http://learnopengl.com/#!Getting-started/Textures"
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include "ResourceReference.h"
#include <SFML\Graphics\Image.hpp>

namespace DCEngine {

  class Texture2D : public Resource {
  public:
    // Constructor (sets default texture mode)
    Texture2D(std::string& imageFile);
    // Generates texture from image data
    void Generate(GLuint width, GLuint height, sf::Image& data);
    //void Generate(GLuint width, GLuint height, unsigned char* data);
    // Binds the texture as the current active GL_TEXTURE_2D texture object
    void Bind() const;

    // Holds the ID of the texture object, used for all texture operations
    // to reference to this particular texture.
    GLuint TextureID;
    // Texture image dimensions
    GLuint Width, Height;
    // Texture format 
    GLuint InternalFormat; // Format of texture object
    GLuint ImageFormat; // Format of loaded image
    // Texture configuration
    GLuint Wrap_S; // Wrapping mode on S axis
    GLuint Wrap_T; // Wrapping mode on T axis
    GLuint FilterMin; // Filtering mode if texture pixels < screen pixels
    GLuint FilterMax; // Filtering mode if texture pixels > screen pixels
    



  };

}