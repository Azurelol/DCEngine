/*****************************************************************************/
/*!
@file      AssetLoader.cpp
@author    Christian Sagel
@par       email: c.sagel\@digipen.edu
@date      4/6/2016
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "AssetLoader.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    @brief AssetLoader constructor
    @param settings The configurations for the AssetLoader instance.
    */
    /**************************************************************************/
    AssetLoader::AssetLoader(const ALSettings & settings) : Settings(settings)
    {
    }

    /**************************************************************************/
    /*!
    @brief AssetLoader destructor.
    */
    /**************************************************************************/
    AssetLoader::~AssetLoader()
    {
    }


    /**************************************************************************/
    /*!
    @brief Loads the specified resource, done by a thread.
    */
    /**************************************************************************/
    bool AssetLoader::ThreadedLoad(const FilePaths & resourcePaths, std::function<void(void)> addMethod)
    {
      return false;
    }

    /**************************************************************************/
    /*!
    @brief Adds a resource into its appropiate resource map using the method provided.
    @param A container containing the resource.
    @param addMethod The method which to invoke the resource path on.
    */
    /**************************************************************************/
    bool AssetLoader::AddResource(const FilePaths & resourcePaths, std::function<void(const std::string&)> addMethod)
    {
      for (auto& path : resourcePaths) {
        addMethod(path);
      }
      return true;
    }
  }
}

// We will create 4 threads to load images
//AssetLoader::ALSettings alSettings(4);
//AssetLoader spriteLoader(alSettings);
//auto spriteSourceLoad = std::bind(&SpriteSource::LoadTexture);
//spriteLoader.Load<SpriteSourceMap, SpriteSourcePtr>(MapSpriteSource, spriteSourceLoad);      

//------------------------------------------------------------------------// 
// Sort the spriteSources by file size
//FileData::SortBiggest sortFunc;
//FileQueue sortedSpriteSources(sortFunc);
//for (auto& spriteSource : MapSpriteSource) {
//  auto fileData = FileData(spriteSource.first, FileSystem::FileSize(spriteSource.second->getAssetPath()));
//  sortedSpriteSources.push(fileData);
//}
//// Create partitions depending on the number of threads
//size_t threadCount = 1;
//std::vector<std::thread> threads;
//std::vector<std::vector<SpriteSourcePtr>> partitions(threadCount);
//int i = 0;
//while (!sortedSpriteSources.empty()) {
//  auto file = sortedSpriteSources.top(); sortedSpriteSources.pop();
//  partitions[i++ % partitions.size()].push_back(MapSpriteSource.at(file.Path));
//}       
//// For each partition, create a tread
//for (auto& partition : partitions) {
//  threads.push_back(std::thread(LoadSpriteSourceTextures, partition));
//}

//------------------------------------------------------------------------//