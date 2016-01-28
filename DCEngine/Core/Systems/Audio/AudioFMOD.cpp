/******************************************************************************/
/*!
@file   AudioFMOD.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   9/16/2015
@brief  The interface for FMOD.
@note   References:
https://katyscode.wordpress.com/2012/10/05/cutting-your-teeth-on-fmod-
part-1-build-environment-initialization-and-playing-sounds/
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "AudioFMOD.h"

// Headers
#include "..\..\Components\EngineReference.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    \brief  Constructor for the AudioFMOD class.
    */
    /**************************************************************************/
    unsigned AudioFMOD::EventInstancesCreated = 0;
    AudioFMOD::AudioFMOD() {
      MaxChannels = 36;
    }

    /**************************************************************************/
    /*!
    \brief  Destructor for the AudioFMOD class.
    */
    /**************************************************************************/
    AudioFMOD::~AudioFMOD() {
      Terminate();      
    }

    /**************************************************************************/
    /*!
    \brief  Initializes FMOD, configuring it.
    */
    /**************************************************************************/
    void AudioFMOD::Initialize() {

      // Initialize the low level API
      //InitializeLowLevelAPI();
      // Initialize the Studio API
      InitializeStudioAPI();
    }

    /**************************************************************************/
    /*!
    \brief  Initializes FMOD's Low Level API.
    */
    /**************************************************************************/
    void AudioFMOD::InitializeLowLevelAPI()
    {
      FMOD_RESULT operationResult;
      // Check if the version of the DLL is the same as the libraries compiled
      unsigned int version;
      operationResult = System.LowLevel->getVersion(&version);
      if (version < FMOD_VERSION)
        DCTrace << "AudioFMOD::Initialize - Error: Outdated version!\n";

      // Check the number of sound cards
      int driverCount = 0;
      System.LowLevel->getNumDrivers(&driverCount);

      // If there are no sound cards, disable output
      if (driverCount == 0) {
        operationResult = System.LowLevel->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
        ErrorCheck(operationResult);
      }
      // If there is at least one sound card
      else {
        //FMOD_SPEAKERMODE speakermode;
        //System->getDriverInfo(0, &speakermode);
        //result = System->getDriverInfo(0, &)
      }

      // Initialize the instnace with 36 channels
      operationResult = System.LowLevel->init(MaxChannels, FMOD_INIT_NORMAL, NULL);
      ErrorCheck(operationResult);

    }

    /**************************************************************************/
    /*!
    \brief  Initializes FMOD's Low Level and Studio systems.
    */
    /**************************************************************************/
    void AudioFMOD::InitializeStudioAPI()
    {
      // Create the FMOD Studio sound system
      ErrorCheck(System->create(&System.Studio));
      // Set FMOD low level studio pointer
      ErrorCheck(System->getLowLevelSystem(&System.LowLevel));
      // Initialize it
      ErrorCheck(System->initialize(MaxChannels, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr));
    }

    /**************************************************************************/
    /*!
    \brief  Updates FMOD's Low Level and Studio systems.
    */
    /**************************************************************************/
    void AudioFMOD::Update(float dt) {
      // Update the low level API system
      ErrorCheck(System.LowLevel->update());
      // Update the 'Studio' API system
      ErrorCheck(System->update());
    }

    /**************************************************************************/
    /*!
    @brief  Terminates the FMOD systems.
    @todo   It crashes if I try this code??
    */
    /**************************************************************************/
    void AudioFMOD::Terminate() {

      // Unload all banks
      for (auto& bank : ActiveBanks) {
        Unload(bank.second);
      }

      //System.LowLevel->
      //System.LowLevel->getChannelsPlaying().

      //delete(MusicPtr);
      //delete CurrentChannel;
    }

    /**************************************************************************/
    /*!
    \brief Checks for FMOD errors.
    \param result The result of an FMOD function call.
    */
    /**************************************************************************/
    bool AudioFMOD::ErrorCheck(FMOD_RESULT result) {
      if (result != FMOD_OK) {
        DCTrace << "AudioFMOD::ErrorCheck - FMOD error! (" << result << ")"
          << FMOD_ErrorString(result) << "\n";
        return false;
      }
      return true;
    }
    



  }
}