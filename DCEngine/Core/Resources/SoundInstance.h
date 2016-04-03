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

  class SoundInstance : public Object {
    friend class Systems::Audio;
  public:

    // Member variables
    SoundCue::SoundCueType Type;
    std::string StudioEventName;
    FMODSoundHandle SoundHandle;    
    PlaybackSettings Settings;    
    bool Paused = false;
    SoundCueHandle SoundCue;    

    // Interface
    static SoundInstance* Dereference(const Zilch::Handle& handle);
    // Methods    
    void SetParameter(String parameter, float value);
    void InterpolateVolume(Real newVolume, Real time);
    void InterpolatePitch(Real newPitch, Real time);
    void Resume();
    void Pause();
    void Stop();
    bool IsPlaying();
    // CTOR
    ZilchDeclareDerivedType(SoundInstance, Object);
    SoundInstance();
    ~SoundInstance();

    static bool StopOnDestroyed;
  private:

    unsigned SoundInstanceID;
    static unsigned Created;
    static unsigned Destroyed;
    static bool ReleaseOnDestroyed;
  };

  //using SoundInstanceHandle = Zilch::Handle
  using SoundInstancePtr = std::shared_ptr<SoundInstance>;
  using SoundInstanceHandle = Zilch::Handle;
  //using SoundInstanceZilchHandle = Zilch::Handle;
  using SoundInstanceContainer = std::map<SoundInstancePtr, SoundCueHandle>;

}