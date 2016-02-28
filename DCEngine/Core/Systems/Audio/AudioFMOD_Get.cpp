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
    @brief  Retrieves a list of event parameters.
    @param  handle The name of the sound bank.
    @return A container of event parameter instances.
    */
    /**************************************************************************/
    EventParameterInfoContainer AudioFMOD::getParameters(EventInstanceHandle instance)
    {
      EventParameterContainer parameters;
      EventParameterInfoContainer parametersInfo;

      // Get the number of parameters
      int parameterCount = 0;
      ErrorCheck(instance->getParameterCount(&parameterCount));
      
      // Get the parameters
      int parametersRetrieved = 0;
      //FMOD::Studio::ParameterInstance** paramList = (FMOD::Studio::ParameterInstance**)malloc(parameterCount * sizeof(void*));

      // Grab every parameter
      parameters.resize(static_cast<size_t>(parameterCount));
      for (int i = 0; i < parameterCount; ++i) {
        // Save an instance of the parameter
        instance->getParameterByIndex(i, &parameters[i]);        
        // Record its settings and name
        FMOD_STUDIO_PARAMETER_DESCRIPTION paramDescription;        
        auto paramInfo = parameters[i]->getDescription(&paramDescription);
        EventParameterInfo info;
        info.Name = paramDescription.name;
        info.Minimum = paramDescription.minimum;
        info.Maximum = paramDescription.maximum;
        info.Type = paramDescription.type;
      }
      
      return parametersInfo;
    }

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
      FMOD::Studio::Bus* bus;
      ErrorCheck(getBus(path, &bus));
      return bus;
    }

    /**************************************************************************/
    /*!
    @brief  Retrieves the bus.
    @param  path The path to the bus.
    @return The success of the operation.
    */
    /**************************************************************************/
    FMOD_RESULT AudioFMOD::getBus(std::string path, FMOD::Studio::Bus ** bus) const
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
        // Load event instances
        LoadEventInstances();
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