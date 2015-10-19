#pragma once
#include "MetaData.h"
#include <cassert>

namespace DCEngine {

  class MetaData;
  class VariantBase {
  public:
    const MetaData* Meta() const;

    template <typename T> T& GetValue();
    template <typename T> const T& GetValue() const;

    void* Data() const;

    void Serialize(std::ostream& os) const;
    
  protected:
    VariantBase();
    VariantBase(const MetaData* meta, void* data);

    const MetaData* VariantMeta;
    void* VariantData;   


  };

  template<typename T>
  inline T & VariantBase::GetValue()
  {
    return *reinterpret_cast<T*>(VariantData);
  }

  template<typename T>
  inline const T & VariantBase::GetValue() const
  {
    return *reinterpret_cast<T*>(VariantData);
  }

}