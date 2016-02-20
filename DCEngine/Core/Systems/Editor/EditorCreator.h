/******************************************************************************/
/*!
@file   EditorCreator.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   1/20/2016
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once

namespace DCEngine {
  namespace Systems {

    class Editor;
    class EditorCreator {
    public:
      EditorCreator(Editor& editor);
      void CreateTransform();
      void CreateSprite();
      void CreateSpriteText();
      void CreateParticleSystem();
      void CreateLight();
      void CreateFromArchetype(const std::string& archetypeName);

    private:
      void Create(std::string name, std::vector<std::string>& components);
      Editor& EditorRef;

    };


  }
}
