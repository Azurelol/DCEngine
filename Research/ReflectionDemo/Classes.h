#pragma once

class Object
{
public:
  Object() {}
};

class DerivedObject : public Object {
public:
  DerivedObject() {}
};

class Cat : public DerivedObject {
public:
  Cat() {}

  int Meows;
  int CatNips;
  int Spots;
};

class Dog : public DerivedObject {
public:
  Dog() {}

  int Woofs;
  int KittensMurdered;
  int Wags;
};


