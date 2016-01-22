/******************************************************************************/
/*!
@file   Graphical.h
@author William Mao, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   1/22/2016
@brief  The Graphical component is the base class from which all graphical
        components derive frm.

@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ComponentReference.h"

namespace DCEngine {
  namespace Components {

    class Graphical : public Component {

      // Properties


      ZilchDeclareDerivedType(Graphical, Component);
      Graphical(std::string graphicalComponent, Entity& owner);
      ~Graphical();
      void Initialize();

    };



  }
}