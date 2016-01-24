/*****************************************************************************/
/*!
@file      Bank.cpp
@author    Christian Sagel
@par       email: c.sagel\@digipen.edu
@date      1/22/2016
@brief     A bank..
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "Bank.h"

#include "../../Core/Engine/Engine.h"

namespace DCEngine {

  /**************************************************************************/
  /*!
  @brief Provides the definition of this class to Zilch for reflection.
  */
  /**************************************************************************/
#if(DCE_USE_ZILCH_INTERNAL_BINDING)
  ZilchDefineType(Bank, "Bank", DCEngineCore, builder, type) {
    DCE_BINDING_DEFINE_PROPERTY(Bank, AssetPath);
  }
#endif

  /**************************************************************************/
  /*!
  @brief  CollisionGroup constructor.
  */
  /**************************************************************************/
  Bank::Bank(std::string bankFile) : Resource("Bank", FileSystem::FileNoExtension(bankFile),
                                              bankFile)
  {
  }

  /**************************************************************************/
  /*!
  @brief Returns the specified Bank resource.
  @param name The name of the bank.
  @return A reference to the Bank object.
  */
  /**************************************************************************/
  BankPtr Bank::Find(std::string name)
  {
    return Daisy->getSystem<Systems::Content>()->getBank(name);
  }

  /**************************************************************************/
  /*!
  @brief  Generates a list of SoundCues for every Event in the bank.
  */
  /**************************************************************************/
  void Bank::Generate()
  {
    DCTrace << Name() << "::Bank::Generate \n";
    // Add the bank to the audio system
    Daisy->getSystem<Systems::Audio>()->Add(AssetPath, mData);
    
    // Get the number of events in the bank
    int eventNumber = 0;
    mData.Handle->getEventCount(&eventNumber);
    // Generate a container of "Event Descriptions"
    std::vector<FMOD::Studio::EventDescription*> eventList;
    eventList.reserve(100);
    auto result = mData.Handle->getEventList(eventList.data(), eventList.capacity(), &eventNumber);
    
    // For every event description, generate a SoundCue
    for (auto& event : eventList) {

    }



    DCTrace << Name() << "::Bank::Generate: There were '" << eventNumber << "' events in the bank \n";
  }

}