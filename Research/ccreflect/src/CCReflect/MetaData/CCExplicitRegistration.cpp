/*--------------------qMopey-Header--------------------
<Game Title>  : qMopey
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 9/7/2013
File Name     : CCExplicitRegistration.cpp
Purpose       : 
-----------------------------------------------------*/

#pragma

#include "Precompiled.h"

MetaManager *META_MANAGER = NULL;

MetaManager::MetaManager( )
{
  assert( META_MANAGER == NULL );
  META_MANAGER = this;
}

MetaManager::~MetaManager( )
{
  assert( META_MANAGER == this );
  META_MANAGER = NULL;
}

void MetaManager::Insert( VariantFunction *instance )
{
  m_fnMap.Add( instance, instance->Name( ) );
}

void MetaManager::GetFunction( const char *name )
{
}

const MetaData *MetaManager::GetMeta( const char *name )
{
  return *m_metaMap[name];
}

Member *MetaManager::CreateMember( const char *memberName, unsigned memberOffset, MetaData *meta )
{
  Member *mem = (Member *)m_allocator.Allocate( sizeof( Member ) );
  new (mem) Member( S( memberName ), memberOffset, meta );
  return mem;
}
