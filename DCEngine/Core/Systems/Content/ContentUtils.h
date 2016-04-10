/*****************************************************************************/
/*!
\file   ContentUtils.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   3/23/2016
\brief  The Content system handles the loading of all content data required
for a project. It stores references to the names of these resources
as well as their file locations. The deserialization of these files
is handled by the factory system.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once

namespace DCEngine {
  namespace Systems {

    #define DCE_CONTENT_DECLARE_RESOURCE(Name)    \
    using Name##Ptr = std::shared_ptr<Name>;      \
    using Name##Container = std::map<Name##Ptr>;  \
    using Name##Handle = std::string              

    /**************************************************************************/
    /*!
    \brief  AssetsQueue A queue of assets that can be accessed for operations
            on them.
    */
    /**************************************************************************/
    struct ResourceQueue {
      std::queue<ResourcePtr> Assets;
      std::mutex AssetsLock;
      // Progress
      unsigned NumLoaded;
      unsigned NumTotal;
      std::string LastLoaded;
      bool Finished;
      std::mutex LockFinished;
      // Methods
      void Lock() { AssetsLock.lock(); }
      void Unlock() { AssetsLock.unlock(); }
      ResourceQueue() : NumLoaded(0), NumTotal(0), Finished(false) {}
    };

    using ResourceHandle = std::string;
      

    template <typename ResourceType>
    class ResourceMap {
    public:
      using Map = std::unordered_map<ResourceHandle, ResourceType>;

      void Add(ResourceType&);
      bool Remove(ResourceType&);
      bool Remove(const ResourceHandle&);
      bool Has(const ResourceHandle&);
      ResourceType Find(const ResourceHandle&);
      const Map& All();

    private:
      using ResourceTypePtr = std::shared_ptr<Resource>;
      Map Container;
    };



  }
}