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
    bool AudioFMOD::CreateSound(std::string& soundFile, FMOD::Sound** soundPtr) {
      DCTrace << "AudioFMOD::CreateSound: " << soundFile << "\n";
      return ErrorCheck(System.LowLevel->createSound(soundFile.c_str(), FMOD_CREATESAMPLE, 0, soundPtr));
    }

    /**************************************************************************/
    /*!
    \brief  Opens a file and buffers it a piece at a time, decompressing each
    bufferred segment on the fly during playback.
    */
    /**************************************************************************/
    bool AudioFMOD::CreateStream(std::string& soundFile, FMOD::Sound** soundPtr) {
      DCTrace << "AudioFMOD::CreateStream \n";
      return ErrorCheck(System.LowLevel->createStream(soundFile.c_str(), FMOD_DEFAULT, 0, soundPtr));
    }

    /**************************************************************************/
    /*!
    @brief  Creates an event instance from an event description and adds it
            to the container of instantiated events.
    @param  event The name of the event to instantiate.
    */
    /**************************************************************************/
    FMOD::Studio::EventInstance * AudioFMOD::AddEventInstance(FMOD::Studio::EventDescription* event) const
    {
      DCTrace << "AudioFMOD::AddEventInstance: \n";
      FMOD::Studio::EventInstance* eventInstance;
      event->createInstance(&eventInstance);
      
      // Get the name of the event?
      FMOD_GUID* id;
      event->getID(id);

      return eventInstance;
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

    /**************************************************************************/
    /*!
    @brief  Loads all event descriptions from a bank, loading them onto memory
            and adding their identifiers to the common map.
    @param  bank A pointer to the bank.
    */
    /**************************************************************************/
    void AudioFMOD::LoadEventDescriptions(FMOD::Studio::Bank * bank)
    {
      DCTrace << "AudioFMOD::LoadEventDescriptions - Loading event descriptions \n";

      int eventCount = 0;
      if (!bank->getEventCount(&eventCount)) {
        DCTrace << "AudioFMOD::LoadEventDescriptions: No events found!\n";
        return;
      }

      int eventsReturned = 0;
      FMOD::Studio::EventDescription ** eventList = (FMOD::Studio::EventDescription **)malloc(eventCount * sizeof(void *));
      if (!bank->getEventList(eventList, eventCount, &eventsReturned)) {
        DCTrace << "AudioFMOD::LoadEventDescriptions: No events retrieved!\n";
        return;
      }

    }      


  }
}