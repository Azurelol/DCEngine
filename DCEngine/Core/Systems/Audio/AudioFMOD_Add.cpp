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

#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    \brief  Loads a sound into memory in its entirety, decompressing it if
    necessary.
    */
    /**************************************************************************/
    bool AudioFMOD::CreateSound(const std::string& soundFile, FMOD::Sound** soundPtr) {
      DCTrace << "AudioFMOD::CreateSound: " << soundFile << "\n";
      return ErrorCheck(System.LowLevel->createSound(soundFile.c_str(), FMOD_CREATESAMPLE, 0, soundPtr));
    }

    /**************************************************************************/
    /*!
    \brief  Opens a file and buffers it a piece at a time, decompressing each
    bufferred segment on the fly during playback.
    */
    /**************************************************************************/
    bool AudioFMOD::CreateStream(const std::string& soundFile, FMOD::Sound** soundPtr) {
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
    FMOD::Studio::EventInstance * AudioFMOD::CreateEventInstance(FMOD::Studio::EventDescription* event) const
    {
      //DCTrace << "AudioFMOD::CreateEventInstance: \n";
      FMOD::Studio::EventInstance* eventInstance;      
      event->createInstance(&eventInstance);

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
    FMOD::Studio::Bank * AudioFMOD::LoadBankFromFile(const std::string handle, const std::string & path)
    {
      DCTrace << "AudioFMOD::LoadBankFromFile - Loading: '" << handle << "' from file: '" << path << "'\n";
      FMOD::Studio::Bank* newBank;
      // Load the bank into the sound system
      ErrorCheck(System->loadBankFile(path.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &newBank));
      // Now that the bank is finished loading, load its sample data
      ErrorCheck(newBank->loadSampleData());
      // Add it to the container of active banks
      BankInfo bankInfo;
      bankInfo.Name = handle;
      bankInfo.Handle = newBank;
      ActiveBanks.insert(std::pair<const std::string, BankInfo>(handle, bankInfo));

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
      
      FMOD_RESULT result;

      int eventCount = 0;
      result = bank->getEventCount(&eventCount);
      if (result != FMOD_OK) {
        DCTrace << "AudioFMOD::LoadEventDescriptions: No events found!\n";
        return;
      }

      int eventsReturned = 0;
      FMOD::Studio::EventDescription ** eventList = (FMOD::Studio::EventDescription **)malloc(eventCount * sizeof(void *));
      result = bank->getEventList(eventList, eventCount, &eventsReturned);
      if (result != FMOD_OK) {
        DCTrace << "AudioFMOD::LoadEventDescriptions: No events retrieved!\n";
        return;
      }

      for (int i = 0; i < eventsReturned; ++i)
      {
        int buff_sz = 0;
        char path[256] = { 0 };
        result = eventList[i]->getPath(path, 255, &buff_sz);
        //printf("%s\n", path);
        
        auto name = FileSystem::FileNoExtension(path);
        AvailableEvents.insert(std::pair<const std::string, FMOD::Studio::EventDescription*>(name, eventList[i]));
      }

    }

    /**************************************************************************/
    /*!
    @brief  Loads single event instances out of available events.
    @param  bank A pointer to the bank.
    */
    /**************************************************************************/
    void AudioFMOD::LoadEventInstances()
    {
      for (auto& event : AvailableEvents) {
        auto instance = CreateEventInstance(event.second);
        InstantiatedEvents.insert(std::pair<std::string, FMOD::Studio::EventInstance*>(event.first, instance));
      }
    }

    /**************************************************************************/
    /*!
    @brief Loads all VCAs from the bank.
    @param bank A pointer to the bank.
    @param vcas A reference to the container of all VCAs within the bank.
    */
    /**************************************************************************/
    void AudioFMOD::LoadVCAs(FMOD::Studio::Bank * bank, VCAContainer& vcas)
    {
      int count;       
      if (!ErrorCheck(bank->getVCACount(&count)))
        return;

      int vcasFound;
      FMOD::Studio::VCA ** vcaList = (FMOD::Studio::VCA **)malloc(count * sizeof(void *));
      if (!ErrorCheck(bank->getVCAList(vcaList, count, &vcasFound)))
        return;

      for (int i = 0; i < vcasFound; ++i) {
        // Grab the path of the VCA
        int buff_sz = 0;
        char path[256] = { 0 };
        ErrorCheck(vcaList[i]->getPath(path, 255, &buff_sz));
        // Extract the name of the VCA
        auto name = FileSystem::FileNoExtension(path);
        vcas.insert(std::pair<VCAHandle, FMOD::Studio::VCA*>(name, vcaList[i]));
      }      
    }

    /**************************************************************************/
    /*!
    @brief Loads all buses from the bank.
    @param bank A pointer to the bank.
    @param buses A reference to the container of all buses within the bank.
    */
    /**************************************************************************/
    void AudioFMOD::LoadBuses(FMOD::Studio::Bank * bank, BusContainer & buses)
    {
      int count;
      if (!ErrorCheck(bank->getBusCount(&count)))
        return;

      int found;
      FMOD::Studio::Bus** busList = (FMOD::Studio::Bus **)malloc(count * sizeof(void *));
      if (!ErrorCheck(bank->getBusList(busList, count, &found)))
        return;


      for (int i = 0; i < found; ++i) {
        // Grab the path of the VCA
        int buff_sz = 0;
        char path[256] = { 0 };
        ErrorCheck(busList[i]->getPath(path, 255, &buff_sz));
        // Extract the name of the VCA
        auto name = FileSystem::FileNoExtension(path);
        buses.insert(std::pair<BusHandle, FMOD::Studio::Bus*>(name, busList[i]));
      }
    }

    /**************************************************************************/
    /*!
    @brief  Loads all channel groups from the bank.
    @param  bank A pointer to the bank.    
    */
    /**************************************************************************/
    void AudioFMOD::LoadChannelGroups(FMOD::Studio::Bank * bank)
    {

    }


    /**************************************************************************/
    /*!
    @brief  Generates SoundCues from all existing events (EventDescriptions).
    */
    /**************************************************************************/
    void AudioFMOD::GenerateSoundCues()
    {
      for (auto& event : AvailableEvents) {
        // Create the SoundCue
        auto soundCue = SoundCuePtr(new SoundCue(event.first, SoundCue::SoundCueType::Event));
        soundCue->Data.Parameters = getParameters(InstantiatedEvents.at(soundCue->Name()));
        // Add it to the Content system
        Daisy->getSystem<Content>()->AddSoundCue(std::string(event.first), soundCue);

      }
    }



  }
}