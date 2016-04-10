/******************************************************************************/
/*!
\file   Engine.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   7/30/2015
\brief  The main driver for the Daisy Chain engine, and its engine class,
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once

// Libraries
#include <unordered_map>
#include <stack>
#include <functional> // std::function, std::bind
// Packages
#include "Types.h"
#include "../Events/EventReference.h"
#include "Math.h"
#include "Actions.h"
#include "Profiler.h"
// Objects
#include "../Objects/ObjectsInclude.h"
#include "../Objects/Entities/EntitiesInclude.h"
#include "..\Systems\System.h"
#include "..\Systems\SystemsInclude.h"
#include "../Binding/CoreBinding.h"
#include "../Binding/CoreBindingTypes.h"
#include "../Binding/CoreBindingObjects.h"
#include "..\Binding\CoreBindingEnums.h"

namespace DCEngine {

  extern std::unique_ptr<Engine> Daisy;

  /**************************************************************************/
  /*!
  @class Engine The engine class encapsulates the whole of the engine's
         systems, objects and resources.
  */
  /**************************************************************************/
  class Engine : public Object {
    friend class EngineLauncher;
  public:

    void Register(ActionPtr action);
    void Deregister(ActionPtr action);
    ZilchDeclareDerivedType(Engine, Object);
    Engine();
    Engine(std::string configFile);
    ~Engine();
    void Initialize();
    void Loop();
    void Terminate();
    auto Stop() { Active = false; }
    float Dt() { return DeltaTime; }

    Keyboard* getKeyboard() { return KeyboardHandle.get(); }
    Mouse* getMouse() { return MouseHandle.get(); }
    GameSession* getGameSession() { return CurrentGameSession.get(); }

    // Events (Template)
    template <typename EventClass, typename Class, typename MemberFunction>
    void Connect(Entity* publisher, MemberFunction fn, Class* inst);
    template<typename EventClass, typename Class, typename MemberFunction>
    void Connect(std::string eventName, Entity* publisher, MemberFunction fn, Class* inst);
    template <typename Publisher, typename Observer>
    void Disconnect(Publisher* publisher, Observer* observer);
    // Events
    void ConnectTo(const std::string& eventName, Entity* publisher, EventDelegate* deleg, Component* inst);
    void ZilchDisconnect(Zilch::Call& call, Zilch::ExceptionReport& report);
    // System Events
    template<typename EventClass, typename SystemClass, typename MemberFunction>
    void Connect(MemberFunction fn, SystemClass* sys);
    template <typename EventClass> void Dispatch(Event* eventObj);

    template<typename T> std::shared_ptr<T> getSystem();
    Profiler& Profiler() { return this->Profile; }
    Systems::ConfigurationFiles& Configuration() { return this->Configurations; }

  private:
        
    bool Active;  
    float DeltaTime;
    bool Paused;
    DCEngine::Profiler Profile;
    Systems::ConfigurationFiles Configurations;
    EngineStatistics Statistics;
    EngineConfigPtr EngineConfiguration;
    GameSessionStrongPtr CurrentGameSession;
    KeyboardPtr KeyboardHandle;
    MousePtr MouseHandle;
    std::string _projectName = "Daisy Project"; 
    std::string _defaultSpace = "Daisy World";
    SystemVec Systems;   
    SpaceMap Spaces; 
    ActionSpace ActionSpace; 
    std::map<std::type_index, std::list<DCEngine::EventDelegate*>> ObserverRegistry;
    // Loading actions
    void DisplayLoadingScreen(bool enable);
    void ConstructSystems();
    void LoadDefaultSpace();
    // Updates
    void Update(float dt);   
    void DispatchUpdateEvents(float dt);    
    void UpdateActions(float dt);
    void LoadConfigurationFiles(const std::string&);
    template <typename Type> bool LoadConfiguration(Type& config, std::string fileName);
    // Events
    void Subscribe();
    void OnContentProjectLoadedEvent(Events::ContentProjectLoaded* event);
    void OnWindowLostFocusEvent(Events::WindowLostFocus* event);
    void OnWindowGainedFocusEvent(Events::WindowGainedFocus* event);    
    void OnEnginePauseEvent(Events::EnginePause* event);
    void OnEngineResumeEvent(Events::EngineResume* event);
    void OnEngineExitEvent(Events::EngineExit* event);
    void OnEnginePauseMenuEvent(Events::EnginePauseMenu* event);
    void OnEngineSaveConfigurationsEvent(Events::EngineSaveConfigurations* event);    
  }; 

  /**************************************************************************/
  /*!
  @class EngineLauncher Helper class that launches the engine,
  */
  /**************************************************************************/
  class EngineLauncher {
  public:
    EngineLauncher() {
      engine_.reset(new Engine());
    }
    void InitializeEngine() {
      engine_->Initialize();
    }
    void StartLoop() {
      engine_->Loop();
    }
    void TerminateEngine() {
      engine_->Terminate();
    }
  private:
    std::unique_ptr<Engine> engine_;
  };

} 

// Templates
#include "Engine.hpp"