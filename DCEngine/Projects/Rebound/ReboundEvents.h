/*****************************************************************************/
/*!
@file   ReboundEvents.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/19/2015
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "../ProjectEvent.h"

namespace DCEngine {
  namespace Events {

    class ChangeMusic : public Event {
    public:
      String NextTrack;
    };

    class ChangeLevel : public Event {
    public:
      String NextLevel;
    };


  }



}

