#pragma once

namespace Engine
{
  // Used to check if a type is a pointer or not
  template<typename T>
  struct isPointer { static const bool value = false; };
  template<typename T>
  struct isPointer<T *> { static const bool value = true; };
  template<typename T>
  struct isPointer<T&> { static const bool value = false; };

  /* Used to check if a type is a array literal or not */
  template<typename T>
  struct isArray { static const bool value = false; };
  template<typename T>
  struct isArray<T *> { static const bool value = false; };
  template<typename T>
  struct isArray<T&> { static const bool value = false; };

#define INTERNAL_ARRAY_OVERLOAD( x ) \
  template <typename T> \
  struct RemQual<T[ x ]> \
  { \
    typedef typename RemQual<T *>::type type; \
  }; \
  \
  template <typename T> \
  struct RemQual<const T[ x ]> \
  { \
    typedef typename RemQual<T *>::type type; \
  }; \
  /* Used to check if a type is a array literal or not */ \
  template<typename T> \
  struct isArray<T[ x ]> { static const bool value = true;  }; \
  /* Used to check if a type is a pointer or not */ \
  template<typename T> \
  struct isPointer<T[ x ]>      { static const bool value = true;  };

#define ARRAY_OVERLOAD( ) \
  INTERNAL_ARRAY_OVERLOAD( __COUNTER__ )

  //
  // RemQual
  // Strips down qualified types/references/pointers to a single unqualified type, for passing into
  // a templated type as a typename parameter.
  //

  template <typename T>
  struct RemQual
  {
    typedef T type;
  };

  template <typename T>
  struct RemQual<const T>
  {
    typedef typename RemQual<T>::type type;
  };

  template <typename T>
  struct RemQual<T&>
  {
    typedef typename RemQual<T>::type type;
  };

  template <typename T>
  struct RemQual<const T&>
  {
    typedef typename RemQual<T>::type type;
  };

  template <typename T>
  struct RemQual<T&&>
  {
    typedef typename RemQual<T>::type type;
  };

  template <typename T>
  struct RemQual<const T *>
  {
    typedef typename RemQual<T *>::type type;
  };

  // Array literal overloads
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