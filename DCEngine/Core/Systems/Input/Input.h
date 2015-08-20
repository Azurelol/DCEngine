/******************************************************************************/
/*!
\file   Input.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/17/2015
\brief  The abstract interface for the Input class.

*/
/******************************************************************************/
#include "..\System.h"
#pragma once

namespace DCEngine{
  namespace Systems {
    class Input : public System {
    public:
      Input();

      void Initialize();
      void Update(float dt);
      void Terminate();

      //KeyIsDown(Key key);
      //KeyIsPressed
      //KeyIsReleased
      //KeyIsUp

    private:


    };

  }

}
