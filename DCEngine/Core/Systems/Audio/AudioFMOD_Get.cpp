/******************************************************************************/
/*!
@file   AudioFMOD_Get.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   1/24/2016
@brief  The interface for FMOD.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "AudioFMOD.h"

namespace DCEngine {
  namespace Systems {


    /**************************************************************************/
    /*!
    @brief  Retrieves a pointer to a given sound bank.
    @param  handle The name of the sound bank.
    @return A pointer to the bank.
    */
    /**************************************************************************/
    FMOD::Studio::Bank * AudioFMOD::getBank(std::string handle)
    {
      // If the bank could not be found
      if (!ActiveBanks.count(handle))
        return nullptr;
      // If the bank could be found...
      return ActiveBanks.at(handle);
    }

    /**************************************************************************/
    /*!
    @brief  Retrieves the bus.
    @param  path The path to the bus.
    @return
    */
    /**************************************************************************/
    FMOD::Studio::Bus * AudioFMOD::getBus(std::string path)
    {
      return nullptr;
    }

    FMOD_RESULT  AudioFMOD::getBus(std::string path, FMOD::Studio::Bus ** bus) const
    {
      return FMOD_RESULT();
    }

    /**************************************************************************/
    /*!
    @brief  Generates audio resources from all currently loaded banks.
    */
    /**************************************************************************/
    void AudioFMOD::GenerateResources()
    {
      for (auto bank : ActiveBanks) {
        DCTrace << "AudioFMOD::GenerateResources: Generating resources for bank '" << bank.first << "' \n";
        // Load event descriptions
        LoadEventDescriptions(bank.second);
        // Load channel groups
        LoadChannelGroups(bank.second);
        // Load VCAs
        LoadVCAs(bank.second);
      }

      // Generate SoundCues
      GenerateSoundCues();

    }




  }
}