/*****************************************************************************/
/*!
@file   ReflectionTest.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/19/2015
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "../../Core/Systems/Reflection/ReflectionMeta.h"

namespace DCEngine
{
  class ObjectTest
  {
  public:
    ObjectTest(int id);
    ~ObjectTest();

    META_ADD_CLASS(ObjectTest);

  private:
    int ID;
    bool active;
    double x;
  };

  class DerivedObject : public ObjectTest {
  public:
    DerivedObject(int id);
    ~DerivedObject();

    META_ADD_CLASS(DerivedObject);

  private:
    int Numbers;
    bool IsCool;
  };

  void ReflectionTest();

}