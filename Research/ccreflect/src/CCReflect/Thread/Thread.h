/*-------------------qMopey-Header----------------------
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 2/23/2013
File Name     : Thread.h
Purpose       : Wrapper for Windows threading API.
------------------------------------------------------*/

#pragma once

class Thread
{
public:
    /*----------------
       Ctors/Dtors
    ----------------*/
  template <typename Signature>
  Thread( Signature sig, bool start_paused = false );
  template <typename Signature, typename Instance>
  Thread( Signature sig, Instance instance, bool start_paused = false );
  ~Thread( );
  
    /*----------------
        Modifiers
    ----------------*/
  void Resume( void );
  void Pause( void );
  void Lock( void );
  void Unlock( void );
  void Quit( void );

  // Bind the worker function to a new instance pointer
  template <typename T>
  void Bind( T self );

    /*----------------
       Data Access
    ----------------*/
  uint32 GetID( void ) const;
  HANDLE GetHandle( void ) const;

private:
  uint32 m_id;
  HANDLE m_handle;
  VariantFunction m_worker;
  CRITICAL_SECTION m_cs;
  bool m_running;

  static DWORD WINAPI ThreadProc( void *function ); 
};

template <typename Signature>
Thread::Thread( Signature sig, bool start_paused )
  : m_worker( "ThreadWorker", sig )
{
  m_handle = CreateThread(
    NULL,
    0,
    ThreadProc,
    &m_worker,
    start_paused ? CREATE_SUSPENDED : 0,
    (LPDWORD)&m_id
  );
}

template <typename Signature, typename Instance>
Thread::Thread( Signature sig, Instance instance, bool start_paused )
  : m_worker( "ThreadWorker", sig )
{
  m_worker.Bind( instance );

  m_handle = CreateThread(
    NULL,
    0,
    ThreadProc,
    &m_worker,
    start_paused ? CREATE_SUSPENDED : 0,
    (LPDWORD)&m_id
  );
}

// Bind the worker function to a new instance pointer
template <typename T>
void Thread::Bind( T self )
{
  m_worker.Bind( self );
}

