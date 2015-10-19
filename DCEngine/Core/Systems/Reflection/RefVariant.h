#pragma once
#include "VariantBase.h"

namespace DCEngine {

  class Variant;
  class RefVariant : public VariantBase {

  public:
    template <typename T> RefVariant(const T& value);
    RefVariant(const RefVariant& rhs);
    RefVariant(const Variant& rhs);
    RefVariant(const MetaData* meta, void* data);
    RefVariant();

    RefVariant& operator=(const RefVariant& rhs);
    RefVariant& operator=(const Variant& rhs);
    template <typename T> RefVariant& operator=(const T& rhs);

  };

  template<typename T>
  inline RefVariant::RefVariant(const T & value) : VariantBase(META_TYPE(T), const_cast<T*>(&value))
  {
  }

  template<typename T>
  inline RefVariant & RefVariant::operator=(const T & rhs)
  {
    VariantMeta = META_TYPE(T);
    VariantData = const_cast<T*>(&rhs);
    return *this;
  }

}