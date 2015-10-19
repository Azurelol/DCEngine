#include "RefVariant.h"
#include "Variant.h"

namespace DCEngine {

  RefVariant::RefVariant() : VariantBase(NULL, NULL)
  {
  }

  RefVariant::RefVariant(const MetaData * meta, void * data) : VariantBase(meta, data)
  {
  }

  RefVariant::RefVariant(const RefVariant & rhs) : VariantBase(rhs.Meta(), rhs.Data() )
  {

  }
  RefVariant::RefVariant(const Variant & rhs)
  {

  }

  RefVariant & RefVariant::operator=(const RefVariant & rhs)
  {
    VariantMeta = rhs.VariantMeta;
    VariantData = rhs.VariantData;
    return *this;
  }

  RefVariant & RefVariant::operator=(const Variant & rhs)
  {
    VariantMeta = rhs.Meta();
    VariantData = rhs.Data();
    return *this;
  }

}