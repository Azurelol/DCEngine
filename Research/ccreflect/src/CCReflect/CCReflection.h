/*--------------------qMopey-Header--------------------
<Game Title>  : qMopey
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 2/25/2013
File Name     : CCReflection.h
-----------------------------------------------------*/

#pragma once

#include <Windows.h>
#undef min
#undef max

#include <cassert>
#include <cstdarg>
#include <cstdio>

typedef float real;
typedef double real64;
typedef signed char	int8;
typedef signed short int16;
typedef signed int int32;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef __int64 int64;
typedef unsigned __int64 uint64;
typedef float f32;
typedef double f64;

// External dependencies
#include "CCLuaIncludes.h"

// STL
#include <vector>

// Containers/Utility
#include "MacroHelpers.h"
#include "BlockAllocator.h"
#include "StaticAssert.h"
#include "CtorLister.h"
#include "ErrorLog.h"
#include "Hash.h"
#include "HashTable.h"
#include "S.h"
#include "Path.h"
#include "File.h"
#include "Array.h"
#include "Link.h"
#include "InList.h"
#include "InHash.h"

// Macros and debug
#include "Singleton.h"
#include "CtorLister.h"
#include "RemoveQualifiers.h"

// Helpers and utilities
#include "CCSFINAE.h"
#include "CCConfig.h"
#include "CCMetaData.h"
#include "CCImplicitRegistration.h"
#include "CCExplicitRegistration.h"
#include "CCVariant.h"
#include "CCRefVariant.h"
#include "CCVariantFunction.h"
#include "CCEnum.h"
#include "CCProperty.h"
#include "CCMetaTypeReg.h"
#include "CCLuaInterface.h"
#include "CCLuaScript.h"
#include "CCLuaReference.h"
#include "CCTextSerialize.h"
#include "Thread.h"
#include "Clock.h"
#include "Timer.h"
