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