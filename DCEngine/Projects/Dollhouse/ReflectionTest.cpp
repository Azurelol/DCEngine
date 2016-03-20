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
    
    // POD
    META_PRINT_TYPE(int);
    META_PRINT_TYPE(float);
    META_PRINT_TYPE(double);
    META_PRINT_TYPE(int*);
    // Classes
    META_PRINT_MEMBERS(ObjectTest);
    META_PRINT_MEMBERS(DerivedObject);
    // Tyoe-checking
    std::string word = "This is a word.";
    trace << "Type of word object: " << META(word)->Name() << "\n";
    if (META(word) != META_TYPE(int)) {
      trace << "The word object is not an int!\n";
    }
    // String wrapper test
    StringWrapper stringtest("String within the S wrapper");
    std::cout << stringtest << "\n";
    META_PRINT_MEMBERS(StringWrapper);
    // Variant Test
    Variant v = 1;
    v = true;
    v = std::string("std::string!\n");
    trace << "v.GetValue = " << v.GetValue<std::string>() << "\n";
    int x = 5;
    v = &x;
    trace << "*v.GetValue = " << *v.GetValue<int*>() << "\n";
    const char *c = "char *!\n";
    v = c;
    trace << "v.GetValue = " << v.GetValue<char*>() << "\n";
    // Serialize variant test
    
    //int y = 6;
    //RefVariant refV;
    //refV = y;
    //const char* d = "String";
    //Variant v2 = d;
    //refV = v2;
    //v2.Serialize(std::cout);
    ////META_PRINT_MEMBERS(Camera);

  }

}