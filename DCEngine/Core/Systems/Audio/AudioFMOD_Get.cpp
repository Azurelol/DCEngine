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
      return ActiveBanks.at(handle).Handle;
    }

    /**************************************************************************/
    /*!
    @brief  Retrieves a reference to the bus.
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
    @brief  Retrieves a reference to the bus.
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
    @brief  Retrieves a reference to the VCA.
    @param  path The path to the VCA.
    @return The success of the operation.
    */
    /**************************************************************************/
    FMOD::Studio::VCA * AudioFMOD::getVCA(const std::string& path)
    {
      FMOD::Studio::VCA* vca;      
      ErrorCheck(getVCA(path, &vca));
      return vca;
    }

    FMOD_RESULT AudioFMOD::getVCA(const std::string& path, FMOD::Studio::VCA ** vca) const
    {
      return FMOD_RESULT();
    }






  }
}