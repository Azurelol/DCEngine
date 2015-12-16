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

namespace DCEngine {
  
  /**************************************************************************/
  /*!
  @brief Provides the definition of this class to Zilch for reflection.
  */
  /**************************************************************************/
  #if(DCE_USE_ZILCH_INTERNAL_BINDING)
  ZilchDefineType(SoundCue, "SoundCue", DCEngineCore, builder, type) {
    DCE_BINDING_DEFINE_RESOURCE_ATTRIBUTE(Sound);
    DCE_BINDING_DEFINE_PROPERTY(SoundCue, Loop);
    DCE_BINDING_DEFINE_PROPERTY(SoundCue, Volume);
    DCE_BINDING_DEFINE_PROPERTY(SoundCue, VolumeVariation);
    DCE_BINDING_DEFINE_PROPERTY(SoundCue, Pitch);
    DCE_BINDING_DEFINE_PROPERTY(SoundCue, PitchVariation);
    DCE_BINDING_DEFINE_PROPERTY(SoundCue, AssetPath);
    DCE_BINDING_PROPERTY_SET_RESOURCE_ATTRIBUTE(propertyAssetPath, attributeSound);
    
  }
  #endif

  /**************************************************************************/
  /*!
  @brief  SoundCue constructor.
  */
  /**************************************************************************/
  SoundCue::SoundCue(std::string soundFile) : 
                     Resource("SoundCue", FileSystem::FileNoExtension(soundFile), soundFile)
  {
  }

  /**************************************************************************/
  /*!
  @brief  Loads this Sound pointer by interfacing with the audio system.
  */
  /**************************************************************************/
  void SoundCue::GenerateSound()
  {
    Daisy->getSystem<Systems::Audio>()->CreateSound(std::string(AssetPath), Data);
    DCTrace << "SoundCue::Load - Finished loading " << Name() << "!\n";
    //Data.Channel->set
  }

  /**************************************************************************/
  /*!
  @brief  Returns the SoundCue resource to be used by the graphics system.
  @return A reference to the SoundCue object.
  */
  /**************************************************************************/
  SoundCuePtr SoundCue::Find(std::string name)
  {
    return Daisy->getSystem<Systems::Content>()->getSoundCue(name);
  }

}