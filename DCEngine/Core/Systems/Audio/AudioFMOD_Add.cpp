/******************************************************************************/
/*!
@file   AudioFMOD_Add.cpp
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
    \brief  Loads a sound into memory in its entirety, decompressing it if
    necessary.
    */
    /**************************************************************************/
    void AudioFMOD::CreateSound(std::string& soundFile, FMOD::Sound** soundPtr) {
      DCTrace << "AudioFMOD::CreateSound: " << soundFile << "\n";
      ErrorCheck(System.LowLevel->createSound(soundFile.c_str(), FMOD_CREATESAMPLE, 0, soundPtr));
    }

    /**************************************************************************/
    /*!
    \brief  Opens a file and buffers it a piece at a time, decompressing each
    bufferred segment on the fly during playback.
    */
    /**************************************************************************/
    void AudioFMOD::CreateStream(std::string& soundFile, FMOD::Sound** soundPtr) {
      DCTrace << "AudioFMOD::CreateStream \n";
      ErrorCheck(System.LowLevel->createStream(soundFile.c_str(), FMOD_DEFAULT, 0, soundPtr));
    }

    /**************************************************************************/
    /*!
    @brief  Adds a bank to the FMOD Studio system.
    @param  handle The handle to be used to access the bank.
    @param  path The filepath where the bank is located.
    @return A pointer to the bank.
    */
    /**************************************************************************/
    FMOD::Studio::Bank * AudioFMOD::LoadBankFromFile(std::string handle, std::string & path)
    {
      DCTrace << "AudioFMOD::LoadBankFromFile - Loading: '" << handle << "' from file: '" << path << "'\n";
      FMOD::Studio::Bank* newBank;
      // Load the bank into the sound system
      ErrorCheck(System->loadBankFile(path.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &newBank));
      // Now that the bank is finished loading, load its sample data
      ErrorCheck(newBank->loadSampleData());

      // Add it to the container of active banks
      ActiveBanks.insert(std::pair<std::string, FMOD::Studio::Bank*>(handle, newBank));

      return newBank;
    }

  }
}