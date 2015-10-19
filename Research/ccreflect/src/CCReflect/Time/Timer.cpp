/*-------------------qMopey-Header----------------------
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 2/28/2013
File Name     : Timer.cpp
Purpose       : Wrapper around QueryPerformanceTimer
                for a very easy to use high resolution
                time keeping object.
Note          : The Timer is just a reference to a Clock
                that provides timing functionality. A
                Clock class must be created before
                the Timer class can be instantiated.
------------------------------------------------------*/

#include "Precompiled.h"

/*------------------------------------------------------------------------------------------------

                                    Constructors / Destructors

------------------------------------------------------------------------------------------------*/
Timer::Timer( Clock& clock ) : m_clock( clock ), m_freq( clock.m_freq )
{
  Start( );
  Elapsed( );
}

/*------------------------------------------------------------------------------------------------

                                            Modifiers

------------------------------------------------------------------------------------------------*/
// Records a time in the "Previous Time" data member
void Timer::Start( void )
{
  m_clock.Query( m_previous );
}

Timer::~Timer( )
{
}

/*------------------------------------------------------------------------------------------------

                                           Data Access

------------------------------------------------------------------------------------------------*/
// Returns the amount of time elapsed, in seconds, between the last
// Start and RecordTime calls
f32 Timer::Elapsed( void )
{
  m_clock.Query( m_current );
  return (m_current.QuadPart - m_previous.QuadPart) / (float)m_freq.QuadPart;
}
  