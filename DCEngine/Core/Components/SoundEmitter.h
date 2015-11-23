/******************************************************************************/
/*!
@file   SoundEmitter.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/23/2015
@brief
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "ComponentReference.h"

namespace DCEngine {

    class SoundEmitter : public Component {
    public:

      Real Volume = 1;
      Real Pitch = 1;
      Boolean IsPlaying = false;

      void PlayCue(String soundCue);
      void Stop();
      void setVolume();
      void getVolume();

      /* Initialize */
      SoundEmitter(Entity& owner) : Component(std::string("SoundEmitter"), owner) {}
      void Initialize();

    private:
      String CurrentSoundCue; //!< The 'SoundCue' that this emitter currently holds.

    };
  
}