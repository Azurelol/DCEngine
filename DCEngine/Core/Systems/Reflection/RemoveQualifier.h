#pragma once

namespace DCEngine {

  /**************************************************************************/
  /*!                Checks whether a type is a pointer or not.
  */
  /**************************************************************************/
  template<typename T> 
  struct isPointer {
    static const bool value = false;
  };

  template<typename T>
  struct isPointer<T*> {
    static const bool value = true;
  };

  template<typename T>
  struct isPointer<T&> {
    static const bool value = false;
  };

  /**************************************************************************/
  /*!                Checks whether a type is an array literal
  */
  /**************************************************************************/
  template<typename T>
  struct isArray {
    static const bool value = false;
  };

  template<typename T>
  struct isArray<T*> {
    static const bool value = false;
  };

  template<typename T>
  struct isArray<T&> {
    static const bool value = false;
  };

  #define INTERNAL_ARRAY_OVERLOAD( x ) \
  template <typename T> \
  struct RemoveQualifier<T[ x ]> \
  { \
    typedef typename RemoveQualifier<T *>::type type; \
  }; \
  \
  template <typename T> \
  struct RemoveQualifier<const T[ x ]> \
  { \
    typedef typename RemoveQualifier<T *>::type type; \
  }; \
  /* Used to check if a type is a array literal or not */ \
  template<typename T> \
  struct isArray<T[ x ]> { static const bool value = true;  }; \
  /* Used to check if a type is a pointer or not */ \
  template<typename T> \
  struct isPointer<T[ x ]>      { static const bool value = true;  };

  #define ARRAY_OVERLOAD( ) \
  INTERNAL_ARRAY_OVERLOAD( __COUNTER__ )


  /**************************************************************************/
  /*!
  @brief The RemoveQualifier templated classes strip down qualified
  types/references/pointers to a single unqualified type, for passing
  into a templated type as a typename specifier.
  */
  /**************************************************************************/
  template <typename T>
  struct RemoveQualifier
  {
    typedef T type;
  };

  template <typename T>
  struct RemoveQualifier<const T>
  {
    typedef typename RemoveQualifier<T>::type type;
  };

  template <typename T>
  struct RemoveQualifier<T&>
  {
    typedef typename RemoveQualifier<T>::type type;
  };

  template <typename T>
  struct RemoveQualifier<const T&>
  {
    typedef typename RemoveQualifier<T>::type type;
  };

  template <typename T>
  struct RemoveQualifier<T&&>
  {
    typedef typename RemoveQualifier<T>::type type;
  };

  template <typename T>
  struct RemoveQualifier<const T *>
  {
    typedef typename RemoveQualifier<T *>::type type;
  };

  //Array literal overloads (LOL)
  ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD();
  ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD();
  ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD();
  ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD();
  ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD();
  ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD();
  ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD();
  ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD();
  ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD();
  ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD();
  ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD();
  ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD();
  ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD();
  ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD();
  ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD();
  ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD();
  ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD();
  ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD();
  ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD();
  ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD();
  ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD();
  ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD();
  ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD();
  ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD();
  ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD();
  ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD();
  ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD();
  ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD();
  ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD();
  ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD();
  ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD();
  ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD();
  ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD();
  ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD();
  ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD();
  ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD();
  ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD();
  ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD();
  ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD();
  ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD(); ARRAY_OVERLOAD();


}