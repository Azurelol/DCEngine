/*--------------------qMopey-Header--------------------
<Game Title>  : qMopey
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 2/2/2013
File Name     : Precompiled.h
Purpose       : Precompiled header for very fast
                compilation time. This file generates
                the header obj.
-----------------------------------------------------*/
#pragma once

#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <stdio.h>
#include <time.h> // ctime
#include <cstring> // memcpy

// POD constructed with default ctor will be default initialized
#ifdef _MSC_VER
#pragma warning( disable : 4345 ) 
#endif

// Reflection
#include "CCReflection.h"