/******************************************************************************/
/*!
\file   Interpolation.cpp
\author Blaine Reiner
\par    email: blaine.reiner\@digipen.edu
\par    DigiPen login: blaine.reiner
\date   11/20/2015
\brief  Implementation file of the Interpolation system
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
namespace DCEngine
{
  template <typename T>
  T Interpolate(const T &start, const T &end, float ratio)
  {
    return start + (end - start) * ratio;
  }
}
