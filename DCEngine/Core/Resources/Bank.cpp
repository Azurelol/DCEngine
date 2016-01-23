/*****************************************************************************/
/*!
@file      Bank.cpp
@author    Christian Sagel
@par       email: c.sagel\@digipen.edu
@date      1/22/2016
@brief     A bank..
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "Bank.h"

namespace DCEngine {

  /**************************************************************************/
  /*!
  @brief Provides the definition of this class to Zilch for reflection.
  */
  /**************************************************************************/
#if(DCE_USE_ZILCH_INTERNAL_BINDING)
  ZilchDefineType(Bank, "Bank", DCEngineCore, builder, type) {
  }
#endif

  /**************************************************************************/
  /*!
  @brief  CollisionGroup constructor.
  */
  /**************************************************************************/
  Bank::Bank(std::string bankFile) : Resource("Bank", FileSystem::FileNoExtension(bankFile),
                                              bankFile)
  {
  }

}