#pragma once
#include "Timer.h"

namespace DCEngine {
  
  /**************************************************************************/
  /*!
  @class SystemTimer Times a system's update.
  */
  /**************************************************************************/
  class SystemMethodTimer : public Time::FunctionTimer {
  public:   
    SystemMethodTimer(std::string methodName, EnumeratedSystem system);
    ~SystemMethodTimer();
    virtual void Report();
  private:
    const std::string MethodName;
    EnumeratedSystem System;
  };


  /**************************************************************************/
  /*!
  @class SystemTimer Times a system's update.
  */
  /**************************************************************************/
  class SystemTimer : public Time::FunctionTimer {
  public:
    SystemTimer(std::string systemName);
    ~SystemTimer();
    virtual void Report();
  private:
    const std::string SystemName;
  };

  /**************************************************************************/
  /*!
  @class Profiler Runs various diagnostics on the engine and stores that 
         data.
  @todo  Decide whether to use a branching Add method and enums for different
         types or different methods like being done currently.
  */
  /**************************************************************************/
  class Profiler {
  public:
    void Add(Time::FunctionTimeSlice systemTime);
    void Add(Time::FunctionTimeSlice systemTime, EnumeratedSystem system);
    float FPS();
    void Update(float dt);
    Time::FunctionTimeSliceVec& SystemTimes();
    Time::FunctionTimeSliceVec& Graphics();
    Time::FunctionTimeSliceVec& Physics();

  private:
    float CurrentFPS;    
    Time::FunctionTimes GraphicsSystemTimes;
    Time::FunctionTimes PhysicsSystemTimes;
    Time::FunctionTimes EngineSystemTimes;
    void AddSystemTime(Time::FunctionTimeSliceVec& container, Time::FunctionTimeSlice& time);
    void CalculateFPS(float dt);
    void Clear();
  };

}
