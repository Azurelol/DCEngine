/*****************************************************************************/
/*!
@file   Font.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   9/30/2015
@brief  The Font resource handles the loading and processing of a TrueType font
from file. It uses the 'Freetype' library to do so. Freetype loads
these fonts and for each glyph generates a bitmap image and caculates
several metrics. We can extract these bitmap images for generating
textures and position each character glyph appropiately using
the loading metrics.
@note   "http://learnopengl.com/#!In-Practice/Text-Rendering"
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/

#include "Font.h"
#include "../Debug/DebugGraphics.h"

namespace DCEngine {

  /**************************************************************************/
  /*!
  @brief Provides the definition of this class to Zilch for reflection.
  */
  /**************************************************************************/
#if(DCE_USE_ZILCH_INTERNAL_BINDING)
  ZilchDefineType(Font, "Font", DCEngineCore, builder, type) {
    // Constructor / Destructor
    //ZilchBindConstructor(builder, type, SoundCue, "owner", Entity&);
    //ZilchBindDestructor(builder, type, SoundCue);
    // Properties
    DCE_BINDING_DEFINE_PROPERTY(Font, AssetPath);
  }
#endif

  /**************************************************************************/
  /*!
  @brief Constructor for the SpriteSource resource.
  @param The name of the font file.
  */
  /**************************************************************************/
  Font::Font(std::string fontFile) : Resource("Font", FileSystem::FileNoExtension(fontFile), fontFile) {
  }

  /**************************************************************************/
  /*!
  @brief Loads a font from file into the Font resource.
  @param The name of the font file.
  */
  /**************************************************************************/
  bool Font::Add() {
    FT_Library ft;

    // Initialize the FreeType library
    if (FT_Init_FreeType(&ft)) {
      DCTrace << "Font::Load - Error! Could not initialize FreeType library! \n";
      return false;
    }

    // Load the font from file
    //std::string fontPath = "Core/Resources/Fonts/";
    //fontPath.append(FontFileName);
    FT_Face face;
    if (FT_New_Face(ft, AssetPath.c_str(), 0, &face)) {
      DCTrace << "Font::Load - Error! Failed to load font \n";
      return false;
    }

    // Define the font size to extract from this face. This function sets
    // the font's width and height parameters. Setting the widh to 0
    // lets the face dynamically calculate the width based on a given height.
    FT_Set_Pixel_Sizes(face, 0, 48);

    // Generate the map of character textures
    GenerateCharacters(face);
    DCTrace << "Font::Load - Font was successfully loaded! \n";

    // Clear FreeType's resources now that we are done processing glyphs
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    return true;


  }

  /**************************************************************************/
  /*!
  @brief Generates a map of character textures given the (font) face.
  @param The name of the font file.
  @note  Currently only generating the first 128 characters of the ASCII
  character set.
  */
  /**************************************************************************/
  void Font::GenerateCharacters(FT_Face face) {

    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Iterate over characters of the ASCII set 
    for (GLubyte c = 0; c < 128; c++) {

      // Load character glyph
      if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
        DCTrace << "Font::GenerateCharacters  - Error! Failed to load glyph! \n";
        continue;
      }

      // Generate texture
      GLuint texture;
      glGenTextures(1, &texture);
      Debug::CheckOpenGLError("Font::GenerateCharacters - Failed to generate texture ID!");

      glBindTexture(GL_TEXTURE_2D, texture);
      Debug::CheckOpenGLError("Font::GenerateCharacters - Failed to bind texture");

      glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        face->glyph->bitmap.width,
        face->glyph->bitmap.rows,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        face->glyph->bitmap.buffer
        );
      Debug::CheckOpenGLError("Font::GenerateCharacters - Failed to generate 'TexImage2D'");

      // Set texture options
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      // Store the character (with its metrics) in the map for later use
      Character character = {
        texture,
        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        face->glyph->advance.x
      };
      Characters.insert(std::pair<GLchar, Character>(c, character));
    }

    DCTrace << "Font::GenerateCharacters - Successfully generated character map\n";

  }

}