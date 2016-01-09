/*****************************************************************************/
/*!
@file   Font.h
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
#pragma once
#include "ResourceReference.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>

namespace DCEngine {

  /**************************************************************************/
  /*!
  @brief A struct containing generated data about a specific character that
  will be used whenever it needs to be rendered.
  */
  /**************************************************************************/
  struct Character {
    GLuint CharacterTextureID; // ID Handle of the glyph texture
    glm::ivec2 Size; // Size of glyph
    glm::ivec2 Bearing; // Ofset from baseline to left/top of glyph
    GLuint Advance; // Offset to advance to next glyph
  };

  class Font : public Resource {
    
  public:

    #if(DCE_USE_ZILCH_INTERNAL_BINDING) 
    ZilchDeclareDerivedType(Font, Resource);
    #endif

    // Properties
    DCE_DEFINE_PROPERTY(String, AssetPath);
    
    std::map<GLchar, Character> Characters;

    Font(const std::string& fontFile, std::string assetPath);
    static std::string Extension() { return ".Font"; }
    void GenerateCharacters(FT_Face face);
    bool Load();

  private:
    std::string AssetPath;

  };

  using FontPtr = std::shared_ptr<Font>;
}