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
      ChangeMusic() : Event("ChangeMusic") {}
      String NextTrack;
    };

    class PlayMusic : public Event {
    public:
      PlayMusic() : Event("PlayMusic") {}
    };

    class ChangeLevel : public Event {
    public:
      ChangeLevel() : Event("ChangeMusic") {}
      String NextLevel;
    };

	  class DamageEvent : public Event {
	  public:
      DamageEvent() : Event("DamageEvent") {}
	  	Real Damage;

	  };

    class DeathEvent : public Event {
    public:
      DeathEvent() : Event("DeathEvent") {}
    };

    class RespawnEvent : public Event {
    public:
      RespawnEvent() : Event("RespawnEvent") {}
    };

  }
}

