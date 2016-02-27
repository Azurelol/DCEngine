#include "Profiler.h"
#include "Engine.h"

namespace DCEngine {
  
  /**************************************************************************/
  /*!
  @brief Adds a system's time to the profile.
  @param systemTime The system and the time elapsed during its udpate method.
  */
  /**************************************************************************/
  void Profiler::Add(SystemTimeSlice systemTime)
  {
    NextSystemTimes.push_back(systemTime);
  }

  /**************************************************************************/
  /*!
  @brief Clear.
  */
  /**************************************************************************/
  void Profiler::Clear()
  {
    SystemTimes = NextSystemTimes;
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

  float localCounter = 0;
  int frameCounter = 0;
  /**************************************************************************/
  /*!
  @brief Calculates the engine's current FPS.
  @param dt The delta time.
  */
  /**************************************************************************/
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


  /**************************************************************************/
  /*!
  @brief Updates the Timer off a delta time.
  @param dt The delta time.
  @return True if the timer hit its specified time, false otherwise.
  */
  /**************************************************************************/
  SystemTimer::SystemTimer(std::string systemName) : SystemName(systemName)
  {
  }

  SystemTimer::~SystemTimer()
  {
    Record();
    Report();
  }

  /**************************************************************************/
  /*!
  @brief Reports the 
  */
  /**************************************************************************/
  void SystemTimer::Report()
  {
    Daisy->Profiler().Add(SystemTimeSlice(SystemName, ElapsedTime));
  }

}