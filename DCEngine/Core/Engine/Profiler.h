#pragma once
#include "Timer.h"

namespace DCEngine {

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
  using SystemTimeSlice = std::pair<std::string, float>;
  using SystemTimeSliceVec = std::vector<SystemTimeSlice>;

  /**************************************************************************/
  /*!
  @class Profiler Runs various diagnostics on the engine and stores that 
         data.
  */
  /**************************************************************************/
  class Profiler {
  public:
    void Add(SystemTimeSlice systemTime);
    float FPS();
    void Update(float dt);
    SystemTimeSliceVec SystemTimes;
    SystemTimeSliceVec NextSystemTimes;
    

  private:
    float CurrentFPS;
    
    void CalculateFPS(float dt);
    void Clear();
  };

}
