/*****************************************************************************/
/*!
@file   SoundCue.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   10/04/2015
@brief  The SoundCue resource encapsulates a sound file.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include "ResourceReference.h"
// Headers
#include "../Systems/Audio/FMODPtrs.h"

namespace DCEngine {

  class SoundCue;
  using SoundCuePtr = std::shared_ptr<SoundCue>;
  using SoundCueHandle = std::string;

  enum class PlayMode {
    Single,
    Looping,
  };

  class SoundCue : public Resource {

  public:

    #if(DCE_USE_ZILCH_INTERNAL_BINDING) 
    ZilchDeclareDerivedType(SoundCue, Resource);
    #endif

    // Member variables
    PlayMode PlayMode; // Add this later
    float Volume = 1.0f;
    float VolumeVariation = 0.0f;
    float Pitch = 1.0f;
    float PitchVariation = 0.0f;
    bool Loop = false;
    FMODSoundPtr Data;
    
    // Properties
    DCE_DEFINE_PROPERTY(String, AssetPath);
    DCE_DEFINE_PROPERTY(bool, Loop);
    DCE_DEFINE_PROPERTY(float, Volume);
    DCE_DEFINE_PROPERTY(float, VolumeVariation);
    DCE_DEFINE_PROPERTY(float, Pitch);
    DCE_DEFINE_PROPERTY(float, PitchVariation);    

    // Methods
    SoundCue(std::string soundFile);
    void GenerateSound();
    static std::string Extension() { return ".SoundCue"; }
    static SoundCuePtr Find(std::string);

  private:
    std::string AssetPath;
  };


 
}