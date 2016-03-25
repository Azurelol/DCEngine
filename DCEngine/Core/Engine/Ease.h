/******************************************************************************/
/*!
@file   Ease.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   2/12/2015
@brief  Ease functions.

@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once

namespace DCEngine {

  class Easing {
  public:

    static Real Linear(Real t) {
      return t;
    }

    static Real QuadIn(Real t) {
      return t * t;
    }

    static Real QuadOut(Real t) {
      return t * (2 - t);
    }

    static Real Calculate(Real t, Ease ease) {
      Real easeVal;
      switch (ease) {
      case Ease::Linear:
        easeVal = Easing::Linear(t);
        break;
      case Ease::QuadIn:
        easeVal = Easing::QuadIn(t);
        break;
      case Ease::QuadOut:
        easeVal = Easing::QuadOut(t);
        break;
      }
      return easeVal;
    }

  };


  // LOL -->

  //class Easing {
  //  template <typename PropertyType> static PropertyType Linear(PropertyType);
  //  template <typename PropertyType> static PropertyType QuadIn(PropertyType);
  //  template <typename PropertyType> static PropertyType QuadOut(PropertyType);

  //};

  //template<typename PropertyType>
  //inline PropertyType Easing::Linear(PropertyType t)
  //{
  //  return t;
  //}

  //template<typename PropertyType>
  //inline PropertyType Easing::QuadIn(PropertyType t)
  //{
  //  return t * t;
  //}

  //template<typename PropertyType>
  //inline PropertyType Easing::QuadOut(PropertyType t)
  //{
  //  return t * (2 - t);
  //}

}