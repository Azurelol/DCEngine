//////////////////////////////////////////////////////////////////////////////
/* File:   FrameRateTimer
 * Author: Daniel Oliveira
 * Copyright: All content © 2014 DigiPen (USA) Corporation, all rights reserved.
 */
/////////////////////////////////////////////////////////////////////////////
#include "stdinc.h"

#include "Windows.h"

#include "FrameRateTimer.h"


namespace Utilities
{
  LARGE_INTEGER gameStart_;
  LARGE_INTEGER startFrame_;
  LARGE_INTEGER endFrame_;

  HANDLE g_timerHandle;

  bool FrameRateTimer::m_initialized = false;

  double FrameRateTimer::m_frequency = 0.0f;
  double FrameRateTimer::m_frameTimeTicks = 0.0f;
  double FrameRateTimer::m_frameTimeMS = 0.0f;
  double FrameRateTimer::m_deltaTime = 0.0f;

  unsigned int FrameRateTimer::m_frameCount = 0;
  float FrameRateTimer::m_timeElapsed = 0.0f;
  float FrameRateTimer::m_framerate = 0.0f;
  bool FrameRateTimer::m_sleep = false;
  void FrameRateTimer::Initialize(int frameRate)
  {
    LARGE_INTEGER temp;

    // Check if QueryPerformanceFrequency has succeeded
    BOOL result = QueryPerformanceFrequency(&temp);
    // TODO assert that result = true

    /* Get the frequency (# of ticks per millisecond). */
    m_frequency = (double)(temp.QuadPart / 1000.0); /* 1000 ms in 1 sec */

    /* Get the # of ticks needed to advance frames. */
    m_frameTimeMS = (1.0 / frameRate) * 1000.0f;
    m_frameTimeTicks = m_frequency * m_frameTimeMS;

    /* Record the GameStart time, with error checking */
    result = QueryPerformanceCounter(&gameStart_);
    // TODO ERROR_IF(result == false,
    //          "QueryPerformanceCounter has returned an error.");

    g_timerHandle = CreateWaitableTimer(NULL, true, NULL);
    if (g_timerHandle == NULL)
    {

    }
  }

  float FrameRateTimer::GetGameTime(void)
  {
    // Returns in seconds
    return float((endFrame_.QuadPart - gameStart_.QuadPart) / (m_frequency * 1000.00) );
  }

  float FrameRateTimer::GetDeltaTime(void)
  {
    if (m_deltaTime / 1000.0f > 1.0f / 30.0f)
      return (1.0f / 30.0f);
    return float(m_deltaTime / 1000.0);
  }

  float FrameRateTimer::GetFramerate(void)
  {
    return m_framerate;
  }

  void FrameRateTimer::FrameStart(void)
  {
    BOOL result = QueryPerformanceCounter(&startFrame_);
    // TODO ERROR_IF(result == false,
    //          "QueryPerformanceCounter has returned an error.");
  }

  void FrameRateTimer::FrameEnd(void)
  {
    BOOL result = QueryPerformanceCounter(&endFrame_);
    // TODO ERROR_IF(result == false,
    //          "QueryPerformanceCounter has returned an error.");

    // Wait until frame ends with a high performance timer
    // Get milliseconds left in frame and then convert into a LARGE_INTEGER value
    double ticksLeft = (m_frameTimeTicks - double(endFrame_.QuadPart - startFrame_.QuadPart));
    /*if (ticksLeft > 0)
    {
      double msLeft = ticksLeft / m_frequency;

      // A value of 1 represents 100 nanoseconds. There are 1,000,000 ns in a ms
      // which means there are 10,000 units of 100 nanoseconds
      // Making this negative causes the timer to use relative timing
      LARGE_INTEGER dueTime;
      dueTime.QuadPart = LONGLONG(-10000.0 * msLeft);

      SetWaitableTimer(g_timerHandle, &dueTime, 0, NULL, NULL, 0);

      WaitForSingleObject(g_timerHandle, DWORD(m_frameTimeMS));

      // Get ending time again after waiting
      BOOL result = QueryPerformanceCounter(&endFrame_);
    }*/

    while (ticksLeft > 0.0f)
    {
      if (m_sleep)
        Sleep(1);
      result = QueryPerformanceCounter(&endFrame_);
      ticksLeft = (m_frameTimeTicks - double(endFrame_.QuadPart - startFrame_.QuadPart));
      
    }

    /* Get time between this and FrameStart */
    m_deltaTime = (endFrame_.QuadPart - startFrame_.QuadPart) / m_frequency;
      
    // Whenever timeElapsed reaches 1 second, calculate the framerate
    ++m_frameCount;
    m_timeElapsed += float(m_deltaTime);
      
    // If more than one second has elapsed, run the calculation
    if (m_timeElapsed >= 1000.0f)
    {
      // Calculate framerate and then reset counters
      m_framerate = float(m_frameCount) * (1000.0f / m_timeElapsed);

      m_timeElapsed -= 1000.0f;
      m_frameCount = 0;
    }
  }

  void FrameRateTimer::SetSleepy(bool sleep)
  {
    m_sleep = sleep;
  }

} // namespace Util
