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
  @brief  SoundCue constructor.
  */
  /**************************************************************************/
  SoundCue::SoundCue(std::string soundFile) : Resource(FileSystem::FileNoExtension(soundFile)), 
                                              SoundFileName(soundFile) {
    //Load();
  }

  /**************************************************************************/
  /*!
  @brief  Loads this Sound pointer by interfacing with the audio system.
  */
  /**************************************************************************/
  void SoundCue::Load()
  {
    Daisy->getSystem<Systems::Audio>()->CreateSound(std::string(SoundFileName), Data);
    DCTrace << "SoundCue::Load - Finished loading " << SoundFileName << "!\n";
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