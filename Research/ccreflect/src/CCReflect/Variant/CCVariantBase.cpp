/*--------------------qMopey-Header--------------------
<Game Title>  : qMopey
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 9/15/2012
File Name     : CCVariantBase.cpp
Purpose       : Common base for VariantBase and
                Variant implementation.
-----------------------------------------------------*/

#include "Precompiled.h"

void *VariantBase::GetData( void ) const
{
  return data;
}

const MetaData *VariantBase::GetMeta( void ) const
{
  return meta;
}

bool VariantBase::IsValid( void ) const
{
  return meta != META_TYPE( VoidType ) && data != NULL && meta != NULL;
}