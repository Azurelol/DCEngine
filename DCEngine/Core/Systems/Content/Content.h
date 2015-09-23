/*****************************************************************************/
/*!
\file   Content.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   9/20/2015
\brief  The Content system handles the loading of all content data required
        for a project. It stores references to the names of these resources
        as well as their file locations. The deserialization of these files
        is handled by the factory system.
*/
/******************************************************************************/
#pragma once
#include "../System.h"

namespace DCEngine {

  class Engine;
  class Factory;

  namespace Systems {

    using ResourceMap = std::map<std::string, std::string>;    

    class Content : public System {
      friend class Engine;
      friend class Factory;
    public:

      void LoadResources();
      void ScanFolder();

    private:

      ResourceMap SoundCueMap;
      ResourceMap SpriteSourceMap;
      ResourceMap LevelMap;
      ResourceMap ArchetypeMap;

      Content();
      void Initialize();
      void Update(float dt); //!< Delete all objects in the to-be-deleted list
      void Terminate();



    };
  }

}

