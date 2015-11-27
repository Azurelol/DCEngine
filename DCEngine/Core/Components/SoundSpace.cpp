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

  /**************************************************************************/
  /*!
  @brief Provides the definition of this class to Zilch.
  @note This can only go in the translational unit (.cpp)
  */
  /**************************************************************************/
  #if(DCE_USE_ZILCH_INTERNAL_BINDING)
  ZilchDefineType(SoundSpace, "SoundSpace", DCEngineCore, builder, type) {
    //DCE_BINDING_INTERNAL_COMPONENT_SET_HANDLE_TYPE;
    // Constructor / Destructor
    ZilchBindConstructor(builder, type, SoundSpace, "owner", Entity&);
    ZilchBindDestructor(builder, type, SoundSpace);
    // Properties
    DCE_BINDING_DEFINE_PROPERTY(SoundSpace, Volume);
    DCE_BINDING_DEFINE_PROPERTY(SoundSpace, Pitch);
    DCE_BINDING_DEFINE_PROPERTY(SoundSpace, Pause);
  }
  #endif

    /**************************************************************************/
    /*!
    @brief  Initializes the 'SoundSpace' component.
    */
    /**************************************************************************/
    void SoundSpace::Initialize() {
      //Daisy->Connect<Events::LogicUpdate>(SpaceRef, &SoundSpace::OnLogicUpdate, this);
      Connect(SpaceRef, Events::LogicUpdate, SoundSpace::OnLogicUpdate);
      TestMusic();

      // Register this space to the sound system
      Daisy->getSystem<Systems::Audio>()->Register(*this);
    }

    void SoundSpace::OnLogicUpdate(Events::LogicUpdate* event) {
      //DCTrace << "SoundSpace::OnLogicUpdate \n";
    }

    /**************************************************************************/
    /*!
    @brief  Plays a 'SoundCue', forwarding the data to the Audio system.
    @param  soundCueName The name of the 'SoundCue' to play.
    */
    /**************************************************************************/
    void SoundSpace::PlayCue(std::string soundCueName)
    {
      Daisy->getSystem<Systems::Audio>()->PlaySound(soundCueName);
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