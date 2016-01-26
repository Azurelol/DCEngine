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
    Daisy->getSystem<Systems::Audio>()->Add(AssetPath, Data);

    int result = 0;
    // Get the number of events in the bank
    int eventCount = 0;
    result = Data.Handle->getEventCount(&eventCount);
    // Generate a container of "Event Descriptions"
    //FMOD::Studio::EventDescription *eventList = nullptr;
    //auto result = mData.Handle->getEventList(&eventList, eventCount, &eventsReturned);

    if (result != FMOD_OK) {
      DCTrace << Name() << "::Bank::Generate: No events found!\n";
      return;
    }

    int eventsReturned = 0;
    FMOD::Studio::EventDescription ** eventList = (FMOD::Studio::EventDescription **)malloc(eventCount * sizeof(void *));
    result = Data->getEventList(eventList, eventCount, &eventsReturned);

    if (result != FMOD_OK) {
      DCTrace << Name() << "::Bank::Generate: Failed to retrieve EventDescriptions from the bank!\n";
      return;
    }
    
    for (int i = 0; i < eventCount; ++i)
    {
      int buff_sz = 0;
      char path[256] = { 0 };
      FMOD_GUID* what;
      result = eventList[i]->getID(what);
      result = eventList[i]->getPath(path, 255, &buff_sz);
      printf("%s\n", path);
    }

    //std::string name;
    //for (unsigned i = 0; i < eventCount; ++i) {
    //  result = eventList[i]->getPath(name., 255, &eventsReturned);
    //  DCTrace << "lol";
    //}


    free(eventList);


    //eventList.reserve(100);
    //FMOD::Studio::EventDescription *eventList = nullptr;
    //auto result = mData.Handle->getEventList(&eventList, eventCount, &eventsReturned);
    //FMOD::Studio::EventInstance *instList = nullptr;
    //int instCount = 0;
    //result = eventList->getInstanceCount(&instCount);
    //result = eventList->getInstanceList(&instList, eventCount, &instCount);

    // For every event description, generate a SoundCue
    //for (auto& event : eventList) {
    //}

    DCTrace << Name() << "::Bank::Generate: There were '" << eventCount << "' events in the bank \n";
  }

}