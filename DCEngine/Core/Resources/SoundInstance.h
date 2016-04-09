/*****************************************************************************/
/*!
@file      SoundInstance.h
@author    Christian Sagel
@par       email: c.sagel\@digipen.edu
@date      2/9/2015
@brief     SoundInstance is an object created whenever a SoundCue is played that 
           acts as the particular instance of that playing of the SoundCue as well
           as a direct representation of the sound currently playing. 
           The SoundInstance object is only created through code: through 
           the SoundEmitter or SoundSpace.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
//#include "ResourceReference.h"
#include "SoundCue.h"

namespace DCEngine {

  namespace Systems {
    class Audio;
  }

  class SoundInstance {
    friend class Systems::Audio;
  public:

    // Member variables
    SoundCue::SoundCueType Type;
    std::string StudioEventName;
    FMODSoundHandle SoundHandle;    
    PlaybackSettings Settings;

    //PlayMode Mode;
    //float Volume = 1.0f;
    //float VolumeVariation = 0.0f;
    //float Pitch = 1.0f;
    //float PitchVariation = 0.0f;
    //bool Loop = false;
    bool Paused = false;
    SoundCueHandle SoundCue;
    //DCE_DEFINE_PROPERTY(PlayMode, Mode);  
    // Methods
    void InterpolateVolume(Real newVolume, Real time);
    void InterpolatePitch(Real newPitch, Real time);
    void Resume();
    void Pause();
    void Stop();
    bool IsPlaying();
    // CTOR
    SoundInstance();
    ~SoundInstance();

    static bool StopOnDestroyed;
  private:

    unsigned SoundInstanceID;
    static unsigned Created;
    static unsigned Destroyed;
    static bool ReleaseOnDestroyed;
  };

  using SoundInstanceHandle = std::shared_ptr<SoundInstance>;
  using SoundInstanceContainer = std::map<SoundInstanceHandle, SoundCueHandle>;

}