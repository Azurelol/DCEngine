#pragma once

// Used to check if a type is a pointer or not
template<typename T>
struct isPointer       { static const bool value = false; };
template<typename T>
struct isPointer<T *>  { static const bool value = true;  };
template<typename T>
struct isPointer<T&>   { static const bool value = false; };

/* Used to check if a type is a array literal or not */
template<typename T>
struct isArray         { static const bool value = false; };
template<typename T>
struct isArray<T *>    { static const bool value = false; };
template<typename T>
struct isArray<T&>     { static const bool value = false; };

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
struct RemQual<T *>
{
  typedef typename RemQual<T>::type type;
};
 
template <typename T>
struct RemQual<T[]>
{
  typedef typename RemQual<T>::type type;
};
 
template <typename T>
struct RemQual<const T *>
{
  typedef typename RemQual<T>::type type;
};
 
template <typename T>
struct RemQual<const T[]>
{
  typedef typename RemQual<T>::type type;
};

template< typename T >
struct RemoveConstRef
{
  typedef T type;
};

template< typename T >
struct RemoveConstRef<const T>
{
  typedef typename RemoveConstRef<T>::type type;
};

template< typename T >
struct RemoveConstRef<T&>
{
  typedef typename RemoveConstRef<T>::type type;
};

template< typename T >
struct RemoveConstRef<const T&>
{
  typedef typename RemoveConstRef<T>::type type;
};

///////////////////////////////

template <typename T>
struct RemQual<std::vector<T>>
{
  typedef std::vector<T> type;
};

template <typename T>
struct RemQual<std::vector<const T>>
{
  typedef typename RemQual<std::vector<T>>::type type;
};
 
template <typename T>
struct RemQual<std::vector<T&>>
{
  typedef typename RemQual<std::vector<T>>::type type;
};
 
template <typename T>
struct RemQual<std::vector<const T&>>
{
  typedef typename RemQual<std::vector<T>>::type type;
};
 
template <typename T>
struct RemQual<std::vector<T&&>>
{
  typedef typename RemQual<std::vector<T>>::type type;
};
 
template <typename T>
struct RemQual<std::vector<T *>>
{
  typedef typename RemQual<std::vector<T>>::type type;
};
 
template <typename T>
struct RemQual<std::vector<const T *>>
{
  typedef typename RemQual<std::vector<T>>::type type;
};

template< typename T >
struct RemoveConstRef<std::vector<T>>
{
  typedef typename RemoveConstRef<std::vector<T>>::type type;
};

template< typename T >
struct RemoveConstRef<std::vector<const T>>
{
  typedef typename RemoveConstRef<std::vector<T>>::type type;
};

template< typename T >
struct RemoveConstRef<std::vector<T&>>
{
  typedef typename RemoveConstRef<std::vector<T>>::type type;
};

template< typename T >
struct RemoveConstRef<std::vector<const T&>>
{
  typedef typename RemoveConstRef<std::vector<T>>::type type;
};

///////////////////////////////

template <typename T>
struct RemQualPtr
{
  typedef T type;
};
 
template <typename T>
struct RemQualPtr<const T>
{
  typedef typename RemQualPtr<T>::type type;
};
 
template <typename T>
struct RemQualPtr<T&>
{
  typedef typename RemQualPtr<T>::type type;
};
 
template <typename T>
struct RemQualPtr<const T&>
{
  typedef typename RemQualPtr<T>::type type;
};
 
template <typename T>
struct RemQualPtr<T&&>
{
  typedef typename RemQualPtr<T>::type type;
};
 
template <typename T>
struct RemQualPtr<const T *>
{
  typedef typename RemQualPtr<T *>::type type;
};

///////////////////////////////

template <typename T>
struct RemPtr
{
  typedef T type;
};

template <typename T>
struct RemPtr<T *>
{
  typedef typename RemPtr<T>::type type;
};

///////////////////////////////

template <typename T>
struct TypeToPtr
{
  typedef T *type;
};
