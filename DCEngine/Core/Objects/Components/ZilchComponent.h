/*****************************************************************************/
/*!
\file   ZilchComponent.h
\author Christian Sagel, Connor Tilley
\par    email: c.sagel\@digipen.edu
\date   9/22/2015
\brief  Zilch components are used by gameplay programmers to write game logic into.
These components, alongside events, drive the logic of a game project.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once

#include <ZILCH\Zilch.hpp>
#include "../Component.h"
//#include "../../Events/UpdateEvents.h"

namespace DCEngine {

  // Forward declarations
  class Entity;
  namespace Systems {
    class Factory;
  }
  class Engine;

  class ZilchComponent : public Component {
    using ZilchOrigin = std::string;
    using ZilchFile = std::string;
    using ZilchCode = std::string;

    friend class Entity;
    friend class Factory;
    friend class Engine;

  public:
#if (DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDeclareDerivedType(ZilchComponent, Component);
#endif
    ZilchComponent(std::string name, ZilchFile scriptName, Entity& owner);
      static bool IsZilchComponent(ComponentPtr component);
      //ZilchComponent() : classScript("Example"), zilchId(CTZ_Example) {};
      //ZilchComponent(std::string scriptName, ZilchComponentTypeId zilchId);
      //void OnLogicUpdate(Events::LogicUpdate * event);
      //void CallFunction(std::string);
      //void ZilchConnect(::DCEngine::Event Event, Entity* EntityConnection, Zilch::Function* funct);
      Zilch::Function* GetFieldOrProperty(std::string functName);
      Zilch::Function* SetFieldOrProperty(std::string functName);
      void ZilchComponent::LogicUpdate(::DCEngine::Events::LogicUpdate);
      Zilch::Handle getInstance();

      virtual ~ZilchComponent();
      virtual void Initialize();

  private:
    void CallConnections(::DCEngine::Event Event);

    //Systems::ZilchInterface* Interface;
    Zilch::ExceptionReport report;
    Zilch::BoundType* zilchClass;
    Zilch::Handle classInstance;
    ZilchFile classScript;
    //ZilchComponentTypeId zilchId;
    using EventPair = std::pair<Zilch::Function*, Entity*>;
    using ConnectList = std::list<EventPair>;
    std::map<std::type_index, ConnectList> EventFunctions;
    Zilch::Function* initFunc;
    Zilch::Function* updateFunc;
    Zilch::Function* Destroy;
    

  };

}