#include "ReflectionTest.h"
//#include "..\..\Core\ComponentsInclude.h"

namespace DCEngine {

  META_DEFINE_CLASS(ObjectTest)
  {
    META_ADD_CLASS_MEMBER(ID);
    META_ADD_CLASS_MEMBER(active);
  }

  ObjectTest::ObjectTest(int id)
  {
  }
  ObjectTest::~ObjectTest()
  {
  }

  META_DEFINE_CLASS(DerivedObject)
  {
    META_ADD_CLASS_MEMBER(Numbers);
    META_ADD_CLASS_MEMBER(IsCool);
  }

  DerivedObject::DerivedObject(int id) : ObjectTest(id)
  {
  }
  DerivedObject::~DerivedObject()
  {
  }

  void ReflectionTest()
  {
    trace << "-- Reflection Test -- \n";
    META_PRINT_TYPE(int);
    META_PRINT_TYPE(float);
    META_PRINT_TYPE(double);
    META_PRINT_TYPE(int*);

    

    META_PRINT_MEMBERS(ObjectTest);
    META_PRINT_MEMBERS(DerivedObject);

    //trace << "double is " << META_STR("double")->Size();

    std::string word = "This is a word.";
    trace << "Type of word object: " << META(word)->Name() << "\n";
    if (META(word) != META_TYPE(int)) {
      trace << "The word object is not an int!\n";
    }

    StringWrapper stringtest("String within the S wrapper");
    std::cout << stringtest << "\n";
    META_PRINT_MEMBERS(StringWrapper);



    //META_PRINT_MEMBERS(Camera);

  }

}