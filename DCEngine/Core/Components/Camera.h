/******************************************************************************/
/*!
\file   Component.h
\author Allan Deutsch, Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/2/2015
\brief  The 3D camera class. 

*/
/******************************************************************************/
#pragma once
#include "..\Engine\Component.h"

namespace DCEngine {
  class Camera : public Component {
  public:

  private:
    float FieldOfView;
    float NearPlane;
    float FarPlane;
    float Size; //!< Size of the orthographic projection


  };


}