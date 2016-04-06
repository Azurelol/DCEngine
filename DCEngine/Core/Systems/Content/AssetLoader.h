/*****************************************************************************/
/*!
@file      AssetLoader.h
@author    Christian Sagel
@par       email: c.sagel\@digipen.edu
@date      4/6/2016
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once

namespace DCEngine {
  namespace Systems {

    // Aliases
    using Extension = std::string;
    using FilePaths = std::list<std::string>;
    using ResourcePool = std::map < Extension, FilePaths>;
        
    // Asset Loader
    class AssetLoader {
    public:
      struct ALSettings {
      };

      
      // Static method
      static bool LoadS(const FilePaths& resourcePaths, std::function<void(const std::string&)> addMethod) {
        for (auto& path : resourcePaths) {
          addMethod(path);
        }
        return true;
      }


      bool Load(const FilePaths& resourcePaths, std::function<void(const std::string&)> addMethod) {
        for (auto& path : resourcePaths) {
          addMethod(path);
        }
        return true;
      }

    private:
      bool Active;
      std::thread Thread;

    };

  }
}