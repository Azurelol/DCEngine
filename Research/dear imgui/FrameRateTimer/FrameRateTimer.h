//////////////////////////////////////////////////////////////////////////////
/* File:   FrameRateTimer
 * Author: Daniel Oliveira
 *
 * A small class that keeps track of the game's frame rate. This is used for
 * keeping the game at 60 FPS and for displaying FPS data.
 * Copyright: All content © 2014 DigiPen (USA) Corporation, all rights reserved.
 */
/////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef FRAME_RATE_TIMER_H
#define FRAME_RATE_TIMER_H
/////////////////////////////////////////////////////////////////////////////
namespace Utilities
{
  class FrameRateTimer
  {
    public:
      // Constructs the FrameRateTimer and gets some information from the OS
      // needed for this FrameRateTimer to run correctly.
      FrameRateTimer();

      // Initializes the FrameRateTimer with a given framerate setting,
      // such as 60 FPS or 30 FPS. This framerate is the MAXIMUM framerate.
      static void Initialize(int frameRate);

      // Returns the amount of time the game has been running until this
      // current frame, in seconds.
      static float GetGameTime(void);

      // Returns the amount of time spent in the last frame, in seconds.
      static float GetDeltaTime(void);

      // Returns the current framerate that the game is running at, based on
      // calculations done with frameStart and frameEnd
      static float GetFramerate(void);

      // Call this function at the beginning of each frame to start
      // recording time
      static void FrameStart(void);

      // Call this function at the end of each frame to stop recording time
      // and wait until the minimum frame time has elapsed.
      static void FrameEnd(void);

      static void SetSleepy(bool sleepy);

    private:
      static bool m_initialized; // Prevent it from being initialized twice

      static double m_frequency; // # of ticks per millisecond
      static double m_frameTimeTicks; // # of ticks in a single frame
      static double m_frameTimeMS; // # of milliseconds in a single frame
      static double m_deltaTime;

      // These are used for finding framerate
      static unsigned int m_frameCount; // Counts frames that happened this second
      static float m_timeElapsed;       // Counts up to 1 second and then resets
      static float m_framerate;         // Updated every second
      static bool m_sleep;
  };
}


/////////////////////////////////////////////////////////////////////////////
#endif
