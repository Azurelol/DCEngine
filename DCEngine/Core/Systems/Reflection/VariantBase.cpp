#include "VariantBase.h"
#include "RefVariant.h"

namespace DCEngine {

  VariantBase::VariantBase() : VariantMeta(NULL), VariantData(NULL)
  {
  }

  VariantBase::VariantBase(const MetaData * meta, void * data) : VariantMeta(meta), VariantData(data)
  {
  }

  const MetaData * VariantBase::Meta() const
  {
    return VariantMeta;
  }

  void * VariantBase::Data() const
  {
    return VariantData;
  }

  void VariantBase::Serialize(std::ostream & os) const
  {
    VariantMeta->Serialize(os, RefVariant(VariantMeta, VariantData));
  }



}