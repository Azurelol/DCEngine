/******************************************************************************/
/*!
@file  SoundSpace                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        .cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/23/2015
@brief
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/

#include "SoundSpace.h"
#include "EngineReference.h"

namespace DCEngine {
  namespace Components {

    /**************************************************************************/
    /*!
    @brief Provides the definition of this class to Zilch.
    @note This can only go in the translational unit (.cpp)
    */
    /**************************************************************************/
    #if(DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDefineType(SoundSpace, "SoundSpace", DCEngineCore, builder, type) {
      //DCE_BINDING_INTERNAL_COMPONENT_SET_HANDLE_TYPE;
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(SoundSpace);
      // Constructor / Destructor
      //ZilchBindConstructor(builder, type, SoundSpace, "owner", Entity&);
      //ZilchBindDestructor(builder, type, SoundSpace);
      // Properties
      ZilchBindMethod(builder, type, &SoundSpace::PlayCueZilch, ZilchNoOverload, "PlayCue", "soundCueName");
      DCE_BINDING_DEFINE_PROPERTY(SoundSpace, Volume);
      DCE_BINDING_DEFINE_PROPERTY(SoundSpace, Pitch);
      DCE_BINDING_DEFINE_PROPERTY(SoundSpace, Pause);
    }
    #endif

    SoundSpace::SoundSpace(Entity & owner) : Component(std::string("SoundSpace"), owner),
      Volume(0), Pitch(0), Pause(0)
    {
    }

    /**************************************************************************/
    /*!
    @brief  Initializes the 'SoundSpace' component.
    */
    /**************************************************************************/
    void SoundSpace::Initialize() {
      Connect(SpaceRef, Events::LogicUpdate, SoundSpace::OnLogicUpdate);
      TestMusic();

      // Register this space to the sound system
      Daisy->getSystem<Systems::Audio>()->Register(*this);
    }

    /**************************************************************************/
    /*!
    @brief  Receives a LogicUpdate event.
    @param  event The update event.
    */
    /**************************************************************************/
    void SoundSpace::OnLogicUpdate(Events::LogicUpdate* event) {
    }

    /**************************************************************************/
    /*!
    @brief  Plays a 'SoundCue', returning a SoundInstance handle to it.
    @param  soundCueName The name of the 'SoundCue' to play.
    @return A SoundInstance, an object that acts as the particular instance
            of that playing of the sound cue.
    */
    /**************************************************************************/
    SoundInstancePtr SoundSpace::PlayCue(std::string soundCueName)
    {
      return Daisy->getSystem<Systems::Audio>()->PlaySound(soundCueName);
    }

    SoundInstancePtr SoundSpace::PlayCue(SoundCuePtr soundCue)
    {
      return Daisy->getSystem<Systems::Audio>()->PlaySound(soundCue);
    }

    SoundInstanceHandle SoundSpace::PlayCueZilch(std::string name)
    {
      return Daisy->getSystem<Systems::Audio>()->PlaySoundZilch(name);
    }

    /**************************************************************************/
    /*!
    @brief  Pauses a 'SoundCue', returning a SoundInstance handle to it.
    @param  soundCueName The name of the 'SoundCue' to play.
    */
    /**************************************************************************/
    void SoundSpace::PauseCue(std::string soundCueName)
    {
      Daisy->getSystem<Systems::Audio>()->PauseSound(soundCueName);
    }

    /**************************************************************************/
    /*!
    @brief  Resumes a 'SoundCue', returning a SoundInstance handle to it.
    @param  soundCueName The name of the 'SoundCue' to play.
    */
    /**************************************************************************/
    void SoundSpace::ResumeCue(std::string soundCueName)
    {
      Daisy->getSystem<Systems::Audio>()->ResumeSound(soundCueName);
    }

    /**************************************************************************/
    /*!
    @brief  Stops a 'SoundCue' from playing.
    @param  soundCueName The name of the 'SoundCue' to stop.
    */
    /**************************************************************************/
    void SoundSpace::StopCue(std::string soundCueName)
    {
      // Do nothing if no name was passed
      if (soundCueName.empty())
        return;

      Daisy->getSystem<Systems::Audio>()->StopSound(soundCueName);
    }


    /* Testers */

    void SoundSpace::TestMusic() {
      // THIS IS MY JAM
      //using namespace Systems;
      //std::string myJam = "spacejam.mp3";
      //Daisy->getSystem<Systems::Audio>(EnumeratedSystem::Audio)->PlayMusic(myJam);
    }


  }
}