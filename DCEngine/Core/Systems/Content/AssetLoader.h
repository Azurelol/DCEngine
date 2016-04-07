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

    // Data structures
    struct FileData {
      std::string Path;
      unsigned Size;
      FileData(const std::string& path, unsigned size) : Path(path), Size(size) {}

      bool operator()(const FileData& lhs, const FileData& rhs) {
        if (lhs.Size != rhs.Size) return lhs.Size > rhs.Size;
        return lhs.Path > rhs.Path;
     } 

      // Sorting function
      struct SortBiggest {
        bool operator()(const FileData& lhs, const FileData& rhs) {
          if (lhs.Size != rhs.Size) return lhs.Size > rhs.Size;
          return lhs.Path > rhs.Path;
        }
      };


    };

    using FileQueue = std::priority_queue<FileData, std::vector<FileData>, FileData::SortBiggest>;

        
    // Asset Loader
    class AssetLoader {
    public:
      struct ALSettings {
        unsigned ThreadCount;
        ALSettings(unsigned count) : ThreadCount(count) {}
      };

      AssetLoader(const ALSettings& settings);
      ~AssetLoader();

      template<typename ResourceMap, typename ResourcePtr>
      bool Load(const ResourceMap & resources, std::function<void()> addMethod);


      // Interface
      static bool AddResource(const FilePaths& resourcePaths, std::function<void(const std::string&)> addMethod);

    private:
      ALSettings Settings;
      bool Active;
      std::vector<std::thread> Threads;
      bool ThreadedLoad(const FilePaths& resourcePaths, std::function<void(void)> addMethod);

    };

    /**************************************************************************/
    /*!
    @brief Loads the specified resource by dividing the tasks among many threads.
    @param resourcePaths The paths to the resource.
    @param loadMethod The method which to use to load.
    */
    /**************************************************************************/
    template<typename ResourceMap, typename ResourcePtr>
    inline bool AssetLoader::Load(const ResourceMap & resources, std::function<void()> addMethod)
    {
      // Sort the resources' assets by file size
      FileData::SortBiggest sortBiggest;
      FileQueue sortedResources(sortBiggest);      
      for (auto& resource : resources) {
        sortedResources.push(FileData(resource.first, FileSystem::FileSize(resource.second->getAssetPath())));
      }
      // Add them to as many vectors we have partitions
      std::vector<std::vector<ResourcePtr>> partitions(Settings.ThreadCount);
      while (!sortedResources.empty()) {
        auto currentResource = sortedResources.top(); sortedResources.pop();

      }
      
      return false;
    }

  }
}