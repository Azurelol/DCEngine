#pragma once
#include <string>

namespace DCEngine {

  /**************************************************************************/
  /*!
  @brief A set of macros for ease of use of the MetaData system. 
  */
  /**************************************************************************/
  #define PASTE( _, __ )  _##__
  #define NAME_GENERATOR_INTERNAL( _ ) PASTE( GENERATED_NAME, _ )
  #define NAME_GENERATOR( ) NAME_GENERATOR_INTERNAL( __COUNTER__ )

  /* META_TYPE: Retrieves the proper MetaData instance of an object by type. */
  #define META_TYPE(TYPE) (MetaCreator<TYPE>::Get())
  /* META: Retrives the proper MetaData instance of an object by an object's type */
  #define META(OBJECT) (MetaCreator<decltype(OBJECT)>::Get( ))
  /* META_STR: Finds a MetaData instance by string name */
  #define META_STR(STRING) (MetaManager::Get(STRING))
  /* DEFINE_META:  Defines a MetaCreator for a specific type of data */
  #define DEFINE_META(TYPE) \
   MetaCreator<TYPE> NAME_GENERATOR( ) ( #TYPE, sizeof(TYPE))

  /**************************************************************************/
  /*!
  @brief A MetaData object is a non-templated class, allowing them to be
         stored in a data structure. There is one MetaData object for each
         type of data registered to the system, and the MetaData object of
         a corresponding type is a representation of that type.
         It stores information about whether or not it is a plain old data
         type (POD), the size of the type, its members, methods, and name.
         Inheritance information can be stored along with multiple 
         inheritance information (though the latter is not implemented).
  */
  /**************************************************************************/
  class MetaData {
  public:
    MetaData(std::string string, unsigned val) : DataName(string), DataSize(val) {}
    ~MetaData() {}

    const std::string& Name(void) const { 
      return DataName; 
    }
    unsigned Size(void) const {
      return DataSize;
    }

  private:
    std::string DataName;
    unsigned DataSize;
  };

  /**************************************************************************/
  /*!
  @brief The MetaCreator is a templated class that manages the creation of a 
         single unique MetaData instance. It then adds its instance into the 
         MetaManager which contains it in some sort of map.
  */
  /**************************************************************************/
  template <typename MetaType>
  class MetaCreator {
  public:
    MetaCreator(std::string name, unsigned size) {
      Initialize(name, size);
    }

    static void Initialize(std::string name, unsigned size) {
      Get()->Initialize(name, size);
    }

    // Ensures a single instance can exist for this class type
    static MetaData* Get() {
      static MetaData instance;
      return &instance;
    }
  };

  /**************************************************************************/
  /*!
  @brief The RemoveQualifier templated classes strip down qualified 
         types/references/pointers to a single unqualified type, for passing 
         into a templated type as a typename specifier.
  */
  /**************************************************************************/
  /* Basic */
  template <typename T>
  struct RemoveQualifier {
    typedef T type;
  };
  /* Const */
  template <typename T>
  struct RemoveQualifier<const T> {
    typedef T type;
  };
  /* Reference */
  template <typename T>
  struct RemoveQualifier<T&> {
    typedef T type;
  };
  /* Const Reference */
  template <typename T>
  struct RemoveQualifier<const T&> {
    typedef T type;
  };
  /* Const Reference to Reference */
  template <typename T>
  struct RemoveQualifier<T&&> {
    typedef T type;
  };
  /* Pointer */
  template <typename T>
  struct RemoveQualifier<T*> {
    typedef T type;
  };
  /* Const Pointer */
  template <typename T>
  struct RemoveQualifier<const T*> {
    typedef T type;
  };

}