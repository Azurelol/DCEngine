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

 //DCE_BINDING_DECLARE_COMPONENT(SoundEmitter)

  namespace Components
  {
    class SoundEmitter : public Component {
    public:
            
      DCE_DEFINE_PROPERTY(SoundCueHandle, Cue);
      DCE_DEFINE_PROPERTY(Real, Volume);
      DCE_DEFINE_PROPERTY(Real, Pitch);
      DCE_DEFINE_PROPERTY(Boolean, IsPlaying);

      void PlayCue(String soundCue);

      /* Initialize */
      ZilchDeclareDerivedType(SoundEmitter, Component);
      SoundEmitter(Entity& owner) : Component(std::string("SoundEmitter"), owner) {}
      void Initialize();

    private:
      String CurrentSoundCue; //!< The 'SoundCue' that this emitter currently holds.
      SoundCueHandle Cue;
      Real Volume = 1;
      Real Pitch = 1;
      Boolean IsPlaying = false;

    };
  }
  
}