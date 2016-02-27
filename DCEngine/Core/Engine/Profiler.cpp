#include "Profiler.h"
#include "Engine.h"

namespace DCEngine {
  
  /**************************************************************************/
  /*!
  @brief Adds a system's time to the profiler.
  @param systemTime The system and the time elapsed during its udpate method.
  */
  /**************************************************************************/
  void Profiler::Add(Time::FunctionTimeSlice systemTime)
  {
    NextSystemTimes.push_back(systemTime);
  }

  /**************************************************************************/
  /*!
  @brief Adds a system method to the profiler.
  @param tiime The method and the time elapsed during its execution.
  */
  /**************************************************************************/
  void Profiler::Add(Time::FunctionTimeSlice time, EnumeratedSystem system)
  {
    switch (system) {

    case EnumeratedSystem::Graphics:
    GraphicsSystemTimes.second.push_back(time);
      break;

    case EnumeratedSystem::Physics:
    PhysicsSystemTimes.second.push_back(time);
      break;

    }
  }


  /**************************************************************************/
  /*!
  @brief Clear.
  */
  /**************************************************************************/
  void Profiler::Clear()
  {
    // Graphics
    GraphicsSystemTimes.first = GraphicsSystemTimes.second;
    GraphicsSystemTimes.second.clear();
    // Physics
    PhysicsSystemTimes.first = PhysicsSystemTimes.second;
    PhysicsSystemTimes.second.clear();
    // Systems
    CurrentSystemTimes = NextSystemTimes;
    NextSystemTimes.clear();
  }

  /**************************************************************************/
  /*!
  @brief Returns the engine's FPS.
  @return The engine's FPS.
  */
  /**************************************************************************/
  float Profiler::FPS()
  {
    return CurrentFPS;
  }

  /**************************************************************************/
  /*!
  @brief Updates the profiler
  @param dt The delta time.
  */
  /**************************************************************************/
  void Profiler::Update(float dt)
  {
    CalculateFPS(dt);
    Clear();
  }

  /**************************************************************************/
  /*!
  @brief Returns a container containing the names and times for all the methods.
  @return The time for the methods.
  */
  /**************************************************************************/
  Time::FunctionTimeSliceVec & Profiler::SystemTimes()
  {
    return CurrentSystemTimes;
  }

  /**************************************************************************/
  /*!
  @brief Returns a container containing the names and times for all the methods.
  @return The time for the methods.
  */
  /**************************************************************************/
  Time::FunctionTimeSliceVec & Profiler::Graphics()
  {
    return GraphicsSystemTimes.first;
  }

  /**************************************************************************/
  /*!
  @brief Returns a container containing the names and times for all the methods.
  @return The time for the methods.
  */
  /**************************************************************************/
  Time::FunctionTimeSliceVec & Profiler::Physics()
  {
    return PhysicsSystemTimes.first;
  }

  /**************************************************************************/
  /*!
  @brief Calculates the engine's current FPS.
  @param dt The delta time.
  */
  /**************************************************************************/
  float localCounter = 0;
  int frameCounter = 0;
  void Profiler::CalculateFPS(float dt)
  {
    localCounter += dt;
    ++frameCounter;
    if (localCounter > 0.5)
    {
      CurrentFPS = (frameCounter / localCounter);
      localCounter = 0;
      frameCounter = 0;
    }
  }

  /*====================
      SYSTEM TIMER
  ====================*/

  /**************************************************************************/
  /*!
  @brief SystemTimer constructor.
  @param systemName The name of the system.
  */
  /**************************************************************************/
  SystemTimer::SystemTimer(std::string systemName) : SystemName(systemName)
  {
  }

  /**************************************************************************/
  /*!
  @brief SystemTimer destructor.
  */
  /**************************************************************************/
  SystemTimer::~SystemTimer()
  {
    Record();
    Report();
  }

  /**************************************************************************/
  /*!
  @brief Reports the system time to the profiler.
  */
  /**************************************************************************/
  void SystemTimer::Report()
  {
    Daisy->Profiler().Add(Time::FunctionTimeSlice(SystemName, ElapsedTime));
  }

  /*====================
    SYSTEM METHOD TIMER
  ====================*/
  /**************************************************************************/
  /*!
  @brief SystemMethodTimer constructor.
  @param methodName The name of the method.
  @param system The system to which this method belongs to.
  */
  /**************************************************************************/
  SystemMethodTimer::SystemMethodTimer(std::string methodName, EnumeratedSystem system)
    : MethodName(methodName), System(system)
  {
  }

  /**************************************************************************/
  /*!
  @brief SystemMethodTimer destructor.
  */
  /**************************************************************************/
  SystemMethodTimer::~SystemMethodTimer()
  {
    Record();
    Report();
  }

  /**************************************************************************/
  /*!
  @brief Reports the system time to the profiler.
  */
  /**************************************************************************/
  void SystemMethodTimer::Report()
  {
    Daisy->Profiler().Add(Time::FunctionTimeSlice(MethodName, ElapsedTime), this->System);
  }

}