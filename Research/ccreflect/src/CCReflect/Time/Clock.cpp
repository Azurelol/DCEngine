/*-------------------qMopey-Header----------------------
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 2/23/2013
File Name     : Clock.cpp
Purpose       : Wrapper around QueryPerformanceTimer
                for a very easy to use high resolution
                time keeping object.
------------------------------------------------------*/

#include "Precompiled.h"

// Placeholder function, since Clock doesn't run on a loop
static void Void( void )
{
}

/*------------------------------------------------------------------------------------------------

                                    Constructors / Destructors

------------------------------------------------------------------------------------------------*/
Clock::Clock( ) : Thread( Void )
{
  // Assign to a single processor
  SetThreadAffinityMask( GetCurrentThread( ), 1 );

  // Grab frequency of this processor
  QueryPerformanceFrequency( &m_freq );

  // Setup initial times
  Start( );
  Stop( );
}

Clock::~Clock( )
{
}

/*------------------------------------------------------------------------------------------------

                                            Modifiers

------------------------------------------------------------------------------------------------*/
// Records current time in start variable
void Clock::Start( void )
{
  QueryPerformanceCounter( &m_start );
}

// Records current time in stop variable
void Clock::Stop( void )
{
  QueryPerformanceCounter( &m_stop );
}

/*------------------------------------------------------------------------------------------------

                                           Data Access

------------------------------------------------------------------------------------------------*/
// Get current time from previous Start call
f32 Clock::Elapsed( void )
{
  QueryPerformanceCounter( &m_current );
  return (m_current.QuadPart - m_start.QuadPart) / (float)m_freq.QuadPart;
}

// Time between last Start and Stop calls
f32 Clock::Difference( void )
{
  return (m_stop.QuadPart - m_start.QuadPart) / (float)m_freq.QuadPart;
}

// Get the current clock count
LONGLONG Clock::Current( void )
{
  QueryPerformanceCounter( &m_current );
  return m_current.QuadPart;
}

void Clock::Query( LARGE_INTEGER& query )
{
  QueryPerformanceCounter( &query );
}
