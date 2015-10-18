#pragma once

namespace Engine {
  class Object
  {
  public:
    Object(int id);
    ~Object();

    META_DATA(Object);

  private:
    int ID;
    bool active;
    double x;
  };

  class DerivedObject : public Object {
  public:
    DerivedObject(int id);
    ~DerivedObject();

    META_DATA(DerivedObject);

  private:
    int Numbers;
    bool IsCool;
  };

}
