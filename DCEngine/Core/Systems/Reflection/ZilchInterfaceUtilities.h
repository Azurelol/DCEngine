/******************************************************************************/
/*!
@file   ZilchInterfaceUtilities.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   2/27/2016
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once

namespace DCEngine {
    
  class Component;
  
  namespace Systems {

    class ZilchInterface;
    class StringArray {

    };
    


    Component* GetComponent(Zilch::Call& call, Zilch::ExceptionReport& report);
    void GetNativeComponent(Zilch::Call& call, Zilch::ExceptionReport& report);
    void GetZilchComponent(Zilch::Call& call, Zilch::ExceptionReport& report);

    struct ComponentData {
      Zilch::BoundType* Type;
      ZilchInterface* Interface;
    };

  }
}

