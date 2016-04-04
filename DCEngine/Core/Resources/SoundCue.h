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
#include "../Systems/Audio/AudioFMOD_Utilities.h"

namespace DCEngine {

  class SoundCue;
  using SoundCueRawPtr = SoundCue*;
  using SoundCuePtr = std::shared_ptr<SoundCue>;
  using SoundCueHandle = std::string;

  enum class PlayMode {
    Single,
    Looping,
  };

  /**************************************************************************/
  /*!
  @class PlaybackSettings A collection of audio playback settings.
  */
  /**************************************************************************/
  struct PlaybackSettings {
    PlaybackSettings() : Mode(PlayMode::Single), Volume(1.0f), VolumeVariation(0.0f),
                         Pitch(1.0f), PitchVariation(0.0f),
                         Loop(false) {}
    PlayMode Mode;
    float Volume = 1.0f;
    float VolumeVariation = 0.0f;
    float Pitch = 1.0f;
    float PitchVariation = 0.0f;
    bool Loop = false;
  };

  /**************************************************************************/
  /*!
  @class SoundCue Encapsulates sound data.
  */
  /**************************************************************************/
  class SoundCue : public Resource {

  public:        
    enum class SoundCueType {
      File, // The SoundCue is generated from a a soun file.
      Event // The SoundCue is generated from a sound Bank.
    };

    // Member variables
    PlayMode Mode; // Add this later
    float Volume = 1.0f;
    float VolumeVariation = 0.0f;
    float Pitch = 1.0f;
    float PitchVariation = 0.0f;
    bool Loop = false;
    SoundCueType Type;
    FMODSoundHandle Data;
    // Properties    
    DCE_DEFINE_PROPERTY(PlayMode, Mode);
    DCE_DEFINE_PROPERTY(bool, Loop);
    DCE_DEFINE_PROPERTY(float, Volume);
    DCE_DEFINE_PROPERTY(float, VolumeVariation);
    DCE_DEFINE_PROPERTY(float, Pitch);
    DCE_DEFINE_PROPERTY(float, PitchVariation);    
    DCE_DEFINE_PROPERTY(String, AssetPath);    
    // Methods
    ZilchDeclareDerivedType(SoundCue, Resource);
    SoundCue(std::string soundFile, SoundCueType type);
    void Generate();
    static std::string Extension() { return ".SoundCue"; }
    static SoundCueRawPtr Find(std::string);

  private:
    std::string AssetPath;

  }; 
}

ZilchDeclareExternalBaseType(DCEngine::PlayMode, Zilch::TypeCopyMode::ValueType);