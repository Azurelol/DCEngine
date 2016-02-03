/******************************************************************************/
/*!
@file   SoundCue.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/23/2015
@brief
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "SoundCue.h"
// Engine
#include "../../Core/Engine/Engine.h"
//#include "../Components/EngineReference.h"

/**************************************************************************/
/*!
@brief Binds the PlayMode enum class.
*/
/**************************************************************************/
ZilchDefineExternalType(DCEngine::PlayMode, "PlayMode", DCEngine::DCEngineCore, builder, type) {
  ZilchBindEnum(builder, type, SpecialType::Enumeration);
  ZilchBindEnumValue(builder, type, DCEngine::PlayMode::Single, "Single");
  ZilchBindEnumValue(builder, type, DCEngine::PlayMode::Looping, "Looping");
}

namespace DCEngine {
  
  /**************************************************************************/
  /*!
  @brief Provides the definition of this class to Zilch for reflection.
  */
  /**************************************************************************/
  #if(DCE_USE_ZILCH_INTERNAL_BINDING)
  ZilchDefineType(SoundCue, "SoundCue", DCEngineCore, builder, type) {
    DCE_BINDING_PROPERTY_DEFINE_UNSIGNED;  
    DCE_BINDING_DEFINE_RESOURCE_ATTRIBUTE(Sound);
    //DCE_BINDING_DEFINE_PROPERTY(SoundCue, Mode);
    DCE_BINDING_DEFINE_PROPERTY(SoundCue, Loop);
    DCE_BINDING_DEFINE_PROPERTY(SoundCue, Volume);
    DCE_BINDING_PROPERTY_SET_UNSIGNED(propertyVolume);
    DCE_BINDING_DEFINE_PROPERTY(SoundCue, VolumeVariation);
    DCE_BINDING_PROPERTY_SET_UNSIGNED(propertyVolumeVariation);
    DCE_BINDING_DEFINE_PROPERTY(SoundCue, Pitch);
    DCE_BINDING_PROPERTY_SET_UNSIGNED(propertyPitch);
    DCE_BINDING_DEFINE_PROPERTY(SoundCue, PitchVariation);
    DCE_BINDING_PROPERTY_SET_UNSIGNED(propertyPitchVariation);
    DCE_BINDING_DEFINE_PROPERTY(SoundCue, AssetPath);
    DCE_BINDING_PROPERTY_SET_RESOURCE_ATTRIBUTE(propertyAssetPath, attributeSound);    
  }
  #endif

  /**************************************************************************/
  /*!
  @brief  SoundCue constructor.
  */
  /**************************************************************************/
  SoundCue::SoundCue(std::string soundFile, WhatType type) : 
                     Resource("SoundCue", FileSystem::FileNoExtension(soundFile), soundFile), Type(type)
  {
  }

  /**************************************************************************/
  /*!
  @brief  Loads this Sound pointer by interfacing with the audio system.
  */
  /**************************************************************************/
  void SoundCue::Generate()
  {
    Daisy->getSystem<Systems::Audio>()->CreateSound(std::string(AssetPath), Data);
    DCTrace << "SoundCue::Load - Finished loading " << Name() << "!\n";
    //Data.Channel->set
  }

  /**************************************************************************/
  /*!
  @brief  Returns the specified SoundCue resource.
  @return A reference to the SoundCue object.
  */
  /**************************************************************************/
  SoundCuePtr SoundCue::Find(std::string name)
  {
    return Daisy->getSystem<Systems::Content>()->getSoundCue(name);
  }



}