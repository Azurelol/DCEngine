/******************************************************************************/
/*!
\file   DebugAudio.h
\author Chen Shu, Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   9/20/2015
\brief  Manages a viewport, attacheing a camera.
\note   This component is accessed by the graphics system for its
projection matrix.

@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ComponentReference.h"

namespace DCEngine {
  namespace Components
  {
    class DebugAudio : public Component {
    public:

      SoundInstanceWeakPtr MyJam;
      SoundCueHandle Track1 = "soulja";
      SoundCueHandle Track2;
      SoundCueHandle Track3;
      SoundCueHandle CurrentSoundCue;
      DCE_DEFINE_PROPERTY(SoundCueHandle, Track1);
      DCE_DEFINE_PROPERTY(SoundCueHandle, Track2);
      DCE_DEFINE_PROPERTY(SoundCueHandle, Track3);
            
      ZilchDeclareDerivedType(DebugAudio, Component);
      DebugAudio(Entity& owner) : Component(std::string("DebugAudio"), owner) {}
      ~DebugAudio();
      void Initialize();

      void OnKeyDownEvent(Events::KeyDown* event);
      void OnKeyUpEvent(Events::KeyUp* event);

      void ChangeTrack(std::string& track);
      void ChangeVolume(float increment);
      void Pause(bool);

    private:
    };
  }

}