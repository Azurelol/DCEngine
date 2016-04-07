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