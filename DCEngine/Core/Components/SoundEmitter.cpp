/******************************************************************************/
/*!
@file   SoundEmitter.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/23/2015
@brief 
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "SoundEmitter.h"
#include "EngineReference.h"

namespace DCEngine {
  namespace Components
  {

    /**************************************************************************/
    /*!
    @brief Provides the definition of this class to Zilch.
    @note This can only go in the translational unit (.cpp)
    */
    /**************************************************************************/
#if(DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDefineType(SoundEmitter, "SoundEmitter", DCEngineCore, builder, type) {
      //DCE_BINDING_INTERNAL_COMPONENT_SET_HANDLE_TYPE;
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(SoundEmitter);
      //DCE_BINDING_DEFINE_ATTRIBUTE(Resource);
      DCE_BINDING_DEFINE_RESOURCE_ATTRIBUTE(SoundCue);
      // Constructor / Destructor
      //ZilchBindConstructor(builder, type, SoundEmitter, "owner", Entity&);
      //ZilchBindDestructor(builder, type, SoundEmitter);
      // Properties
      DCE_BINDING_DEFINE_PROPERTY(SoundEmitter, Cue); // ->Attributes.push_back(attributeSoundCue);
      DCE_BINDING_PROPERTY_SET_RESOURCE_ATTRIBUTE(propertyCue, attributeSoundCue);
      DCE_BINDING_DEFINE_PROPERTY(SoundEmitter, Volume);
      DCE_BINDING_DEFINE_PROPERTY(SoundEmitter, Pitch);
      DCE_BINDING_DEFINE_PROPERTY(SoundEmitter, IsPlaying);
      //ZilchBindProperty(builder, type, &SoundEmitter::getVolume, &SoundEmitter::setVolume, "Volume");
      //ZilchBindProperty(builder, type, &SoundEmitter::getPitch, &SoundEmitter::setPitch, "Pitch");
      //ZilchBindProperty(builder, type, &SoundEmitter::getIsPlaying, &SoundEmitter::setIsPlaying, "IsPlaying");
    }
#endif


    void SoundEmitter::Initialize()
    {

    }

    /**************************************************************************/
    /*!
    @brief  Plays a 'SoundCue' by sending a request to the SoundSpace.
    @param  soundCue The name of the 'SoundCue' to play.
    */
    /**************************************************************************/
    void SoundEmitter::PlayCue(String soundCue)
    {
      CurrentSoundCue = soundCue;
      SpaceRef->getComponent<SoundSpace>()->PlayCue(soundCue);
    }

    /**************************************************************************/
    /*!
    @brief  Stops the currently playing 'Soundue'
    */
    /**************************************************************************/
    void SoundEmitter::Stop()
    {
      SpaceRef->getComponent<SoundSpace>()->StopCue(CurrentSoundCue);
    }



  }
}