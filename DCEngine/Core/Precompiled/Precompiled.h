/******************************************************************************/
/*!
@file   Precompiled.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   8/1/2015
@brief  The Precompiled header for Daisy Chain Engine.
@note   The use of this file requires that it be included in every translational
        unit (.cpp) in the project.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once

/**************************************************************************/
/*!
      STANDARD LIBRARY / STANDARD TEMPLATE LIBRARY
*/
/**************************************************************************/
#include <memory>
#include <unordered_map>
#include <string>
#include <functional>
#include <chrono>
#include <utility>
#include <algorithm>

/**************************************************************************/
/*!
      MULTIMEDIA LIBRARIES
*/
/**************************************************************************/
// SFML

// Dear ImgUi
#include <IMGUI/imgui.h>

#include <SFML\System\Vector2.hpp>
// GLEW

// FMOD
#include <FMOD\fmod.hpp>
#include <FMOD\fmod_errors.h>

/**************************************************************************/
/*!
    BINDING
*/
/**************************************************************************/
#include <ZILCH\Zilch.hpp>

/**************************************************************************/
/*!
      ENGINE FILES
*/
/**************************************************************************/
#include "../Engine/Types.h"
#include "../Engine/Macros.h"
//#include "../Engine/Data.h"
//#include <Windows.h>