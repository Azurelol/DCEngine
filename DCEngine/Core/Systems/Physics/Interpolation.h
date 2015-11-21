/******************************************************************************/
/*!
\file   Interpolation.h
\author Blaine Reiner
\par    email: blaine.reiner\@digipen.edu
\par    DigiPen login: blaine.reiner
\date   11/20/2015
\brief  Interface file of the Interpolation system
*/
/******************************************************************************/
#pragma once

namespace DCEngine
{
  // Linnear Interpolation function
  template <typename T>
  T Interpolate(const T &start, const T &end, float ratio);

}

