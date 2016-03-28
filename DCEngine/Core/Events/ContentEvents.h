/******************************************************************************/
/*!
@file   ContentEvents.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   3/26/2015
@brief  These are internal events from the engine.
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include "EventReference.h"

namespace DCEngine {
  namespace Events {

    class ContentFileMoved : public Event {
    public:
      ContentFileMoved() : Event("ContentFileMoved") {}
      std::string FileName;
      std::string Extension;
    };

    class ContentFileUpdated : public Event {
    public:
      ContentFileUpdated() : Event("ContentFileUpdated") {}
      std::string FileName;
      std::string Extension;
    };

    class ContentFileFound : public Event {
    public:
      ContentFileFound() : Event("ContentFileFound") {}
      std::string FileName;
      std::string Extension;
    };

    class ContentFileDeleted : public Event {
    public:
      ContentFileDeleted() : Event("ContentFileDeleted") {}
      std::string FileName;
      std::string Extension;
    };

    class ContentFileScanComplete : public Event {
    public:
      ContentFileScanComplete() : Event("ContentFileScanComplete") {}
    };

    class ContentProjectLoaded : public Event {
    public:
      ContentProjectLoaded() : Event("ContentProjectLoaded") {}

    };

  }
}