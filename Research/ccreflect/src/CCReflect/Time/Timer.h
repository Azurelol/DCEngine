/*-------------------qMopey-Header----------------------
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 2/28/2013
File Name     : Timer.h
Purpose       : Wrapper around QueryPerformanceTimer
                for a very easy to use high resolution
                time keeping object.
Note          : The Timer is just a reference to a Clock
                that provides timing functionality. A
                Clock class must be created before
                the Timer class can be instantiated.
------------------------------------------------------*/

#pragma once

class Timer
{
public:
    /*----------------
       Ctors/Dtors
    ----------------*/
  Timer( Clock& clock );
  ~Timer( );

    /*----------------
        Modifiers
    ----------------*/
  // Records a time in the "Previous Time" data member
  void Start( void );

    /*----------------
       Data Access
    ----------------*/
  // Returns the amount of time elapsed, in seconds, between the last
  // Start and RecordTime calls
  f32 Elapsed( void );

private:
  Clock& m_clock;
  LARGE_INTEGER m_freq;
  LARGE_INTEGER m_current, m_previous;

  const Timer& operator=( const Timer& rhs );
};
