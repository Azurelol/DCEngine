/*-------------------qMopey-Header----------------------
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 2/23/2013
File Name     : Thread.cpp
Purpose       : Wrapper for Windows threading API.
------------------------------------------------------*/
#include "Precompiled.h"

/*------------------------------------------------------------------------------------------------

                                    Constructors / Destructors

------------------------------------------------------------------------------------------------*/
Thread::~Thread( )
{
  CloseHandle( m_handle );
}

/*------------------------------------------------------------------------------------------------

                                            Modifiers

------------------------------------------------------------------------------------------------*/
void Thread::Resume( void )
{
  ResumeThread( m_handle );
}

void Thread::Pause( void )
{
  SuspendThread( m_handle );
}

void Thread::Lock( void )
{
  EnterCriticalSection(&m_cs);
}

void Thread::Unlock( void )
{
  LeaveCriticalSection( &m_cs );
}

void Thread::Quit( void )
{
  m_running = false;
}

DWORD WINAPI Thread::ThreadProc( void *function )
{
  VariantFunction *fn = reinterpret_cast<VariantFunction *>(function);
  (*fn)( );
  return EXIT_SUCCESS;
}

/*------------------------------------------------------------------------------------------------

                                           Data Access

------------------------------------------------------------------------------------------------*/
unsigned Thread::GetID( void ) const
{
  return m_id;
}

HANDLE Thread::GetHandle( void ) const
{
  return m_handle;
}
