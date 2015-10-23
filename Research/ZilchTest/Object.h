#pragma once

  class Object
  {
  public:
    Object(int id);
    ~Object();

  private:
    int ID;
    bool active;
    double x;
  };

  class DerivedObject : public Object {
  public:
    DerivedObject(int id);
    ~DerivedObject();


  private:
    int Numbers;
    bool IsCool;
  };
