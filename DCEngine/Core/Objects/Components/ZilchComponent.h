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
  namespace Systems {
    class Factory;
  }
  class Entity;
  class Engine;
  
    class ZilchComponent : public Component {
      using ZilchOrigin = std::string;
      using ZilchFile = std::string;
      using ZilchCode = std::string;

      friend class Entity;
      friend class Factory;
      friend class Engine;

    public:
      static bool IsZilchComponent(ComponentPtr component);
      Zilch::BoundType* BoundType();
      ZilchDeclareDerivedType(ZilchComponent, Component);
      ZilchComponent(std::string name, Entity& owner);
      ZilchComponent();
      virtual ~ZilchComponent();
      virtual void Initialize();
      void Serialize(Zilch::JsonBuilder& builder);
      void Deserialize(Zilch::JsonValue* properties);

    private:
      // Functions
      void FindFunctions();
      // LogicUpdate method
      bool LogicUpdateDirectly;
      void OnLogicUpdateEvent(Events::LogicUpdate* event);

      //ZilchComponent() : classScript("Example"), zilchId(CTZ_Example) {};
      //ZilchComponent(std::string scriptName, ZilchComponentTypeId zilchId);
      //void OnLogicUpdate(Events::LogicUpdate * event);
      //void CallFunction(std::string);
      //void ZilchConnect(::DCEngine::Event Event, Entity* EntityConnection, Zilch::Function* funct);
      Zilch::Function* GetFieldOrProperty(std::string functName);
      Zilch::Function* SetFieldOrProperty(std::string functName);
    private:
      // Members
      Zilch::ExceptionReport Report;
      void CallConnections(::DCEngine::Event Event);
      // Functions
      using EventPair = std::pair<Zilch::Function*, Entity*>;
      using ConnectList = std::list<EventPair>;
      std::map<std::type_index, ConnectList> EventFunctions;
      Zilch::Function* InitializeFunc;
      Zilch::Function* OnLogicUpdateFunc;
      Zilch::Function* DestroyFunc;

    };

  
}