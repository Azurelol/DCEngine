/*-------------------qMopey-Header----------------------
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 2/23/2013
File Name     : Clock.h
Purpose       : Wrapper around QueryPerformanceTimer
                for a very easy to use high resolution
                time keeping object.
Note          : In order to properly use
                QueryPerformanceCounter you must place
                the timer within a single thread, and
                set processor affinity.
------------------------------------------------------*/

#pragma once

class Clock : public Thread
{
public:
    /*----------------
       Ctors/Dtors
    ----------------*/
  Clock( );
  ~Clock( );

    /*----------------
        Modifiers
    ----------------*/
  // Records current time in start variable
  void Start( void );

  // Records current time in stop variable
  void Stop( void );

    /*----------------
       Data Access
    ----------------*/
  // Time since last Start call
  f32 Elapsed( void );

  // Time between last Start and Stop calls
  f32 Difference( void );

  // Get the current clock count
  LONGLONG Current( void );

  META_DATA( Clock );

private:
  LARGE_INTEGER m_freq;
  LARGE_INTEGER m_start, m_stop, m_current;

  friend class Timer;
  // Callbacks for Timer
  void Query( LARGE_INTEGER& query );
};
