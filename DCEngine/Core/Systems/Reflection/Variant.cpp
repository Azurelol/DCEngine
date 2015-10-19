#include "Variant.h"

namespace DCEngine {

  Variant::Variant(const MetaData * meta, void * data) : VariantBase(meta, data)
  {
  }

  Variant::Variant() : VariantBase(NULL, NULL)
  {
  }


  Variant & Variant::operator=(const Variant & rhs)
  {
    // Self assignment ho!
    if (this == &rhs)
      return *this;
    
    if (VariantMeta == rhs.VariantMeta)
      VariantMeta->Copy(VariantData, rhs.VariantData);
    else {
      // Do not create an instance of a NULL meta     
      assert(rhs.VariantMeta);
      VariantMeta->Delete(VariantData);
      VariantMeta = rhs.Meta();

      // If the meta does not match, create a copy
      if (VariantMeta)
        VariantData = VariantMeta->NewCopy(&rhs.VariantData);
    }
    return *this;
  }
  
}
