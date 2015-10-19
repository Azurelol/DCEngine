/*****************************************************************************/
/*!
@file   Variant.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   10/18/2015
@brief  The Variant class acts as a container to a variable which gets
        registered to the MetaData system.
        Credit to Randy Gaul:
        http://www.randygaul.net/2013/01/04/c-reflection-part-4-variant/
*/
/******************************************************************************/
#pragma once
#include "VariantBase.h"

namespace DCEngine{

  /**************************************************************************/
  /*!
  @brief The Variant class can hold any sort of data which we can register
         with the MetaData system. Since it is not a templated class,
         we can store arrays of it. 
  */
  /**************************************************************************/
  class Variant : public VariantBase{

  public:
    // Constructors
    template <typename T> Variant(const T& value);
    Variant(const MetaData* m, void* d);
    Variant();
        
    // Operators
    Variant& operator=(const Variant& rhs);
    template <typename TYPE> Variant& operator=(const TYPE& rhs);
    
  };

  /**************************************************************************
                TEMPLATE DEFINITIONS
  **************************************************************************/
  /**************************************************************************/
  /*!
  @brief  Templated constructor for the Variant class.        
  @param  The value of the type.
  */
  /**************************************************************************/
  template<typename T>
  inline Variant::Variant(const T & value) : VariantBase(META_TYPE(T), NULL)
  {
    VariantData = VariantMeta->NewCopy(&value);
  }

  template<typename TYPE>
  inline Variant & Variant::operator=(const TYPE & rhs)
  {
    // If the Meta does not match, create a new copy
    if (VariantMeta != META_TYPE(TYPE)) {

      // Do not create instances of 'NULL' meta.
      assert(META_TYPE(TYPE)); 

      VariantMeta->Delete(VariantData);
      VariantMeta = META_TYPE(TYPE);
      VariantData = VariantMeta->NewCopy(&rhs);
    }
    else {
      VariantMeta->Copy(VariantData, &rhs);
    }
    return *this;
  }
}