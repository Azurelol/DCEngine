#pragma once

// Reflection demonstration by Randy Gaul
// Reflection lets C++ users store information about the different types
// of data their program uses. This means non-templated structs are used
// to store relevant information about types that would usually be lost
// during compilation. These TypeInfo structs can be used during run-time
// to write more type-aware code. This means that reflection can be used
// to automate certain processes that deal with interpreting memory.
// Examples of areas that can be automated: script binding, serialization,
// visual editor support, etc.

#include <cstdio>
#include <cassert>
#include <cstring>


  // Stores information about a type. Things like the name of a type,
  // size in bytes, and other things are important to keep track of.
  struct TypeInfo
  {
    const char* name;
    int size;

    // You can add an array of TypeInfo pointers here to represent
    // relationships between different types (like inheritance parent
    // child relationships, or struct/class data members).
  };

  // Used to store a pointer to a TypeInfo struct as a static parameter
  // of a templated static function. This lets the user lookup TypeInfo
  // pointers given a template type parameter.
  template < typename T >
  struct TypeLookupByTemplate
  {
    static TypeInfo* GetType(TypeInfo* typeInfo)
    {
      static TypeInfo* s_typeInfo = NULL;

      if (!s_typeInfo)
      {
        s_typeInfo = typeInfo;
      }

      return s_typeInfo;
    }
  };

  // You can make this larger if you need to
#define MAX_TYPE_INFOS 128
  TypeInfo typeInfos[MAX_TYPE_INFOS];
  int typeInfosCount;

  // Add a type to the type infos array. For now this just stores
  // name and size of a type.
#define ADD_TYPE( T ) \
    do { \
        assert( typeInfosCount < MAX_TYPE_INFOS ); \
        TypeInfo* typeInfo = typeInfos + typeInfosCount++; \
        typeInfo->size = sizeof( T ); \
        typeInfo->name = #T; \
        TypeLookupByTemplate< T >::GetType( typeInfo ); \
    } while ( 0 );

  // Allows code to get a pointer to a TypeInfo through a template type lookup ID.
#define GET_TYPE_BY_TEMPLATE( T ) TypeLookupByTemplate< T >::GetType( NULL )

  TypeInfo* GetTypeByString(const char* typeNameCharPointer)
  {
    for (int i = 0; i < typeInfosCount; ++i)
    {
      if (!strcmp(typeNameCharPointer, typeInfos[i].name))
      {
        return typeInfos + i;
      }
    }

    // Unable to find a specific type by string
    assert(false);

    return NULL;
  }

  // Lookup a type info struct by string ID.
  // Looking up a type with an O( N ) loop will probably be fast enough for most
  // use cases. If this ever becomes a problem a hash table can be used here.
#define GET_TYPE_BY_STRING( typeNameCharPointer ) GetTypeByString( typeNameCharPointer )

  // A Variable struct contains a pointer to some memory. The typeInfo pointer
  // stores information about the type of data that the void pointer points to.
  // This is useful to pass many types of data to a single function, thus red-
  // ucing code duplication in many areas.
  struct Variable
  {
    // Functions to initialize the data and typeInfo pointers
    void Set(void* dataPtr, TypeInfo* typeInfoPtr)
    {
      data = dataPtr;
      typeInfo = typeInfoPtr;
    }

    template < typename T >
    void Set(T& typedData)
    {
      data = &typedData;
      typeInfo = GET_TYPE_BY_TEMPLATE(T);
    }

    // If any code would like to retrieve the explicit data the code
    // must provide a templated type to cast to.
    template < typename T >
    T& GetValue()
    {
      // An assert here can force type safety
      assert(GET_TYPE_BY_TEMPLATE(T) == typeInfo);

      return *(T*)data;
    }

    void* data;
    TypeInfo* typeInfo;
  };
  
  void ReflectionDemo() {

    // Add some type information to the reflection system
    ADD_TYPE(int);
    ADD_TYPE(float);

    // Print out stored type information from the reflection system
    TypeInfo* intTypeInfo = GET_TYPE_BY_STRING("int");
    printf("%s\n", intTypeInfo->name);
    printf("%d\n", intTypeInfo->size);

    TypeInfo* floatTypeInfo = GET_TYPE_BY_TEMPLATE(float);
    printf("%s\n", floatTypeInfo->name);
    printf("%d\n", floatTypeInfo->size);

    // Create an integer and then create a Variable that describes the integer
    // and holds a pointer to the integer in memory.
    int x = 10;
    Variable var;
    var.Set(x);

    // Variables let you query name and size information
    printf("%s\n", var.typeInfo->name);
    printf("%d\n", var.typeInfo->size);

    // Variables also let you cast to the contained type
    printf("%d\n", var.GetValue< int >());

  }

